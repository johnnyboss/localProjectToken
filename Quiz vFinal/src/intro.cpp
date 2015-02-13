#include "intro.h"

void intro::create(int _subject)
{
    subject = _subject;
    images.clear();

    text1.init("Otari Medium.otf", 60);
    text2.init("Otari Medium.otf", 60);

    text3.init("Otari Light.otf", 35);
    text4.init("Otari Light.otf", 35);


string animacao;

    if(subject == 0)
    {
        text1.setText("Casa de banho");
        text2.setText("Bathroom");
        animacao = "banho/animacao";
    }
    else if(subject == 1)
    {
        text1.setText("Cozinha");
        text2.setText("Kitchen");
        animacao = "cozinha/animacao";
    }
    else if(subject == 2)
    {
        text1.setText("Quarto");
        text2.setText("Bedroom");
        animacao = "quarto/animacao";
    }
    else if(subject == 3)
    {
        text1.setText("Sala");
        text2.setText("Living room");
        animacao = "sala/animacao";
    }


    text3.setText("PT");
    text4.setText("ENG");

    quiz1.init("Otari Medium.otf", 60);
    quiz2.init("Otari Medium.otf", 60);
    quiz3.init("Otari Medium.otf", 60);
    quiz4.init("Otari Medium.otf", 60);

    quizpalavra1.init("Otari Bold.otf", 60);
    quizpalavra2.init("Otari Bold.otf", 60);

    info1.loadFont("Otari Medium.otf", 60);
    info2.loadFont("Otari Light.otf", 50);
    quiz1.setText("- A casa é um quiz - Home quiz");
    quiz2.setText("- A casa é um quiz - Home quiz");
    quiz3.setText("- A casa é um quiz - Home quiz");
    quiz4.setText("- A casa é um quiz - Home quiz");

    setup(0, 0, ofGetWidth(), ofGetHeight());
    if(subject != 3)
        logo.loadImage("logo.png");
    else
        logo.loadImage("logo2.png");
    position = 0;
    value1 = 0;
    value2 = quiz1.getWidth()*.95;
    value3 = value2 + quiz2.getWidth()*.95;
    value7 = value3 + quiz3.getWidth()*.95;


    for(int i = 1;  i < 7; i++)
    {
        ofImage aux;
        aux.loadImage(animacao + ofToString(i) + ".png");

        images.push_back(aux);
    }
    mode = 0;

    button aux;
    aux.create(200 , 150,300,200, "portuguese", false);
    aux.setVisible(false);
    buttons.push_back(aux);//0
    aux.create(ofGetWidth()-700, 150,500,200, "english", false);
    buttons.push_back(aux);//1
    lang = 2;

    dist = 0;
    for(int i = 0; i < images.size(); i++)
    {
        dist += images[i].width*.6 + 40;
    }

    value4 = 0;
    value5 = dist;
    value6 = dist*2;
}

void intro::dealwithtouches(int x, int y, int button)
{
    if(mode == 1)
    {
        for (int i = 0; i < buttons.size(); i++)
        {
            if(buttons[i].isClicked(x,y))
            {
                lang = i;
            }
        }
    }

    if(mode == 0)
    {
        timer = ofGetElapsedTimef();
        mode = 1;
        buttons[0].setVisible(true);
        buttons[1].setVisible(true);
    }
}

