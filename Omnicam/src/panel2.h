//
//  panel1.h
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef openNiSample007_panel2_h
#define openNiSample007_panel2_h
#include "panel.h"
#include "keypad.h"
#include "graphPosition.h"
#include "joystickCalib.h"

class Panel2:public Panel
{
public:
    void create();
    void dealwithtouches(int x, int y, int button);
    void update(string message);
    void draw();

    Keypad keypad;
    int vMax, vMin, goSpeed;
    GraphPosition posGraph;
    ofImage logo, active, inactive, back, degrade2;
    ofTrueTypeFont digital;
    ofTrueTypeFont info, top;
    float actuator, actualspeed, actualposition;
    bool servopower, camerapower, tally;
    bool block;

};

#endif
