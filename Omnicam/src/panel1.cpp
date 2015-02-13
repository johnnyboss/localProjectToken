//
//  panel1.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "panel1.h"

void Panel1::create()
{
    digital.loadFont("QuartzMS.ttf", 14);
    info.loadFont("micross.ttf", 10);
    top.loadFont("micross.ttf", 12);

    back.loadImage("degrade.jpg");
    degrade2.loadImage("degrade2.png");

    setup(0, 80, ofGetWidth(), (ofGetHeight()-80)/2.0);

    slider aux2;
    aux2.create(panelArea.x + 10, panelArea.y + 20, panelArea.width - 20, 80, 0.5, "omnicam1pos");
    sliders.push_back(aux2);

    button aux3;
    aux3.create(panelArea.x + 1050 , panelArea.y + 230,65,50, "gotoleft", false," Left");
    buttons.push_back(aux3);//0
    aux3.create(buttons[0].buttonarea.x + buttons[0].buttonarea.width + 1, panelArea.y + 230,65,50, "gotoreset", false,"Reset");
    buttons.push_back(aux3);//1
    aux3.create(buttons[1].buttonarea.x + buttons[1].buttonarea.width + 1, panelArea.y + 230,65,50, "gotoright", false,"Right");
    buttons.push_back(aux3);//2

    aux3.create(panelArea.x + panelArea.width - 120,  panelArea.y + panelArea.height - 55, 80,50, "takecontrol", true, "Activate\nOmni C");
    buttons.push_back(aux3);//3

    aux3.create(panelArea.x + 580, panelArea.y + 230, 70,50, "position1", false, "Pos 1");
    buttons.push_back(aux3);//4
    aux3.create(buttons[4].buttonarea.x + buttons[4].buttonarea.width + 1, panelArea.y + 230,70,50, "position2", false,"Pos 2");
    buttons.push_back(aux3);//5
    aux3.create(buttons[5].buttonarea.x + buttons[5].buttonarea.width + 1, panelArea.y + 230,70,50, "position3", false,"Pos 3");
    buttons.push_back(aux3);//6
    aux3.create(buttons[4].buttonarea.x , panelArea.y + 290,70,50,"position4", false, "Pos 4");
    buttons.push_back(aux3);//7
    aux3.create(buttons[7].buttonarea.x + buttons[7].buttonarea.width + 1, panelArea.y + 290,70,50, "position5", false,"Pos 5");
    buttons.push_back(aux3);//8
    aux3.create(buttons[8].buttonarea.x + buttons[8].buttonarea.width + 1, panelArea.y + 290,70,50, "position6", false,"Pos 6");
    buttons.push_back(aux3);//9

    aux3.create(buttons[4].buttonarea.x , panelArea.y + 350, 70,50, "position7", false,"Pos 7");
    buttons.push_back(aux3);//10
    aux3.create(buttons[5].buttonarea.x , panelArea.y + 350, 70,50, "position8", false,"Pos 8");
    buttons.push_back(aux3);//11
    aux3.create(buttons[6].buttonarea.x , panelArea.y + 350, 70,50, "position9", false,"Pos 9");
    buttons.push_back(aux3);//12

    aux3.create(buttons[4].buttonarea.x , panelArea.y + 410, 70,50, "savePosition", true, "Save\nPosition");
    buttons.push_back(aux3);//13

    aux3.create(buttons[9].buttonarea.x,panelArea.y + 410, 70,50,"setPosition", true, "Set\nPosition");
    buttons.push_back(aux3);//14

    aux3.create(panelArea.x + 440, panelArea.y + 230, 70,50,"vMax", true, " Max\nSpeed");
    aux3.recordedValue = 80;
    vMax = 80;
    buttons.push_back(aux3);//15
    aux3.create(buttons[15].buttonarea.x , panelArea.y + 290, 70,50,"vMin", true, " Min\nSpeed");
    aux3.recordedValue = 30;

    vMin = 30;
    buttons.push_back(aux3);//16
    aux3.create(panelArea.x + 440, panelArea.y + 350, 70,50,"maxVelocity", true, " Go\nSpeed");
    aux3.recordedValue = 30;
    goSpeed = 30;
    buttons.push_back(aux3);//17

    aux3.create(panelArea.x + 20, panelArea.y + panelArea.height - 55,80,50,"activatetouchpad",true,"Activate\n Touch");
    aux3.setActive(false);
    buttons.push_back(aux3);//18

    aux3.create(panelArea.x + panelArea.width - 220,panelArea.y + panelArea.height - 55, 80, 50, "omnicam1stop", false, " STOP" );
    aux3.setActive(false);
    buttons.push_back(aux3);

    label aux;
    aux.create(5 + panelArea.x, 5 + panelArea.y, "isConnected", "State", "Not Connected");
    labels.push_back(aux);//0
    aux.create(120 + panelArea.x, 5 + panelArea.y, "omnicam1motorbattery", "Motor", 0);
    labels.push_back(aux);//1
    aux.create(180 + panelArea.x, 5 + panelArea.y, "omnicam1electbattery", "Electronics", 0);
    labels.push_back(aux);//2
    aux.create(280 + panelArea.x, 5 + panelArea.y, "omnicam1temp1", "M1Temperature", 0);
    labels.push_back(aux);//3
    aux.create(390 + panelArea.x, 5 + panelArea.y, "omnicam1temp2", "M2Temperature", 0);
    labels.push_back(aux);//4

    keypad.create(0,0, true);

    keypad.setActive(false);
    toSend = false;

    posGraph.create(panelArea.x + 10, panelArea.y + 110, "omnicam1graph");

    logo.loadImage("logo.png");
    logo.resize(logo.width/2.0, logo.height/2.0);

    active.loadImage("topactive.png");
    inactive.loadImage("topinactive.png");
    actualspeed = 0;
    actuator = 0;
    block = true;
}