void intro::draw()
{
    if(subject == 0)
    {
        ofBackground(40,200,209);
    }
    else if(subject == 1)
    {
        ofBackground(255,248,232);
    }
    else if(subject == 2)
    {
        ofBackground(240,208,16);
    }
    else if(subject == 3)
    {
        ofBackground(229,48,121);
    }


    if(isActive())
    {
        if(mode == 0)
        {
            value1 += .5;
            value2 += .5;
            value3 += .5;
            value7 += .5;

            if(value1 > ofGetWidth())
                value1 = value2 - quiz1.getWidth()*.95;

            if(value2 > ofGetWidth())
                value2 = value3 - quiz2.getWidth()*.95;

            if(value3 > ofGetWidth())
                value3 = value7 - quiz3.getWidth()*.95;

            if(value7 > ofGetWidth())
                value7 = value1 - quiz4.getWidth()*.95;

            if(subject == 0)
            {
                ofSetColor(255,0,0);
                quiz1.setColor(255,255,255,1);
                quiz1.setColorSpecific(107,29,145,1,4);
                quiz2.setColor(255,255,255,1);
                quiz2.setColorSpecific(107,29,145,1,4);
                quiz3.setColor(255,255,255,1);
                quiz3.setColorSpecific(107,29,145,1,4);
                quiz4.setColor(255,255,255,1);
                quiz4.setColorSpecific(107,29,145,1,4);
            }
            else if(subject == 1)
            {
                quiz1.setColor(255,0,0,1);
                quiz1.setColorSpecific(0,0,0,1,4);
                quiz2.setColor(255,0,0,1);
                quiz2.setColorSpecific(0,0,0,1,4);
                quiz3.setColor(255,0,0,1);
                quiz3.setColorSpecific(0,0,0,1,4);
                quiz4.setColor(255,0,0,1);
                quiz4.setColorSpecific(0,0,0,1,4);
            }
            else if(subject == 2)
            {
                quiz1.setColor(255,255,255,1);
                quiz1.setColorSpecific(216,72,8,1,4);
                quiz2.setColor(255,255,255,1);
                quiz2.setColorSpecific(216,72,8,1,4);
                quiz3.setColor(255,255,255,1);
                quiz3.setColorSpecific(216,72,8,1,4);
                quiz4.setColor(255,255,255,1);
                quiz4.setColorSpecific(216,72,8,1,4);
            }
            else if(subject == 3)
            {
                quiz1.setColor(255,255,255,1);
                quiz1.setColorSpecific(0,54,66,1,4);
                quiz2.setColor(255,255,255,1);
                quiz2.setColorSpecific(0,54,66,1,4);
                quiz3.setColor(255,255,255,1);
                quiz3.setColorSpecific(0,54,66,1,4);
                quiz4.setColor(255,255,255,1);
                quiz4.setColorSpecific(0,54,66,1,4);
            }

            quiz1.drawQuiz(value1, 120);
            quiz2.drawQuiz(value2, 120);
            quiz3.drawQuiz(value3, 120);
            quiz4.drawQuiz(value7, 120);

        }
        else if (mode == 1)
        {
            if(subject == 0)
            {
                text1.setColor(255,255,255,1);
                text2.setColor(107,29,145,1);
                text3.setColor(255,255,255,1);
                text4.setColor(107,29,145,1);
            }
            else if(subject == 1)
            {
                text1.setColor(255,0,0,1);
                text2.setColor(0,0,0,1);
                text3.setColor(255,0,0,1);
                text4.setColor(0,0,0,1);
            }
            else if(subject == 2)
            {
                text1.setColor(216,72,8,1);
                text2.setColor(255,255,255,1);
                text3.setColor(216,72,8,1);
                text4.setColor(255,255,255,1);
            }
            else if(subject == 3)
            {
                text1.setColor(255,255,255,1);
                text2.setColor(0,54,66,1);
                text3.setColor(255,255,255,1);
                text4.setColor(0,54,66,1);

            }

            ofSetColor(255,255,255);
            text1.drawCenter(400,120);
            text2.drawCenter(ofGetWidth() - 400,120);
            text3.drawCenter(400, 220);
            text4.drawCenter(ofGetWidth() - 400,220);

            if(subject == 0)
                ofSetColor(255,255,255);
            else if(subject == 1)
                ofSetColor(255,0,0);
            else if(subject == 2)
                ofSetColor(216,72,8);
            else if(subject == 3)
                ofSetColor(255,255,255);


            ofRect(400-text1.getWidth()/2 + 5, 137, 50,6);

            if(subject == 0)
                ofSetColor(107,29,145);
            else if(subject == 1)
                ofSetColor(0,0,0);
            else if(subject == 2)
                ofSetColor(255,255,255);
            else if(subject == 3)
                ofSetColor(0,54,66);

            ofRect(ofGetWidth() - 400-text2.getWidth()/2 + 5, 137, 50,6);

        }

        if(ofGetElapsedTimef() - timer > 30)
        {
            mode = 0;
        }

        value4 -= .5;
        if(value4 < -dist)
            value4 = dist*2;

        value5 -= .5;
        if(value5 < -dist)
            value5 = dist*2;

        value6 -=.5;
        if(value6 < -dist)
            value6 = dist*2;

        ofEnableAlphaBlending();
        ofSetColor(255,255,255);
        int auxi = 0;


        for (int i = 0; i < images.size(); i++)
        {

            images[i].setAnchorPercent(0.0,1.0);
            images[i].draw(value4 + auxi, ofGetHeight() /2 + 170, images[i].width*.6, images[i].height*.6);
            images[i].draw(value5 + auxi, ofGetHeight() /2 + 170, images[i].width*.6, images[i].height*.6);
            images[i].draw(value6 + auxi, ofGetHeight() /2 + 170, images[i].width*.6, images[i].height*.6);
            auxi += images[i].width*.6 + 40;
        }


        float sizex = ofGetWidth()/3;
        float r = (float)(sizex / logo.width);

        float sizey = (int)(logo.height * r);

        if(mode == 0)
            logo.draw(ofGetWidth() / 2 - sizex/2, ofGetHeight() - 200, sizex,sizey);


    }
}
