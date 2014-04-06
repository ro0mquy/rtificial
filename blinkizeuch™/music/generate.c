#include "4klang.inh"
#include <inttypes.h>
#include <stdio.h>

SAMPLE_TYPE buffer[MAX_SAMPLES * 2];

int main() {
	__4klang_render(buffer);
	FILE* const audio_file = fopen("audio.out", "w");
	if(audio_file != NULL) {
		fwrite(buffer, sizeof(SAMPLE_TYPE), MAX_SAMPLES * 2, audio_file);
		fclose(audio_file);
	} else {
		fprintf(stderr, "Could not open audio out file\n");
	}
	FILE* const note_file = fopen("notes.out", "w");
	if(note_file != NULL) {
		fwrite(&__4klang_note_buffer, sizeof(int), MAX_SAMPLES / 8, note_file);
		fclose(note_file);
	} else {
		fprintf(stderr, "Could not open notes out file\n");
	}
	FILE* const envelope_file = fopen("envelopes.out", "w");
	if(envelope_file != NULL) {
		fwrite(&__4klang_envelope_buffer, sizeof(float), MAX_SAMPLES / 8, envelope_file);
		fclose(envelope_file);
	} else {
		fprintf(stderr, "Could not open envelopes out file\n");
	}
	FILE* const maxsamples_file = fopen("maxsamples.out", "w");
	if(maxsamples_file != NULL) {
		fprintf(maxsamples_file, "%d\n", MAX_SAMPLES);
		fclose(maxsamples_file);
	} else {
		fprintf(stderr, "Could not open maxsamples out file\n");
	}

	return 0;
}
