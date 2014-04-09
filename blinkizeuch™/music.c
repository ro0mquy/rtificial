#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "music.h"
#include "util.h"

static void fill_audio(void* userdata, Uint8* stream, int len);

bool music_load(music_t* const music) {
	music->audio_data = NULL;
	music->note_data = NULL;
	music->envelope_data = NULL;

	FILE* const maxsamples_file = fopen("music/maxsamples.out", "r");
	if(maxsamples_file == NULL) {
		perror(NULL);
		return false;
	}
	fscanf(maxsamples_file, "%d\n", &music->max_samples);
	fclose(maxsamples_file);

	music->audio_data = malloc(music->max_samples * 2 * sizeof(int16_t));
	music->note_data = malloc(music->max_samples / 8 * sizeof(int));
	music->envelope_data = malloc(music->max_samples / 8 * sizeof(float));

	FILE* const audio_file = fopen("music/audio.out", "r");
	if(audio_file == NULL) {
		perror(NULL);
		music_destroy(music);
		return false;
	}
	fread(music->audio_data, sizeof(int16_t), music->max_samples * 2, audio_file);
	fclose(audio_file);
	FILE* const note_file = fopen("music/notes.out", "r");
	if(note_file == NULL) {
		perror(NULL);
		music_destroy(music);
		return false;
	}
	fread(music->note_data, sizeof(int), music->max_samples / 8, note_file);
	fclose(note_file);
	FILE* const envelope_file = fopen("music/envelopes.out", "r");
	if(envelope_file == NULL) {
		perror(NULL);
		music_destroy(music);
		return false;
	}
	fread(music->envelope_data, sizeof(float), music->max_samples / 8, envelope_file);
	fclose(envelope_file);

	music->playback_position = 0;
	return true;
}

void music_destroy(music_t* const music) {
	util_safe_free(music->audio_data);
	util_safe_free(music->note_data);
	util_safe_free(music->envelope_data);
}

bool music_open_audio(music_t* const music) {
	SDL_AudioSpec wanted = {
		.freq = 44100,
		.format = AUDIO_S16LSB,
		.channels = 2,
		.samples = 1024, // maybe this is a good value?
		.callback = fill_audio,
		.userdata = music
	};
	if(SDL_OpenAudio(&wanted, NULL) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void music_close_audio(void) {
	SDL_CloseAudio();
}

void music_play(bool play) {
	SDL_PauseAudio(!play);
}

void music_set_time(music_t* const music, int time) {
	int position = (int64_t) time * 44100 / 1000;
	SDL_LockAudio();
	music->playback_position = position;
	SDL_UnlockAudio();
}

int music_get_time(music_t* const music) {
	SDL_LockAudio();
	int position = music->playback_position;
	SDL_UnlockAudio();
	return position * 1000 / 44100;
}

void music_update_uniforms(music_t* const music, GLint uniform_envelopes, GLint uniform_notes) {
	SDL_LockAudio();
	int position = music->playback_position;
	SDL_UnlockAudio();
	int index = position * 32 / 256;
	glUniform1fv(uniform_envelopes, 32, &music->envelope_data[index]);
	glUniform1iv(uniform_notes, 32, &music->note_data[index]);
}

static void fill_audio(void* userdata, Uint8* stream, int len) {
	music_t* const music = (music_t*) userdata;
	int samples = len / 4; // samples are 2 bytes, 2 samples for stereo
	int write_len = len;
	if(samples + music->playback_position > music->max_samples) {
		write_len = 4 * (music->max_samples - music->playback_position);
	}
	memcpy(stream, &music->audio_data[music->playback_position * 2], write_len);
	memset(stream + write_len, 0, len - write_len);
	music->playback_position += write_len / 4;
}
