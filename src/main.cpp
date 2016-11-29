#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
/***********************************************************************

THE EyeHarp: BY ZACHARIAS VAMVAKOUSIS
20/3/2011

***************************************************************************/

//========================================================================
int main( ){
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	int horizontal = desktop.right;
	int vertical = desktop.bottom;
	printf("%d %d\n", horizontal, vertical);
    ofAppGlutWindow window;
	
//	ofSetupOpenGL(&window, 1024,575, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(&window, 1366,740, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
