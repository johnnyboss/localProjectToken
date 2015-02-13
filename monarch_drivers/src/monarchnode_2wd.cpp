
#include <stdlib.h>
#include <stdio.h>
#include <std_msgs/String.h>
#include <ros/ros.h>
#include <ros/console.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>			// odom
#include <geometry_msgs/Twist.h>		// cmd_vel
#include <sensor_msgs/Imu.h>			// imu
#include <sensor_msgs/Range.h>			// sonars
#include <cereal_port/CerealPort.h>

#include "monarch_drivers/monarch_2wd.h"	//definitions
#include "monarch_drivers/BatteryState.h"	//battery msg
#include "monarch_drivers/CapacitiveState.h"	//capacitive sensors msg
#include "monarch_drivers/BumpersState.h"	//bumpers msg
#include "monarch_drivers/Temperature.h"	//temperature msg


socialRobot* myrobot;
double speed_x, speed_teta;
ros::Publisher *inertial_pub, *battery_pub, *capacitive_pub, *temperature_pub, *bumpers_pub, *sonars_pub;
unsigned long int seqID = 0;


void stopCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("stopCallback");
    if( msg->data == "Stop" && !myrobot->getForceStop())
    {
        myrobot->setForceStop(true);
	ROS_INFO("stop");
    }
    else if (msg->data == "Move" && myrobot->getForceStop())
    {
        myrobot->setForceStop(false);
	ROS_INFO("move");	
    }
}


void publish_IMU (double yaw){
	
	/** Publish IMU data (R,P,Y) **/
	sensor_msgs::Imu imu_msg;
	imu_msg.header.seq = seqID;
	imu_msg.header.stamp =  ros::Time::now();
	imu_msg.header.frame_id = "base_link";

	// QUATERNION ONLY FROM YAW (the robot is moving in a plane):
	imu_msg.orientation = tf::createQuaternionMsgFromYaw(yaw);
	
	//QUATERNION FROM RPY:
	//tf::Quaternion q; 
	//q = tf::createQuaternionFromRPY(roll,pitch,yaw);
	//tf::quaternionTFToMsg(q, imu_msg.orientation);
	
	imu_msg.orientation_covariance[0] = 0.00001;
	imu_msg.orientation_covariance[4] = 0.00001;
	imu_msg.orientation_covariance[8] = 0.00001;

	inertial_pub->publish(imu_msg);
	seqID++;  
}

//receive cmds_vel from nav_stack
void cmdVelReceived(const geometry_msgs::Twist::ConstPtr& cmd_vel)
{
	//if the robot is charging, we need to undock:
	if(myrobot->bat_cable > 16 && (fabs(cmd_vel->linear.x)>0.0001) || (fabs(cmd_vel->angular.x)>0.0001) ){
	  myrobot->undock();
	  //ROS_FATAL("undocking after docking");
	}
	
	speed_x = cmd_vel->linear.x;
	speed_teta = cmd_vel->angular.z;
}

//publish battery info
void publish_batteries (double bat_motor, double bat_elec, double bat_cable, double bat_pc){
  			
	monarch_drivers::BatteryState battery_msg;
	battery_msg.header.stamp = ros::Time::now();	
	battery_msg.bat_motor = bat_motor;
	battery_msg.bat_elec = bat_elec;
	battery_msg.bat_cable = bat_cable;
	battery_msg.bat_pc = bat_pc;
	battery_pub->publish (battery_msg);	
}

//publish temperature & humidity
void publish_temperature(double temperature, double humidity){
  
	monarch_drivers::Temperature temperature_msg;
	temperature_msg.header.stamp = ros::Time::now();	
	temperature_msg.temperature = temperature;
	temperature_msg.humidity = humidity;
	temperature_pub->publish (temperature_msg);	  
}

//publish capacitive sensors (5)
void publish_capacitive(bool *capacitive_sensors){
  
	monarch_drivers::CapacitiveState capacitive_msg;
	capacitive_msg.header.stamp = ros::Time::now();	
	capacitive_msg.sensor1 = capacitive_sensors[0];
	capacitive_msg.sensor2 = capacitive_sensors[1];
	capacitive_msg.sensor3 = capacitive_sensors[2];
	capacitive_msg.sensor4 = capacitive_sensors[3];
	capacitive_msg.sensor5 = capacitive_sensors[4];	
	capacitive_pub->publish (capacitive_msg);	  
}

