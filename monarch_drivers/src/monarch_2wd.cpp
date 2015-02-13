    
#include "monarch_drivers/monarch_2wd.h"
#include <stdint.h>
#include <stdlib.h>
#include <cereal_port/CerealPort.h>

// int left_acc = 0;
// int right_acc = 0;
double dx = 0;
double linearSpeed, rotSpeed;
// int counter = 0;
bool force_stop = false;
double last_yaw = 0;
int n_clicks_f_right = 26177;//(int)(2000 * 14);
int n_clicks_f_left = 26177;//(int)(2000 * 14);
int n_clicks_r_right = 26177;//(int)(2000 * 14);
int n_clicks_r_left = 26177;//(int)(2000 * 14);
int l_acc, r_acc;
ros::Time current_time, last_time;

socialRobot::socialRobot(const char* port, const char* i_port)
{
	serial_port = port;
	inertial_port = i_port;	
	leds_serial_port="/dev/sr-lightboard";
	sensor_serial_port="/dev/sr-sensorboard";

	serial_port_ = new cereal::CerealPort();
	leds_serial_port_ = new cereal::CerealPort();
	sensor_serial_port_ = new cereal::CerealPort();
	inertial_serial_port_ = new cereal::CerealPort();
	
	this->resetOdometry();
	motor_drivers_enabled = false;
	bumpers[0] = bumpers[1] = bumpers[2] = bumpers[3] = false;
	capacitive_sensors[0] = capacitive_sensors[1] = capacitive_sensors[2] = capacitive_sensors[3] = capacitive_sensors[4] = false;
}

// Destructor
socialRobot::~socialRobot()
{
	delete serial_port_;
	delete leds_serial_port_;
	delete sensor_serial_port_;
	delete inertial_serial_port_;
}

//Initialize
int socialRobot::setup()
{
  
	try
	{ 
		inertial_serial_port_->open((char*)inertial_port.c_str(), BAUDRATE); 
		char command[2];
		command[0] = (char)0x50;
		command[1] = (char)0x02;
		inertial_serial_port_->write(command, 2); 
		char buffer[4];
		try
		{ 
			inertial_serial_port_->readBytes(buffer, 4, 1000); 
		}
		catch(cereal::Exception& e)
		{ 
			ROS_FATAL("ERROR writing INERTIAL_CMD in serial port");

		}
	}
	catch(cereal::Exception& e)
	{
		ROS_FATAL("Robot -- Failed to open inertial serial port!");
		return(-1);
		//ROS_BREAK();
	} 
  
	try
	{
		serial_port_->open(serial_port.c_str(), BAUDRATE);  //115200
		current_time = ros::Time::now();
  	   	last_time = ros::Time::now();
		r_acc = l_acc = 0;
		force_stop = false;
		
	}
	catch(cereal::Exception& e)
	{
		ROS_ERROR("ERROR Can not open port %s", serial_port.c_str());
		return(-1);
	}
    
	try
	{
		leds_serial_port_->open(leds_serial_port.c_str(),BAUDRATE);
	}
	catch(cereal::Exception& e)
	{
		ROS_ERROR("ERROR Can not open port %s", leds_serial_port.c_str());
		return(-1);
	}
	try
	{
		sensor_serial_port_->open(sensor_serial_port.c_str(),BAUDRATE);
	}
	catch(cereal::Exception& e)
	{
		ROS_ERROR("ERROR Can not open port %s", sensor_serial_port.c_str());
		return(-1);
	}
	
	drawMouth(0);
	sleep(1);
	RGB_Leds_Control(0,0,60,60,0);
	RGB_Leds_Control(1,0,60,60,0);
	RGB_Leds_Control(2,0,0,60,0);
	RGB_Leds_Control(3,0,0,60,0);
	RGB_Leds_Control(4,60,0,0,0);
	getBatteries();
	
	if(bat_cable > 16){
	  undock();
	}
	
	return(0);
}

