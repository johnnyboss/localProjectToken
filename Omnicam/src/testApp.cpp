#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    for (int i = 0; i < 3; i++)
    {
        savedSpeeds1.push_back(0);
        savedSpeeds2.push_back(0);
    }

    for(int i = 0; i < 9; i++)
    {
        savedPositions1.push_back(0);
        savedPositions2.push_back(0);
    }

    for(int i = 0; i < 6; i++)
    {
        savedInverts1.push_back(false);
        savedInverts2.push_back(false);
    }

    msgSend	= "";
    msgReceive	= "";

    //are we connected to the server - if this fails we
    //will check every few seconds to see if the server exists
    //weConnected = tcpClient.setup("192.168.1.54", 11998);
    weConnected = tcpClient.setup("127.0.0.1",11999);
    //("192.168.1.10", 11999);


    // tcpClient.setMessageDelimiter("TCP");

    ofSetFrameRate(30);

    back.loadImage("OmniCam4Sky_Layers Azul.jpg");

    info.loadFont("arialbd.ttf", 12);
    ofSetWindowPosition(-1000,-1000);

}


//--------------------------------------------------------------
void testApp::update()
{
    receive();
    //send();

}

void testApp::send(string msg)
{
    if(weConnected)
    {
        cout<<"Message sent "<<msg<<endl;
        tcpClient.send(msg);
    }
}