void Panel1::dealwithtouches(int x, int y, int button)
{
    if(buttons[18].isClicked(x,y) && button == 1)
    {
        if(!buttons[18].isActive())
        {
            keypad.setActive(true);
            keypad.changePos(buttons[18].buttonarea.x + buttons[18].buttonarea.width, panelArea.y + panelArea.height - 210);
        }
        else
        {
            keypad.isPass = true;
            keypad.npresses = 0;
            block = true;
            buttons[3].setActive(false);
            toSend = true;
            msg = "omnicam1releasecontrol";
        }


        buttons[18].setActive(!buttons[18].isActive());
    }

    if(buttons[18].isActive() && keypad.isActive() && block)
    {
        if(keypad.isActive())
        {
            keypad.dealwithtouches(x,y);
            if(keypad.getValue() == 24245)
            {
                keypad.isPass = false;
                keypad.setActive(false);
                keypad.value = 0;
                block = false;
            }
        }
    }

    if(isClicked(x,y) && buttons[18].isActive() && !buttons[3].isClicked(x,y) && !block)
    {
        if(!keypad.isActive() && button == 1 )
        {
            Panel::dealwithtouches(x, y, button);

            if(buttons[0].isClicked(x, y))
            {
                sliders[0].update(0);
                toSend = true;
                int value = -posGraph.lenght/2.0f + posGraph.offset;
                msg = "omnicam1goto";
                msg += "_" + ofToString(value);
            }
            else if(buttons[1].isClicked(x, y))
            {
                sliders[0].update(0.5);
                toSend = true;
                int value = 0;
                msg = "omnicam1goto";
                msg += "_" + ofToString(value);
            }
            else if(buttons[2].isClicked(x, y))
            {
                sliders[0].update(1);
                toSend = true;
                int value = posGraph.lenght/2.0f + posGraph.offset;
                msg = "omnicam1goto";
                msg += "_" + ofToString(value);
            }

            if(sliders[0].isClicked(x, y))
            {
                int value = sliders[0].current * posGraph.lenght - posGraph.lenght/2;
                sliders[0].position = value + posGraph.offset;
                if(!buttons[13].isActive())
                {
                    toSend = true;
                    msg = "omnicam1goto";
                    msg += "_" + ofToString(value + posGraph.offset);

                }
            }

            if(buttons[13].isActive())
            {
                for (int i = 4; i < 13; i++)
                {
                    if(buttons[i].isClicked(x, y))
                    {
                        int value = sliders[0].current * posGraph.lenght - posGraph.lenght/2;
                        value += posGraph.offset;
                        buttons[i].recordedValue = value;
                        buttons[13].setActive(false);
                        sliders[0].current = sliders[0].real;
                    }
                }
            }
            else
            {
                for (int i = 4; i < 13; i++)
                {
                    if(buttons[i].isClicked(x, y))
                    {
                        toSend = true;
                        sliders[0].current = buttons[i].recordedValue;
                        int value = buttons[i].recordedValue;
                        msg = "omnicam1goto";
                        msg += "_" + ofToString(value);
                    }
                }
            }
        }

        if(buttons[14].isActive() & button == 1)
        {
            keypad.changePos(buttons[14].buttonarea.x + buttons[14].buttonarea.width, panelArea.y + panelArea.height - keypad.panelArea.height);
            keypad.setActive(true);
            keypad.dealwithtouches(x, y);
            keypad.buttons[15].setVisible(true);
            for (int i = 4; i < 13; i++)
            {
                if(buttons[i].isClicked(x, y))
                {
                    if(keypad.getValue() == 0)
                        buttons[i].recordedValue = actualposition;
                    else
                        buttons[i].recordedValue = keypad.getValue();

                    buttons[14].setActive(false);
                    keypad.buttons[15].setVisible(false);
                    keypad.setActive(false);
                    keypad.value =0;
                }
            }

            if(keypad.buttons[15].isClicked(x,y))
            {
                toSend = true;
                sliders[0].current = (keypad.getValue()+posGraph.offset+posGraph.lenght/2.0f)/posGraph.lenght;
                msg = "omnicam1goto";
                msg += "_" + ofToString(keypad.getValue());
                keypad.setActive(false);
                buttons[14].setActive(false);
                keypad.buttons[15].setVisible(false);
                keypad.value = 0;
            }

            if(keypad.exit)
            {
                keypad.setActive(false);
                buttons[14].setActive(false);
                keypad.buttons[15].setVisible(false);
                keypad.value = 0;
            }
        }

        if(buttons[19].isClicked(x,y))
        {
            toSend = true;
            msg = "omnicam1stop";
        }

        if(buttons[15].isActive() && button == 1)
        {
            keypad.changePos(buttons[15].buttonarea.x + buttons[15].buttonarea.width, panelArea.y + panelArea.height - keypad.panelArea.height);
            keypad.setActive(true);
            keypad.dealwithtouches(x, y);
            if(keypad.getValue() > 1)
                buttons[15].recordedValue = keypad.getValue();

            if(keypad.exit)
            {
                if(buttons[15].recordedValue > 50)
                    buttons[15].recordedValue = 50;
                keypad.setActive(false);
                buttons[15].setActive(false);
                vMax = buttons[15].recordedValue;
                keypad.value = 0;
                toSend = true;
                msg = "omnicam1vmax_"+ofToString(vMax);
                posGraph.updateMax(vMax);
            }
        }

        if(buttons[16].isActive() && button == 1)
        {
            keypad.changePos(buttons[16].buttonarea.x + buttons[16].buttonarea.width, panelArea.y + panelArea.height - keypad.panelArea.height);
            keypad.setActive(true);
            keypad.dealwithtouches(x, y);
            if(keypad.getValue() > 1)
                buttons[16].recordedValue = keypad.getValue();

            if(keypad.exit)
            {
                if(buttons[16].recordedValue > 10)
                    buttons[16].recordedValue = 10;

                keypad.setActive(false);
                buttons[16].setActive(false);
                vMin = buttons[16].recordedValue;
                keypad.value = 0;
                toSend = true;
                msg = "omnicam1vmin_"+ofToString(vMin);
                posGraph.updateMin(vMin);
            }
        }

        if(buttons[17].isActive() && button == 1)
        {
            keypad.changePos(buttons[17].buttonarea.x + buttons[14].buttonarea.width, panelArea.y + panelArea.height - keypad.panelArea.height);
            keypad.setActive(true);
            keypad.dealwithtouches(x, y);
            if(keypad.getValue() > 1)
                buttons[17].recordedValue = keypad.getValue();

            if(keypad.exit)
            {
                if(buttons[17].recordedValue > vMax)
                    buttons[17].recordedValue = vMax;

                keypad.setActive(false);
                buttons[17].setActive(false);
                goSpeed = buttons[17].recordedValue;
                keypad.value = 0;
                toSend = true;
                msg = "omnicam1posvelmax_"+ofToString(goSpeed);
            }
        }

        if(button == 2)
        {
            if(sliders[0].isClicked(x, y))
            {
                sliders[0].clicked(x,y);
                if(!buttons[13].isActive())
                {
                    toSend = true;
                    int value = sliders[0].current * posGraph.lenght - posGraph.lenght/2;
                    msg = "omnicam1goto";
                    msg += "_" + ofToString(value);
                }
            }
        }
    }
}

