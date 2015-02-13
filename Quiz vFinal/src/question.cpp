//
//  panel1.cpp
//  openNiSample007
//
//  Created by Joao Freire on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "question.h"

void questionPage::create(int _subject)
{
    subject = _subject;
    hover = 0;
    ofSetCircleResolution(200);
    setup(0,0,ofGetWidth(), ofGetHeight());
    button aux;
    mode = 0;
    string folder;
    if(subject == 0)
    {
        folder = "banho/banho";
    }
    else if(subject == 1)
    {
        folder = "cozinha/cozinha";
    }
    else if(subject == 2)
    {
        folder = "quarto/quarto";
    }
    else if(subject == 3)
    {
        folder = "sala/sala";
    }

    for (int i = 1; i < 5; i++)
    {
        aux.create(i*ofGetWidth()/4 - ofGetWidth()/8 - 90, 250, 180, 180, ofToString(i), false);
        string path = folder + ofToString(i)+".png";
        aux.imageinactive.loadImage(path);
        buttons.push_back(aux);
    }

    for (int i = 1; i < 5; i++)
    {
        aux.create(i*ofGetWidth()/4 - ofGetWidth()/8 - 90, 450, 180, 180, ofToString(i+4), false);
        string path = folder + ofToString(i+4)+".png";
        aux.imageinactive.loadImage(path);
        buttons.push_back(aux);
    }

    for (int i = 1; i < 5; i++)
    {
        aux.create(i*ofGetWidth()/4 - ofGetWidth()/8 - 90, 650, 180, 180, ofToString(i+8), false);
        string path = folder + ofToString(i+8)+".png";
        aux.imageinactive.loadImage(path);
        buttons.push_back(aux);
    }

    aux.create(ofGetWidth()/2 - 72, 0, 72, 73, "back", false);
    aux.imageinactive.loadImage("return1.png");
    buttons.push_back(aux);

    aux.create(100, 480 , 80, 80, "A", false);
    buttons.push_back(aux);
    aux.create(100, 600 , 80, 80, "B", false);
    buttons.push_back(aux);
    aux.create(100, 720, 80, 80, "C", false);
    buttons.push_back(aux);

    aux.create(ofGetWidth()/2 , 0, 72, 73, "forward", false);
    aux.imageinactive.loadImage("return2.png");
    buttons.push_back(aux);

    _right = false;

    clock.init("verdanab.ttf", 50);

    final1.init("Otari Bold Italic.otf", 60);
    final2.init("Otari Bold Italic.otf", 20);
    final3.init("Otari Bold Italic.otf", 50);
    final4.init("Otari Regular.otf", 40);
    question1.init("Otari Bold.otf", 60);
    question2.init("Otari Bold.otf", 80);
    question3.init("Otari Medium.otf", 25);

    fbo1.allocate(500,500, GL_RGBA);
    fbo2.allocate(500,500, GL_RGBA);

    A.loadImage("A.png");
    B.loadImage("B.png");
    C.loadImage("C.png");
    check.loadImage("check.png");

}

void questionPage::setBuffer(int activate)
{
    int Led_activo;
    if(activate == 20)
        Led_activo = 0;
    else
        Led_activo = exp2(activate);

    int ValorH = (Led_activo >> 8);
    int ValorL = (Led_activo & 0xFF);

    unsigned char *chars = new unsigned char[3];
    chars[0] = 'Q';
    chars[1] = ValorH;
    chars[2] = ValorL;

    int val = mySerial.writeBytes(chars , 3);
}

