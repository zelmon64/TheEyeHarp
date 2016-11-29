
#include "ofApp.h"
#include "stdio.h"
#include "EyeHarpVARS.h"


//--------------------------------------------------------------
ofApp::ofApp(){

}

ofApp::~ofApp(){
	fclose(record);
//	myTobii.~tobii();
	tribe.~MyGaze();
	HARP.~EyeHarp();
}

//--------------------------------------------------------------
void ofApp::setup(){
    int bufferSize		= 512;
//    glutSetCursor(GLUT_CURSOR_CROSSHAIR); 
    //glutSetCursor(GLUT_CURSOR_NONE);
	ofBackground(50,60,30);
	//---- setup standard application settings
    ofSetFrameRate(FRAMERATE);
	ofSetVerticalSync(true);
	//ofHideCursor();
	//ofToggleFullscreen();
	gaze=false;
	HARP.setup();
	tribe.setup();
	//myTobii.setup();
	
	ofSoundStreamListDevices();
	ofSoundStreamSetup(2, 0, SAMPLERATE, bufferSize, 4);
	//soundstream.setup(this, 2, 0, SAMPLERATE, bufferSize, 4);
	
	mouseDwell=0;
	printf("\nPress Esc to exit when the EyeHarp window is active or Ctrl+c in the current terminal\n");
	//showCircle=true;
//	for(float i=-5;i<6;i+=0.1)   cout<<i<<'\t'<<floor(i)<<'\n';
	
	smooth.x=0;
	smooth.y=0;
	bpos=0;
	tracker=MOUSE;
	sacadic=true;
	read=NULL;
	record=NULL;
	firstRead=false;
	help=true;
	FILE* conf;
	conf=fopen("configuration.txt", "r");
	float screenx, screeny;
	fscanf(conf,"%f %f", &screenx, &screeny);
	ratiox = ofGetScreenWidth() / screenx;
	ratioy = ofGetScreenHeight() / screeny;
	fclose(conf);
	prFixation = false;
}

//--------------------------------------------------------------
void ofApp::update(){
	if(read!=NULL && !feof(read)){
		int temp;
		fscanf(read,"%f,%f,%d,%d\n",&eyeSmoothed.x,&eyeSmoothed.y,&sacadic,&temp);
		if(firstRead){
			firstRead=false;
			HARP.stepSeq.curSample=temp;
		}
	}
	else{
		if (gaze) {
			if (tribe.m_api.is_connected()) {
				tracker = EYETRIBE;
				tribe.m_api.get_frame(gdata);
				//if (gdata.raw.x > ofGetWindowPositionX() && gdata.raw.x<ofGetWindowPositionX() + ofGetWidth() && gdata.raw.y>ofGetWindowPositionY() && gdata.raw.y < ofGetWindowPositionY() + ofGetHeight()) {
				

				if (gdata.state == 7){
					raw.x = gdata.raw.x*ratiox;
					raw.y = gdata.raw.y*ratioy;
				}

				mySmooth();
				int X = smooth.x - ofGetWindowPositionX();
				int Y = smooth.y - ofGetWindowPositionY();
				eyeSmoothed = ofPoint(X,Y);
				eyeSmoothed = ofPoint(smooth.x - ofGetWindowPositionX(), smooth.y - ofGetWindowPositionY());

			}
			else {
				cout << "did not connect to EyeTribe\n";
				eyeSmoothed = ofPoint( ofGetWindowPositionX(), ofGetWindowPositionY());
			}
		}
		/*else if(gaze &&  myTobii.success){
			tracker=TOBII;
			if(myTobii.eventParams.X>ofGetWindowPositionX() && myTobii.eventParams.X<ofGetWindowPositionX()+ofGetWidth() && myTobii.eventParams.Y>ofGetWindowPositionY() && myTobii.eventParams.Y<ofGetWindowPositionY()+ofGetHeight()){
				raw.x=myTobii.eventParams.X;
				raw.y=myTobii.eventParams.Y;
			}
			
			mySmooth();
			eyeSmoothed=ofPoint(smooth.x-ofGetWindowPositionX(),smooth.y-ofGetWindowPositionY());
			
		}*/
		else{
			sacadic=true;
			eyeSmoothed=ofPoint(mousex,mousey);
		}
		if(record!=NULL)
			fprintf(record,"%f,%f,%d,%d\n",eyeSmoothed.x,eyeSmoothed.y,sacadic,HARP.stepSeq.curSample);
	}
	HARP.update(eyeSmoothed,&sacadic);	
}


void ofApp::mySmooth(){
	//cout<<gdata.state<<'\n';
	if(tracker==TOBII || (tracker==EYETRIBE && gdata.state==7)){
		bool fix = fixation(); 
		if (fix && !prFixation) {
			smooth.x = avgNew.x;
			smooth.y = avgNew.y;
		}
		if(fix){
			smooth.x=smooth.x*SM+raw.x*(1-SM);
			smooth.y=smooth.y*SM+raw.y*(1-SM);
		}
		prFixation = fix;
	}
}

