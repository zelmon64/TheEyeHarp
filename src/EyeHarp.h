#ifndef _EYEHARP
#define _EYEHARP

#include "EyeHarpVARS.h"
#include "ofMain.h"
#include "eye.h"
#include "DistinctControl.h"
#include "XORswitch.h"
#include "stepSequencer.h"
#include "control.h"
#include "switch.h"
#include "slider.h"
#include "EyeHarpVARS.h"
#include "ofxMidi.h"
#include "recordChords.h"
//#include <vrpn_Button.h>
//#include <vrpn_Analog.h>

/***********************************************************************

THE MOUSEHARP: BY ZACHARIAS VAMVAKOUSIS
20/3/2011

***************************************************************************/

enum{

	EYE, STEPSEQ

};


class EyeHarp{
    public:
        Eye eye;
//        arpeggiator * arpeggio;
//    stepSeq stepseq;
        void setup(int discNotesNumber, int stepSequencerNotesNumber, bool chordsONOFF, bool showScale,bool scalePreset, bool clickDwell,bool tomidi, bool LoopBeLoopMidi, bool semitoneActive, int trans,bool breath);
		void update(ofPoint Gaze,bool*sacadic);
		void draw();
		void resized(int w, int h);
		void audioRequested 	(float * input, int bufferSize, int nChannels);
		void keyPressed  (int key);
		void newMidiMessage(ofxMidiMessage& msg);
		void exitApp();
		bool cc1, cc2, cc7, cc11, afterTouch;
		float MINVOL;
        int numArp;
        int chord;
		bool soundWorking;
		int sampleDIVframe;
//        loopedNote loopNote;
        stepSequencer stepSeq;
        Switch layer;
		int width;
		int height;
        ofxMidiOut midiOut;
		ofxMidiIn midiIn;
		ofxMidiMessage midiMessage;
		int breath;
        int midinote;
		int melody_midi;//midi channels
		bool showCircle;
		Switch exit;
		Switch focusPoints;
		Switch configure;
		Switch showScale;
		recordChords chordLoop;
		bool presetscale; //0 if we want to the user to control all seperate semitones. 1 if we want to desplay presets.
//        vrpn_Analog_Remote* VRPNAnalog;
		xorSwitch musicalModes;
		DistControl melodyMidi;
		DistControl sequencerMidi;
		DistControl Scale[7];
        ~EyeHarp();
    private:
        bool chordChanged;
        bool volumeChanged;
        xorSwitch masterMultiPlex;
        Switch randomChord;
        Switch fullScreen;
        control tempo;
        control masterVolume;
        slider tempoSlider;
        slider masterVolumeSlider;
        slider transposeSlider;
		int		sampleRate;
		float 	volume;
		float   targetVolume;
		//------------------- for the simple sine wave synthesis
		float 	targetFrequency;
		float 	phase;
		float 	phaseAdder;
		float 	phaseAdderTarget;
		float   vibPhase;
		float   vibWidth;//vibrato Width
        float distVol;
        float prDist;

		ofPoint gaze;
		ofPoint prGaze; //the previous frame gaze point;4
		float velocity;//the velocity of the gaze;
		int note;
		int octave;
		bool midiAvailable;
        float curMasterVolume;
        DistControl transpose;
        int prChord;
};




#endif
