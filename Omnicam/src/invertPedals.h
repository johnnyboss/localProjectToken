//
//  panel1.h
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef openNiSample007_invertpedals_h
#define openNiSample007_invertpedals_h
#include "panel.h"

class invertPedals:public Panel
{
public:
    void create(int startx, int starty);
    void setNumConsoles(int _nconsoles);
    void dealwithtouches(int x, int y, int button);
    void update(string message);
    void draw();

    int nconsoles;
    ofTrueTypeFont info;
    ofImage active, inactive;
};

#endif