int socialRobot::shutdown()
{
	drawMouth(3);
    
	RGB_Leds_Control(0,0,0,0,10);
	RGB_Leds_Control(1,0,0,0,10);
	RGB_Leds_Control(2,0,0,0,10);
	RGB_Leds_Control(3,0,0,0,10);
	RGB_Leds_Control(4,0,0,0,10);
	
	getBatteries();
	/*if(bat_cable > 16)	//DPortugal (01-12-2014) -- not necessary: the robots docks by hardware
	  dock();*/
	
	try
	{
		serial_port_->close();
		leds_serial_port_->close();
		sensor_serial_port_->close();
	} catch(cereal::Exception& e)
	{
		ROS_ERROR("ERROR closing the serial port");
		return(-1);
	}
	return(0);
}

double socialRobot::getYaw()
{
	char command[1];
	command[0] = (char)0x40;
	
	try
	{ 
		inertial_serial_port_->write(command, 1); 
	}
	catch(cereal::Exception& e)
	{ 
		ROS_ERROR("ERROR writing INERTIAL_CMD in serial port");
		return -1;
	}

	int Yaw_int, Pitch_int, Roll_int, Checksum_read,Checksum_Total ;
        
	int size = 10;
	char buffer[size];
	try
	{ 
		inertial_serial_port_->readBytes(buffer, size, 1000); 
	}
	catch(cereal::Exception& e)
	{ 
		ROS_ERROR("ERROR reading INERTIAL from serial port");
		
		return(-1); 
	}
	Yaw_int = (int)((int)buffer[1] << 8) + (int)(buffer[2]);
        if (Yaw_int > 32767) 
		Yaw_int = -(65535 - Yaw_int);

        double yaw = -(double)Yaw_int / 100.0;	//yaw provided in angles
	yaw *= M_PI / 180.0;			//converted to rads
	
	return yaw;
}

//Checksum
bool socialRobot::checksum(char data[], int length)
{
	uint16_t checksum1 = (int)((unsigned char)data[length-2]); //High
	checksum1 = checksum1 << 8;
	checksum1 = checksum1 + (int)((unsigned char)data[length-1]); //low
	uint16_t checksum2=0;
	for(int i=0; i<(length-2); i++)
	{
		checksum2 += (int)((unsigned char)data[i]);
   	}
	if(checksum1 != checksum2)
	{
		return false;
	}
	return true;
}

//Send velocity command to socialRobot
int socialRobot::drive(double linear_speed , double angular_speed)
{
	if(linear_speed == 0 && angular_speed == 0)
	  RGB_Leds_Control(4,60,60,0,10);
	else
	  RGB_Leds_Control(4,0,0,60,10);
	char command[9];
	double rightWVel = (linear_speed + angular_speed*0.16);
	double leftWVel = -(linear_speed - angular_speed*0.16);
	//ROS_FATAL("w1:%f", left_f_vel);
	leftWVel *= 442;
	rightWVel *= 442;
    
	int leftWheel = (int)leftWVel;
	int rightWheel = (int)rightWVel;
	if(force_stop)
	{
		leftWheel = 0;
		rightWheel = 0;
	}
	//ROS_FATAL("%f    %f", leftWVel, rightWVel);
	command[0] = (char)VELOCITY_CMD;  //0x56
	command[1] = (char)((int)leftWheel >> 8);
	command[2] = (char)((int)leftWheel & 0xFF);
	command[3] = (char)((int)rightWheel >> 8);
	command[4] = (char)((int)rightWheel & 0xFF);
	command[5] = (char)((int)0 >> 8);
	command[6] = (char)((int)0 & 0xFF);
	command[7] = (char)((int)0 >> 8);
	command[8] = (char)((int)0 & 0xFF);
    
	try
	{
		serial_port_->write(command, 9);
	}
	catch(cereal::Exception& e)
	{
		ROS_ERROR("ERROR writing VELOCITY_CMD in serial port");
		return(-1);
	}
	int size = 4;
	char data_buffer[size];
	try
	{
		serial_port_->readBytes(data_buffer, size, 1000);
	}
	catch(cereal::Exception& e)
	{
		ROS_ERROR("ERROR reading VELOCITY_CMD from serial port");
		return(-1);
	}
	return(0);
}


