#include "ofApp.h"


//--------------------------------------------------------------
void testApp::setup(){
		
   	state = APP_JUST_LAUNCHED;
    ofSetFrameRate(60);
	mesa = 0;
	promo = 0;
	faceDetectionApp = 0;

	alpha2 = 0;
	valor = 0.5;
	frames_inicio = 255; //aparecer barra branca
	frames_loading = frames_inicio + 7; //fazer load aos ferns
	frames_begin_tracking = frames_loading + 255; //desaparecer a barra azul
	
	//vidGrabberSmall.allocate(320, 240);
	//vidGrabberBig.allocate(640, 480);
	
	background = new ofImage();
	background->loadImage("elements/imagens/background_brochura0.jpg");
	alpha = 255;
	helper.loadImage("elements/imagens/paginaBackground.jpg");
	cover.loadImage("elements/imagens/trigger.png");
}


//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
    bool bNewFrame = false;
	if( ofGetFrameNum() > 5 ){
		
		if(state == APP_JUST_LAUNCHED) //INICIALIZA O TRACKER DA BROCHURA
		{
			if( ofGetFrameNum() > frames_inicio && ofGetFrameNum() <= frames_begin_tracking )
			{
				load_trackers();
			}
			if (ofGetFrameNum() == frames_inicio)
			{
				vidGrabber.initGrabber(320, 240, true);
			}
		}
		else if (ofGetFrameNum() > frames_begin_tracking)
		{
            
			vidGrabber.update();
			bNewFrame = vidGrabber.isFrameNew();
			
			if( bNewFrame ){
				if (state != APP_PAGINA_5) //FAZ TRACKING DA BROCHURA
				{
					tracker.updatePixels(vidGrabber.getPixels());
			
					tracker.updateTracking();
					deal_with_tracker();
					
				}
				
				else //APP FACE DETECTION
				{
					if(faceDetectionApp)
					{
						faceDetectionApp->update(vidGrabber.getPixels());
					}
					else 
					{
						tracker.updatePixels(vidGrabber.getPixels());
						tracker.updateTracking();
						deal_with_tracker();
					}
				}
			}
		}
	}
}


