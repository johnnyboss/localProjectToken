//
//  joystickCalib.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "joystickCalib.h"

void joystickCalib::create(int startx, int starty, int i)
{
    ID = i;
    setup(startx, starty, 500, 350);
    info.loadFont("micross.ttf", 10);

    button aux3;
    aux3.create(panelArea.x + 5 , panelArea.y+10, 80, 50,"ok", false, " Next");
    aux3.setVisible(false);
    buttons.push_back(aux3);//0
    aux3.create(panelArea.x +5, panelArea.y + 10, 80, 50,"save", false, " Save");
    aux3.setVisible(false);
    buttons.push_back(aux3);//1
    aux3.create(panelArea.x +5, panelArea.y + 75, 80, 50,"cancel", false, "Cancel");
    buttons.push_back(aux3);//2
    aux3.create(panelArea.x + 5, panelArea.y + 10, 80, 50,"start", false, " Start");
    buttons.push_back(aux3);//3
    aux3.create(panelArea.x + 5, panelArea.y + 10, 80, 50,"quit", false, " Quit");
    aux3.setVisible(false);//4
    buttons.push_back(aux3);
    step = 0;
    for (int i = 0; i < 6; i++)
    {
        values.push_back(0);
    }

    active.loadImage("topactive.png");
    inactive.loadImage("topinactive.png");

    keypad.create(0,0, true);

    keypad.setActive(false);

}

void joystickCalib::dealwithtouches(int x, int y, int button)
{
    if(isActive())
    {
        if(keypad.value == 102040600)
        {
            Panel::dealwithtouches(x, y, button);
            if(buttons[0].isClicked(x, y))
            {
                msg = "joystick" + ofToString(ID);
                msg += "trim_" + ofToString(step);
                toSend = true;
                step++;
                if(step == 13)
                {
                    buttons[0].setVisible(false);
                    buttons[1].setVisible(true);
                }
            }
            else if (buttons[1].isClicked(x, y))
            {
                toSend = true;
                msg = "joystick" + ofToString(ID) + "trim_save";
                step++;
                buttons[1].setVisible(false);
                buttons[4].setVisible(true);
            }
            else if(buttons[2].isClicked(x, y) || buttons[4].isClicked(x,y))
            {
                setActive(false);
                keypad.npresses = 0;
                keypad.value = 0;
            }
            else if (buttons[3].isClicked(x, y))
            {
                msg = "joystick" + ofToString(ID) + "trim_"+ofToString(step);
                buttons[0].setVisible(true);
                buttons[3].setVisible(false);
                toSend = true;
                step++;
            }
            else if(buttons[4].isClicked(x,y))
            {
                step = 0;
                keypad.npresses = 0;
            }
        }
        else
        {
            keypad.dealwithtouches(x, y);
            if(keypad.exit)
            {
                keypad.setActive(false);
                keypad.isPass = true;
            }
        }
    }
}

void joystickCalib::activateKeypad()
{
    keypad.value = 0;
    keypad.changePos( panelArea.x, panelArea.y);
    keypad.setActive(true);
}

void joystickCalib::update(string message)
{
    vector<string> msgs = ofSplitString(message, "_");
    if(msgs[0] == "joystick" + ofToString(ID) + "trm")
    {
        for (int i = 0; i < 6; i++)
        {
            values[i] = ofToInt(msgs[i+1]);
        }
    }

}

void joystickCalib::draw()
{
    if(isActive())
    {
        if(keypad.value == 102040600)
        {
            ofSetColor(6, 61,2);
            ofFill();
            roundedRect(panelArea);
            Panel::draw();

            if(isActive())
            {
                ofSetColor(255,255,255);

                for(int i = 0; i < buttons.size(); i++)
                {
                    if(buttons[i].isVisible())
                    {
                        if(buttons[i].isActive())
                            active.draw(buttons[i].buttonarea);
                        else
                            inactive.draw(buttons[i].buttonarea);

                        info.drawString(buttons[i].message, buttons[i].buttonarea.x + 15, buttons[i].buttonarea.y + 23 );
                    }
                }
            }

            int startx = panelArea.x + 95;
            int starty = panelArea.y + 25;
            if(step == 0)
                info.drawString("Place all the actuadors in the middle \nand press start", startx, starty);


            if(step == 1)
            {
                if(values[0] < 30000)
                    ofSetColor(255,0,0);
                else if(values[0] < 50000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[0]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Maximum value for the pedals", startx, starty);
            }
            else if(step == 2)
            {
                if(values[0] > 35000)
                    ofSetColor(255,0,0);
                else if(values[0] > 15000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[0]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Minimum value for the PEDALS",startx, starty);
            }
            else if(step == 3)
            {
                if(values[1] < 30000)
                    ofSetColor(255,0,0);
                else if(values[1] < 50000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[1]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Maximum value for the PAN", startx, starty);
            }
            else if(step == 4)
            {
                if(values[1] > 35000)
                    ofSetColor(255,0,0);
                else if(values[1] > 15000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[1]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Minimum value for the PAN", startx, starty);
            }
            else if(step == 5)
            {
                if(values[2] < 30000)
                    ofSetColor(255,0,0);
                else if(values[2] < 50000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[2]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Maximum value for the TILT", startx, starty);
            }
            else if(step == 6)
            {
                if(values[2] > 35000)
                    ofSetColor(255,0,0);
                else if(values[2] > 15000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[2]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Minimum value for the TILT", startx, starty);

            }
            else if(step == 7)
            {
                if(values[3] < 30000)
                    ofSetColor(255,0,0);
                else if(values[3] < 50000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[3]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Maximum value for the ROLL", startx, starty);

            }
            else if(step == 8)
            {
                if(values[3] > 35000)
                    ofSetColor(255,0,0);
                else if(values[3] > 15000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[3]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Minimum value for the ROLL", startx, starty);
            }
            else if(step == 9)
            {
                if(values[4] < 30000)
                    ofSetColor(255,0,0);
                else if(values[4] < 50000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[4]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Maximum value for the ZOOM", startx, starty);
            }
            else if(step == 10)
            {
                if(values[4] > 35000)
                    ofSetColor(255,0,0);
                else if(values[4] > 15000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[4]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Minimum value for the ZOOM", startx, starty);
            }
            else if(step == 11)
            {
                if(values[5] < 30000)
                    ofSetColor(255,0,0);
                else if(values[5] < 50000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[5]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Maximum value for the FOCUS",startx, starty);
            }
            else if(step == 12)
            {
                if(values[5] > 35000)
                    ofSetColor(255,0,0);
                else if(values[5] > 15000)
                    ofSetColor(0,255,255);
                else
                    ofSetColor(0,255,0);

                info.drawString(ofToString(values[5]), startx, starty + 20);
                ofSetColor(255,255,255);
                info.drawString("Minimum value for the FOCUS", startx, starty);
            }
            else if(step == 13)
            {
                ofSetColor(255,255,255);
                info.drawString("To save the calibration press 'SAVE'\notherwise press 'CANCEL'", startx, starty);
            }
            else if(step == 14)
            {
                ofSetColor(255,255,255);
                info.drawString("Calibration Saved", startx, starty);
            }
        }
         else
    {
        keypad.draw();
    }
    }

}

void joystickCalib::released()
{
    Panel::released();
    keypad.released();
}


