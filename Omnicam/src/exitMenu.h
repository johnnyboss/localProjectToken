//
//  panel1.h
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef openNiSample007_exitMenu_h
#define openNiSample007_exitMenu_h
#include "panel.h"

class exitMenu:public Panel
{
public:
    void create(int startx, int starty);
    void dealwithtouches(int x, int y, int button);
    void onExit();
    void save();
    void draw();

    ofTrueTypeFont info;
    ofImage active, inactive;
    bool toSavePositions, toSaveVelocities, toExit;
};

#endif
