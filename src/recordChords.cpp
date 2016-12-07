#include "recordChords.h"

void recordChords::setup(stepSequencer* StepSeq, int*Chord, bool *ShowChords, bool *Cchange) {
	stepSeq = StepSeq;
	barCount = 0;
	curPos = 0;
	state = 0;
	pos = 0;
	chord = Chord;
	showChords = ShowChords;
	button.setup("RecLoop", false, ofPoint(1.55, 0.8), 0.1, 1000, 0.6, 0.2, 0.1, false);
	cchanged = Cchange;
}

void recordChords::update(ofPoint gaze) {
	button.update(gaze);
	if (button.changed) {
		state = (state + 1) % 3;
		switch (state) {
		case PAUSE:
			button.setup("PRESS TO RECORD", false, ofPoint(1.55, 0.8), 0.1, 1000, 0.6, 0.2, 0.1, false);
			*showChords = true;
			break;
		case RECORD:
			button.setup("RECORDING...\nPRESS TO STOP", false, ofPoint(1.55, 0.8), 0.1, 1000, 0.6, 0.2, 0.1, false);
			loop.clear();
			barCount = 0; 
			record temp;
			temp.bar = 0;
			temp.chord = *chord;
			temp.sampleCount = 0;
			loop.push_back(temp);
			*showChords = true;
			break;
		case PLAY:
			if (loop[loop.size() - 1].sampleCount > stepSeq->noteSamples*(stepSeq->numberOfNotes.value - 2)) {
				record temp;
				temp.bar = loop[loop.size() - 1].bar+1;
				temp.chord = *chord;
				temp.sampleCount = 0;
				loop.push_back(temp);
			}
			printVector();
			pos = 0;
			barCount = 0;
			button.setup("PLAYING...\nPRESS TO STOP", false, ofPoint(1.55, 0.8), 0.1, 1000, 0.6, 0.2, 0.1, false);
			*showChords = false;
			break;
		}
	}
	if (state == PLAY) {
		//printf("%d: %d, %d\n", pos, barCount, loop[pos].bar);
		if (barCount == loop[pos].bar && stepSeq->curSample >= loop[pos].sampleCount) {
			*chord = loop[pos].chord;
			*cchanged = true;
			stepSeq->updatePitch();
			pos = (pos + 1);
			if (pos == loop.size()) {
				barCount = 0;
				pos = 0;
			}
		}
	}
}

void recordChords::getPos() {
	if (stepSeq->curSample % stepSeq->noteSamples == 0) {
		if (stepSeq->curSample == 0)
			barCount++;
		//printf("%d\t%d\n", barCount,*curSample / *noteSamples);
	}
}

void recordChords::addChord(int Chord) {
	if (state == RECORD) {
		record temp;
		temp.bar = barCount;
		temp.chord = Chord;
		temp.sampleCount = stepSeq->curSample;
		loop.push_back(temp);
	}
}

void recordChords::printVector() {
	for (auto i = 0; i < loop.size(); ++i)
		printf("%d\t%d\t%d\n", loop[i].bar, loop[i].sampleCount, loop[i].chord);
}

void recordChords::keyPressed(int Key) {
	if(Key=='$')
		printVector();
}

void recordChords::resized(int W, int H) {
	button.resized(W, H);
}


void recordChords::draw() {
	button.draw();
}