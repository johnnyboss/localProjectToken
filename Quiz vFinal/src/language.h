//
//  panel1.h
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef language_h
#define language_h
#include "panel.h"


class language:public Panel
{
public:
    void create();
    void dealwithtouches(int x, int y, int button);
    void draw();

    ofTrueTypeFont info;
    int lang;
    int position;


};

#endif
