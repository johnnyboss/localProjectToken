//
//  joystickCalib.h
//  openNiSample007
//
//  Created by Joao Freire on 6/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef openNiSample007_joystickCalib_h
#define openNiSample007_joystickCalib_h

#include "panel.h"
#include "keypad.h"

class joystickCalib:public Panel
{
public:
    void create(int startx, int starty, int i);
    void dealwithtouches(int x, int y, int button);
    void update(string message);
    void initialize(string message);
    void released();
    void activateKeypad();
    void draw();
    int step;
    vector <int> values;
    int ID;
    ofTrueTypeFont info;

    ofImage active, inactive;
    Keypad keypad;


};

#endif
