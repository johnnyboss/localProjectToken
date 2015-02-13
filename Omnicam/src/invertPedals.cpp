//
//  Panel2.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "invertPedals.h"

void invertPedals::create(int startx, int starty)
{
    setup(startx, starty, 180, 70);

    button aux;

    aux.create(panelArea.x + 10, panelArea.y + 10,80,50, "INVERT1", true, "Invert\nPedal C");
    buttons.push_back(aux);

    aux.create(panelArea.x + 10 + buttons[0].buttonarea.width + 1, panelArea.y + 10,80,50, "INVERT2", true, "Invert\nPedal I");
    buttons.push_back(aux);
    setActive(false);

    active.loadImage("topactive.png");
    inactive.loadImage("topinactive.png");

    info.loadFont("micross.ttf", 10);
}

void invertPedals::setNumConsoles(int _nconsoles)
{
    nconsoles = _nconsoles;
    if(nconsoles == 1)
        buttons[1].setVisible(false);
}

void invertPedals::dealwithtouches(int x, int y, int button)
{

    Panel::dealwithtouches(x, y, button);
    if(isActive())
    {
    if(buttons[0].isClicked(x, y) && buttons[0].isActive())
    {
        toSend = true;
        msg = "omnicam1pedalinvert_ON";
    }
    else if(buttons[0].isClicked(x, y) && !buttons[0].isActive())
    {
        toSend = true;
        msg = "omnicam1pedalinvert_OFF";
    }

    if(buttons[1].isClicked(x, y) && buttons[1].isActive())
    {
        toSend = true;
        msg = "omnicam2pedalinvert_ON";
    }
    else if(buttons[1].isClicked(x, y) && !buttons[1].isActive())
    {
        toSend = true;
        msg = "omnicam2pedalinvert_OFF";
    }
    }
}

void invertPedals::update(string message)
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



void invertPedals::draw()
{
    Panel::draw();
    if(isActive())
    {
        ofSetColor(255,255,255);

        for(int i = 0; i < buttons.size(); i++)
        {
            if(buttons[i].isActive())
                active.draw(buttons[i].buttonarea);
            else
                inactive.draw(buttons[i].buttonarea);

            info.drawString(buttons[i].message, buttons[i].buttonarea.x + 15, buttons[i].buttonarea.y + 23 );
        }
    }

}