void questionPage::setHover(int x, int y)
{
    if(isActive() && mode == 1)
    {
        if(buttons[13].buttonarea.inside(x,y))
            hover = 1;
        else if(buttons[14].buttonarea.inside(x,y))
            hover = 2;
        else if(buttons[15].buttonarea.inside(x,y))
            hover = 3;
        else
            hover = 0;
    }
}
void questionPage::dealwithtouches(int x, int y, int button)
{
    if(isActive())
    {
        if(mode == 0)
        {
            for (int i = 0; i < buttons.size(); i++)
            {
                if(buttons[i].isClicked(x,y) && i < 12)
                {
                    pergunta = i;
                    setBuffer(pergunta);
                    mode = 1;
                    hover = 0;
                    buttons[12].imageinactive.loadImage("return2.png");
                    timer = ofGetElapsedTimef();
                    question3.init("Otari Medium.otf", 25);


                }
            }
        }
        else if (mode == 1)
        {
            if(buttons[13].isClicked(x,y))
            {
                mode = 2;
                if(blocos[pergunta].right == 1)
                {
                    _right = true;
                    if(lang == 0)
                    {
                        final1.setText("Parabéns... Acertou!");
                        final2.setText(blocos[pergunta].respostas[0]);
                        final4.setText(blocos[pergunta].resposta);
                    }
                    else
                    {
                        final1.setText("Well done... You are right!");
                        final2.setText(blocos[pergunta].answers[0]);
                        final4.setText(blocos[pergunta].answertext);
                    }
                }
                else
                {
                    _right == false;
                    if(lang == 0)
                    {
                        final1.setText("Oops... Falhou!");
                        int a = blocos[pergunta].right - 1;
                        final2.setText(blocos[pergunta].respostas[a]);
                        final4.setText(blocos[pergunta].resposta);

                    }
                    else
                    {
                        final1.setText("Oops... Try again!");
                        int a = blocos[pergunta].right - 1;
                        final2.setText(blocos[pergunta].answers[a]);
                        final4.setText(blocos[pergunta].answertext);
                    }
                }
                timer = ofGetElapsedTimef();
            }
            else if(buttons[14].isClicked(x,y))
            {
                mode = 2;
                if(blocos[pergunta].right == 2)
                {
                    _right = true;
                    if(lang == 0)
                    {
                        final1.setText("Parabéns... Acertou!");
                        final2.setText(blocos[pergunta].respostas[1]);
                        final4.setText(blocos[pergunta].resposta);
                    }
                    else
                    {
                        final1.setText("Well done... You are right!");
                        final2.setText(blocos[pergunta].answers[1]);
                        final4.setText(blocos[pergunta].answertext);
                    }
                }
                else
                {
                    _right == false;
                    if(lang == 0)
                    {
                        final1.setText("Oops... Falhou!");
                        int a = blocos[pergunta].right - 1;
                        final2.setText(blocos[pergunta].respostas[a]);
                        final4.setText(blocos[pergunta].resposta);
                    }
                    else
                    {
                        final1.setText("Oops... Try again!");
                        int a = blocos[pergunta].right - 1;
                        final2.setText(blocos[pergunta].answers[a]);
                        final4.setText(blocos[pergunta].answertext);
                    }
                }
                timer = ofGetElapsedTimef();
            }
            else if(buttons[15].isClicked(x,y))
            {
                mode = 2;
                if(blocos[pergunta].right == 3)
                {
                    _right = true;
                    if(lang == 0)
                    {
                        final1.setText("Parabéns... Acertou!");
                        final2.setText(blocos[pergunta].respostas[2]);
                        final3.setText("é a resposta certa!");
                        final4.setText(blocos[pergunta].resposta);
                    }
                    else
                    {
                        final1.setText("Well done... You are right!");
                        final2.setText(blocos[pergunta].answers[2]);
                        final4.setText(blocos[pergunta].answertext);
                    }
                }
                else
                {
                    _right == false;
                    if(lang == 0)
                    {
                        final1.setText("Oops... Falhou!");
                        int a = blocos[pergunta].right - 1;
                        final2.setText(blocos[pergunta].respostas[a]);
                        final4.setText(blocos[pergunta].resposta);
                    }
                    else
                    {
                        final1.setText("Oops... Try again!");
                        int a = blocos[pergunta].right - 1;
                        final2.setText(blocos[pergunta].answers[a]);
                        final4.setText(blocos[pergunta].answertext);
                    }
                }
                timer = ofGetElapsedTimef();
            }
        }
    }
}

