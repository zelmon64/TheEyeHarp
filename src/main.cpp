#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
/***********************************************************************

THE EyeHarp: BY ZACHARIAS VAMVAKOUSIS
20/3/2011

***************************************************************************/

//========================================================================
int main( ){
	
    ofAppGlutWindow window;
	
//	ofSetupOpenGL(&window, 1024,575, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(&window, 1366,740, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
