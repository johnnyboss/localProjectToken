#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxThread.h"
#include "ofxDirList.h"
#include "ofvBrochuraManager.h"
#include "ofx3DModelLoader.h"
#include "ofvFaceDetectionApp.h"


typedef enum{
	APP_JUST_LAUNCHED,
	APP_RUN_NORMALLY,
	APP_TRANSITION,
	APP_PAGINA_1,
	APP_PAGINA_2,
	APP_PAGINA_3,
	APP_PAGINA_4,
	APP_PAGINA_5,
}appState;

class testApp : public ofBaseApp, public ofxThread {

public:

	void setup();
	void update();
	void updateCapture();
	void draw();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	void load_trackers();
	void deal_with_tracker();
	void draw_pagecontent();
	
private:	
	ofVideoGrabber 		vidGrabber;
	ofxCvColorImage     vidGrabberSmall, vidGrabberBig;
	ofImage *background, helper, cover, faceDetectionStillShot;
	
	ofvFaceDetectionApp *faceDetectionApp;
	appState state, oldstate, newstate;
	
	unsigned char* facedetectionpixels;
	ofvBrochuraManager tracker;
	ofVideoPlayer *promo;
	ofx3DModelLoader *mesa;
	
	int transitionFrame;
	float alpha, alpha2, valor;
	int frames_inicio, frames_loading, frames_begin_tracking;
	
	
};

#endif
