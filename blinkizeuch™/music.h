#ifndef MUSIC_H
#define MUSIC_H

#include <inttypes.h>
#include <stdbool.h>

#include <libzeuch/gl.h>

typedef struct {
	int16_t* audio_data;
	int* note_data;
	float* envelope_data;
	int max_samples;
	int playback_position;
} music_t;

bool music_load(music_t* music);
void music_destroy(music_t* music);
bool music_open_audio(music_t* music);
void music_close_audio(void);
void music_play(bool play);
void music_set_time(music_t* music, int time);
int music_get_time(music_t* music);
void music_update_uniforms(music_t* const music, GLint uniform_envelopes, GLint uniform_notes);

#endif
