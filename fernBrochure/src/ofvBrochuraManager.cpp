/*
 *  ofvBrochuraManager.cpp
 *  fernmatch
 *
 *  Created by Freire on 12/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofvBrochuraManager.h"

void ofvBrochuraManager::setup(int i, int cameraWidth, int cameraHeight)
{
	camW = cameraWidth;
	camH = cameraHeight;
	
	
	
//	pag0.initTracker(modelPage0, camW, camH);
//	pag0.setMode('5');
	
	if (i == 1)
	{
		printf("Notice - Opening Fern Tracker\n");
        
		pag1.setup("brochura/pag1.jpg", camW, camH);
		//pag1.setMode('5');
	}
	
	else if (i == 2)
	{
		pag2.setup("brochura/pag2.jpg", camW, camH);
		//pag2.setMode('5');
	}
	
	else if (i == 3)
	{
		pag3.setup("brochura/pag3.jpg", camW, camH);
		//pag3.setMode('5');
	}
	
	else if (i == 4)
	{
		pag4.setup("brochura/pag4.jpg", camW, camH);
		//pag4.setMode('5');
	}
	else if (i == 5)
	{
		pag5.setup("brochura/pag5.jpg", camW, camH);
		//pag5.setMode('5');
	}
	
	else if (i == 6)
	{
		printf("Notice - Tracker opened\n");
		colorImg.allocate(camW, camH);
		img.allocate(camW, camH);
		state = BROCHURA_NONE;
		bPag5 = true;
		frames0 = 0;
		frames1 = 0;
		frames2 = 0;
		frames3 = 0;
		frames4 = 0;
		frames5 = 0;
		
		for (int i=0; i<4; i++)
		{
			ofPoint aux;
			aux = ofPoint(0,0);
			oldcorners.push_back(aux);
		}

	}
	
}

void ofvBrochuraManager::setup(string modelPage0, int cameraWidth, int cameraHeight)
{
	camW = cameraWidth;
	camH = cameraHeight;
	
	printf("Notice - Opening Fern Tracker\n");
	pag1.setup(modelPage0, camW, camH);
	//pag1.setMode('4');
	
	printf("Notice - Tracker opened\n");
	
	colorImg.allocate(camW, camH);
	img.allocate(camW, camH);
}


void ofvBrochuraManager::processFrames()
{
	colorImg.mirror(false, false);
	img = colorImg;
	img.convertToRange(30, 240);
	
	bool found = false;
	
	if(bPag1)
	{
		pag1.update(img);
		
		if( pag1.trackedMarker.tracked)
		{
            for(int i = 0; i < 4; i++)
            {
                corners.push_back(pag1.trackedMarker.points[i]);
            }
            updateCorners();
			//frames0 = 0;
			frames1++;		
			frames2 = 0;
			frames3 = 0;
			frames4 = 0;
			frames5 = 0;
			found = true;
			lastTimeSeen = ofGetElapsedTimef();		
		}
	}
	
	else if (bPag2)
	{
		pag2.update(img);
		
		if( pag2.trackedMarker.tracked )
		{
			//corners = pag2.getTrackedCorners();
			//updateCorners();
			//frames0 = 0;
			frames1 = 0;		
			frames2++;
			frames3 = 0;
			frames4 = 0;
			frames5 = 0;
			found = true;
			lastTimeSeen = ofGetElapsedTimef();
		}
	}
	
	else if (bPag3)
	{
		pag3.update(img);
	
		if( pag3.trackedMarker.tracked )
		{
			//corners = pag3.getTrackedCorners();	
			//updateCorners();
			//frames0 = 0;
			frames1 = 0;		
			frames2 = 0;
			frames3++;
			frames4 = 0;
			frames5 = 0;
			found = true;
			lastTimeSeen = ofGetElapsedTimef();
		}
	}		
	
	else if (bPag4)
	{
		pag4.update(img);
	
		if( pag4.trackedMarker.tracked )
		{
			//corners = pag4.getTrackedCorners();	
			//updateCorners();
			//frames0 = 0;
			frames1 = 0;		
			frames2 = 0;
			frames3 = 0;
			frames4++;
			frames5 = 0;
			found = true;
			lastTimeSeen = ofGetElapsedTimef();
		}
	}		
	
	else if (bPag5)
	{
		pag5.update(img);
		
		if( pag5.trackedMarker.tracked )
		{
			//corners = pag5.getTrackedCorners();	
			//updateCorners();
			//frames0 = 0;
			frames1 = 0;		
			frames2 = 0;
			frames3 = 0;
			frames4 = 0;
			frames5++;
			found = true;
			lastTimeSeen = ofGetElapsedTimef();
		}
	}
	/*
	else if (bPag0)
	{
		pag0.update(img);
		
		if( pag0.last_frame_ok ){
			corners = pag0.getTrackedCorners();			
			frames0++;
			frames1 = 0;		
			frames2 = 0;
			frames3 = 0;
			frames4 = 0;
			frames5 = 0;
			found = true;
			lastTimeSeen = ofGetElapsedTimef();
			//cout << "pag2"<<endl;
		}
	}*/
	
	if( state == BROCHURA_NONE && found == false )
	{
		if (bPag1)
		{
			bPag1 = false;
			bPag2 = true;
			bPag3 = false;
			bPag4 = false;
			bPag5 = false;
			//bPag0 = false;
		}
		else if (bPag2)
		{
			bPag1 = false;
			bPag2 = false;
			bPag3 = true;
			bPag4 = false;
			bPag5 = false;
			//bPag0 = false;
		}
		else if (bPag3)
		{
			bPag1 = false;
			bPag2 = false;
			bPag3 = false;
			bPag4 = true;
			bPag5 = false;
			//bPag0 = false;
		}
		else if (bPag4)
		{
			bPag1 = false;
			bPag2 = false;
			bPag3 = false;
			bPag4 = false;
			bPag5 = true;
			//bPag0 = false;
		}
		else if (bPag5)
		{
			bPag1 = true;
			bPag2 = false;
			bPag3 = false;
			bPag4 = false;
			bPag5 = false;
			//bPag0 = true;
		}
		/*
		else if (bPag0)
		{
			bPag1 = true;
			bPag2 = false;
			bPag3 = false;
			bPag4 = false;
			bPag5 = false;
			bPag0 = false;
		}*/
	}
	
	lastTimeSeenGoodMag = lastTimeSeen;
	flagChanged = false;
}