void Panel1::update(string message)
{
    vector <string> msg1 = ofSplitString(message, " ");
    for (int j=0; j < msg1.size(); j++)
    {
        vector<string> msgs = ofSplitString(msg1[j], "_");

        for (int i = 0; i < buttons.size(); i++)
        {
            if(buttons[i].ID == msgs[0])
                buttons[i].update(ofToInt(msgs[1]));
        }

        for (int i = 0; i < sliders.size(); i++)
        {
            if(sliders[i].ID == msgs[0])
            {
                sliders[i].updatereal((ofToFloat(msgs[1])+1000.0f)/2000.0f);
            }
        }

        for (int i = 0; i < labels.size(); i++)
        {
            if(labels[i].ID == msgs[0])
            {
                labels[i].info = msgs[1];
            }
        }

        if(msgs[0] == "omnicam1driver")
        {
            if(msgs[1] == "OFF")
            {
                buttons[3].setActive(false);

            }
        }

        if(msgs[0] == "omnicam1update")
        {
            actualspeed = ofToFloat(msgs[2]);
            actualposition = ofToFloat(msgs[1]);
            posGraph.updatecurrent(((ofToFloat(msgs[1]) - posGraph.offset) + posGraph.lenght/2.0f)/posGraph.lenght, ofToFloat(msgs[2]));
            sliders[0].updatereal(((ofToFloat(msgs[1]) - posGraph.offset) + posGraph.lenght/2.0f)/posGraph.lenght);
        }

        if(msgs[0] == "omnicam1neg3brake")
        {
            float value = ofToFloat(msgs[1]);
            posGraph.updateFarLeft(value);
        }

        if(msgs[0] == "omnicam1neg2brake")
        {
            float value = ofToFloat(msgs[1]);
            posGraph.updateNearLeft(value);
        }

        if(msgs[0] == "omnicam1pos2brake")
        {
            posGraph.updateNearRight(ofToFloat(msgs[1]));
        }

        if(msgs[0] == "omnicam1pos3brake")
        {
            posGraph.updateFarRight(ofToFloat(msgs[1]));
            posGraph.lenght = float((posGraph.points[4].x+50) - (posGraph.points[1].x-50));
            sliders[0].size = posGraph.lenght;
            posGraph.offset = (int)((float)(((posGraph.points[4].x+50) + (posGraph.points[1].x-50))/2));
            sliders[0].offset = posGraph.offset;
        }

        if(msgs[0] == "omnicam1actuator")
        {
            actuator = ofToFloat(msgs[1]);
        }

        if(msgs[0] ==  "omnicam1servopower")
        {
            if(ofToInt(msgs[1]) == 0)
                servopower = false;
            else
                servopower = true;
        }

        if(msgs[0] == "omnicam1camerapower")
        {
            if(ofToInt(msgs[1]) == 0)
                camerapower = false;
            else
                camerapower = true;
        }

        if(msgs[0] == "omnicam1tally")
        {
            if(ofToInt(msgs[1]) == 50 || ofToInt(msgs[1]) == 0)
                tally = false;
            else
                tally = true;
        }
    }
}

