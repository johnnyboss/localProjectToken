//
//  panel1.h
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef intro_h
#define intro_h
#include "panel.h"
#include "ofxTextSuite.h"


class intro:public Panel
{
public:
    void create(int _subject);
    void dealwithtouches(int x, int y, int button);
    void draw();

    ofImage pflag, eflag;
    ofxTextBlock text1, text2, text3, text4;
    ofTrueTypeFont info1, info2;
    int lang;
    ofImage logo;
    string test1, test2, test3, test4;
    int position;
    float value1, value2, value3, value4, value5, value6, value7;

    vector <ofImage> images;

    int mode, dist, timer, subject;
   ofxTextBlock quiz1, quiz2, quiz3, quiz4, quizpalavra1, quizpalavra2;



};

#endif