int socialRobot::calculateOdometry()
{
	char command[1];
	command[0]= ENCODERS_TICKS; //0x4A
	try
	{
		serial_port_->write(command,1);
	}
	catch(cereal::Exception& e)
	{
		ROS_ERROR("ERROR writing ENCODERS_TICKS in serial port");
		return(-1);
	}
	int size = 12;
	char buffer[size];
	try
	{
		serial_port_->readBytes(buffer, size, 200);
		current_time = ros::Time::now();
	}
	catch(cereal::Exception& e)
	{
		
		ROS_ERROR("ERROR reading ENCODERS_TICKS from serial port");
		return(-1);
	}
    
	double dt = (current_time - last_time).toSec();
	
	int left_front_motor, right_front_motor,left_rear_motor, right_rear_motor;
	
	left_front_motor = (int)((int)buffer[1] * 256.0 + (int)buffer[2]);
	right_front_motor = (int)((int)buffer[3] * 256.0 + (int)buffer[4]);
	left_rear_motor = (int)((int)buffer[5] * 256.0 + (int)buffer[6]);
	right_rear_motor = (int)((int)buffer[7] * 256.0 + (int)buffer[8]);
	
	double dl = -(double)((left_front_motor * M_PI * 2 * 0.05 / n_clicks_f_left));
	double dr = (double)((right_front_motor * M_PI * 2 * 0.05 / n_clicks_f_left));
	dx = (dr + dl) /2.0;
	
	this->odometry_x_ += ((dl + dr) / 2.0) * cos((double) odometry_yaw_);   //m
	this->odometry_y_ += ((dl + dr) / 2.0) * sin((double) odometry_yaw_);	//m
    
	/*if(linearSpeed == 0 && rotSpeed != 0 )
		counter++;
	else
		counter = 0;*/
	
	last_time = ros::Time::now();
	return(0);
}

// Reset socialRobot odometry
void socialRobot::resetOdometry()
{
	setOdometry(0.0, 0.0, 0.0);
}

// Set socialRobot odometry
void socialRobot::setOdometry(double new_x, double new_y, double new_yaw)
{
	odometry_x_ = new_x;
	odometry_y_ = new_y;
	odometry_yaw_ = new_yaw;
}


inline double socialRobot::RTOD (double a)
{
	return 180.0 * a / M_PI;
}
inline double socialRobot::DTOR(double a)
{
	return M_PI * a / 180.0;
}

void socialRobot::setYaw(double _yaw)
{
	last_yaw = this->odometry_yaw_;
	this->odometry_yaw_ = _yaw;
}

void socialRobot::setVelYaw(double _vel_th)
{
	this->vel_th = _vel_th;
}

double socialRobot::getOdometry_x() {return odometry_x_;}
double socialRobot::getDx() {return dx;}
double socialRobot::getOdometry_y() {return odometry_y_;}
double socialRobot::getOdometry_yaw() {return odometry_yaw_;}
double socialRobot::getVelocity_x(){return vel_x;}
double socialRobot::getVelocity_y(){return vel_y;}
double socialRobot::getVelocity_th(){return vel_th;}
bool socialRobot::getForceStop(){return force_stop;}
void socialRobot::setForceStop(bool _force_stop){ force_stop = _force_stop;}

void socialRobot::getCapacitive()
{
    char command[1];
    command[0]= 0x52; //0x4A
    try
    {
    leds_serial_port_->write(command,1);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR writing CAPACITIVE in serial port");

    }
    int size = 6;
    char buffer[size];
    try
    {
     leds_serial_port_->readBytes(buffer, size, 1000);
    }
    catch(cereal::Exception& e)
    {
      ROS_ERROR("ERROR reading CAPACITIVE from serial port");
    }
       
    if((buffer[1] & 0x01) != 0){
      capacitive_sensors[0] = true;
//       ROS_FATAL("CAPACITIVE1");
    }else{
      capacitive_sensors[0] = false;
    }
   
    if((buffer[1] & 0x02) != 0){
//       ROS_FATAL("CAPACITIVE2");
      capacitive_sensors[1] = true;
    }else{
      capacitive_sensors[1] = false;
    }
    
    if((buffer[1] & 0x04) != 0){
//       ROS_FATAL("CAPACITIVE3");
      capacitive_sensors[2] = true;
    }else{
      capacitive_sensors[2] = false;
    }
    
    if((buffer[1] & 0x08) != 0){
//       ROS_FATAL("CAPACITIVE4");
      capacitive_sensors[3] = true;
    }else{
      capacitive_sensors[3] = false;
    }
    
    if((buffer[1] & 0x10) != 0){
//       ROS_FATAL("CAPACITIVE5");
      capacitive_sensors[4] = true;
    }else{
      capacitive_sensors[4] = false;
    }
    
}

	
	
