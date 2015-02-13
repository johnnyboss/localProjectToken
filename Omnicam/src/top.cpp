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
    digital.loadFont("micross.ttf", 20);
    setup(0, 28, ofGetWidth(), 50);

    button aux;
    aux.create(15, panelArea.y,75,50, "IPEDALS", true, "Invert\nPedals");
    buttons.push_back(aux);//0
    aux.create(buttons[0].buttonarea.x + buttons[0].buttonarea.width + 1, panelArea.y,75,50, "ICONSOLE", true, "Invert\nServos");
    buttons.push_back(aux);//1
    aux.create(buttons[1].buttonarea.x + buttons[1].buttonarea.width + 15, panelArea.y,100,50, "JOYCALIB1", true, "Joystick C\nCalibration");
    buttons.push_back(aux);//2
    aux.create(buttons[2].buttonarea.x + buttons[2].buttonarea.width + 1, panelArea.y, 100,50,"SERVOTRIM1", true, "Servo C\nTrimming");
    buttons.push_back(aux);//3
    aux.create(buttons[3].buttonarea.x + buttons[3].buttonarea.width + 15, panelArea.y,100,50, "JOYCALIB2", true, "Joystrick I\nCalibration");
    buttons.push_back(aux);//4
    aux.create(buttons[4].buttonarea.x + buttons[4].buttonarea.width + 1, panelArea.y,100,50, "SERVOTRIM2", true, "Servo I\nTrimming");
    buttons.push_back(aux);//5

    aux.create(buttons[5].buttonarea.x + buttons[5].buttonarea.width + 20, panelArea.y,100,50, "EXIT", true, "   EXIT");
    buttons.push_back(aux);//6

    pedals.create(buttons[0].buttonarea.x, buttons[0].buttonarea.y + 60);
    servos.create(buttons[1].buttonarea.x, buttons[1].buttonarea.y + 60);
    joycalib1.create(buttons[2].buttonarea.x, buttons[1].buttonarea.y + 60, 1);
    joycalib1.setActive(false);
    joycalib2.create(buttons[4].buttonarea.x, buttons[1].buttonarea.y + 60, 2);
    joycalib2.setActive(false);

    servotrim1.create(buttons[3].buttonarea.x , buttons[1].buttonarea.y + 60);
    servotrim1.setActive(false);
    servotrim2.create(buttons[5].buttonarea.x , buttons[1].buttonarea.y + 60);
    servotrim2.setActive(false);

    onExit.create(ofGetWidth()/2-90, ofGetHeight()/2 - 70);
    onExit.setActive(false);

    active.loadImage("topactive.png");
    inactive.loadImage("topinactive.png");

    info.loadFont("micross.ttf", 10);


}

void topMenu::setnumconsoles(int _nconsoles)
{
    nconsoles = _nconsoles;
    /* if(nconsoles == 1)
     {

         servos.setNumConsoles(1);
     }*/
}

void topMenu::setnumomnicams(int _nomnicams)
{
    nomnicams = _nomnicams;
    if(nomnicams == 1)
    {
        buttons[4].setVisible(false);
        buttons[5].setVisible(false);
        pedals.setNumConsoles(1);
    }
    else if(nomnicams == 2 && nconsoles == 1)
    {
        servos.setNumConsoles(2);
        buttons[4].setVisible(false);
        buttons[5].setVisible(false);
        pedals.setNumConsoles(2);
    }
}