void questionPage::drawMode0()
{
    ofBackground(color1);
    ofSetColor(color2);
    ofRect(0,0,ofGetWidth()/2, ofGetHeight());
    ofSetColor(255,255,255);
    ofEnableAlphaBlending();

    for(int i = 0; i < 12; i++)
    {

        ofSetColor(255,255,255);
        buttons[i].imageinactive.draw(buttons[i].buttonarea);

        if(lang == 0)
            question3.setText(portugues[i]);
        else
            question3.setText(english[i]);
        if(i == 0 || i == 1 || i == 4 || i == 5 || i ==8 || i ==9)
            question3.setColor(color1.r, color1.g, color1.b,255);
        else
            question3.setColor(color2.r, color2.g, color2.b,255);

        question3.drawCenter(buttons[i].buttonarea.x + 90,buttons[i].buttonarea.y + buttons[i].buttonarea.height - 15 );
    }

    if(lang == 0)
        question1.setText("como se");
    else
        question1.setText("how to");
    question1.setColor(color1.r, color1.g, color1.b,255);
    question1.drawLeft(100,0);

    if(lang == 0)
        question2.setText("obtém?");
    else
        question2.setText("get it?");

    question2.setColor(color1.r, color1.g, color1.b,255);
    question2.drawLeft(150, 70);

    if(lang == 0)
        question1.setText("como");
    else
        question1.setText("how");
    question1.setColor(color2.r, color2.g, color2.b,255);
    question1.drawLeft(ofGetWidth()/2 + 100,0);

    if(lang == 0)
        question2.setText("funciona?");
    else
        question2.setText("it works?");

    question2.setColor(color2.r, color2.g, color2.b,255);
    question2.drawLeft(ofGetWidth()/2 + 150, 70);

}