void socialRobot::getBumpers()
{
    char command[1];
    command[0]= 0x53; //0x4A
    try
    {
    sensor_serial_port_->write(command,1);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR writing BUMPERS in serial port");

    }
    int size = 5;
    char buffer[size];
    try
    {
    sensor_serial_port_->readBytes(buffer, size, 1000);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR reading BUMPERS from serial port");

    }
    
    if ((buffer[1] & 0x08) != 0){	//right bump
      //ROS_FATAL("RIGHT BUMP");
      bumpers[0] = true;
    }else{
      bumpers[0] = false;
    }
    
    if ((buffer[1] & 0x04) != 0){	//left bump
      //ROS_FATAL("LEFT BUMP");
      bumpers[1] = true;
    }else{
      bumpers[1] = false;
    }
        
    if ((buffer[1] & 0x02) != 0){	//front bump
      //ROS_FATAL("FRONT BUMP");
      bumpers[2] = true;
    }else{
      bumpers[2] = false;
    }
    
    if ((buffer[1] & 0x01) != 0){	//rear bump
      //ROS_FATAL("REAR BUMP");
      bumpers[3] = true;
    }else{
      bumpers[3] = false;
    }
    
    /*if ( ((buffer[1] & 0x08) != 0) || ((buffer[1] & 0x04) != 0) || ((buffer[1] & 0x02) != 0) || ((buffer[1] & 0x01) != 0) ){ 
      bump = true;
      ROS_FATAL("BUMP");
    }else{
      bump = false;
    }*/
	
}
void socialRobot::getTemperatureSensor()
{
    char command[1];
    command[0]= 0x56; //0x4A
    try
    {
    sensor_serial_port_->write(command,1);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR writing TEMPERATURE in serial port");

    }
    int size = 6;
    char buffer[size];
    try
    {
    sensor_serial_port_->readBytes(buffer, size, 1000);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR reading TEMPERATURE from serial port");

    }
    
    temperature = (double)((unsigned char)buffer[1]);
    humidity = (double)((unsigned char)buffer[2]);
   // ROS_INFO("%f %f", temperature, humidity);
}


void socialRobot::getSonars()	/** TODO **/
{
    char command[1];
    command[0]= 0x54; //0x4A
    try
    {
    sensor_serial_port_->write(command,1);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR writing SONARS in serial port");

    }
    int size = 28;
    char buffer[size];
    try
    {
    sensor_serial_port_->readBytes(buffer, size, 1000);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR reading SONARS from serial port");

    }
     
   
    //int sonars_cm[12], i;
    
    //for (i=0; i<24; i++){ 
    //  ROS_INFO("buffer[%d] = %d", i+1, ((unsigned char)buffer[i+1]));
    //}   
    
    /*
    for (i=0; i<12; i++){
     //number = buf[0] | buf[1] << 8;
     ROS_INFO("value[%d] = %d", i, (buffer[2*i+1] | buffer[2*i+2] << 8) );
    }*/
    
    for (int i=0; i<12; i++){ 
      
      sonars[i] = 0.013895* (double)(256*((unsigned char)buffer[2*i+1]) + ((unsigned char)buffer[2*i+2])); //cms
      sonars[i]/=100;	//meters
      
      //ROS_INFO("sonar[%d] = %f", i, sonars[i]);
    }
    
    /*
    for (i=0; i<12; i++){ 
      sonars_cm[i] = (int) (0.013895* (256.0*buffer[i+1] + buffer[i+13]) );
      ROS_INFO("sonar[%d] = %d", i, sonars_cm[i]);
    } */   
    
}


