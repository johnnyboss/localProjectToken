//
//  servoTrim.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "servoTrim.h"

void servoTrim::create(int startx, int starty)
{
    mono.loadFont("micross.ttf", 10);
    arrow.loadFont("micross.ttf", 14);

    setup(startx, starty, 445, 170);

    button aux3;
    aux3.create(panelArea.x + 10 , panelArea.y + 20,70,50, "_1", true, " Pan");
    buttons.push_back(aux3);//0
    aux3.create(buttons[0].buttonarea.x + buttons[0].buttonarea.width +1 , panelArea.y + 20,70,50, "_2", true," Tilt");
    buttons.push_back(aux3);//1
    aux3.create(buttons[1].buttonarea.x + buttons[1].buttonarea.width +1, panelArea.y + 20,70,50, "_3", true," Roll");
    buttons.push_back(aux3);//2

    aux3.create(buttons[2].buttonarea.x + buttons[2].buttonarea.width +1, panelArea.y + 20,70,50, "_4", true,"Zoom");
    buttons.push_back(aux3);//3

    aux3.create(buttons[3].buttonarea.x + buttons[3].buttonarea.width+1, panelArea.y + 20,70,50, "_5", true,"Focus");
    buttons.push_back(aux3);//4

    aux3.create(panelArea.x + 10, panelArea.y + 100,70,50, "_D_16", false);
    aux3.setVisible(false);
    buttons.push_back(aux3);//5

    aux3.create(buttons[5].buttonarea.x + buttons[5].buttonarea.width +1, panelArea.y + 100,70,50, "_D_4", false);
    buttons.push_back(aux3);//6
    aux3.setVisible(false);
    aux3.create(buttons[6].buttonarea.x + buttons[6].buttonarea.width+1, panelArea.y + 100,70,50, "_D_1", false);
    buttons.push_back(aux3);//7

    aux3.create(buttons[7].buttonarea.x + buttons[7].buttonarea.width+1, panelArea.y + 100,70,50, "_U_1", false);
    buttons.push_back(aux3);//8

    aux3.create(buttons[8].buttonarea.x + buttons[8].buttonarea.width+1, panelArea.y + 100,70,50, "_U_4", false);
    aux3.setVisible(false);
    buttons.push_back(aux3);//9

    aux3.create(buttons[9].buttonarea.x + buttons[9].buttonarea.width+1, panelArea.y + 100,70,50, "_U_16", false);
    aux3.setVisible(false);
    buttons.push_back(aux3);//10
    aux3.create(buttons[4].buttonarea.x + buttons[4].buttonarea.width+1, panelArea.y + 20,70,50, "exit", false," Exit");
    buttons.push_back(aux3);//11

    active1.loadImage("topactive.png");
    inactive1.loadImage("topinactive.png");

    active2.loadImage("button_active.png");
    inactive2.loadImage("button_back.png");

    keypad.create(0,0, true);

    keypad.setActive(false);

}

void servoTrim::dealwithtouches(int x, int y, int button)
{
    if(isActive())
    {
        if(keypad.value == 10203010)
        {
            Panel::dealwithtouches(x, y, button);
            for (int i = 0; i < 5; i++)
            {
                if(buttons[i].isClicked(x, y))
                {
                    buttons[active].setActive(false);
                    active = i;
                    buttons[active].setActive(true);
                    msg1 = buttons[active].ID;
                }
            }

            for (int i = 5; i < 11; i++)
            {
                if(buttons[i].isClicked(x, y))
                {
                    toSend = true;
                    msg2 = buttons[i].ID;
                    break;
                }
                else
                {
                    toSend = false;
                    msg2 = "";
                }
            }

            if(buttons[11].isClicked(x, y))
            {
                keypad.value = 0;
                setActive(false);
                keypad.npresses = 0;
            }
        }
        else
        {
            keypad.dealwithtouches(x, y);
            if(keypad.exit)
            {
                keypad.setActive(false);
            }
        }
    }
}

