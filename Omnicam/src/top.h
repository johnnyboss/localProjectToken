//
//  panel1.h
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef openNiSample007_top_h
#define openNiSample007_top_h
#include "panel.h"
#include "invertPedals.h"
#include "invertServos.h"
#include "joystickCalib.h"
#include "servoTrim.h"
#include "exitMenu.h"


class topMenu:public Panel
{
public:
    void create();
    void dealwithtouches(int x, int y, int button);
    void update(string message);
    void draw();
    void released();
    bool ismenuactive();
    void setnumconsoles(int _nconsoles);
    void setnumomnicams(int _nomnicams);

    invertPedals pedals;
    invertServos servos;
    joystickCalib joycalib1;
    joystickCalib joycalib2;

    servoTrim servotrim1, servotrim2;

    exitMenu onExit;
    int nconsoles, nomnicams;


    ofTrueTypeFont digital, info;
    ofImage active, inactive;
};

#endif