void socialRobot::getBatteries()
{
    char command[1];
    command[0]= 0x51; //0x4A
    try
    {
    sensor_serial_port_->write(command,1);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR writing VOLTAGE in serial port");

    }
    int size = 7;
    char buffer[size];
    try
    {
    sensor_serial_port_->readBytes(buffer, size, 1000);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR reading VOLTAGE from serial port");

    }
    //ROS_INFO("Got this reply: %s", buffer[1]);


    bat_motor = (double)((unsigned char)buffer[1])/10.0;
    bat_elec = (double)((unsigned char)buffer[2])/10.0;
    bat_cable = (double)((unsigned char)buffer[3])/10.0;
    
    
    command[0]= 0x52;
    try
    {
    sensor_serial_port_->write(command,1);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR writing VOLTAGE in serial port");
    }

    size = 6;
    char buffer_pc[size];

    try
    {
    sensor_serial_port_->readBytes(buffer_pc, size, 1000);
    }
    catch(cereal::Exception& e)
    {
    ROS_ERROR("ERROR reading VOLTAGE from serial port");
    }

    //AnalogPC1 = (double)((double)buffer_pc[1] / 10.0);
    //AnalogPC2 = (double)((double)buffer_pc[2] / 10.0);    
    
    bat_pc = (double)((unsigned char)buffer_pc[1])/10.0;
      
    //ROS_INFO("motor:%f elec:%f cable;%f",bat_motor, bat_elec, bat_cable); 

}

void socialRobot::getChargingStatus()
{
    if(bat_cable > 16)
    {
        char command[1];
        command[0]= 0x58; //0x4A
        try
        {
            sensor_serial_port_->write(command,1);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR writing CHARGING in serial port");

        }
        int size = 5;
        char buffer[size];
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading CHARGING from serial port");

        }

	if ((buffer[1] & 0x01) != 0) 
	ROS_WARN("Electronics Charging");
	if ((buffer[1] & 0x02) != 0) 
	ROS_WARN("Motors Charging");
	if ((buffer[1] & 0x04) != 0) 
	ROS_WARN("PC1 Charging");
	if ((buffer[1] & 0x08) != 0) 
	ROS_WARN("PC2 Charging");
    }
}

void socialRobot::undock()
{
	char command[2];
        command[0]= 0x45; //Motor battery ON
        command[1]= 0x02;
	sensor_serial_port_->write(command,2);

	int size = 4;
        char buffer[size];
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading MOTOR UNDOCK from serial port");

        }
        
        command[0]= 0x46; //Electronic battery ON
        command[1]= 0x02;
	sensor_serial_port_->write(command,2);
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading ELECTRONIC UNDOCK from serial port");

        }
        
        command[0]= 0x40; //Motor battery ON
        command[1]= 0x02;
	sensor_serial_port_->write(command,2);
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading PC1 UNDOCK from serial port");

        }
        command[0]= 0x41; //Motor battery ON
        command[1]= 0x02;
	sensor_serial_port_->write(command,2);
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading PC2 UNDOCK from serial port");

        }
        
}
void socialRobot::dock()
{
	char command[2];
        command[0]= 0x45; //Motor battery ON
        command[1]= 0x01;
	sensor_serial_port_->write(command,2);

	int size = 4;
        char buffer[size];
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading MOTOR UNDOCK from serial port");

        }
        
        command[0]= 0x46; //Electronic battery ON
        command[1]= 0x01;
	sensor_serial_port_->write(command,2);
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading ELECTRONIC UNDOCK from serial port");

        }
        
        command[0]= 0x40; //Motor battery ON
        command[1]= 0x01;
	sensor_serial_port_->write(command,2);
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading PC1 UNDOCK from serial port");

        }
        command[0]= 0x41; //Motor battery ON
        command[1]= 0x01;
	sensor_serial_port_->write(command,2);
        try
        {
            sensor_serial_port_->readBytes(buffer, size, 1000);
        }
        catch(cereal::Exception& e)
        {
            ROS_ERROR("ERROR reading PC2 UNDOCK from serial port");

        }
        
}
void socialRobot::setIntensity(int intensity, char panel)
{
	if (intensity < 0)
		intensity = 0;
	if(intensity > 15)
		intensity = 15;
	
	char command[3];
	command[0] = (char)0x41;
	command[1] = (char)intensity; // Value between 0-15
	leds_serial_port_->write(command, 3);
	int contador = 0;
	char buffer[30];
	
	try
	{
		leds_serial_port_->readBytes(buffer, 30, 1000);
	}
	catch(cereal::Exception& e)
	{
	}
}


