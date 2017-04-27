#include "song.h"

void song::setup(int* Note, bool* Changed, bool* Flat) {
	note = Note;
	changed = Changed;
	flat = Flat;
	if(songNumber!=0)
		loadsong();
	pos = 0;
	ilumina = -1;
	/*for (int i = 0; i < 7; i++) {
		printf("%d\n", (scale+i)->value);
	}*/
}

void song::loadsong() {
	string ssong = to_string(songNumber) + ".sng";
	input = fopen(ssong.c_str(), "r");
	cout << "Loading file: " <<ssong.c_str() << endl;
	if (!input) {
		std::perror("File not found!\n");
	}
	else {
		fscanf(input, "%d", &transpose);
		for (int i = 0; i < 7; i++) {
			fscanf(input, "%d", &(scale[i].value));
		}
		while (!feof(input)) {
			NOTE temp; char flat='e';
			fscanf(input, "%d %d %c", &temp.octave, &temp.note, &flat);
			if (flat != 'e') {
				temp.flat = flat == 'b' ? true : false;
				melody.push_back(temp);
			}
		}
		/*for (int i = 0; i < melody.size(); i++) {
			printf("%d %d %c\n", melody[i].octave, melody[i].note, melody[i].flat ? 'b' : '_');
		}*/
	}
}

void song::update() {
	if (melody.size() > 0) {
		ilumina = melody[pos].octave * 7 + melody[pos].note - 5;
		//printf("[%d]", ilumina);
		if (*changed) {
			int octave, rnote;
			rnote = *note + 4;
			octave = rnote / 7;
			rnote = rnote % 7 + 1;
			if (octave == melody[pos].octave && rnote == melody[pos].note && *flat == melody[pos].flat) {
				pos = (pos + 1) % melody.size();
				//ilumina = melody[pos].octave * 7 + melody[pos].note - 4;
				//printf("ilumina: %d, note: %d, flat: %d, octave: %d, rnote: %d\n", ilumina, *note, *flat, octave, rnote);
			}

		}
	}
}