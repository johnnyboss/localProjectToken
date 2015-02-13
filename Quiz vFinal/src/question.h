//
//  panel1.h
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef question_h
#define question_h
#include "panel.h"
#include "ofxTextSuite.h"
 struct bloco
    {
        int right;
        vector<string> answers;
        vector<string> respostas;
        string question;
        string pergunta;
        string answertext;
        string resposta;
    };

class questionPage:public Panel
{
public:
    void create(int _subject);
    void dealwithtouches(int x, int y, int button);
    void draw();
    void addBlock(string pergunta,string question, string rsp1, string answ1,string rsp2, string answ2,string rsp3, string answ3, int rightansw, string texto, string text);
    void pacman(int resolution, float radius, float loaded);
    void setBuffer(int activate);
    void drawClock(float loaded);
    void drawMode0();
    void drawMode1();
    void setHover(int x, int y);
    void drawMode2();
    ofxTextBlock  final1, final2, final3, final4, question1, question2, question3, clock;
    ofImage A, B, C, check;
    int hover;
    vector<string> portugues;
    vector <string> english;
    string ID;
    int lang, mode, pergunta, timer;
    vector<bloco> blocos;
    bool _right;
    ofColor color1, color2;
    ofSerial mySerial;
    unsigned char *buf[12];
    ofFbo fbo1, fbo2;
    int subject;

};

#endif