//-------------------------------------------------
void ofvBrochuraManager::updatePixels( unsigned char * pixels )
{
	
	colorImg = pixels;
    flagChanged = true;
		
	processFrames();
	
}

//----------------------------------------------
void ofvBrochuraManager::updateTracking(){
	
	float timeSinceMagSeen = ofGetElapsedTimef()-lastTimeSeenGoodMag;
	
	if( timeSinceMagSeen <= 1.0 ){
		
		if( state != BROCHURA_1 && frames1 > 10 ){
			state =BROCHURA_1;
			whichPageStr = "BROCHURA_1";
		}
		else if( state != BROCHURA_2 && frames2 > 10 ){
			state = BROCHURA_2;
			whichPageStr = "BROCHURA_2";			
		}
		else if( state != BROCHURA_3 && frames3 > 10 ){
			state = BROCHURA_3;
			whichPageStr = "BROCHURA_3";			
		}
		else if( state != BROCHURA_4 && frames4 > 10 ){
			state = BROCHURA_4;
			whichPageStr = "BROCHURA_4";			
		}
		else if( state != BROCHURA_5 && frames5 > 10 ){
			state = BROCHURA_5;
			whichPageStr = "BROCHURA_5";			
		}
    }
	else
	{
		if( timeSinceMagSeen > 2.0){
			state = BROCHURA_NONE;
			whichPageStr = "BROCHURA_NONE";						
		}	
	}
	
}

