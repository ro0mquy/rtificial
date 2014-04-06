#include <stdio.h>
#include <stdlib.h>

#include "music.h"
#include "util.h"

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

	return true;
}

void music_destroy(music_t* const music) {
	util_safe_free(music->audio_data);
	util_safe_free(music->note_data);
	util_safe_free(music->envelope_data);
}
