#pragma once

#include "ofMain.h"
#include "DistinctControl.h"
typedef struct {
	int note;
	int octave;
	bool flat;
}NOTE;

class song {
public:
	void setup(int* note, bool* changed, bool* flat);
	void update();
	FILE* input;
	void loadsong();
	int transpose;
	DistControl* scale;
	vector<NOTE> melody;
	bool* changed;
	int* note;
	bool* flat;
	int pos;
	int ilumina;
	int songNumber;
};