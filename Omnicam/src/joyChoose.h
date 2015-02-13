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

class topMenu:public Panel
{
public:
    void create();
    void dealwithtouches(int x, int y, int button);
    void update(string message);
    void draw();
    

    ofTrueTypeFont digital;
};

#endif