//--------------------------------------------------------------
void testApp::draw(){
	//ofSetColor(255,255,255);
	//vidGrabber.draw(0,0);
	helper.draw(0,0, ofGetWidth(), ofGetHeight());
	draw_pagecontent();
	ofSetColor(0, 255, 0);
	ofDrawBitmapString(ofToString(ofGetFrameRate(),1) + "fps", 10, 20);
	ofSetColor(255,255,255);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	switch(key)
	{
			//add a gesture
		case 'q':
		{
			if(state == APP_PAGINA_5)
			{
				faceDetectionStillShot.grabScreen(0, 0, 1024, 768);
				faceDetectionStillShot.saveImage("color.jpg");
								
				delete faceDetectionApp;
				faceDetectionApp = 0;
				
				vidGrabber.close();
				vidGrabber.initGrabber(320, 240, true);
			}
		}
		break;
		case 'p':
		{
			if (promo)
			{
				if(promo->isPaused())
					promo->setPaused(false);
				else if(promo->isPlaying())
					promo->setPaused(true);
				else if(promo->getIsMovieDone())
					promo->play();
			}
		}

	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

void testApp::load_trackers()
{
	if (ofGetFrameNum()<frames_loading)
	{
		tracker.setup(ofGetFrameNum()-frames_inicio, 320, 240); 
	}
	else if (ofGetFrameNum() == frames_begin_tracking)
	{
		state = APP_RUN_NORMALLY;
		transitionFrame = 0;
	}
	//we do this to give the window time to go fullscreen
	//ofSetFullscreen(true);
}

void testApp::deal_with_tracker()
{
	if (tracker.isPag1() >= 1.0 && !mesa && state != APP_PAGINA_1)
	{
		if (promo)
		{
			delete promo;
			promo = 0;
		}
		mesa = new ofx3DModelLoader ();
		mesa->loadModel("elements/mesa/flip.3ds", 3);
		mesa->setRotation(0, 90, 1, 0, 0);
			
		background->loadImage("elements/imagens/background_brochura1.jpg");
		
		transitionFrame = ofGetFrameNum();
		alpha = 0;
			
		newstate = APP_PAGINA_1;
		state = APP_TRANSITION;
	}
	
	else if (tracker.isPag2() >= 1.0 || tracker.isPag3() >= 1.0 || tracker.isPag4() >= 1.0)
	{
		if (mesa)
		{
			delete mesa;
			mesa = 0;
		}
		
		if(!promo)
		{
			promo = new ofVideoPlayer ();
			if (tracker.isPag2())
			{
				background->loadImage("elements/imagens/pagina2.jpg");
				
				promo->loadMovie("elements/videos/parede.mp4");
                promo->play();
				
				newstate = APP_PAGINA_2;
			}
			else if (tracker.isPag3())
			{
				background->loadImage("elements/imagens/pagina3.jpg");
				
				promo->loadMovie("elements/videos/prelada.mp4");
				promo->play();
				newstate = APP_PAGINA_3;
			}
			else if (tracker.isPag4())
			{
				background->loadImage("elements/imagens/pagina4.jpg");
				
				promo->loadMovie("elements/videos/montra.mp4");
				promo->play();
				newstate = APP_PAGINA_4;
			}
			
			alpha=0;
			
			state = APP_TRANSITION;
			
			transitionFrame = ofGetFrameNum();
			
			promo->setLoopState(OF_LOOP_NONE);
			
		}
		
		else if    ((state == APP_PAGINA_2 && !tracker.isPag2() && !tracker.isNoBrochura()) 
				|| (state == APP_PAGINA_3 && !tracker.isPag3() && !tracker.isNoBrochura())
				|| (state == APP_PAGINA_4 && !tracker.isPag4() && !tracker.isNoBrochura()))
		{
			delete promo;
			promo = 0;
		}
		
		else if (state == APP_PAGINA_1 && !tracker.isPag1() && !tracker.isNoBrochura())
		{
			delete mesa;
			mesa = 0;
		}
	}
	
	else if (tracker.isPag5() >= 1.0)
	{
		
		if(!faceDetectionApp)
		{
			if(mesa)
			{
				delete mesa;
				mesa = 0;
			}
			
			if(promo)
			{
				delete promo;
				promo = 0;
			}
			
			vidGrabber.close();
			vidGrabber.initGrabber(640, 480, true);
			
			faceDetectionApp = new ofvFaceDetectionApp;
			faceDetectionApp->setup(640, 480);
			
			background->loadImage("elements/imagens/pagina5.jpg");
			
			newstate = APP_PAGINA_5;
			state = APP_TRANSITION;
			
			alpha = 0;
			transitionFrame = ofGetFrameNum();
		}
	}
	 
	if (state == APP_PAGINA_2 || state == APP_PAGINA_3 || state == APP_PAGINA_4)
	{
		if (promo)
		{
			if(!promo->isPaused())
			{
				//promo->play();
				promo->update();
			}
		}
	}
}

void testApp::draw_pagecontent()
{
	ofSetColor(255,255,255,alpha);
	background->draw(0, 0);
	if (state == APP_TRANSITION)
	{
		//if (ofGetFrameNum() - transitionFrame < 500)
		//	alpha+=0.51;
		//else
        alpha = 255;
			state = newstate;
			
		//if (promo)
		//{
		//	promo->stop();
		//}
	}
	
	else if (state == APP_RUN_NORMALLY)
	{
		if (alpha2 == 255)
			valor = -0.5;
		else if (alpha2 == 100)
			valor = .5;
		
		alpha2 = alpha2 +valor;
		ofSetColor(255,255,255,alpha2);
		cover.draw((ofGetWidth()-cover.width)/2, 530);
	}
	
	else if (state == APP_PAGINA_1 && mesa)
	{
		
		GLfloat lightOnePosition[] = {512, 0, 0.0, 1.0};
		GLfloat lightOneColor[] = {10.99, 10.99, 10.99, 0.0};		
		//ofSetColor(255, 255, 255);
		//some model / light stuff
		glEnable (GL_DEPTH_TEST);
		// glShadeModel (GL_SMOOTH);
		//glEnable(GL_LIGHTING);
		/* initialize lighting */
		glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
		glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
		glEnable (GL_LIGHT0);
		glEnable (GL_LIGHTING);
		// glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
		glEnable (GL_COLOR_MATERIAL);
		
		glPushMatrix();
		mesa->setPosition(512,400,0);
		if(tracker.isPag1())
		{
			ofPoint angles = tracker.getAngles();
			mesa->setRotation(1, angles.x, 1, 0, 0);
			mesa->setRotation(2, -angles.y, 0, 1, 0);
			//mesa.setRotation(3, angles.z, 0, 0, 1);
		}
		mesa->draw();
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}
	
	else if (state == APP_PAGINA_2 || state == APP_PAGINA_3 || state == APP_PAGINA_4)
	{
		if (promo)
		{
			if (promo->height == 480)
				promo->draw(192,120);
			else
				promo->draw(192,178);
		}
	}
	
	else if (state == APP_PAGINA_5)
	{
		if(faceDetectionApp)
		{
			faceDetectionApp->draw();
		}
		else 
		{

			faceDetectionStillShot.draw(0, 0);
		}

	}
	
	if (ofGetFrameNum() < frames_inicio) //APARECE BARRA BRANCA
	{
		ofSetColor(255,255,255, ofGetFrameNum());
		ofFill;
		ofRect((ofGetWidth()-447)/2, 519, 447 , 4);
	}
		
	if (ofGetFrameNum() >= frames_inicio && ofGetFrameNum() <frames_loading) 
	{
		ofSetColor(255,255,255,255);
		ofFill;
		ofRect((ofGetWidth()-447)/2, 519, 447 , 4);
		ofSetColor(51,165,221,255);
		ofRect((ofGetWidth()-447)/2, 519, 447*(ofGetFrameNum()-frames_inicio)/5, 4);
		
	}
	else if (ofGetFrameNum() > frames_loading && ofGetFrameNum() < frames_begin_tracking)
	{
		ofSetColor(51,165,221,255-(ofGetFrameNum()-frames_loading));
		ofFill;
		ofRect(289, 519, 447, 4);
	}
	
	if (state == APP_PAGINA_1 
	|| state == APP_PAGINA_2 
	|| state == APP_PAGINA_3 
	|| state == APP_PAGINA_4 
	|| (state == APP_PAGINA_5 && !faceDetectionApp))
		vidGrabber.draw(1024-110,768-60-20,80,60);
		
}
