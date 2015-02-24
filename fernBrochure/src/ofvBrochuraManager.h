/*
 *  ofvBrochuraManager.h
 *  fernmatch
 *
 *  Created by Freire on 12/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxFern.h"
#include "ofxThread.h"
#include "ofxVectorMath.h"

typedef enum{
	BROCHURA_NONE,
	BROCHURA_0,
	BROCHURA_1,
	BROCHURA_2,
	BROCHURA_3,
	BROCHURA_4,
	BROCHURA_5,
}whichPage;

class ofvBrochuraManager {
	
public:
	
	void setup(int i, int cameraWidth, int cameraHeight);
	void setup(string modelPag0, int cameraWidth, int cameraHeight);

	
	void processFrames();
	void updatePixels(unsigned char * pixels);
	void updateTracking();
	void debugDraw();
	void updateCorners();
	float polygonArea(vector <ofPoint> & poly);	
	
	ofPoint getAngles();
	GLfloat* get_matrix();
	
	void reset();
	
	bool isPag0(){
		return (state == BROCHURA_0);
	}
	
	bool isPag1(){
		return (state == BROCHURA_1);
	}
	
	bool isPag2(){
		return (state == BROCHURA_2);
	}
	
	bool isPag3(){
		return (state == BROCHURA_3);
	}
	
	bool isPag4(){
		return (state == BROCHURA_4);
	}
	
	bool isPag5(){
		return (state == BROCHURA_5);
	}
	
	bool isNoBrochura(){
		return (state == BROCHURA_NONE);
	}

private:
	ofxFern pag0; //CAPA
	ofxFern pag1; //PAG1
	ofxFern pag2; //PAG2
	ofxFern pag3; //PAG3
	ofxFern pag4; //PAG4
	ofxFern pag5; //PAG5
	
	int camW, camH;
	
	whichPage state;
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage img;
	
	bool flagChanged;
	bool bPag1, bPag2, bPag3, bPag4, bPag5, bPag0;
	
	int frames1, frames2, frames3, frames4, frames5, frames0;
	int currentPage;
	
	
	
	float lastTimeSeen;
	float lastTimeSeenGoodMag;
	float area;
	
	string whichPageStr;
	vector <ofPoint> corners;
	
	//ANGLE VARIABLES
	ofPoint center;
	float angleX;
	float angleY;
	float angleZ;
	float newangleX;
	float newangleY;
	float newangleZ;
	vector <ofPoint> oldcorners;
};