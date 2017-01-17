#include "EyeHarp.h"

#define MINVOL 0.5
EyeHarp::~EyeHarp(){

}
void EyeHarp::setup(int discNotesNumber, int stepSequencerNotesNumber, bool chordsONOFF, bool showScaleInit, bool clickDwell,bool tomidi){
    //midiOut.listPorts();
	string s1("LoopBe");
	exit.setup("QUIT", false, ofPoint(1.1, 0.8), 0.1, 1000, 0, 0, 0, false);
	midiAvailable=false;
	for(int i=0;i<midiOut.getNumPorts();i++){
		string s2(midiOut.getPortName(i));
		if(s2.compare(0,6,s1)==0){
			midiOut.openPort(i);
			printf("Connected to LoopBe Vitrual Midi port: Sending melodies to channel 1 and arpeggios to Channel 2\n");
			midiAvailable=true;
		}
	}
	if(midiAvailable==false)
		printf("LoopBe Virtual Midi Port Not Found. Midi Out Disabled!\n");

    chord=chordsONOFF;
	eye.setup(&chord,Scale,&(configure.value), discNotesNumber,tomidi);

	// 0 input channels
	// 22050 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)

	sampleRate 			=  SAMPLERATE;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
	targetVolume		= 0.1f;
//	lAudio = new float[256];
//	rAudio = new float[256];
    note=0;
    octave=0;
    char** masterNames=new char*[3];
    for(int i=0;i<3;i++)
        masterNames[i]=new char[20];
    strcpy(masterNames[0],"Master\nVolume");
    strcpy(masterNames[1],"Tempo");
    strcpy(masterNames[2],"Trans\npose");
    ofPoint sliderPos=ofPoint(1.55,0);
    ofPoint stepPosUP=ofPoint(1.35,0.2);
    ofPoint stepPosDW=ofPoint(1.35,-.2f);
    masterMultiPlex.setup(3,masterNames,0,ofPoint(1.12,-0.4),HALF_PI,0.05,800);
    tempo.setup("Tempo",140,ofPoint(-1.25, -0.83),ofPoint(-1.61, -0.83),30.0f,600.0f,0.085f,300,5,.9f,.1f,.0f,true);
    masterVolume.setup("Volume",150, stepPosUP,stepPosDW,0,1,0.045f,500,20,.9f,.1f,.0f);
    transpose.setup("Transp",stepPosUP,stepPosDW,-5,6,0,1,0.045f,800,0.6f,0.2f,0.0f,false);
    tempoSlider.setup(sliderPos,tempo.value,tempo.min,tempo.max,0.7,true);
    masterVolumeSlider.setup(sliderPos,masterVolume.color, 0, 255, 0.7,true);
    transposeSlider.setup(sliderPos,transpose.value,transpose.min,transpose.max,0.7,true);
	focusPoints.setup("FocusPoints",false,ofPoint(1.6,0.85),0.08,800,.8,.4,0,false);
    Scale[0].setup("I",eye.harmonic[0].posUP_,eye.harmonic[0].posDW_,-1,1,0,1,eye.Msize,500,0.5,0.2,0.0);
    Scale[1].setup("II",eye.harmonic[1].posUP_,eye.harmonic[1].posDW_,1,3,2,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[2].setup("III",eye.harmonic[2].posUP_,eye.harmonic[2].posDW_,3,5,4,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[3].setup("IV",eye.harmonic[3].posUP_,eye.harmonic[3].posDW_,4,6,5,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[4].setup("V",eye.harmonic[4].posUP_,eye.harmonic[4].posDW_,6,8,7,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[5].setup("VI",eye.harmonic[5].posUP_,eye.harmonic[5].posDW_,8,10,9,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[6].setup("VII",eye.harmonic[6].posUP_,eye.harmonic[6].posDW_,10,12,11,1, eye.Msize,500,0.5,0.2,0.0);
    curMasterVolume=masterVolume.value;
    prChord=chord;
    char** names = new char*[2];
    for(int i=0;i<2;i++)
        names[i]=new char[20];
   
    layer.setup("Layer",0,ofPoint(1.48,-0.69),0.16,800,0.6,0.2,0.1,false);
//    loopNote.setup(60, 61, 0.2, 100, 1000);
	stepSeq.setup(Scale,&(transpose.value), stepSequencerNotesNumber,&tempo,700,1.0f,.3f,.1f, &sampleRate,&chord, &midiOut,&(configure.value));
    eye.arpInterface.setup(Scale,&(transpose.value),&chord,&(stepSeq.totalSamples),&sampleRate,&(stepSeq.curSample));
    randomChord.setup("Random\nChord",false,ofPoint(-1.6f,0.85f),0.05f,1000,0.6,0.2,0.1,false);
    fullScreen.setup("FullScreen",false,ofPoint(1.5f,0.9f),0.05f,1000,0.6,0.2,0.1,false);
    eye.disc.beat=&stepSeq.beat;
    eye.disc.noteFrames=&stepSeq.noteFrames;
	//if the interface starts with midi out
	/*masterVolume.setValueByColor(0);
	masterVolumeSlider.setValue(0);*/
	configure.setup("Setup",false,ofPoint(1.55,0),0.1,800,0.6,0.2,0.1,false);
	chordLoop.setup(&stepSeq,&chord,&(eye.disc.chordONOFF.value),&chordChanged);
	showScale.setup("ShowScale", showScaleInit, ofPoint(-1.5, -0.8), 0.1, 800, 0.5, 0.2, 0, false);
	if (tomidi) {
		masterVolume.setValueByColor(0);
		masterVolumeSlider.setValue(0);
	}
}

void EyeHarp::update(ofPoint Gaze,bool *sacadic){
	//cout<<startingFrame();
	
    chordChanged=false;
	
	prGaze=gaze;
    gaze=Gaze;
	
	configure.update(gaze,sacadic);
    volumeChanged=false;
    layer.update(gaze);
	
    //masterMultiPlex.update(gaze);
    //fullScreen.update(gaze);
    if(eye.timbrePresets.changed){
		if(eye.timbrePresets.selected==3){
			masterVolume.setValueByColor(0);
			masterVolumeSlider.setValue(0);
		}
		else{
			masterVolumeSlider.setValue(150);
			masterVolume.setValueByColor(masterVolumeSlider.value);
			if(midiAvailable){
				midiOut.sendNoteOff(MIDICH1, midinote, 0);
				stepSeq.releaseAllMidi(); 
			}
		}
	}

    if(fullScreen.changed)  ofToggleFullscreen();
    if(masterMultiPlex.selected==0){
        //masterVolume.update(gaze);
       // masterVolumeSlider.update(gaze);
        if(masterVolume.changed){
            masterVolumeSlider.setValue(masterVolume.color);
        }
        if(masterVolumeSlider.changed){
            masterVolume.setValueByColor(masterVolumeSlider.value);
        }
    }
    if(masterMultiPlex.selected==1){
        //tempo.update(gaze);
        tempoSlider.update(gaze);
        if(tempo.changed){
            tempoSlider.setValue(tempo.value);
        }
        if(tempoSlider.changed){
            tempo.setColorByValue(tempoSlider.value);
        }
    }
    if(masterMultiPlex.selected==2){
        transposeSlider.update(gaze);
        transpose.update(gaze);
        if(transposeSlider.changed){
            transpose.setColorByValue(transposeSlider.value);
        }
        if(transpose.changed){
            transposeSlider.setValue(transpose.value);
            stepSeq.updatePitch();
            eye.arpInterface.updateChords();
        }
    }

	velocity=ofDist(gaze.x,gaze.y,prGaze.x,prGaze.y)*FRAMERATE;//fixation detection

    
//    if (layer.selected==HARM)    harm.update(gaze);
	if (layer.value){
        targetVolume=0;
        distVol=0;
        stepSeq.update(gaze,sacadic,&velocity);
		if(configure.value){
			focusPoints.update(gaze,sacadic);
			tempo.update(gaze);
		}
		if(focusPoints.changed)
			Switch::focuspoints=!Switch::focuspoints;
		
		if(tempo.changed){
			stepSeq.updateTempo();
		   eye.arpInterface.updateTempo();
		}
        if(stepSeq.numberOfNotes.changed)   eye.arpInterface.updateTempo();
		midiOut.sendNoteOff(MIDICH1, midinote, 0);
    }
	else{
		eye.update(gaze, &velocity,sacadic);

		if (ofDist(gaze.x, gaze.y, width / 2, height / 2) < height / 2) {
			exit.setup("QUIT", false, ofPoint(1.1, 0.8), 0.1, 1000, 0, 0, 0, false);
			exit.resized(width, height);
		}
		if (configure.value) {
			showScale.update(gaze);
			exit.update(gaze);
			if (exit.changed) {
				if (exit.value)
					exit.setup("QUIT", true, ofPoint(1.55, 0.4), 0.1,1000, 0.5, 0, 0, false);
				else
					ofExit();
				exit.resized(width, height);
			}
		}
		else{
			if(showScale.value)
				for (int i = 0; i < 7; i++) {
					Scale[i].update(gaze);
					if (Scale[i].changed) {
						stepSeq.updatePitch();
						eye.arpInterface.updateChords();
					}
				}
		}
		chordLoop.update(Gaze);
		/*if (eye.disc.sharp45.changed) {
			if (eye.disc.sharp45.value) {
				Scale[3].value++;
				Scale[4].value++;
			}
			else {
				Scale[3].value--;
				Scale[4].value--;
			}
			stepSeq.updatePitch();
			eye.arpInterface.updateChords();
		}*/
		for(int i=0;i<stepSeq.numberOfNotes.value;i++)
		{
			for(int j=0;j<stepSeq.numberOfNotes.value;j++){
				stepSeq.seqNote[i][j].releaseOnTimeAfterChordChange();
			}
		}
        if(eye.advanced.value){
            if(eye.multiplex.selected==0)
                randomChord.update(gaze);
            if(eye.multiplex.selected==3){//the scale is common in all layers. So it had to be put inside the EyeHarp
                for(int i=0;i<7;i++){
                    Scale[i].update(gaze);
                    if(Scale[i].changed){
                        stepSeq.updatePitch();
                        eye.arpInterface.updateChords();
                    }
                }
            }
        }
        else{
            if(Scale[0].changed){
                stepSeq.updatePitch();
                eye.arpInterface.updateChords();
            }
        }
        if(eye.disc.notesONOFF.value){
            if(eye.disc.note>=0){
                if(eye.disc.chordONOFF.value){
                    if(eye.disc.note>=eye.disc.NotesNumber.value-CHORDSNUM){
//                        if(eye.disc.fixation.value || velocity<100 ){
                            if(eye.disc.note==eye.disc.NotesNumber.value-CHORDSNUM){ chord=5;}
                            if(eye.disc.note==eye.disc.NotesNumber.value-CHORDSNUM+1){ chord=1;}
                            if(eye.disc.note==eye.disc.NotesNumber.value-CHORDSNUM+2){ chord=4;}
                            if(eye.disc.note==eye.disc.NotesNumber.value-CHORDSNUM+3){ chord=0;}
                            if(eye.disc.note==eye.disc.NotesNumber.value-CHORDSNUM+4){ chord=3;}
                            if(eye.disc.note==eye.disc.NotesNumber.value-CHORDSNUM+5){ chord=6;}
                            if(eye.disc.note==eye.disc.NotesNumber.value-CHORDSNUM+6){ chord=2;}
                            chordChanged=true;
                            if(prChord!=chord){
                                stepSeq.updatePitch();
                                //eye.arpInterface.updateChords();
								chordLoop.addChord(chord);
                                prChord=chord;
                            }
//                        }
                    }
                    else{
						octave = (int)(eye.disc.note + 4) / 7;
                        note=Scale[(eye.disc.note+4)%7].value;
                        if(eye.disc.distVolume.value){
                            if(eye.disc.dist>eye.disc.neutralRegion && eye.disc.dist<ofGetHeight()/2 && (!eye.disc.fixation.value || velocity<100)){

                                if(prDist<eye.disc.dist){
                                    distVol=(MINVOL+(1-MINVOL)*((float)(eye.disc.dist-eye.disc.neutralRegion)/(height/2-eye.disc.neutralRegion)));
					
                                    targetVolume=eye.volume.value*distVol;
                                    volumeChanged=true;
                                }
                                else{
                                    if(velocity<100){
										distVol=(MINVOL+(1-MINVOL)*((float)(eye.disc.dist-eye.disc.neutralRegion)/(height/2-eye.disc.neutralRegion)));
										targetVolume=eye.volume.value*distVol;
                                        volumeChanged=true;
                                    }
                                }
                            }
                            else{
                                targetVolume=distVol*eye.volume.value;
                                volumeChanged=true;
                            }
                        }
                        else{
                            targetVolume=eye.volume.value;
                            volumeChanged=true;}
                    }
                }
                else{
                    octave=(int)(eye.disc.note+4)/7;
                    note=Scale[(eye.disc.note+4)%7].value;
                    if(eye.disc.distVolume.value){
                            if(eye.disc.dist>eye.disc.neutralRegion && eye.disc.dist<ofGetHeight()/2 ){

                                if(prDist<eye.disc.dist){
									distVol=(MINVOL+(1-MINVOL)*((float)(eye.disc.dist-eye.disc.neutralRegion)/(height/2-eye.disc.neutralRegion)));
                                    targetVolume=eye.volume.value*distVol;
                                    volumeChanged=true;
                                }
                                else{
                                    if(velocity<100){
                                        distVol=(MINVOL+(1-MINVOL)*((float)(eye.disc.dist-eye.disc.neutralRegion)/(height/2-eye.disc.neutralRegion)));
                                        targetVolume=eye.volume.value*distVol;
                                        volumeChanged=true;
                                    }
                                }
                            }
                            else    {
                                targetVolume=distVol*eye.volume.value;
                                volumeChanged=true;
                            }
                        }
                        else{
                            targetVolume=eye.volume.value;
                            volumeChanged=true;
                        }
                }
                prDist=eye.disc.dist;
                targetFrequency = pow(2,eye.oct+octave)*TUNING/8*pow(2,(note+transpose.value)/12.0);
                phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
            }
            else{
                targetVolume=0;
                volumeChanged=true;
            }
        }
        else if(eye.disc.note>=0 && !((eye.disc.note>=eye.disc.NotesNumber.value-7) && eye.disc.chordONOFF.value )){
            if(eye.disc.distVolume.value)            targetVolume=eye.volume.value*distVol;
            else    targetVolume=eye.volume.value;
            volumeChanged=true;
        }
    }
	int dist=ofDist(gaze.x,gaze.y,width/2,height/2);
	if(midiAvailable){
		if(eye.timbrePresets.selected==3 ){
			
			if(ofDist(gaze.x,gaze.y,eye.width2,eye.height2)>eye.disc.releaseDist && ofDist(gaze.x, gaze.y, eye.width2, eye.height2)<eye.disc.releaseDistEnd && velocity<100){
					midiOut.sendNoteOff(MIDICH1, midinote, 0);//if we look outside, release
				}
			if(volumeChanged && velocity<100){
				int tempVol=volume*200;
				if(tempVol<50)
					tempVol=50;
				if(tempVol>127)
					tempVol=127;
				midiOut.sendAftertouch(1,tempVol);
				midiOut.sendControlChange(1,2,tempVol);
				//midiOut.sendControlChange(1,1,targetVolume*128);
				midiOut.sendAftertouch(1,targetVolume*128);
				midiOut.sendControlChange(1,7,tempVol);
			}
			if(eye.disc.changed){
				midiOut.sendNoteOff(MIDICH1, midinote, 0);
				midinote=36+12*(octave+eye.octave.value)+note+transpose.value;
				/*float temp=ofGetFrameNum()-stepSeq.seqNote[0][0].beat;
				if(temp>stepSeq.seqNote[0][0].beatDist/2.0)
					temp-=stepSeq.seqNote[0][0].beatDist;
				cout<<midinote<<'\t'<<temp<<'\n';*/
				midiOut.sendNoteOn(MIDICH1, midinote, targetVolume*128);
				
	//            if(eye.disc.percussive.value)
	//                midiOut.sendNoteOff(1, midinote, 0);
			}
			/*if(volumeChanged))
				midiOut.sendControlChange(MIDICH1,7,targetVolume*128);*/
			stepSeq.sendMidi();
		}
		
	}
}

void EyeHarp::audioRequested 	(float * output, int bufferSize, int nChannels){
   // if(eye.timbrePresets.selected!=3){
        // sin (n) seems to have trouble when n is very large, so we
        // keep phase in the range of 0-TWO_PI like this:
        while (phase > TWO_PI){
            phase -= TWO_PI;
        }
        while (vibPhase > TWO_PI){
            vibPhase -= TWO_PI;
        }

        for (int i = 0; i < bufferSize; i++){
                curMasterVolume=0.99f*curMasterVolume+0.01f*masterVolume.value;
            if(eye.disc.percussive.value)   phaseAdder=phaseAdderTarget;
            else
                phaseAdder = eye.glissanto.value * phaseAdder + (1-eye.glissanto.value) * phaseAdderTarget;
        if(targetVolume>volume){//attack
                volume=(1-eye.attack.value)*volume+eye.attack.value*targetVolume;
                }
            if(targetVolume<volume){//release
                volume=eye.release.value*volume+(1-eye.release.value)*targetVolume;
            }
            phase += phaseAdder;
            vibPhase += eye.vibratoPhaseAdder;
            vibWidth = 0.9999*vibWidth+0.0001*(eye.vibratoW.value*distVol);
            float cosPhase=cos(vibPhase)*vibWidth*(phaseAdder*8);
            float sample=0;
            for(int  j=0;j<7;j++){
                float amplitude=pow(10.0f,eye.harmonic[j].value/10.0f);
                sample += amplitude*sin((j+1)*(phase+cosPhase));
            }
            sample*=volume;
            sample+=stepSeq.getSample();//
            //sample+=eye.arpInterface.getSample();

            output[i*nChannels+1]= sample*curMasterVolume;
            output[i*nChannels] = sample*curMasterVolume;
			chordLoop.getPos();
            if(stepSeq.curSample==0 && randomChord.value){
    //            int tempChord=rand()%100;
    //            if(tempChord<25) chord=0;
    //            if(tempChord>=25 && tempChord<42)   chord=3;
    //            if(tempChord>=42 && tempChord<59)   chord=4;
    //            if(tempChord>=59 && tempChord<73)   chord=1;
    //            if(tempChord>=73 && tempChord<86)   chord=5;
    //            if(tempChord>=86 && tempChord<95)   chord=2;
    //            if(tempChord>=95 && tempChord<100)   chord=6;
                chord=rand()%7;
                stepSeq.updatePitch();
                eye.arpInterface.updateChords();
            }
        }
   // }
//    else{
//        for (int i = 0; i < bufferSize; i++){
//            stepSeq.curSample=(stepSeq.curSample+1)%stepSeq.totalSamples;
//        }
//    }
}

void EyeHarp::draw(){
    if(!layer.value){
        eye.draw();
		chordLoop.draw();
		if (configure.value) {
			showScale.draw();
			exit.draw();
		}
		else {
			if (showScale.value)
				for (int i = 0; i<7; i++)
					Scale[i].draw();
		}
        if(eye.multiplex.selected==0 && eye.advanced.value)
            randomChord.draw();
    }
//    if(layer.selected==HARM)     harm.draw();
	else {
		
		stepSeq.draw();
		
		if(configure.value){
			tempo.draw();
			focusPoints.draw();
		}
	}
    layer.draw();
	configure.draw();
	
    //masterMultiPlex.draw();
    //fullScreen.draw();
    if(masterMultiPlex.selected==0){
       // masterVolume.draw();
       // masterVolumeSlider.draw();
    }
    if(masterMultiPlex.selected==1){
        //tempo.draw();
        tempoSlider.draw();
    }
    if(masterMultiPlex.selected==2){
        transpose.draw();
        transposeSlider.draw();
    }
	if (showCircle || (focusPoints.value && layer.value == 1)) {
		ofNoFill();
		ofCircle(gaze, height*0.076);
		ofFill();
		/*string mousPos = ofToString(gaze.x)+ " ," + ofToString(gaze.y);
		ofDrawBitmapString(mousPos, gaze);*/
	}

	/*char FR[20];
	itoa(ofGetFrameRate(),FR,10);
	ofSetColor(255,255,255);
	ofDrawBitmapString(FR,50,50);*/
}


void EyeHarp::resized(int w, int h){
	width=w;height=h;
    layer.resized(w,h);
    eye.resized(w,h);
    stepSeq.resized(w,h);
    tempo.resized(w,h);
    fullScreen.resized(w,h);
    masterVolume.resized(w,h);
    masterMultiPlex.resized(w,h);
    masterVolumeSlider.resized(w,h);
	chordLoop.resized(w,h);
    tempoSlider.resized(w,h);
    randomChord.resized(w,h);
    transpose.resized(w,h);
    transposeSlider.resized(w,h);
    for(int i=0;i<7;i++)        Scale[i].resized(w,h);
	focusPoints.resized(w,h);
	configure.resized(w,h);
	showScale.resized(w, h);
	exit.resized(w,h);
}


void EyeHarp::keyPressed(int key){
	eye.disc.keyPressed(key);
	stepSeq.keyPressed(key);
	chordLoop.keyPressed(key);
}