void questionPage::drawMode1()
{
    question3.init("Otari Medium.otf", 30);
    ofBackground(color2);
    if(lang == 0)
        question1.setText(blocos[pergunta].pergunta);
    else
        question1.setText(blocos[pergunta].question);

    ofColor colorA, colorB, colorC;
    colorA = color1;
    colorB = color1;
    colorC = color1;

    if(hover == 0)
    {
        colorA.a = 160;
        colorB.a = 160;
        colorC.a = 160;
    }
    else if(hover == 1)
    {
        colorA.a = 255;
        colorB.a = 160;
        colorC.a = 160;
    }
    else if(hover == 2)
    {
        colorA.a = 160;
        colorB.a = 255;
        colorC.a = 160;
    }
    else if(hover == 3)
    {
        colorA.a = 160;
        colorB.a = 160;
        colorC.a = 255;
    }


    ofSetColor(colorA);
    A.draw(buttons[13].buttonarea.x, buttons[13].buttonarea.y, buttons[13].buttonarea.height, buttons[13].buttonarea.height);
    ofSetColor(colorB);
    B.draw(buttons[14].buttonarea.x , buttons[14].buttonarea.y, buttons[13].buttonarea.height, buttons[13].buttonarea.height);
    ofSetColor(colorC);
    C.draw(buttons[15].buttonarea.x , buttons[15].buttonarea.y, buttons[13].buttonarea.height, buttons[13].buttonarea.height);

    question1.setColor(color1.r, color1.g, color1.b, 255);
    question3.setColor(color1.r, color1.g, color1.b, 255);

    // question1.wrapTextX(1000);
    question1.wrapTextArea(900, 310);

    if(/*question1.lines.size() == 1 && */question1.scale < 0.65)
    {
        question1.forceScale(0.65);
        question1.wrapTextForceLines(question1.lines.size()+1);
        if(question1.lines.size() > 3)
        {
            question1.wrapTextArea(900, 310);
            question1.forceScale(0.65);
        }
    }

    if(question1.scale > 1.0)
        question1.forceScale(1.0);

    if(pergunta == 3 && subject == 2 && lang != 0)
        question1.wrapTextX(500);

    if(pergunta == 10 && subject == 2 && lang == 0)
    {
        //question1.forceScale(1);
        question1.wrapTextX(850);
        question1.forceScale(0.9);
    }

    if(pergunta == 6 && subject == 2 && lang != 0)
    {
        question1.forceScale(.8);
    }

    if(pergunta == 0 && subject == 1 && lang != 0)
    {
        question1.forceScale(1);
        question1.wrapTextX(950);
    }

    if(pergunta == 7 && subject == 1 && lang != 0)
    {
        question1.wrapTextX(950);
    }

    if(subject == 1 && pergunta == 8 && lang == 0)
    {
        question1.forceScale(1);
        question1.wrapTextX(600);
    }

    if((pergunta == 4) && subject == 3 && lang == 0)
    {
        float c = question1.scale;
        //question1.wrapTextX(950);
        question1.forceScale(c);
    }

    if(pergunta == 2 && subject == 3 && lang != 0)
    {
        float c = question1.scale;
        question1.wrapTextX(950);
        question1.forceScale(c + .1);
    }

    /*  if((pergunta == 8) && subject == 3 && lang == 0)
    {
        float c = question1.scale;
        question1.wrapTextX(1050);
        question1.forceScale(c + .05 );
    }*/
   if((pergunta == 10) && subject == 3 && lang != 0)
    {
        float c = question1.scale;
        question1.wrapTextX(950);
        question1.forceScale(1);
    }

    if(pergunta == 10 && subject == 0 )
    {
        float c = question1.scale;
        question1.wrapTextX(850);
        question1.forceScale(1);
    }

//NOVAS CORRECOS 20-03

    if(pergunta == 1 && subject == 0 )
    {
        question1.wrapTextX(1050);
        question1.forceScale(0.78);
    }

    if(pergunta == 7 && subject == 0 && lang == 0)
    {
        question1.wrapTextX(1050);
        question1.forceScale(0.78);
    }

    if((pergunta == 0 || pergunta == 9 || pergunta == 6) && subject == 1 && lang == 0)
    {
        question1.wrapTextX(1050);
        question1.forceScale(0.89);
    }

    if((pergunta == 1 || pergunta == 5 || pergunta == 8) && subject == 0 && lang != 0)
    {
        question1.wrapTextArea(1100, 310);
       // question1.forceScale(0.72);
    }

    if(subject == 1 && lang == 0 && pergunta == 10)
    {
         question1.wrapTextArea(1100, 310);
        //question1.wrapTextArea(1050, 360);
    }

    if(subject == 2 && pergunta == 2 && lang == 0)
    {
        question1.forceScale(0.75);

    }

    if(subject == 2 && pergunta == 7)
    {
        question1.wrapTextArea(1100, 310);
    }

    if(subject == 2 && pergunta == 10 /*&& lang == 0*/)
    {
        question1.wrapTextArea(1000, 310);
    }

    if(subject == 2 && pergunta == 6 && lang != 0)
    {
        question1.wrapTextArea(1000, 310);
    }

    if(subject == 3 && pergunta == 5 && lang == 0)
    {
        question1.forceScale(question1.scale - .1);
    }

    if(subject == 3 && (pergunta == 9 || pergunta == 7 || pergunta == 10 || pergunta == 11) && lang == 0)
    {
        question1.wrapTextX(1150);
        question1.forceScale(question1.scale - .1);
    }

    if(subject == 3 && (pergunta == 1 || pergunta == 5 || pergunta == 2) && lang != 0)
{
        question1.wrapTextX(1150);
        question1.forceScale(question1.scale - .1);
    }
    question1.drawJustified(100,140, 900);
    float scale;
    scale = 1;

    for(int i = 0; i < 3; i++)
    {
        if(lang == 0)
            question3.setText(blocos[pergunta].respostas[i]);
        else
            question3.setText(blocos[pergunta].answers[i]);


        question3.wrapTextX(780);

        /*if(question3.scale < 0.8 && question3.lines.size() == 1)
        {
            question3.wrapTextForceLines(2);
        }*/

        if(question3.lines.size() > 1 /*&& question3.scale < 0.65*/)
        {
            question3.wrapTextArea( 850, 100);
        }

        if(question3.scale < 0.72)
            question3.scale = 0.72;

        if(question3.scale < scale && question3.scale > 0.71)
        {
            scale = question3.scale;
        }
    }


    for(int i = 0; i < 3; i++)
    {
        if(lang == 0)
            question3.setText(blocos[pergunta].respostas[i]);
        else
            question3.setText(blocos[pergunta].answers[i]);


        //question3.wrapTextArea( 780, 100);
        int size;
        if(scale < 0.8)
        {
            size = 660;
            question3.wrapTextArea(660, 100);
        }
        else
        {
            size = 780;
            question3.wrapTextX(780);
        }


        question3.forceScale(scale);
        question3.setColor(color1.r, color1.g, color1.b, 255);

        if(subject == 0 && scale < 1 && (pergunta == 3))
         {
            question3.forceScale(0.79);
            scale = 0.79;
         }

         if(subject == 0 && scale < 1 && (pergunta == 6 ||pergunta == 8 ))
         {
             if(i == 0 || i == 2)
             {
                 question3.forceScale(1);
            scale = 1;
             }
             else
             {
                 question3.wrapTextX(660);
                 question3.forceScale(1);
                 scale = 1;
             }
         }

       /*  if(subject == 0 && pergunta == 10)
         {
             question3.wrapTextX(660);
         }*/

        if(subject == 1 && (pergunta == 2 || pergunta == 7))
        {
            question3.forceScale(1);
            scale = 1;
        }


         if(subject == 2 && (pergunta == 1 ) && lang == 0)
         {
            question3.forceScale(1);
            scale = 1;
         }



        if(subject == 3 && pergunta == 10 /*&& lang != 0*/)
        {
            question3.forceScale(1);
            scale = 1;
        }



          buttons[13 + i].buttonarea.width = question3.getWidth() + 100;

        if(question3.lines.size() == 1)
            if(scale < 0.8)
            {
                question3.drawJustified(buttons[13].buttonarea.x + 100, buttons[13 + i].buttonarea.y + 17 , size);
            }
            else
                question3.drawJustified(buttons[13].buttonarea.x + 100, buttons[13 + i].buttonarea.y +7, size);
        else if(question3.lines.size() == 2)
        {
            if(scale < 0.8)
            {
               // question3.setColor(255,0,0,255);
                question3.drawJustified(buttons[13].buttonarea.x + 100, buttons[13 + i].buttonarea.y + 3 , size);
            }
            else
            {
                if(((subject == 2 && pergunta == 0) || (subject == 1 && pergunta == 11)) && lang != 0 )
                {
                    question3.drawJustified(buttons[13].buttonarea.x + 100, buttons[13 + i].buttonarea.y + 3 , size);

                }
                else
                question3.drawJustified(buttons[13].buttonarea.x + 100, buttons[13 + i].buttonarea.y - 13, size);
            }
        }
        else
        {

            question3.drawJustified(buttons[13].buttonarea.x + 100, buttons[13 + i].buttonarea.y - 15 , size);
        }


    }

    ofSetColor(255,255,255);
    buttons[pergunta].imageinactive.draw( ofGetWidth() - 450, buttons[13].buttonarea.y - 50 , 410,410);//buttons[pergunta].imageinactive.width*.7, buttons[pergunta].imageinactive.height*.7);

    float loaded = ofMap(ofGetElapsedTimef() - timer, 0, 30, 0, 1);
    if(ofGetElapsedTimef() - timer > 30)
    {
        mode = 0;
        timer = ofGetElapsedTimef();
        buttons[12].imageinactive.loadImage("return1.png");
        question3.init("Otari Medium.otf", 25);
        setBuffer(20);
    }

    drawClock(loaded);
}