bool ofApp::fixation() {
	gbuffer[bpos].x = raw.x;
	gbuffer[bpos].y = raw.y;
	bool fix;
	int i;
	avgNew.x = 0; avgNew.y = 0;
	int prpos = bpos - 1;
	if (prpos < 0)
		prpos += DISPL;
	for (i = 0; i <DISPL ; i ++) {
		avgNew.x += gbuffer[i].x;
		avgNew.y += gbuffer[i].y;
	}
	avgNew.x /= (float)DISPL;
	avgNew.y /= (float)DISPL;
	fix = true;
	for (i = 0; i <DISPL; i++) {
		if (ofDist(gbuffer[i].x, gbuffer[i].y, avgNew.x, avgNew.y) > STH) {
			fix=false;
		}
	}
	bpos = (bpos + 1) % DISPL;
	return fix;
}


//--------------------------------------------------------------
void ofApp::draw(){


	//ofSetColor(255, 255, 255);
    HARP.draw();
	/*
	string temp="Mag:"+to_string(Switch::Mag)+"\nMagFac:"+to_string(Switch::MagFac)
		+"\nMagRegion:"+to_string(Switch::MagRegion)+"\nDwellZoom:"+to_string(stepSequencer::timeThre)
		+"\nMagDynamic:"+to_string(HARP.stepSeq.magDynamic);
	ofSetColor(255);
	ofDrawBitmapString(temp,ofGetWidth()*0.9,140);*/
	if(help && HARP.configure.value==false && HARP.layer.value==0){
		string temp="Press 'k' to switch between dwell time and mouse click selection method.\nPress 'f' to toggle fullscreen.\nPress 'm' to switch between mouse and gaze input.\nPress 'n' to show/hide the gaze pointer.\nPress 'h' to hide/show keyboard shortcuts.\nPress 'Esc' to exit.";
		ofSetColor(255);
		ofDrawBitmapString(temp,5,10);
	}
	/*string temp = to_string(mousex) + ',' + to_string(mousey);
	ofDrawBitmapString(temp,ofPoint(mousex-100,mousey));*/
	/*string temp = to_string(gdata.state);
	ofDrawBitmapString(temp, ofPoint(ofGetWidth()/2,ofGetHeight()/2)); */
}

void ofApp::audioRequested 	(float * output, int bufferSize, int nChannels){

        HARP.audioRequested 	(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
		case	'f':
		case	'F':
			ofToggleFullscreen();
			break;
        case '+':
            HARP.layer.value=!HARP.layer.value;
            break;
		case 'm':
		case 'M':
			gaze=!gaze;
			break;
		case 'n':
		case 'N':
			HARP.showCircle=!HARP.showCircle;
			break;
		/*case 'x':
		case 'X':
			Switch::focuspoints=!Switch::focuspoints;
			break;*/
		case 'G':
		case 'g':
			if(record==NULL){
				string filename=ofGetTimestampString()+".csv";
				record=fopen(filename.c_str(),"w");
			}
			else
				fclose(record);
			break;
		case 'H':
		case 'h':
			help=!help;
			break;
			/*if(read==NULL){
				read=fopen("performance.csv","r");
			}
			else
				fclose(read);
			break;*/
		case 'K':
		case 'k':
			Switch::click=!Switch::click;
			break;
		case 'z':
		case 'Z':
			Switch::Mag-=0.1;
			break;
		case 'a':
		case 'A':
			Switch::Mag+=0.1;
			break;
		case 's':
		case 'S':
			Switch::MagFac+=0.1;
			break;
		case 'x':
		case 'X':
			Switch::MagFac-=0.1;
			break;
		case 'c':
		case 'C':
			Switch::MagRegion-=10;
			break;
		case 'd':
		case 'D':
			Switch::MagRegion+=10;
			break;
		case 'v':
		case 'V':
			stepSequencer::timeThre-=10;
			break;
		case 'b':
		case 'B':
			stepSequencer::timeThre+=10;
			break;
		case 'j':
		case 'J':
			HARP.stepSeq.magDynamic=!HARP.stepSeq.magDynamic;
			break;
		//case 'l':
		//case 'L':
		//	//printf("%d, %d, %f, %f\n", ofGetScreenWidth(), ofGetScreenHeight(), eyeSmoothed.x, eyeSmoothed.y);
		//	printf("%d, %d, %f, %f\n", ofGetScreenWidth(), ofGetScreenHeight(), ofGetWindowWidth(), ofGetWindowHeight());
		//	//HARP.eye.disc.replaySameActive=!HARP.eye.disc.replaySameActive;
		//	//Switch::lenseSlave=!Switch::lenseSlave;

	}
	HARP.keyPressed(key);
	
}




//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mousex=x;
    mousey=y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	printf("xRaw: %f   yRaw: %f  Smooth: %f, %f\n, window: %d, %d\n", gdata.raw.x,  gdata.raw.y, eyeSmoothed.x, eyeSmoothed.y , ofGetWindowPositionX(), ofGetWindowPositionY());
	Switch::pressed=true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    if(w<1.777777778*h)
        HARP.resized(w,0.5625*w);
    else
        HARP.resized(1.777777778*h,h);
}


