//
//  Panel2.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "invertServos.h"

void invertServos::create(int startx, int starty)
{
    digital.loadFont("micross.ttf", 10);
    setup(startx, starty, 325, 150);

    button aux;

    aux.create(panelArea.x + 10, panelArea.y + 15,75,50, "PAN1", true, "Pan C");
    buttons.push_back(aux);//0
    aux.create(buttons[0].buttonarea.x + buttons[0].buttonarea.width + 1, panelArea.y + 15,75,50, "TILT1", true, "Tilt C");
    buttons.push_back(aux);//1
    aux.create(buttons[1].buttonarea.x + buttons[1].buttonarea.width + 1, panelArea.y + 15,75,50, "ROLL1", true, "Roll C");
    aux.setVisible(false);
    buttons.push_back(aux);//2
    aux.setVisible(true);
    aux.create(buttons[1].buttonarea.x + buttons[1].buttonarea.width + 1, panelArea.y + 15,80,50, "ZOOM1", true, "Zoom C");
    buttons.push_back(aux);//3
    aux.create(buttons[3].buttonarea.x + buttons[3].buttonarea.width + 1, panelArea.y + 15,80,50, "FOCUS1", true, "Focus C");
    buttons.push_back(aux);//4
    aux.create(panelArea.x +10  , panelArea.y + 85,75,50, "PAN2", true, "Pan I");
    buttons.push_back(aux);//5
    aux.create(buttons[5].buttonarea.x + buttons[5].buttonarea.width + 1, panelArea.y + 85,75,50, "TILT2", true, "Tilt I");
    buttons.push_back(aux);//6
    aux.create(buttons[6].buttonarea.x + buttons[6].buttonarea.width + 1, panelArea.y + 85,75,50, "ROLL2", true, "Roll I");
    aux.setVisible(false);
    buttons.push_back(aux);//7
    aux.setVisible(true);
    aux.create(buttons[6].buttonarea.x + buttons[6].buttonarea.width + 1, panelArea.y + 85,80,50, "ZOOM2", true, "Zoom I");
    buttons.push_back(aux);//8
    aux.create(buttons[8].buttonarea.x + buttons[8].buttonarea.width + 1, panelArea.y + 85,80,50, "FOCUS2", true, "Focus I");
    buttons.push_back(aux);//9

    setActive(false);
    active.loadImage("topactive.png");
    inactive.loadImage("topinactive.png");

}

void invertServos::setNumConsoles(int _nconsoles)
{
    nconsoles = _nconsoles;
    if(nconsoles == 1)
    {
        for (int i = 5; i < 10; i++)
        {
            buttons[i].setVisible(false);
        }
    }
}

void invertServos::dealwithtouches(int x, int y, int button)
{

    Panel::dealwithtouches(x, y, button);
    if(isActive())
    {
        for(int i = 0; i < 5; i++)
        {
            if(buttons[i].isClicked(x,y) && buttons[i].isActive() && buttons[i].isVisible())
            {
                toSend = true;
                msg = "servo1servoinvert_"+ofToString(i+1)+"_ON";
            }
            else if(buttons[i].isClicked(x,y) && !buttons[i].isActive())
            {
                toSend = true;
                msg = "servo1servoinvert_"+ofToString(i+1)+"_OFF";
            }
        }

        for(int i = 5; i < 10; i++)
        {
            if(buttons[i].isClicked(x,y) && buttons[i].isActive() && buttons[i].isVisible())
            {
                toSend = true;
                msg = "servo2servoinvert_"+ofToString(i-4)+"_ON";
            }
            else if(buttons[i].isClicked(x,y) && !buttons[i].isActive())
            {
                toSend = true;
                msg = "servo2servoinvert_"+ofToString(i-4)+"_OFF";
            }
        }
    }
}

void invertServos::update(string message)
{
    vector <string> msg = ofSplitString(message, " ");
    for (int j=0; j < msg.size(); j++)
    {
        vector<string> msgs = ofSplitString(msg[j], "_");

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
    }
}


void invertServos::draw()
{
    Panel::draw();
    if(isActive() )
    {
        ofSetColor(255,255,255);

        for(int i = 0; i < buttons.size(); i++)
        {
            if(buttons[i].isVisible())
            {
                if(buttons[i].isActive() )
                    active.draw(buttons[i].buttonarea);
                else
                    inactive.draw(buttons[i].buttonarea);

                digital.drawString(buttons[i].message, buttons[i].buttonarea.x + 15, buttons[i].buttonarea.y + 23 );
            }
        }
    }
}