void servoTrim::activateKeypad()
{
    keypad.value = 0;
     keypad.changePos( panelArea.x, panelArea.y);
    keypad.setActive(true);
}

void servoTrim::update(string message)
{
    vector<string> msgs = ofSplitString(message, " ");

    for (int i = 0; i < buttons.size(); i++)
    {
        if(buttons[i].ID == msgs[0])
            buttons[i].update(ofToInt(msgs[1]));
    }

    for (int i = 0; i < sliders.size(); i++)
    {
        if(sliders[i].ID == msgs[0])
        {
            sliders[i].update(ofToFloat(msgs[1]));
        }
    }
}

void servoTrim::initialize(string message)
{

    vector<string> msgs = ofSplitString(message, " ");
    for (int i = 1; i < msgs.size(); i+=2)
    {
        for (int j = 0; j < buttons.size(); j++)
        {
            if(buttons[j].ID == msgs[i])
                buttons[j].update(ofToInt(msgs[i+1]));
        }

        for (int j = 0; j < sliders.size(); j++)
        {
            if(sliders[j].ID == msgs[i])
                sliders[j].update(ofToFloat(msgs[i+1]));
        }

        for (int j = 0; j < labels.size(); j++)
        {
            if(labels[j].ID == msgs[i])
                labels[j].changeinfo(msgs[i+1]);
        }
    }
}

void servoTrim::draw()
{
    if(isActive())
    {
        if(keypad.value == 10203010)
        {
            ofSetColor(0,168,255);
            ofFill();
            roundedRect(panelArea);
            Panel::draw();
            ofSetLineWidth(2);

            ofSetColor(255,255,255);
            for(int i = 0; i < buttons.size(); i++)
            {
                if(buttons[i].isVisible() && isActive() && (i < 5 || i > 10))
                {
                    if(buttons[i].isActive())
                        active1.draw(buttons[i].buttonarea);
                    else
                        inactive1.draw(buttons[i].buttonarea);

                    ofSetColor(255,255,255);
                    mono.drawString(buttons[i].message, buttons[i].buttonarea.x + 15, buttons[i].buttonarea.y + 30 );
                }
                else if(i > 4 && i < 11 && isActive())
                {
                    if(buttons[i].isActive())
                        active2.draw(buttons[i].buttonarea);
                    else
                        inactive2.draw(buttons[i].buttonarea);

                    ofSetColor(255,255,255);

                }
            }
            string s;
           // s = "<<<";
            //arrow.drawString(s, buttons[5].buttonarea.x + buttons[5].buttonarea.width/2 - 14, buttons[5].buttonarea.y + buttons[5].buttonarea.height/2 + 5);

            //s = "<<";
            //arrow.drawString(s, buttons[6].buttonarea.x + buttons[6].buttonarea.width/2 - 10, buttons[6].buttonarea.y + buttons[6].buttonarea.height/2 + 5);

            s = "<";
            arrow.drawString(s, buttons[7].buttonarea.x + buttons[7].buttonarea.width/2 - 6, buttons[7].buttonarea.y + buttons[7].buttonarea.height/2 + 5);

            s = ">";
            arrow.drawString(s, buttons[8].buttonarea.x + buttons[8].buttonarea.width/2 - 4, buttons[8].buttonarea.y + buttons[8].buttonarea.height/2 + 5);

           // s = ">>";
           // arrow.drawString(s, buttons[9].buttonarea.x + buttons[9].buttonarea.width/2 - 10, buttons[9].buttonarea.y + buttons[9].buttonarea.height/2 + 5);

           // s = ">>>";
            //arrow.drawString(s, buttons[10].buttonarea.x + buttons[10].buttonarea.width/2 - 14, buttons[10].buttonarea.y + buttons[10].buttonarea.height/2 + 5);

            ofSetLineWidth(1);
        }
        else
        {
            keypad.draw();
        }
    }
}

void servoTrim::released()
{
    Panel::released();
    keypad.released();
}