void questionPage::drawMode2()
{
    ofBackground(color1);

    final1.setColor(color2.r,color2.g,color2.b,255);
    final1.draw(100,60);

    final2.setColor(color2.r,color2.g,color2.b,255);
    final2.wrapTextX(ofGetWidth() - 600);// buttons[pergunta].imageinactive.width*.9);
    // final2.drawLeft(500 - 30 +80*.6 + 5,ofGetHeight() - 100 + 30 - final2.getHeight()/3);
    if(final2.lines.size() > 1 && subject != 1 && pergunta != 2 && lang != 0)
        final2.drawJustified(500 - 30 +80*.6 + 5,ofGetHeight() - 100 + 30 - final2.getHeight()/3, ofGetWidth() - 600);
    else
        final2.drawJustified(500 - 30 +80*.6 + 5,ofGetHeight() - 100 + 30 , ofGetWidth() - 600);
    ofSetColor(255,255,255);

    buttons[pergunta].imageinactive.draw(0, 60 + final1.getHeight() + 150, 470, 470);//buttons[pergunta].imageinactive.width*.9, buttons[pergunta].imageinactive.height*.9);

//final4.wrapTextArea(ofGetWidth() -  560, 520

    final4.wrapTextArea(ofGetWidth() -  582, 520);// buttons[pergunta].imageinactive.width + 50, buttons[pergunta].imageinactive.height*.8);
    final4.setColor(color2.r,color2.g,color2.b,255);
    final4.drawJustified(500 - 30, 60 + final1.getHeight() + 120, ofGetWidth() -  560 + 50);//buttons[pergunta].imageinactive.width + 50);

    ofSetColor(color2.r, color2.g, color2.b);
    if(blocos[pergunta].right == 1)
        A.draw(500-30,ofGetHeight() - 100 + 80*.3, 80*.6, 80*.6);
    else if(blocos[pergunta].right == 2)
        B.draw(500-30,ofGetHeight() - 100+ 80*.3,80*.6, 80*.6);
    else if(blocos[pergunta].right == 3)
        C.draw(500-30,ofGetHeight() - 100+ 80*.3, 80*.6, 80*.6);

    ofSetColor(255,255,255);
    check.draw(500 -30 + 80*.6 + 10 + final2.getWidth(), ofGetHeight() - 100 +80*.3, 80*.6, 80*.6);
    if(ofGetElapsedTimef() - timer > 45)
    {
        mode = 0;
        timer = ofGetElapsedTimef();
        buttons[12].imageinactive.loadImage("return1.png");
        setBuffer(20);
        question3.init("Otari Medium.otf", 25);
    }
}
void questionPage::draw()
{
//   Panel::draw();
    if(isActive())
    {
        if(mode == 0)
        {
            drawMode0();
        }
        else if (mode == 1)
        {
            drawMode1();
        }
        else if (mode == 2)
        {
            drawMode2();
        }

        ofSetColor(255,255,255);
        buttons[12].imageinactive.draw(buttons[12].buttonarea.x, buttons[12].buttonarea.y, buttons[12].buttonarea.width*2,buttons[12].buttonarea.height );
    }
}