void socialRobot::RGB_Leds_Control(char device, char Red, char Green, char Blue, char time) //values between 0 - 100. 
{
	char buffer[4];
	char command[6];
	command[0] = 0x43;
	command[1] = device;
	command[2] = Red;
	command[3] = Green;
	command[4] = Blue;
	command[5] = time;
	
	leds_serial_port_->write(command, 6);
	
	try
	{
		leds_serial_port_->readBytes(buffer, 4,2000);
	}
	catch(cereal::Exception& e)
	{
		//leds_serial_port_->write(command, 6);
	}
}

void socialRobot::drawMouth(int shape) //0 - happy 1-sad 3- off
{
	char result[32];
	for (int i = 0; i < 32; i++)
	{
		result[i] = (char)0;
	}
	if(shape == 0)
	{
		for (int w = 0; w < 32; w++)
		{
			for (int h = 0; h < 8; h++)
			{
				//if(h == 7 )
				if ((h == 1 && (w == 3 || w == 32-3))||
				    (h == 2 && ((w > 2 && w < 6) || (w > 32-6 && w < 32 - 2)))||
				    (h == 3 && (w > 4  &&  w < 32 - 4))||
				    (h == 4 && (w > 5 &&  w < 32 - 5))||
				    (h == 5 && (w > 6 &&  w < 32 - 6)))
				{
					result[31-w] += (char)pow(2, 7-h);
				}
				else
					result[31-w] += (char)0;
			}
		}
	}
	else if(shape == 1)
	{
		for (int w = 0; w < 32; w++)
		{
			for (int h = 0; h < 8; h++)
			{
				//if(h == 7 )
				if ((h == 5 && (w == 5 || w == 32-5))||
				    (h == 4 && ((w > 4 && w < 8) || (w > 32-8 && w < 32 - 4)))||
				    (h == 3 && (w > 6 &&  w < 32 - 6))||
				    (h == 2 && (w > 7 &&  w < 32 - 7))||
				    (h == 1 && (w > 9 &&  w < 32 - 9)))
				{
					result[31-w] += (char)pow(2, 7-h);
				}
				else
					result[31-w] += (char)0;
			}
		}
	}
	else if(shape == 2)
	{
		for (int w = 0; w < 32; w++)
		{
			for (int h = 0; h < 8; h++)
			{
				//if(h == 7 )
				if ((h == 3 && (w > 6 &&  w < 32 - 6))||
				    (h == 2 && (w > 7 &&  w < 32 - 7))||
				    (h == 1 && (w > 9 &&  w < 32 - 9)))
				{
					result[31-w] += (char)pow(2, 7-h);
				}
				else
					result[31-w] += (char)0;
			}
		}
	}
	else if(shape == 3)
	{
		for (int w = 0; w < 32; w++)
		{
			for (int h = 0; h < 8; h++)
			{
				result[31-w] += (char)0;
			}
		}
	}
	char command[33];
	command[0] = 0x40;
	//command[1] = (byte)panels[index].letter;
	for (int j = 1; j < 33; j++)
	{
		command[j] = (char)result[j - 1];
	}
	leds_serial_port_->flush();
	leds_serial_port_->write(command, 33);

	for (int i = 0; i < 32; i++)
	{
		result[i] = 0;
	}
	char buffer[30];
	try
	{
		leds_serial_port_->readLine(buffer, 4, 1000);
	}
	catch(cereal::Exception& e)
	{
		leds_serial_port_->flush();
	}
}

/*void get_sonars(){
}*/