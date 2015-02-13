
#include "testApp.h"
#include "ofMain.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( )
{

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1440,900, OF_FULLSCREEN);			// <-------- setup the GL context
//ofSetupOpenGL(&window, 800,600, OF_WINDOW);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}