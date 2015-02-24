/*
 *  FaceDetectionApp.cpp
 *  fernmatch
 *
 *  Created by Freire on 1/5/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofvFaceDetectionApp.h"


void ofvFaceDetectionApp::setup(int _width, int _height)
{
	
	width = _width;
	height = _height;
	
	scaleFactor = 2;
	
	delta = 80;
	
 	loadXmlSettings();
	
	
	colorImg.allocate(width, height);
	bwImg.allocate(width, height);
	
	smallImg.allocate(width / scaleFactor, height / scaleFactor);
	
	facefinder.setup("elements/haarcascades/haarcascade_frontalface_alt2.xml");
	
}

//--------------------------------------------------------------
void ofvFaceDetectionApp::update(unsigned char* pixels)
{
	
	colorImg.setFromPixels(pixels, width, height);
	bwImg = colorImg;
		
	smallImg.scaleIntoMe(bwImg);
    facefinder.findHaarObjects(smallImg);
    //facefinder.findHaarObjects(smallImg, 10, 99999999, 10);
		
	handleNewFaces();
		
	for (int i=0; i < faces.size(); i++)
	{
		faces[i]->update();
	}
		
	if (faces.size() > 0)
	{
			
	}
	eraseDeadFaces();
	
}

//--------------------------------------------------------------
void ofvFaceDetectionApp::draw()
{
	colorImg.draw(192, 120);
	ofEnableAlphaBlending();
	for (int i=0; i<faces.size(); i++)
	{
		faces[i]->draw();
	}
	ofNoFill();
	
}


void ofvFaceDetectionApp::loadXmlSettings()
{
	settings.loadFile("elements/ARElements/settings.xml");
	
	type = settings.getValue("TYPE", 7);
	
	/*settings.pushTag("FRASES");
	
	nfrases = settings.getNumTags("FRASE");
	
	for (int i=0; i < nfrases; i++)
	{
		string aux;
		aux = settings.getValue("FRASE", "", i);
		frases.push_back(aux);
	}
	
	settings.popTag();*/
	
	settings.pushTag("IMAGES");
	
	nhats = settings.getNumTags("HAT");
	nbeards = settings.getNumTags("BEARD");
	nballoons = settings.getNumTags("BALLOON");
	//nframes = settings.getNumTags("FRAME");
	
	for (int i = 0; i < nhats; i++)
	{
		string aux;
		aux = settings.getValue("HAT", "", i);
		hats.push_back(aux);
	}
	
	for (int i = 0; i < nbeards; i++)
	{
		string aux;
		aux = settings.getValue("BEARD", "", i);
		beards.push_back(aux);
	}
	
	for (int i = 0; i < nballoons; i++)
	{
		string aux;
		aux = settings.getValue("BALLOON", "", i);
		balloons.push_back(aux);
	}
	
	/*for (int i = 0; i < nframes; i++)
	{
		string aux;
		aux = settings.getValue("FRAME", "", i);
		frames.push_back(aux);
	}*/
	
	
	settings.popTag();
	
	
}


void ofvFaceDetectionApp::handleNewFaces()
{
	for (int i=0; i < facefinder.blobs.size(); i++)
	{
		ofPoint* pointFinded = new ofPoint(facefinder.blobs[i].boundingRect.x, facefinder.blobs[i].boundingRect.y);
		if (faces.size() == 0) 
		{
			createNewFace(facefinder.blobs[i].boundingRect.x, facefinder.blobs[i].boundingRect.y,
						  facefinder.blobs[i].boundingRect.width, facefinder.blobs[i].boundingRect.height);
			return;
		}
		
		bool exists = false;
		int	 index = 0;
		for (int j = 0; j < faces.size(); j++)
		{
			ofPoint* existingPoint = new ofPoint(faces[j]->x, faces[j]->y);
			if (existFace(pointFinded, existingPoint))
			{
				exists = true;
				index = j;
				break;
			}
		}
		
		if(!exists)
		{
			createNewFace(facefinder.blobs[i].boundingRect.x, facefinder.blobs[i].boundingRect.y,
						  facefinder.blobs[i].boundingRect.width, facefinder.blobs[i].boundingRect.height);
		}
		else {
			faces[index]->updateFace(pointFinded, facefinder.blobs[i].boundingRect.width, 
									 facefinder.blobs[i].boundingRect.height, currentdelta);
		}
	}
}

bool ofvFaceDetectionApp::existFace(ofPoint* pointFinded, ofPoint* existingPoint)
{
	return calcdist(pointFinded, existingPoint);
}

void ofvFaceDetectionApp::createNewFace(float _x, float _y, float _width, float _height)
{
	ofvFace* face = new ofvFace();
	
	switch (type) {
		case 1:
			face->nameHat = hats[ofRandom(0, nhats)]; 
			break;
		case 2:
			face->nameBeard = beards[ofRandom(0, nbeards)]; 
			break;
		case 3:
			face->nameBalloon = balloons[ofRandom(0, nballoons)]; 
			face->nameFrase = frases[ofRandom(0, nfrases)];
			break;
		case 4:
			face->nameHat = hats[ofRandom(0, nhats)]; 
			face->nameBeard = beards[ofRandom(0, nbeards)];
			break;
		case 5:
			face->nameHat = hats[ofRandom(0, nhats)]; 
			face->nameBalloon = balloons[ofRandom(0, nballoons)];
			face->nameFrase = frases[ofRandom(0, nfrases)];
			break;
		case 6:
			face->nameBeard = beards[ofRandom(0, nbeards)]; 
			face->nameBalloon = balloons[ofRandom(0, nballoons)];
			face->nameFrase = frases[ofRandom(0, nfrases)];
			break;
		case 7:
			face->nameHat = hats[ofRandom(0, nhats)]; 
			face->nameBeard = beards[ofRandom(0, nbeards)]; 
			face->nameBalloon = balloons[ofRandom(0, nballoons)];
			//face->nameFrase = frases[ofRandom(0, nfrases)];
			break;
		case 8:
			face->nameHat = hats[nhats-1];
			break;
		default:
			break;
	}
	face->createFace(_x, _y, _width, _height, type, 2);
	faces.push_back(face);
}

void ofvFaceDetectionApp::eraseDeadFaces()
{
	for (int i=0; i < faces.size(); i++)
	{
		if(faces[i]->dead)
		{
			ofvFace* face = faces[i];
			faces.erase(faces.begin() + i);
			delete face;
		}
	}
}

//--------------------------------------------------------------
bool ofvFaceDetectionApp::calcdist(ofPoint* p1, ofPoint* p2)
{
	float aux1 = (p1->x - p2->x) * (p1->x - p2->x);
	float aux2 = (p1->y - p2->y) * (p1->y - p2->y);
	currentdelta = sqrt(aux1 + aux2);
	
	
	if (currentdelta < delta)
		return true;
	else
		return false;
}

