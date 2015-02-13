#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    //ofSetFrameRate(60);
    subject = 3;

    if(subject == 0)
        XML.loadFile("banho/Questions.xml");
    else if(subject == 1)
        XML.loadFile("cozinha/Questions.xml");
    else if(subject == 2)
        XML.loadFile("quarto/Questions.xml");
    else if(subject == 3)
        XML.loadFile("sala/Questions.xml");

    int numQuizTags = XML.getNumTags("QUIZ");
    XML.pushTag("QUIZ",0);
    int numTemas = XML.getNumTags("TEMA");
    XML.pushTag("TEMA",0);

    perguntas.color1.set(XML.getValue("R", 0, 0), XML.getValue("G", 0, 0), XML.getValue("B", 0, 0));
    perguntas.color2.set(XML.getValue("R", 0, 1), XML.getValue("G", 0, 1), XML.getValue("B", 0, 1));

    int numElementos = XML.getNumTags("LEGENDA");

    for (int i = 0; i < numElementos; i++)
    {
        string aux = XML.getValue("LEGENDA", "", i);
        if(i < 12)
            perguntas.portugues.push_back(aux);
        else
            perguntas.english.push_back(aux);
    }


    int indexaux = 0;
    for (int i = 0; i < 12; i++)
    {
        string aux = XML.getValue("PERGUNTA","", i);

        string auxx = XML.getValue("QUESTION","",i);

        string aux1 = XML.getValue("RESPOSTA","", indexaux);
        string auxx1 = XML.getValue("ANSWER","", indexaux);
        indexaux++;

        string aux2 = XML.getValue("RESPOSTA","", indexaux);
        string auxx2 = XML.getValue("ANSWER","", indexaux);
        indexaux++;

        string aux3 = XML.getValue("RESPOSTA","", indexaux);
        string auxx3 = XML.getValue("ANSWER","", indexaux);
        indexaux++;

        int right = XML.getValue("CERTA",0,i);
        string aux4 = XML.getValue("TEXTO", "", i);
        string auxx4 = XML.getValue("TEXT", "", i);
        perguntas.addBlock(aux,auxx, aux1,auxx1, aux2,auxx2, aux3,auxx3, right, aux4,auxx4);
    }
    int bla = 0;

    perguntas.mySerial.setup("COM3", 57600);
    perguntas.setBuffer(20);

    ofSetLogLevel(OF_LOG_SILENT);
    //ofHideCursor();
    ofSetFrameRate(60);

    exitState = 0;


}


//--------------------------------------------------------------
void testApp::update()
{
    if(exitState == 1 && ofGetElapsedTimef() - timerExit> 10)
    {
        exitState = 2;
    }
    else if(exitState == 3 && ofGetElapsedTimef() - timerExit> 10)
    {
        system("shutdown /p");
    }
}


//--------------------------------------------------------------
void testApp::draw()
{
    ofSetColor(255,255,255);
    introduction.draw();
    perguntas.draw();
    if(perguntas.isActive() && perguntas.mode == 0 && (ofGetElapsedTimef() - perguntas.timer > 30))
    {
        introduction.lang = 2;
        introduction.setActive(true);
        perguntas.setActive(false);
    }

     if(exitState == 2)
     {
        ofSetColor(255,0,0, 120);
        ofCircle(touchExit.x, touchExit.y, 10);
     }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}



//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    perguntas.setHover(x,y);

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    perguntas.setHover(x,y);
    if(exitState == 0)
    {
        if(x < 100 && y < ofGetWidth() - 100)
        {
            timerExit = ofGetElapsedTimef();
            exitState = 1;
            touchExit.set(x,y,0);
        }
    }
    else if(exitState == 2)
    {
        if(x > ofGetWidth() - 100 && y < ofGetWidth() - 100)
        {
            timerExit = ofGetElapsedTimef();
            exitState = 3;
        }
        else
        {
            exitState = 0;
        }
    }


    //introduction.dealwithtouches(x,y,button);
    //perguntas.dealwithtouches(x,y,button);

    //timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if(exitState != 2)
    {
        exitState = 0;
    }

    perguntas.setHover(x,y);
    introduction.dealwithtouches(x,y,button);
    perguntas.dealwithtouches(x,y,button);

    if(introduction.isClicked(x,y)  && introduction.lang < 2)
    {
        introduction.setActive(false);
        perguntas.setActive(true);
        perguntas.timer = ofGetElapsedTimef();
        perguntas.lang = introduction.lang;
    }
    else if(perguntas.isActive())
    {
        if(perguntas.buttons[12].isClicked(x,y) && perguntas.mode == 0)
        {
            introduction.lang = 2;
            introduction.setActive(true);
            introduction.timer = ofGetElapsedTimef();
            perguntas.setActive(false);
        }
        else if(perguntas.buttons[12].isClicked(x,y) && (perguntas.mode == 1 || perguntas.mode == 2))
        {
            perguntas.mode = 0;
            perguntas.timer = ofGetElapsedTimef();
            perguntas.buttons[12].imageinactive.loadImage("return1.png");
            perguntas._right = false;
            perguntas.setBuffer(20);
            perguntas.question3.init("Otari Medium.otf", 25);
            perguntas.setActive(false);
            introduction.setActive(true);
            introduction.mode = 1;
            introduction.lang = 3;
            introduction.timer = ofGetElapsedTimef();
        }

        if(perguntas.buttons[16].isClicked(x,y) && (perguntas.mode == 1 || perguntas.mode == 2))
        {
           perguntas.mode = 0;
            perguntas.timer = ofGetElapsedTimef();
            perguntas.buttons[12].imageinactive.loadImage("return1.png");
            perguntas._right = false;
            perguntas.setBuffer(20);
            perguntas.question3.init("Otari Medium.otf", 25);
        }
    }

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    introduction.create(subject);
    perguntas.create(subject);
    perguntas.setActive(false);
}

//----------------------------------------------------------


