/*
 *  FaceDetectionApp.h
 *  fernmatch
 *
 *  Created by Freire on 1/5/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFVFACEDETECTIONAPP
#define _OFVFACEDETECTIONAPP

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxCvHaarFinder.h"
#include "ofvFace.h"
#include "ofxXmlSettings.h"

class ofvFaceDetectionApp {
public:
	void setup(int _width, int _height);
	void update(unsigned char * pixels);
	void draw();
	
	void loadXmlSettings();
	void handleNewFaces();
	void createNewFace(float _x, float _y, float _width, float _height);
	void eraseDeadFaces();
	bool existFace(ofPoint* p1, ofPoint* p2);
	bool calcdist(ofPoint* p1, ofPoint* p2);
	
	int							width, height;
	float						scaleFactor;
	float						currentdelta;
	int							delta;
	
	int countdown, countdown2;
	int nhats, nbeards, nballoons, nframes, nfrases, type;
	vector <string> frases;
	vector <string> hats;
	vector <string> balloons;
	vector <string> beards;
	vector <string> frames;
	
	bool						bNewFrame;
	
	ofVideoGrabber				vidGrabber;
	
	ofxXmlSettings				settings;
	ofxCvHaarFinder				facefinder;
	ofxCvColorImage				colorImg;
	ofxCvGrayscaleImage			bwImg, smallImg;
	
	ofImage						glasses, hat, beard, balloon;
	
	vector <ofvFace*>			faces;
};

#endif
