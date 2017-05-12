#include "EyeHarp.h"

//#define MINVOL 0.3
enum {
	MAJOR, MINOR, HITZAZ, HITZAZSKIAR, DORIAN, PHRYGIAN, MYXOLYDIAN
};
EyeHarp::~EyeHarp(){

}
void EyeHarp::setup(int discNotesNumber, int stepSequencerNotesNumber, bool chordsONOFF, bool showScaleInit, bool scalePreset, bool clickDwell,bool tomidi, bool LoopBeLoopMidi, bool semitoneActive, int trans,bool breath){
    //midiOut.listPorts();
	eye.disc.testSong.scale = Scale;
	string s1;
	string s2;
	MINVOL = 0.5;
	soundWorking = false;
	presetscale = false;
	presetscale = scalePreset;
	sequencerNote::sequencer_midi = 2;
	cc1 = 0; cc2 = 0; cc7 = 1; cc11 = 0; afterTouch = 0;
	sampleDIVframe = SAMPLERATE / FRAMERATE;
	if (LoopBeLoopMidi == false) {
		s1 = "LoopBe";
		s2 = "loopMI";
	}
	else
	{
		s1 = "loopMI";
		s2 = "LoopBe";
	}
	exit.setup("QUIT", false, ofPoint(1.1, 0.8), 0.1, 1000, 0, 0, 0, false);
	midiAvailable=false;
	midiOut.listPorts();
	for (int i = 0; i < midiOut.getNumPorts(); i++) {
		string s(midiOut.getPortName(i));
		if (s.compare(0, 6, s1) == 0) {
			midiOut.openPort(i);
			cout << "Connected to " << s << ". Sending melodies to channel 1 and arpeggios to Channel 2\n";
			midiAvailable = true;
			break;
		}
	}
	if (!midiAvailable) {
		for (int i = 0; i < midiOut.getNumPorts(); i++) {
			string s(midiOut.getPortName(i));
			if (s.compare(0, 6, s2) == 0) {
				midiOut.openPort(i);
				cout << "\nConnected to " << s << ". Sending melodies to channel 1 and arpeggios to Channel 2\n";
				midiAvailable = true;
				break;
			}
		}
	}
	if(midiAvailable==false)
		printf("Virtual Midi Port Not Found. Midi Out Disabled!\n");

	if (breath) {
		int port;
		midiIn.listPorts();
		printf("Select midi IN (breath sensor) device (number 0 to %d): ", midiIn.getNumPorts() - 1);
		cin >> port;
		if (port >= 0 && port < midiIn.getNumPorts())
			midiIn.openPort(port);
		else
			printf("Wrong number given!\n");
		printf("midi In connected\n");/*
		midiIn.addListener(this);
		midiIn.setVerbose(true);*/
	}
    chord=chordsONOFF;
	eye.setup(&chord,Scale,&(configure.value), discNotesNumber,tomidi,semitoneActive);

	// 0 input channels
	// 22050 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)

	sampleRate 			=  SAMPLERATE;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.99f;
	targetVolume		= 0.99f;
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
    tempo.setup("Tempo",140,ofPoint(-1.25, -0.83),ofPoint(-1.61, -0.83),30.0f,600.0f,0.085f,500,1,.9f,.1f,.0f,true);
    masterVolume.setup("Volume",150, stepPosUP,stepPosDW,0,1,0.045f,500,20,.9f,.1f,.0f);
    transpose.setup("Transp",  ofPoint(1.64, 0.4), ofPoint(1.32, 0.4), -5,6,trans,1,0.07f,800,0.6f,0.2f,0.0f,false);
    tempoSlider.setup(sliderPos,tempo.value,tempo.min,tempo.max,0.7,true);
    masterVolumeSlider.setup(sliderPos,masterVolume.color, 0, 255, 0.7,true);
    transposeSlider.setup(sliderPos,transpose.value,transpose.min,transpose.max,0.7,true);
	focusPoints.setup("FocusPoints",false,ofPoint(1.6,0.85),0.08,800,.8,.4,0,false);
	melody_midi = 1;
	melodyMidi.setup("midiChannel", ofPoint(-1.6, 0.2), ofPoint(-1.6, -0.2), 1, 16, melody_midi,1,0.09, 800, 0.6, 0.2, 0, false);
	sequencerMidi.setup("midiChannel", ofPoint(-1.6, 0.2), ofPoint(-1.6, -0.2), 1, 16, sequencerNote::sequencer_midi, 1, 0.09, 800, 0.6, 0.2, 0, false);
    Scale[0].setup("I",eye.harmonic[0].posUP_,eye.harmonic[0].posDW_,-1,1,0,1,eye.Msize,500,0.5,0.2,0.0);
    Scale[1].setup("II",eye.harmonic[1].posUP_,eye.harmonic[1].posDW_,1,3,2,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[2].setup("III",eye.harmonic[2].posUP_,eye.harmonic[2].posDW_,3,5,4,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[3].setup("IV",eye.harmonic[3].posUP_,eye.harmonic[3].posDW_,4,6,5,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[4].setup("V",eye.harmonic[4].posUP_,eye.harmonic[4].posDW_,6,8,7,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[5].setup("VI",eye.harmonic[5].posUP_,eye.harmonic[5].posDW_,8,10,9,1, eye.Msize,500,0.5,0.2,0.0);
    Scale[6].setup("VII",eye.harmonic[6].posUP_,eye.harmonic[6].posDW_,10,12,11,1, eye.Msize,500,0.5,0.2,0.0);
	eye.setup(&chord, Scale, &(configure.value), discNotesNumber, tomidi, semitoneActive);

	curMasterVolume=masterVolume.value;
    prChord=chord;
    /*char** names = new char*[2];
    for(int i=0;i<2;i++)
        names[i]=new char[20];*/
	char* musicalModesNames[7];
	musicalModesNames[0] = "Major";
	musicalModesNames[1] = "Minor";
	musicalModesNames[2] = "Hitzaz";
	musicalModesNames[3] = "Hitzaz\nskiar";
	musicalModesNames[4] = "Dorian";
	musicalModesNames[5] = "Phrygian";
	musicalModesNames[6] = "Myxolydian";
	musicalModes.setup(6, musicalModesNames, 0, ofPoint(-1.6, -1.15), HALF_PI, 0.082, 1000);

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
	showScale.setup("ShowScale", showScaleInit>0, ofPoint(-1.5, -0.8), 0.1, 800, 0.5, 0.2, 0, false);
	if (tomidi) {
		masterVolume.setValueByColor(0);
		masterVolumeSlider.setValue(0);
	}
	for (int i = 0; i < 7; i++) {
		eye.disc.scale[i] = &(Scale[i].value);
		//cout << *eye.disc.scale[i] << endl;
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
				midiOut.sendNoteOff(melody_midi, midinote, 0);
				stepSeq.releaseAllMidi(); 
			}
		}
	}

    if(fullScreen.changed)  ofToggleFullscreen();
	
    //if(masterMultiPlex.selected==0){
    //    //masterVolume.update(gaze);
    //   // masterVolumeSlider.update(gaze);
    //    if(masterVolume.changed){
    //        masterVolumeSlider.setValue(masterVolume.color);
    //    }
    //    if(masterVolumeSlider.changed){
    //        masterVolume.setValueByColor(masterVolumeSlider.value);
    //    }
    //}
    //else if(masterMultiPlex.selected==1){
    //    //tempo.update(gaze);
    //    tempoSlider.update(gaze);
    //    if(tempo.changed){
    //        tempoSlider.setValue(tempo.value);
    //    }
    //    if(tempoSlider.changed){
    //        tempo.setColorByValue(tempoSlider.value);
    //    }
    //}
    //else if(masterMultiPlex.selected==2){
    //    transposeSlider.update(gaze);
    //    transpose.update(gaze);
    //    if(transposeSlider.changed){
    //        transpose.setColorByValue(transposeSlider.value);
    //    }
    //    if(transpose.changed){
    //        transposeSlider.setValue(transpose.value);
    //        stepSeq.updatePitch();
    //        eye.arpInterface.updateChords();
    //    }
    //}

	velocity=ofDist(gaze.x,gaze.y,prGaze.x,prGaze.y)*FRAMERATE;//fixation detection

    
//    if (layer.selected==HARM)    harm.update(gaze);
	if (layer.value){//if in the stepSeq
		if (eye.timbrePresets.selected == 3 && !configure.value) {
			sequencerMidi.update(gaze);
			if (sequencerMidi.changed) {
				stepSeq.releaseAllMidi();
				sequencerNote::sequencer_midi = sequencerMidi.value;
			}
		}
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
		midiOut.sendNoteOff(melody_midi, midinote, 0);
    }
	else{//if in the melody layer
		eye.update(gaze, &velocity,sacadic);
		/*if (eye.disc.changed) {
			printf("%d%c\n", eye.disc.note, eye.disc.semi ? 'b' : ' ');
		}*/
		if (eye.timbrePresets.selected == 3 && !configure.value && !showScale.value) {
			melodyMidi.update(gaze);
			if (melodyMidi.changed) {
				midiOut.sendNoteOff(melody_midi, midinote, 0);
				melody_midi = melodyMidi.value;
			}
		}
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
			transpose.update(gaze);
			if (transpose.changed) {
				stepSeq.updatePitch();
				eye.arpInterface.updateChords();
			}
		}
		else{
			if (showScale.value) {
				if (presetscale) {
					musicalModes.update(gaze);
					if (musicalModes.changed) {
						switch (musicalModes.selected) {
						case MAJOR:
							eye.scale[0].value = 0;
							eye.scale[1].value = 2;
							eye.scale[2].value = 4;
							eye.scale[3].value = 5;
							eye.scale[4].value = 7;
							eye.scale[5].value = 9;
							eye.scale[6].value = 11;
							break;
						case MINOR:
							eye.scale[0].value = 0;
							eye.scale[1].value = 2;
							eye.scale[2].value = 3;
							eye.scale[3].value = 5;
							eye.scale[4].value = 7;
							eye.scale[5].value = 8;
							eye.scale[6].value = 11;
							break;
						case HITZAZ:
							eye.scale[0].value = 0;
							eye.scale[1].value = 1;
							eye.scale[2].value = 4;
							eye.scale[3].value = 5;
							eye.scale[4].value = 7;
							eye.scale[5].value = 8;
							eye.scale[6].value = 10;
							break;
						case HITZAZSKIAR:
							eye.scale[0].value = 0;
							eye.scale[1].value = 1;
							eye.scale[2].value = 4;
							eye.scale[3].value = 5;
							eye.scale[4].value = 7;
							eye.scale[5].value = 8;
							eye.scale[6].value = 11;
							break;
						case DORIAN:
							eye.scale[0].value = 0;
							eye.scale[1].value = 2;
							eye.scale[2].value = 3;
							eye.scale[3].value = 5;
							eye.scale[4].value = 7;
							eye.scale[5].value = 9;
							eye.scale[6].value = 10;
							break;
						case PHRYGIAN:
							eye.scale[0].value = 0;
							eye.scale[1].value = 1;
							eye.scale[2].value = 3;
							eye.scale[3].value = 5;
							eye.scale[4].value = 7;
							eye.scale[5].value = 8;
							eye.scale[6].value = 10;
							break;
						case MYXOLYDIAN:
							eye.scale[0].value = 0;
							eye.scale[1].value = 2;
							eye.scale[2].value = 4;
							eye.scale[3].value = 5;
							eye.scale[4].value = 7;
							eye.scale[5].value = 9;
							eye.scale[6].value = 10;
							break;
						}
						eye.scale[0].changed = true;
					}
				}
				else {
					for (int i = 0; i < 7; i++) {
						Scale[i].update(gaze);
						if (Scale[i].changed) {
							stepSeq.updatePitch();
							eye.arpInterface.updateChords();
						}
					}
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
                        note=Scale[(eye.disc.note+4)%7].value - eye.disc.semi;
                        if(eye.disc.distVolume.value){
                            if(eye.disc.dist>eye.disc.neutralRegion && eye.disc.dist<ofGetHeight()/2 && (!eye.disc.fixation.value || velocity<eye.disc.FIXVEL)){

                                if(prDist<eye.disc.dist){
                                    distVol=(MINVOL+(1-MINVOL)*((float)(eye.disc.dist-eye.disc.neutralRegion)/(height/2-eye.disc.neutralRegion)));
									if (eye.disc.semi)
										distVol = (distVol - 0.4)*1.4;
                                    targetVolume=eye.volume.value*distVol;
                                    volumeChanged=true;
                                }
                                else{
                                    if(velocity<eye.disc.FIXVEL){
										distVol=(MINVOL+(1-MINVOL)*((float)(eye.disc.dist-eye.disc.neutralRegion)/(height/2-eye.disc.neutralRegion)));
										if (eye.disc.semi)
											distVol = (distVol - 0.4)*1.4;
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
                    note=Scale[(eye.disc.note+4)%7].value-eye.disc.semi;
					//cout << "NOTE: " << note;
                    if(eye.disc.distVolume.value){
                            if(eye.disc.dist>eye.disc.neutralRegion && (eye.disc.dist<ofGetHeight()/2 || eye.disc.semi==1)){

                                if(prDist<eye.disc.dist){
									distVol=(MINVOL+(1-MINVOL)*((float)(eye.disc.dist-eye.disc.neutralRegion)/(height/2-eye.disc.neutralRegion)));
									if (eye.disc.semi)
										distVol = (distVol - 0.4)*1.4;
									//cout << distVol << endl;
                                    targetVolume=eye.volume.value*distVol;
                                    volumeChanged=true;
                                }
                                else{
                                    if(velocity<eye.disc.FIXVEL){
                                        distVol=(MINVOL+(1-MINVOL)*((float)(eye.disc.dist-eye.disc.neutralRegion)/(height/2-eye.disc.neutralRegion)));
										if (eye.disc.semi)
											distVol = (distVol - 0.4)*1.4;
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
			if(velocity<eye.disc.FIXVEL)
				if((eye.disc.inRelease && dist<eye.disc.neutralRegion && dist>eye.disc.inreleaseRegion)||(dist>eye.disc.releaseDist && dist<eye.disc.releaseDistEnd)  && !eye.disc.semi){
					midiOut.sendNoteOff(melody_midi, midinote, 0);//if we look outside, release
				}
			if(volumeChanged && velocity<eye.disc.FIXVEL && targetVolume!=0){
				int tempVol=targetVolume*200;
				if(tempVol<1)
					tempVol=1;
				if(tempVol>127)
					tempVol=127;
				if(cc1) midiOut.sendControlChange(melody_midi, 1, targetVolume * 128);
				if (cc2) midiOut.sendControlChange(melody_midi,2,tempVol);
				if (cc7) midiOut.sendControlChange(melody_midi, 7, tempVol);
				if (cc11) midiOut.sendControlChange(melody_midi, 11, tempVol);
				if (afterTouch) midiOut.sendAftertouch(melody_midi, tempVol);
			}
			if(eye.disc.changed){
				midiOut.sendNoteOff(melody_midi, midinote, 0);
				midinote=36+12*(octave+eye.octave.value)+note+transpose.value;
				/*float temp=ofGetFrameNum()-stepSeq.seqNote[0][0].beat;
				if(temp>stepSeq.seqNote[0][0].beatDist/2.0)
					temp-=stepSeq.seqNote[0][0].beatDist;
				cout<<midinote<<'\t'<<temp<<'\n';*/
				if(targetVolume!=0)
					midiOut.sendNoteOn(melody_midi, midinote, targetVolume*128);
				
	//            if(eye.disc.percussive.value)
	//                midiOut.sendNoteOff(1, midinote, 0);
			}
			/*if(volumeChanged))
				midiOut.sendControlChange(melody_midi,7,targetVolume*128);*/
			stepSeq.sendMidi();
		}
		
	}
	if (!soundWorking) {
		for (int i = 0; i < sampleDIVframe; i++) {
			stepSeq.getSample();
			chordLoop.getPos();
		}
	}
}

void EyeHarp::audioRequested 	(float * output, int bufferSize, int nChannels){
   // if(eye.timbrePresets.selected!=3){
        // sin (n) seems to have trouble when n is very large, so we
        // keep phase in the range of 0-TWO_PI like this:
	/*cout << "." << endl;*/
	soundWorking = true;
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
			transpose.draw();
		}
		else {
			if (showScale.value) {
				if (presetscale) {
					musicalModes.draw();
				}
				else {
					for (int i = 0; i < 7; i++)
						Scale[i].draw();
				}
			}
			if (eye.timbrePresets.selected == 3 && !showScale.value)
				melodyMidi.draw();
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
		else {
			sequencerMidi.draw();
		}
	}
    layer.draw();
	configure.draw();
	
    //masterMultiPlex.draw();
    //fullScreen.draw();
    //if(masterMultiPlex.selected==0){
    //   // masterVolume.draw();
    //   // masterVolumeSlider.draw();
    //}
    //if(masterMultiPlex.selected==1){
    //    //tempo.draw();
    //    tempoSlider.draw();
    //}
    //if(masterMultiPlex.selected==2){
    //    transpose.draw();
    //    transposeSlider.draw();
    //}
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
	musicalModes.resized(w, h);
	melodyMidi.resized(w, h);
	sequencerMidi.resized(w, h);
}


void EyeHarp::keyPressed(int key){
	eye.disc.keyPressed(key);
	stepSeq.keyPressed(key);
	chordLoop.keyPressed(key);
}

void EyeHarp::newMidiMessage(ofxMidiMessage& msg) {
	// make a copy of the latest message
	midiMessage = msg;
	int sensitivity = 3;
	int thr = 50;
	if (msg.status == MIDI_CONTROL_CHANGE || msg.status == MIDI_AFTERTOUCH || msg.status == MIDI_POLY_AFTERTOUCH) {
		breath = msg.value * 3;
		if (breath > 127)
			breath = 127;
		if (breath < thr) {
			midiOut.sendNoteOff(1, eye.disc.note, 0);
			
		}
		else {
			midiOut.sendControlChange(1, 7, breath);
				midiOut.sendNoteOn(1, eye.disc.note, breath);
		}
	}
	
}