bool topMenu::ismenuactive()
{
    if(joycalib1.isActive() || joycalib2.isActive() || servotrim1.isActive() || servotrim2.isActive() ||
            servos.isActive() || pedals.isActive() || onExit.isActive())
        return true;
    else
        return false;
}
void topMenu::dealwithtouches(int x, int y, int button)
{

    if(!ismenuactive())
    {
        Panel::dealwithtouches(x, y, button);
    }
    if(buttons[0].isClicked(x, y) && !pedals.isActive() && !ismenuactive())
    {
        pedals.setActive(true);
    }
    else if (buttons[0].isClicked(x, y) && pedals.isActive())
    {
        pedals.setActive(false);
        buttons[0].setActive(false);
    }

    if(buttons[1].isClicked(x, y) && !servos.isActive() && !ismenuactive())
    {
        servos.setActive(true);
    }
    else if (buttons[1].isClicked(x, y) && servos.isActive())
    {
        buttons[1].setActive(false);
        servos.setActive(false);
    }

    pedals.dealwithtouches(x, y, button);
    servos.dealwithtouches(x, y, button);

    if(pedals.toSend)
    {
        toSend = true;
        msg = pedals.msg;
        pedals.toSend = false;
    }

    if(servos.toSend)
    {
        toSend = true;
        msg = servos.msg;
        servos.toSend = false;
    }
    joycalib1.dealwithtouches(x, y, button);

    if(joycalib1.toSend && joycalib1.isActive())
    {
        toSend = true;
        msg = joycalib1.msg;
    }

    if(buttons[2].isActive() && buttons[2].isClicked(x, y) && !ismenuactive())
    {
        joycalib1.buttons[0].setVisible(false);
        joycalib1.buttons[1].setVisible(false);
        joycalib1.buttons[3].setVisible(true);

        joycalib1.setActive(true);
        joycalib1.activateKeypad();
        joycalib1.step = 0;
        toSend = true;
        msg = "joystick1trim_ON";
    }

    else if((buttons[2].isActive() && !joycalib1.isActive()) || (buttons[2].isClicked(x, y) && joycalib1.isActive()) )
    {
        toSend = true;
        msg = "joystick1trim_OFF";
        joycalib1.step = 0;
        buttons[2].setActive(false);
        joycalib1.buttons[3].setVisible(true);
        joycalib1.buttons[4].setVisible(false);
        joycalib1.setActive(false);
    }

    joycalib2.dealwithtouches(x, y, button);

    if(joycalib2.toSend && joycalib2.isActive())
    {
        toSend = true;
        msg = joycalib2.msg;
    }

    if(buttons[4].isActive() && buttons[4].isClicked(x, y) && !ismenuactive())
    {
        joycalib2.buttons[0].setVisible(false);
        joycalib2.buttons[1].setVisible(false);
        joycalib2.buttons[3].setVisible(true);
        joycalib2.setActive(true);
        joycalib2.activateKeypad();
        joycalib2.step = 0;
        toSend = true;
        msg = "joystick2trim_ON";
    }

    else if((buttons[4].isActive() && !joycalib2.isActive()) || (joycalib2.isActive() && buttons[4].isClicked(x, y)))
    {
        toSend = true;
        msg = "joystick2trim_OFF";
        buttons[4].setActive(false);
        joycalib2.setActive(false);

    }

    servotrim1.dealwithtouches(x, y, button);

    if(servotrim1.toSend)
    {
        toSend = true;
        msg = "servo1trim" + servotrim1.msg1 + servotrim1.msg2;
    }

    if(buttons[3].isActive() && buttons[3].isClicked(x, y) && !ismenuactive())
    {
        servotrim1.setActive(true);
        servotrim1.activateKeypad();
        toSend = true;
        msg = "servo1trim_ON";
    }

    else if((buttons[3].isActive() && !servotrim1.isActive()) || (buttons[3].isClicked(x, y) && servotrim1.isActive()))
    {
        buttons[3].setActive(false);
        toSend = true;
        msg = "servo1trim_OFF";
        servotrim1.setActive(false);
    }



    servotrim2.dealwithtouches(x, y, button);

    if(servotrim2.toSend)
    {
        toSend = true;
        msg = "servo2trim" + servotrim2.msg1 + servotrim2.msg2;
    }

    if(buttons[5].isActive() && buttons[5].isClicked(x, y) && !ismenuactive())
    {
        servotrim2.setActive(true);
        servotrim2.activateKeypad();
        toSend = true;
        msg = "servo2trim_ON";
    }

    else if((buttons[5].isActive() && !servotrim2.isActive()) || (buttons[5].isClicked(x, y) && servotrim2.isActive()))
    {
        buttons[5].setActive(false);
        toSend = true;
        msg = "servo2trim_OFF";
        servotrim2.setActive(false);
    }

    onExit.dealwithtouches(x,y,button);
    if(buttons[6].isActive() && buttons[6].isClicked(x,y) && !ismenuactive())
    {
        onExit.setActive(true);
    }
    else if((buttons[6].isActive() && !onExit.isActive()) || (buttons[6].isClicked(x, y) && onExit.isActive()))
    {
        buttons[6].setActive(false);
        onExit.setActive(false);
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
    joycalib1.update(message);
    joycalib2.update(message);
}

void topMenu::released()
{
    Panel::released();
    joycalib1.released();
    joycalib2.released();
    servotrim1.released();
    servotrim2.released();
    onExit.released();
}

void topMenu::draw()
{
    for(int i = 0; i < buttons.size(); i++)
    {
        if(buttons[i].isActive() && buttons[i].isVisible())
            active.draw(buttons[i].buttonarea);
        else
            inactive.draw(buttons[i].buttonarea);

        ofSetColor(255,255,255);
        info.drawString(buttons[i].message, buttons[i].buttonarea.x + 15, buttons[i].buttonarea.y + 23 );
    }
    pedals.draw();
    servos.draw();
    joycalib1.draw();
    joycalib2.draw();
    servotrim1.draw();
    servotrim2.draw();
    onExit.draw();
}