void testApp::receive()
{
    if(weConnected)
    {
        msgReceive = tcpClient.receive();
        if(msgReceive.length() > 0)
        {
            // cout<<"msg received "<<msgReceive<<endl;
            vector<string> msgs = ofSplitString(msgReceive, " ");
            if(msgs[0] == "initialize")
            {
                initialize(msgs);
            }

            else if(msgs[0] == "what")
            {
                send("interface");
            }

            else if(msgs[0] == "create" || (startFrame < 10 && startFrame > 0))
            {
                startFrame++;

                vector <string> submsg1 = ofSplitString(msgs[1], "_");
                if(submsg1.size() > 0)
                {
                    if(ofToInt(submsg1[1]) == 1)
                    {
                        nomnicams = 1;
                        panel1.setActive(true);
                        panelTop.setnumomnicams(1);
                    }
                    else if(ofToInt(submsg1[1]) == 2)
                    {
                        nomnicams = 2;
                        panel1.setActive(true);
                        panel2.setActive(true);
                    }

                    vector <string> submsg2 = ofSplitString(msgs[2], "_");
                    {
                        if(ofToInt(submsg2[1]) == 1 && panel1.isActive() && panel2.isActive())
                        {
                            nconsoles = 1;
                            panelTop.setnumconsoles(1);
                            panelTop.setnumomnicams(2);
                        }
                        else if(ofToInt(submsg2[1]) == 1 && panel1.isActive())
                        {
                            nconsoles = 1;
                            panelTop.setnumconsoles(1);
                        }

                        if(ofToInt(submsg2[1]) == 2)
                        {
                            nconsoles = 2;
                        }
                    }
                    ofSetFullscreen(true);
                }
                createstring();
            }
            else if(startFrame > 0)
            {
                panel1.update(msgReceive);
                panel2.update(msgReceive);
                panelTop.update(msgReceive);
            }
        }
    }
}
//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(0, 0, 0);
    ofSetColor(255,255,255,255);
    panel1.draw();
    panel2.draw();
    panelTop.draw();

    if(weConnected)
        ofDrawBitmapString("Connected", 10,10);
    else
        ofDrawBitmapString("Not Connected", 10,10);

    ofSetLineWidth(5);
    ofSetColor(0,191,255);
    ofFill();
    ofRect(0,0,ofGetWidth(), 28);
    ofNoFill();
    ofRect(0,2,ofGetWidth(), ofGetHeight()-5);
    ofSetLineWidth(1);
    ofSetColor(255,255,255);

    info.drawString("Omnicam System Controller", 5, 20);
    ofSetColor(255, 255, 255);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    if(!panelTop.ismenuactive())
    {
        panel1.dealwithtouches(x, y, 1);
        if(panel1.toSend)
        {
            send(panel1.msg);
            panel1.toSend = false;
            panel1.msg = "";
        }

        panel2.dealwithtouches(x, y, 1);
        if(panel2.toSend)
        {
            send(panel2.msg);
            panel2.toSend = false;
            panel2.msg = "";
        }
    }

    panelTop.dealwithtouches(x, y, 1);
    if(panelTop.toSend)
    {
        send(panelTop.msg);
        panelTop.toSend = false;
    }

    if(panelTop.onExit.toExit)
    {
        save();
    }

    if(panel1.buttons[3].isClicked(x,y) && panel1.buttons[18].isActive() && !panel1.block)
    {
        if(panel1.buttons[3].isActive())
        {
            panel1.buttons[3].setActive(false);
            send("omnicam1releasecontrol");
        }
        else
        {
            panel1.buttons[3].setActive(true);
            if(nconsoles == 1)
                panel2.buttons[3].setActive(false);

            send("omnicam1takecontrol");

        }
    }

    if(panel2.buttons[3].isClicked(x,y) && panel2.buttons[18].isActive() && !panel2.block && nomnicams == 2)
    {
        if(panel2.buttons[3].isActive())
        {
            panel2.buttons[3].setActive(false);
            send("omnicam2releasecontrol");
        }
        else
        {
            panel2.buttons[3].setActive(true);

            if(nconsoles == 1)
                panel1.buttons[3].setActive(false);

            send("omnicam2takecontrol");
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    panel1.released();
    panel1.keypad.released();
    panelTop.released();
    panel2.released();
    panel2.keypad.released();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    panel1.create();
    panel2.create();
    panelTop.create();

    if(savedSpeeds1[0] > 0)
    {
        panel1.posGraph.updateMax(savedSpeeds1[0]);
        panel1.buttons[15].recordedValue = savedSpeeds1[0];
        panel1.vMax = savedSpeeds1[0];
    }

    if(savedSpeeds1[1] > 0)
    {
        panel1.posGraph.updateMin(savedSpeeds1[1]);
        panel1.buttons[16].recordedValue = savedSpeeds1[1];
        panel1.vMin = savedSpeeds1[1];
    }

    panel1.goSpeed = savedSpeeds1[2];

    if(savedSpeeds2[0] > 0)
    {
        panel2.posGraph.updateMax(savedSpeeds2[0]);
        panel2.buttons[15].recordedValue = savedSpeeds2[0];
        panel2.vMax = savedSpeeds2[0];
    }

    if(savedSpeeds2[1] > 0)
    {
        panel2.posGraph.updateMin(savedSpeeds2[1]);
        panel2.buttons[16].recordedValue = savedSpeeds2[1];
        panel2.vMin = savedSpeeds2[1];
    }

    panel2.goSpeed = savedSpeeds2[2];

    for (int i = 0; i < 9; i++)
    {
        panel1.buttons[i+4].recordedValue = savedPositions1[i];
        panel2.buttons[i+4].recordedValue = savedPositions2[i];
    }

    for (int i = 0; i < 5; i++)
    {
        panelTop.servos.buttons[i].setActive(savedInverts1[i]);
        panelTop.servos.buttons[i+5].setActive(savedInverts2[i]);
    }
    panelTop.pedals.buttons[0].setActive(savedInverts1[5]);
    panelTop.pedals.buttons[1].setActive(savedInverts2[5]);
}

void testApp::initialize(vector <string> msgs)
{
    for(int i = 1; i < msgs.size(); i++)
    {
        vector<string> submsg = ofSplitString(msgs[i], "_");
        if(submsg.size() > 0)
        {
            if(submsg[0] == "omnicam1vmax")
            {
                savedSpeeds1[0] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1vmin")
            {
                savedSpeeds1[1] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1gotospeed")
            {
                savedSpeeds1[2] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but0")
            {
                savedPositions1[0] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but1")
            {
                savedPositions1[1] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but2")
            {
                savedPositions1[2] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but3")
            {
                savedPositions1[3] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but4")
            {
                savedPositions1[4] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but5")
            {
                savedPositions1[5] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but6")
            {
                savedPositions1[6] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but7")
            {
                savedPositions1[7] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1but8")
            {
                savedPositions1[8] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam1invertservo1")
            {
                savedInverts1[0] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam1invertservo2")
            {
                savedInverts1[1] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam1invertservo3")
            {
                savedInverts1[2] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam1invertservo4")
            {
                savedInverts1[3] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam1invertservo5")
            {
                savedInverts1[4] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam1invertpedals")
            {
                savedInverts1[5] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam2vmax")
            {
                savedSpeeds2[0] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2vmin")
            {
                savedSpeeds2[1] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2gotospeed")
            {
                savedSpeeds2[2] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but0")
            {
                savedPositions2[0] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but1")
            {
                savedPositions2[1] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but2")
            {
                savedPositions2[2] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but3")
            {
                savedPositions2[3] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but4")
            {
                savedPositions2[4] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but5")
            {
                savedPositions2[5] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but6")
            {
                savedPositions2[6] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but7")
            {
                savedPositions2[7] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2but8")
            {
                savedPositions2[8] = ofToFloat(submsg[1]);
            }
            else if(submsg[0] == "omnicam2invertservo1")
            {
                savedInverts2[0] = ofToBool(submsg[1]);
                cout<<savedInverts1[0]<<endl;
            }
            else if(submsg[0] == "omnicam2invertservo2")
            {
                savedInverts2[1] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam2invertservo3")
            {
                savedInverts2[2] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam2invertservo4")
            {
                savedInverts2[3] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam2invertservo5")
            {
                savedInverts2[4] = ofToBool(submsg[1]);
            }
            else if(submsg[0] == "omnicam2invertpedals")
            {
                savedInverts2[5] = ofToBool(submsg[1]);
            }
        }
    }
}

void testApp::createstring()
{
    string saveVelocities;
    if(startFrame == 1)
    {
        saveVelocities += " omnicam1vmax_"+ofToString(panel1.vMax);

        saveVelocities += " omnicam1pedalinvert_";
        if(panelTop.pedals.buttons[0].isActive())
            saveVelocities += "ON";
        else
            saveVelocities += "OFF";

        saveVelocities += " omnicam2pedalinvert_";
        if(panelTop.pedals.buttons[1].isActive())
            saveVelocities += "ON";
        else
            saveVelocities += "OFF";

        for (int i = 0; i < 5; i++)
        {
            saveVelocities += " servo1servoinvert_"+ofToString(i+1)+"_";
            if(panelTop.servos.buttons[i].isActive())
                saveVelocities += "ON";
            else
                saveVelocities += "OFF";

            saveVelocities += " servo2servoinvert_"+ofToString(i+1)+"_";
            if(panelTop.servos.buttons[i+5].isActive())
                saveVelocities += "ON";
            else
                saveVelocities += "OFF";
        }
        send(saveVelocities);
    }
    else if(startFrame == 2)
    {
        saveVelocities += " omnicam2vmax_"+ofToString(panel2.vMax);
        send(saveVelocities);
    }
    else if(startFrame == 3)
    {
        saveVelocities += " omnicam1vmin_"+ofToString(panel1.vMin);
        send(saveVelocities);
    }
    else if(startFrame == 4)
    {
        saveVelocities += " omnicam2vmin_"+ofToString(panel2.vMin);
        send(saveVelocities);
    }
    else if(startFrame == 5)
    {
        saveVelocities += " omnicam1posvelmax_"+ofToString(panel1.goSpeed);
        send(saveVelocities);
    }
    else if(startFrame == 6)
    {
        saveVelocities += " omnicam2posvelmax_"+ofToString(panel2.goSpeed);
        send(saveVelocities);
    }
    else if(startFrame == 7)
    {
        saveVelocities = ("omnicam1releasecontrol");
        send(saveVelocities);
    }
    else if(startFrame == 8)
    {
        saveVelocities = ("omnicam2releasecontrol");
        send(saveVelocities);
    }
}

void testApp::save()
{
    string savePositions;
    string saveVelocities;
    if(panelTop.onExit.toSavePositions)
    {
        for (int i = 0; i < 9; i++)
        {
            int value =  panel1.buttons[i+4].recordedValue;// * panel1.posGraph.lenght - panel1.posGraph.lenght/2;
            savePositions += " omnicam1but"+ofToString(i)+"_";
            savePositions += ofToString(value);
            value =  panel2.buttons[i+4].recordedValue;
            savePositions += " omnicam2but"+ofToString(i)+"_";
            savePositions += ofToString(value);
        }
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
            int value =  (int)(savedPositions1[i]);
            savePositions += " omnicam1but"+ofToString(i)+"_";
            savePositions += ofToString(value);
            value =  (int)(savedPositions2[i]);
            savePositions += " omnicam2but"+ofToString(i)+"_";
            savePositions += ofToString(value);
        }
    }


    saveVelocities += " omnicam1vmax_"+ofToString(20);
    saveVelocities += " omnicam1vmin_"+ofToString(2);
    saveVelocities += " omnicam1gotospeed_"+ofToString(10);
    saveVelocities += " omnicam2vmax_"+ofToString(20);
    saveVelocities += " omnicam2vmin_"+ofToString(2);
    saveVelocities += " omnicam2gotospeed_"+ofToString(10);

    for (int i = 0; i < 5; i++)
    {
        savePositions += " omnicam1invertservo"+ofToString(i+1)+"_";
        if(panelTop.servos.buttons[i].isActive())
            savePositions += "true";
        else
            savePositions += "false";

        savePositions += " omnicam2invertservo"+ofToString(i+1)+"_";
        if(panelTop.servos.buttons[i+5].isActive())
            savePositions += "true";
        else
            savePositions += "false";
    }

    savePositions += " omnicam1invertpedals_";
    if(panelTop.pedals.buttons[0].isActive())
        savePositions += "true";
    else
        savePositions += "false";

    savePositions += " omnicam2invertpedals_";
    if(panelTop.pedals.buttons[1].isActive())
        savePositions += "true";
    else
        savePositions += "false";

    tosendonexit = "exit";
    tosendonexit += saveVelocities + savePositions;
    cout<<tosendonexit<<endl;
    send(tosendonexit);
}