void Panel1::draw()
{

    Panel::draw();
    if(isActive())
        posGraph.draw();

    ofSetColor(255,255,255);
    if(isActive())
    {
        for (int i = 4; i < 13; i++)
        {
            float value =  (buttons[i].recordedValue-posGraph.offset+posGraph.lenght/2)/ posGraph.lenght;
            ofDrawBitmapString(ofToString(i-3), sliders[0].sliderArea.x + value * sliders[0].sliderArea. width - 2.5, sliders[0].sliderArea.y + 12);
        }

        float value =  (posGraph.lenght/2 - posGraph.offset)/ posGraph.lenght;
        ofCircle(sliders[0].sliderArea.x + value * sliders[0].sliderArea. width,   sliders[0].sliderArea.y + sliders[0].sliderArea.height - 41,5);
        for(int i = 0; i < buttons.size(); i++)
        {
            if(buttons[i].isActive())
                active.draw(buttons[i].buttonarea);
            else
                inactive.draw(buttons[i].buttonarea);

            ofSetColor(255,255,255);
            if(i < 3 || (i > 3 && i < 13))
                info.drawString(buttons[i].message, buttons[i].buttonarea.x + 15, buttons[i].buttonarea.y + 28 );
            else if(i == 19)
                info.drawString(buttons[i].message, buttons[i].buttonarea.x + 15, buttons[i].buttonarea.y + 30 );
            else
                info.drawString(buttons[i].message, buttons[i].buttonarea.x + 15, buttons[i].buttonarea.y + 24 );
        }

        //PAINEL DE LEITURAS
        int displayx = 20;
        int displayy = panelArea.y + panelArea.height - 250;
        ofSetColor(255,255,255);
        degrade2.draw(displayx, displayy, 400, 190);
        ofSetColor(0,0,0);
        roundedRect(displayx - 2, displayy - 2, 405, 194);
        ofSetColor(0, 166, 255);
        roundedRect(displayx, displayy, 400, 190);

        ofSetColor(255, 255, 255);
        top.drawString("Batteries", displayx + 60, displayy + 35);
        info.drawString("Motor", displayx + 40, displayy +55);
        info.drawString("Electronics", displayx + 120, displayy + 55);
        digital.drawString(labels[1].info, displayx + 50, displayy + 75);
        digital.drawString(labels[2].info, displayx + 140, displayy + 75);

        top.drawString("Temperature", displayx + 60, displayy + 120);
        info.drawString("Motor1", displayx + 30, displayy + 140);

        if(labels[4].info == "255")
        {
            // labels[4].info = "---";
        }
        else
        {
            digital.drawString(labels[4].info, displayx + 140, displayy + 160);
            info.drawString("Motor2", displayx + 120, displayy + 140);
        }

        if(labels[3].info == "255")
        {
            labels[3].info = "---";
        }



        digital.drawString(labels[3].info, displayx + 50, displayy + 160);



        top.drawString("Speed" , displayx + 260, displayy + 35);

        info.drawString("Max", displayx + 230, displayy + 55);
        info.drawString("Min", displayx + 280,  displayy + 55);
        info.drawString("GoTo", displayx + 315,  displayy + 55);

        if(vMax > 20)
            ofSetColor(255,0,0);

        digital.drawString(ofToString(vMax), displayx + 237,  displayy + 75);
        ofSetColor(255,255,255);
        digital.drawString(ofToString(vMin), displayx + 280,  displayy + 75);
        if(goSpeed > 20)
            ofSetColor(255,0,0);

        digital.drawString(ofToString(goSpeed), displayx + 315,  displayy + 75);

        ofSetColor(255,255,255);
        top.drawString("Real", displayx + 240, displayy + 100);
        top.drawString("Actuate", displayx + 300, displayy + 100);
        digital.drawString(ofToString(abs((int)(float(actualspeed/vMax)*100))) + "%",displayx + 247, displayy + 120);
        int value2;
        value2 = abs((int)(float(actuator/vMax)*100));
        if(value2 < 0 || value2 > 100)
            value2 = 0;

        digital.drawString(ofToString(value2) + "%",displayx + 310, displayy + 120);
        top.drawString("Current Position", displayx + 235, displayy + 145);

        digital.drawString(ofToString(actualposition),displayx + 280, displayy + 170);


    }
    ofFill();

    info.drawString("Servo\nPower", buttons[0].buttonarea.x - 202, buttons[0].buttonarea.y +10);
    info.drawString("Camera\n Power", buttons[0].buttonarea.x - 120, buttons[0].buttonarea.y +10);
    info.drawString("Tally", buttons[0].buttonarea.x - 160, buttons[0].buttonarea.y +75);

    ofSetCircleResolution(20);
    if(camerapower)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);



    ofCircle(buttons[0].buttonarea.x - 180, buttons[0].buttonarea.y + 37, 10);

    if(servopower)
        ofSetColor(0,255,0);
    else
        ofSetColor(255,0,0);

    ofCircle(buttons[0].buttonarea.x - 100, buttons[0].buttonarea.y + 37, 10);

    if(tally)
        ofSetColor(255,0,0);
    else
        ofSetColor(64,64,64);

    ofCircle(buttons[0].buttonarea.x - 140, buttons[0].buttonarea.y + 110, 25);

    keypad.draw();


    ofSetColor(255,255,255);
    // logo.draw(ofGetWidth() - logo.width*0.8, ofGetHeight() - logo.height);
}

