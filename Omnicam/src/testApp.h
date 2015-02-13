#ifndef _TEST_APP
#define _TEST_APP




#include "ofMain.h"
#include "ofxNetwork.h"
#include "panel1.h"
#include "panel2.h"
#include "top.h"
#include "button.h"
#include "helperFunctions.h"


class testApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    void receive();
    void send(string msg);
    void initialize(vector <string> message);
    void save();
    void createstring();

    Panel1 panel1;
    Panel2 panel2;
    topMenu panelTop;

    ofxTCPClient tcpClient;
    string msgSend, msgReceive, msgExit;

    bool weConnected;

    int startFrame;

    ofFile file;

    ofImage back;

    int nconsoles, nomnicams;
    string tosendonexit;
    ofTrueTypeFont info;
    vector<float> help;


    vector <float> savedPositions1, savedPositions2;
    vector <bool> savedInverts1, savedInverts2;
    vector <int> savedSpeeds1, savedSpeeds2;

};

#endif