void questionPage::drawClock(float loaded)
{
    fbo2.begin();
    ofClear(0,0,0);
    ofPushMatrix();
    //ofTranslate(250, 250);
    ofColor white;
    white.set(255,255,255,255);
    clock.setText(ofToString(30 - (int)(ofGetElapsedTimef() - timer)));
    clock.setColor(white.r, white.g, white.b, white.a);
    clock.drawCenter(270,200);
    //info.drawString(ofToString(30 - (int)(ofGetElapsedTimef() - timer)), 0,0);
    ofPopMatrix();
    fbo2.end();


    fbo1.begin();
    ofPushMatrix();
    ofTranslate(250, 250);
    ofSetColor(white);
    ofCircle(0,0, 60);
    ofSetColor(color2);
    pacman(64, 80, loaded);

    ofPopMatrix();
    fbo1.end();

    ofPixels pixels1, pixels2;
    fbo1.readToPixels(pixels1);
    fbo2.readToPixels(pixels2);
    for (int i = 0; i < fbo1.getWidth(); i++)
    {
        for (int j = 0; j < fbo1.getHeight(); j++)
        {
            if(pixels1.getColor(i,j) == white && pixels2.getColor(i,j) == white)
                pixels2.setColor(i,j,color2);
        }
    }
    //fbo2.set

    ofSetColor(255,255,255);
    fbo1.draw(ofGetWidth() - 380,-120);//x y
    ofImage aux4;
    aux4.setFromPixels(pixels2);
    aux4.draw(ofGetWidth() - 380,-120);
}
void questionPage::addBlock(string pergunta,string question, string rsp1, string answ1,string rsp2, string answ2,string rsp3, string answ3, int rightansw, string texto, string text)
{
    bloco aux;
    aux.pergunta = pergunta;
    aux.question = question;
    aux.respostas.push_back(rsp1);
    aux.respostas.push_back(rsp2);
    aux.respostas.push_back(rsp3);
    aux.answers.push_back(answ1);
    aux.answers.push_back(answ2);
    aux.answers.push_back(answ3);
    aux.right = rightansw;
    aux.answertext = text;
    aux.resposta = texto;
    blocos.push_back(aux);
}

void questionPage::pacman(int resolution, float radius, float loaded)
{
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    mesh.addVertex(ofVec2f(0, 0));
    float minTheta = -loaded * 180, maxTheta = +loaded * 360;

    for(int i = 0; i < resolution; i++)
    {
        ofVec2f pos(0, radius);
        float theta = ofMap(i, 0, resolution - 1, 0, maxTheta);
        pos.rotate(theta + 180);
        mesh.addVertex(pos);
    }
    mesh.draw();
}
