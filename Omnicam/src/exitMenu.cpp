//
//  Panel2.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "exitMenu.h"

void exitMenu::create(int startx, int starty)
{
    setup(startx, starty, 255, 80);

    button aux;

    aux.create(panelArea.x + 10, panelArea.y + 10,80,50, "SavePosition", true, "Save\nPositions");
    buttons.push_back(aux);

   // aux.create(buttons[0].buttonarea.x + buttons[0].buttonarea.width + 1, panelArea.y + 10,80,50, "SaveSpeed", true, "Save\nVelocities");
    //buttons.push_back(aux);

    aux.create(buttons[0].buttonarea.x + buttons[0].buttonarea.width + 71,panelArea.y + 10,80,50, "Exit", true, "EXIT");
    buttons.push_back(aux);

    setActive(false);

    active.loadImage("topactive.png");
    inactive.loadImage("topinactive.png");

    info.loadFont("micross.ttf", 10);
    toSavePositions = false;
    toSaveVelocities = false;
}


void exitMenu::dealwithtouches(int x, int y, int button)
{

    Panel::dealwithtouches(x, y, button);
    if(isActive())
    {
        if(buttons[0].isClicked(x, y) && buttons[0].isActive())
        {
            toSavePositions = true;
        }
        else if(buttons[0].isClicked(x, y) && !buttons[0].isActive())
        {
            toSavePositions = false;
        }

        if(buttons[1].isClicked(x, y) && buttons[1].isActive())
        {
            toSaveVelocities = true;
        }
      /*  else if(buttons[1].isClicked(x, y) && !buttons[1].isActive())
        {
            toSaveVelocities = false;
        }*/

        if(buttons[1].isClicked(x,y))
        {
            toExit = true;
            cout<<"leave bastard"<<endl;
        }
    }
}


void exitMenu::draw()
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

