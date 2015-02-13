#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "intro.h"
#include "question.h"

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
    void pacman(int resolution, float radius, float loaded);

	ofxXmlSettings XML;
	int page;

	intro introduction;
	questionPage perguntas;

	int timerExit, exitState, subject;
	ofPoint touchExit;

};

#endif