//--------------------------------------------------------------
GLfloat* ofvBrochuraManager::get_matrix()
{
	
	CvPoint2D32f cvsrc[4];
	cvsrc[0].x = 0;
	cvsrc[0].y = 0;
	cvsrc[1].x = ofGetWidth();
	cvsrc[1].y = 0;
	cvsrc[2].x = ofGetWidth();
	cvsrc[2].y = ofGetHeight();
	cvsrc[3].x = 0;
	cvsrc[3].y = ofGetHeight();	
	
	CvPoint2D32f cvdst[4];
	for(int i = 0; i < 4; i++){
		cvdst[i].x = corners[i].x;
		cvdst[i].y = corners[i].y;
	}
	
	CvMat* translate = cvCreateMat(3, 3, CV_32FC1);
	CvMat* src_mat = cvCreateMat( 4, 2, CV_32FC1 );
	CvMat* dst_mat = cvCreateMat( 4, 2, CV_32FC1 );
	
	//copy our points into the matrixes
	cvSetData( src_mat, cvsrc, sizeof(CvPoint2D32f));
	cvSetData( dst_mat, cvdst, sizeof(CvPoint2D32f));
	
	cvFindHomography(src_mat, dst_mat, translate);
	
	float *matrix = translate->data.fl;
	
	GLfloat* myMatrix;
	myMatrix = new GLfloat[16];
	
	//we set it to the default - 0 translation
	//and 1.0 scale for x y z and w
	for(int i = 0; i < 16; i++){
		if(i % 5 != 0) myMatrix[i] = 0.0;
		else myMatrix[i] = 1.0;
	}
	
	myMatrix[0]		= matrix[0];
	myMatrix[4]		= matrix[1];
	myMatrix[12]	= matrix[2];
	
	myMatrix[1]		= matrix[3];
	myMatrix[5]		= matrix[4];
	myMatrix[13]	= matrix[5];	
	
	myMatrix[3]		= matrix[6];
	myMatrix[7]		= matrix[7];
	myMatrix[15]	= matrix[8];
	
	return myMatrix;
}

void ofvBrochuraManager::debugDraw()
{
	ofSetColor(255,255,255);
	for (int i=0; i<4; i++)
	{
		ofCircle(corners[i].x, corners[i].y, 5);
	}
	
	/*//glMatrixMode( GL_MODELVIEW );
	glMultMatrixf(get_matrix());
	//glLoadMatrixf(get_matrix());
	glPushMatrix();
	ofSetColor(255, 0, 0);
	teste.draw(0,0);
	glPopMatrix();
	glLoadIdentity();*/
	
}

void ofvBrochuraManager::updateCorners()
{
	
	
	if (corners.size() == 4)
	{
		for (int i=0; i<4; i++)
		{
			oldcorners[i] += (corners[i] - oldcorners[i])/800;
			
			
		}
	}
}

ofPoint ofvBrochuraManager::getAngles()
{	
	if (corners.size() == 4)
	{
		ofxVec2f p2 = corners[1];
		ofxVec2f p1 = corners[0];
		ofxVec2f diff = p2 - p1;
	
		newangleZ = atan2(diff.y, diff.x) * RAD_TO_DEG;
	
		while(newangleZ > 360.0){
			newangleZ -= 360.0;
		}
		while(newangleZ < 0){
			newangleZ += 360.0;
		}
	
		float sideL		= ofxVec3f(corners[0]-corners[3]).length();
		float sideR		= ofxVec3f(corners[1]-corners[2]).length();
		float shortT	= ofxVec3f(corners[0]-corners[1]).length();
		float shortB	= ofxVec3f(corners[2]-corners[3]).length();
		
		if( sideR != 0.0 ){
			newangleY = ofMap( sideL / sideR, 0.8, 1.25, -50, 50, true);
		}
	
		if( shortT != 0.0 ){
			newangleX = ofMap( shortB / shortT, 0.8, 1.25, -50, 50, true);
		}		

//		ofLerpDegrees(newangleX, angleX, 0.17);
//		ofLerpDegrees(newangleY, angleX, 0.17);
//		ofLerpDegrees(newangleZ, angleX, 0.17);
		while(angleZ > 360.0){
			angleZ -= 360.0;
		}
		while(angleZ < 0){
			angleZ += 360.0;
		}
		
		angleX += (newangleX - angleX)/800;
		angleY += (newangleY - angleY)/800;
		angleZ += (newangleZ - angleZ)/800;
	
		ofPoint angle;
		angle.x = angleX;
		angle.y = angleY;
		angle.z = angleZ;
	
		return angle;
	}
}

void ofvBrochuraManager::reset()
{
	state = BROCHURA_NONE;
	bPag1 = true;
}

float ofvBrochuraManager::polygonArea(vector <ofPoint> & poly){
	
	if( poly.size() == 4 ){		
		return 0.5 * ( (poly[1].y-poly[3].y)*poly[0].x+(poly[2].y-poly[0].y)*poly[1].x+(poly[3].y-poly[1].y)*poly[2].x+(poly[0].y-poly[2].y)*poly[3].x );
	}
	
	return 0.0;
}

