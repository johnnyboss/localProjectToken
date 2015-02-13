//
//  servoTrim.h
//  openNiSample007
//
//  Created by Joao Freire on 6/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef openNiSample007_servoTrim_h
#define openNiSample007_servoTrim_h

#include "panel.h"
#include "keypad.h"

class servoTrim:public Panel
{
public:
    void create(int startx, int starty);
    void dealwithtouches(int x, int y, int button);
    void update(string message);
    void initialize(string message);
    void released();
    void activateKeypad();
    void draw();

    int active;
    string msg1, msg2;
    ofTrueTypeFont mono, arrow;
    ofImage active1, active2, inactive1, inactive2;
    Keypad keypad;

};

#endif
