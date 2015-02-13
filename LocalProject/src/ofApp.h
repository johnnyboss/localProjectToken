#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

struct token
{
    string name;
    string ref;
    int value;
    void change_val(int _val)
    {
        value += _val;
    }
};

struct relation
{
    string short_name;
    int yes_val;
    int no_val;
};

struct question
{
    int ID;
    string short_name;
    string text;
    vector<struct relation> relations;
};

struct button
{
    int x;
    int y;
    ofRectangle bound;
    
    string text;
    void create(int _x, int _y)
    {
        x = _x;
        y = _y;
        bound.x = _x;
        bound.y = _y;
        bound.width = 120;
        bound.height = 90;
    }
    void draw()
    {
        ofRectRounded(bound, 20);
    }
};

class ofApp : public ofBaseApp
{

public:

    void setup();
    void update();
    void drawToken(int _ID);
    void draw();
    void drawTokenArea(int _ID);
    int getTokenIndexByName(string ref);
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxXmlSettings XMLToken, XMLQuestion;
    ofTrueTypeFont TTF, token_descriptor;
    
    string xmlStructure;
    string message;
    
    vector <token> tokens;
    vector <question> questions;
    vector<ofRectangle> tokenArea;
    int currentQuestion;
    button yes_button, no_button;
    int game_status = 0;
    string win_str, loose_str;

};

