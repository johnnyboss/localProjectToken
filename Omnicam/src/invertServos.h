//
//  panel1.h
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef openNiSample007_servoinvert_h
#define openNiSample007_servoinvert_h
#include "panel.h"

class invertServos:public Panel
{
public:
    void create(int startx, int starty);
    void dealwithtouches(int x, int y, int button);
    void update(string message);
    void draw();
    void setNumConsoles(int _nconsoles);
    
    int nconsoles;
    ofTrueTypeFont digital;
    ofImage active, inactive;

};

#endif