//publish bumpers (4)
void publish_bumpers (bool *bumpers){
  
	monarch_drivers::BumpersState bumpers_msg;
	bumpers_msg.header.stamp = ros::Time::now();  
	bumpers_msg.right_bump = bumpers[0];
	bumpers_msg.left_bump = bumpers[1];
	bumpers_msg.front_bump = bumpers[2];
	bumpers_msg.rear_bump = bumpers[3];
	bumpers_pub->publish (bumpers_msg);  
}

void publish_sonars(double *sonars){
  
  for(int i=0; i<12; i++){
	sensor_msgs::Range sonar;
	sonar.header.stamp = ros::Time::now(); 
	char frame_id[10];
	sprintf(frame_id, "/sonar_%d", i);
	sonar.header.frame_id = frame_id;
	sonar.field_of_view = 0.34906585;	// 20 degress
	sonar.min_range = 0.1;
	sonar.max_range = 4.5;
	sonar.range = sonars[i];
	sonars_pub->publish(sonar);
  }
  
}

int main(int argc, char** argv){
    
	ros::init(argc, argv, "robotnode");
	ros::NodeHandle n;
	ros::NodeHandle pn("~");
	
	// ==== PARAMETERS  ==== 
	
	std::string inertial_port, robot_port, imu_topic;
	std::string base_frame_id, odom_frame_id;
	bool capacitive_used, temperature_used;
	
	pn.param<std::string>("robot_port", robot_port, "/dev/sr-motorboard" ); //Robot Port
	pn.param<std::string>("base_frame_id", base_frame_id, "base_link");
	pn.param<std::string>("odom_frame_id", odom_frame_id, "odom");	
	pn.param<std::string>("inertial_port", inertial_port, "/dev/ttyACM1"); //default should be: "/dev/sr-imu" (however this /dev is commonly mapped to the mouse input...)
	pn.param<std::string>("imu_topic", imu_topic, "/imu_data");
	pn.param("capacitive_used", capacitive_used, true);	//Publish (or not) Capacitive and temperature information
	pn.param("temperature_used", temperature_used, true);
	
	//Debug:
	//ROS_INFO("Inertial Port: %s", inertial_port.c_str());
	//ROS_INFO("IMU Topic: %s", imu_topic.c_str());
	
	// ==== PUBLISHERS  ==== 
	
	ros::Publisher pub_c, pub_t;
	
	if (capacitive_used){	  
	  pub_c = pn.advertise<monarch_drivers::CapacitiveState>("/capacitive_state", 10);
	  capacitive_pub = &pub_c;
	}
	
	if (temperature_used){	  
	  pub_t = pn.advertise<monarch_drivers::Temperature>("/temperature", 10, true); //latched topic
	  temperature_pub = &pub_t;
	}	

	ros::Publisher pub_bump = pn.advertise<monarch_drivers::BumpersState>("/bumpers_state", 10);
	bumpers_pub = &pub_bump;
	
	ros::Publisher pub_i = pn.advertise<sensor_msgs::Imu>(imu_topic, 1000);
	inertial_pub = &pub_i;
	
	ros::Publisher pub_b = pn.advertise<monarch_drivers::BatteryState>("/battery_state", 10, true); //latched topic
	battery_pub = &pub_b;
	
	ros::Publisher pub_s = n.advertise<sensor_msgs::Range>("/sonars", 50);
	sonars_pub = &pub_s;
	   
	double last_x, last_y, last_yaw, yaw, last_vx, last_vyaw, acc_x, acc_yaw;
	double vel_x, vel_y, vel_yaw;
	double dt;
	
	myrobot = new socialRobot ( robot_port.c_str(), inertial_port.c_str() );
    
	ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("/odom", 50);
	tf::TransformBroadcaster tf_broadcaster;
	ros::Subscriber cmd_vel_sub  = n.subscribe<geometry_msgs::Twist>("/cmd_vel", 50, cmdVelReceived);
	ros::Subscriber sub_stop = n.subscribe("stopper", 1000, stopCallback);		
	
	//wait (1.5 seconds) for serial port
	//ros::Duration(1.5).sleep();
    
	if( myrobot->setup() == 0) ROS_INFO("===== Connected to ROBOT =====");
	else{
		ROS_FATAL("ERROR!!! Could not connect to ROBOT.");
		//ROS_BREAK();
	}
	
	myrobot->getBatteries();
	ROS_INFO("Robot Battery: %.1f V", myrobot->bat_elec);
	
	if (myrobot->bat_cable > 0.10){
		ROS_WARN("Warning! Recharging Cable Plugged in.");	
	}else{
	  if (myrobot->bat_motor < 0.10){
		ROS_WARN("Warning! Motors are disconnected.");
	  }	
	  if (myrobot->bat_elec < 12.35){
		ROS_WARN("Warning! Low Battery! Please Charge.");
	  }
	}
	
	

	char bat_count = 0;
	ros::Rate r(10.0);
	ros::Time current_time, last_time;
	last_time = ros::Time::now();
	
	while(n.ok())
	{
		
		//Publish battery states
		bat_count ++;
		if (bat_count >= 50){	// every 5 seconds (latched)
		  myrobot->getBatteries();
		  publish_batteries (myrobot->bat_motor, myrobot->bat_elec, myrobot->bat_cable, myrobot->bat_pc);
		  bat_count = 0;
		  
		  //Publish Temperature & humidity (if used)
		  if(temperature_used){
		    myrobot->getTemperatureSensor();
		    publish_temperature (myrobot->temperature, myrobot->humidity);
		  }
		  
		}
		
		myrobot->getSonars();
		publish_sonars(myrobot->sonars);
		
		//Publish Capacitive Sensors states (10Hz)
		if(capacitive_used){
		  myrobot->getCapacitive();
		  publish_capacitive (myrobot->capacitive_sensors);
		}
		
		myrobot->getBumpers();	
		publish_bumpers (myrobot->bumpers);
		
		last_x = myrobot->getOdometry_x();
		last_yaw = myrobot->getOdometry_yaw();
		last_vx = vel_x;
		last_vyaw = vel_yaw;
		
		yaw = myrobot->getYaw();
		publish_IMU(yaw);
		myrobot->setYaw(yaw);
		
		current_time = ros::Time::now();
		myrobot->calculateOdometry();
		myrobot->drive( speed_x, speed_teta);
		
		float dx = (myrobot->getOdometry_x() - last_x);
		float dy = (myrobot->getOdometry_y() - last_y);
		dt = (current_time - last_time).toSec();
		vel_x = (myrobot->getDx())/dt; //- last_x)/dt;
		last_time = ros::Time::now();
		//if(vel_x < 0)
		//  ROS_FATAL("%f",vel_x);
		//if(vel_x < 0)
		//	vel_x *= -1;
		vel_y = 0;//(myrobot->getOdometry_y() - last_y)/dt;
		vel_yaw = (myrobot->getOdometry_yaw() - last_yaw)/dt;
		//if(vel_x != 0 || vel_yaw != 0)
		//ROS_FATAL("VEL_X:%f   VEL_YAW:%f", vel_x, vel_yaw);
		//ROS_INFO("ORIENTATION: %f", myrobot->getOdometry_yaw()*180/M_PI);
		
		// ******************************************************************************************
		//first, we'll publish the transforms over tf
		geometry_msgs::TransformStamped odom_trans;
		odom_trans.header.stamp = ros::Time::now();
		odom_trans.header.frame_id = odom_frame_id;
		odom_trans.child_frame_id = base_frame_id;
		odom_trans.transform.translation.x = myrobot->getOdometry_x();
		odom_trans.transform.translation.y = myrobot->getOdometry_y();
		odom_trans.transform.translation.z = 0.0;
		odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(myrobot->getOdometry_yaw());
		tf_broadcaster.sendTransform(odom_trans);
		
        
		// ******************************************************************************************
		//next, we'll publish the odometry message over ROS
		nav_msgs::Odometry odom;
		odom.header.stamp = ros::Time::now();
		odom.header.frame_id = odom_frame_id;
		
		//set the position
		odom.pose.pose.position.x = myrobot->getOdometry_x();
		odom.pose.pose.position.y = myrobot->getOdometry_y();
		odom.pose.pose.position.z = 0.0;
		odom.pose.pose.orientation = tf::createQuaternionMsgFromYaw(myrobot->getOdometry_yaw());
		
		//set the velocity
		odom.child_frame_id = base_frame_id;
		odom.twist.twist.linear.x = vel_x;
		odom.twist.twist.linear.y = 0;
		odom.twist.twist.angular.z = vel_yaw;
		
		//publish the odometry
		odom_pub.publish(odom);
        
		ros::spinOnce();
		r.sleep();
	}
	
	myrobot->shutdown();
    
}
