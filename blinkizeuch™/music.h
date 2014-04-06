#ifndef MUSIC_H
#define MUSIC_H

#include <inttypes.h>
#include <stdbool.h>

typedef struct {
	int16_t* audio_data;
	int* note_data;
	float* envelope_data;
	int max_samples;
} music_t;

bool music_load(music_t* music);
void music_destroy(music_t* music);

#endif
