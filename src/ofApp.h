#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "MyGaze.h"
#include "tobii.h"
#include "EyeHarp.h"
#define GBS 1
#define GBE 1
#define STH 45
#define SM 0.95
/***********************************************************************

THE MOUSEHARP: BY ZACHARIAS VAMVAKOUSIS
20/3/2011

***************************************************************************/
enum eyetracker { EYETRIBE, TOBII, MOUSE};
class ofApp : public ofBaseApp {

	public:

		ofApp();
		~ofApp();
		void setup();
		void update();
		void draw();
        void audioRequested 	(float * output, int bufferSize, int nChannels);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

        int mousex;
        int mousey;
		bool gaze;
		bool firstRead;
		//----- scenes
		tobii myTobii;
		MyGaze tribe;
		EyeHarp                 HARP;
		bool sacadic;
		gtl::GazeData gdata;
		eyetracker tracker;
		ofPoint eyeSmoothed;
        ofSoundStream soundstream;

        int mouseDwell;
		void mySmooth();
		bool sacade();
		ofPoint smooth;
		bool help;
		ofPoint gbuffer[GBS+GBE];
		int bpos;
		ofPoint raw;
		int BS;
		FILE * read;
		
		FILE * record;
		ofPoint avgNew;
};

#endif
