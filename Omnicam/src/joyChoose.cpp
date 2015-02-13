//
//  Panel2.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "top.h"

void topMenu::create()
{
    digital.loadFont("digital.ttf", 20);
    setup(0, 0, ofGetWidth(), 80);
    
    button aux;
    
    aux.create(0, 15, "LENGTH", true);
    buttons.push_back(aux);
    aux.create(70, 15, "INVERT PEDALS", true);
    buttons.push_back(aux);
    aux.create(140, 15, "INVERT CONSOLE", true);
    buttons.push_back(aux);
    aux.create(210, 15, "JOYSTICK CALIBRATION", true);
    buttons.push_back(aux);
    aux.create(280, 15, "SERVO TRIMMING", true);
    buttons.push_back(aux);
    aux.create(350, 15, "SET MAXIMUM SPEED", true);
    buttons.push_back(aux);
    aux.create(420, 15, "SET MINIMUM SPEED", true);
    buttons.push_back(aux);
    
}

void topMenu::dealwithtouches(int x, int y, int button)
{
        
    Panel::dealwithtouches(x, y, button);
    
    if(buttons[0].isClicked(x, y))
    {
    }
    
}

void topMenu::update(string message)
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



void topMenu::draw()
{
    Panel::draw();
    
}

