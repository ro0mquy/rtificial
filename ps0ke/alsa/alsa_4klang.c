#include <stdio.h>
#include <pthread.h>
#include <alloca.h> // ALSA
#include <unistd.h> // sleep()
#include <string.h> // memcpy()
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include "4klang.h"
#define AUDIO_CHANNELS 2

void initAudio(int threaded);
void playAudio();
static void* __playAudio(void* arg);
void cleanup();

int main(){

	/* init stuff */

	initAudio(1);

	/* compile shader */
	sleep(1);

	playAudio();

	/* draw loop */
	sleep(180);

	cleanup();
	return 0;
}


static SAMPLE_TYPE audio_buffer[MAX_SAMPLES * AUDIO_CHANNELS];

static snd_pcm_t* alsa_handle;
static snd_pcm_uframes_t audio_frames;

void initAudio(int threaded) {
	// run 4klang rendering in a thread
	if (threaded) {
		pthread_t audio_render_thread;
		pthread_create(&audio_render_thread, NULL, __4klang_render, audio_buffer);
	} else {
		__4klang_render(audio_buffer);
	}

	// setup alsa
	int rounding_direction = 0;

	// open PCM device for playback
	snd_pcm_open(&alsa_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

	// allocate hardware parameters object with default values
	snd_pcm_hw_params_t* params;
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(alsa_handle, params);

	// set hardware parameters
	// interleaved mode
	snd_pcm_hw_params_set_access(alsa_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	// signed 16bit little endian
#	ifdef INTEGER_16BIT
		snd_pcm_hw_params_set_format(alsa_handle, params, SND_PCM_FORMAT_S16_LE);
#	else
#		error "4klang export sample type does not match alsa sample format"
#	endif
	// stereo
	snd_pcm_hw_params_set_channels(alsa_handle, params, AUDIO_CHANNELS);
	// 44.1 kb/s sampling rate
	unsigned int sampling_rate = SAMPLE_RATE;
	snd_pcm_hw_params_set_rate_near(alsa_handle, params, &sampling_rate, &rounding_direction);
	// set period size to 32 frames
	audio_frames = 32;
	snd_pcm_hw_params_set_period_size_near(alsa_handle, params, &audio_frames, &rounding_direction);
	// write parameters
	snd_pcm_hw_params(alsa_handle, params);
}

void playAudio() {
	pthread_t audio_play_thread;
	pthread_create(&audio_play_thread, NULL, __playAudio, NULL);
}

static void* __playAudio(void* arg) {
	size_t audio_buffer_size = audio_frames * AUDIO_CHANNELS;

	for (size_t audio_buffer_offset = 0;
			audio_buffer_offset < MAX_SAMPLES * AUDIO_CHANNELS;
			audio_buffer_offset += audio_buffer_size
		) {
		// write data directly from 4klang buffer to sound card
		size_t bytes_written = snd_pcm_writei(alsa_handle,
				audio_buffer + audio_buffer_offset,
				audio_frames
				);

		if (-EPIPE == bytes_written){
			// handle underrun
			snd_pcm_prepare(alsa_handle);
		}
	}

	// exit thread
	return NULL;
}

void cleanup() {
	// stop audio immediately
	snd_pcm_drop(alsa_handle);
	snd_pcm_close(alsa_handle);
}
