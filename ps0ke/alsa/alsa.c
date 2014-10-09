#include <stdio.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

int main(){
	int rounding_direction = 0;

	// open PCM device for playback
	snd_pcm_t *handle;
	snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

	// allocate hardware parameters object with default values
	snd_pcm_hw_params_t *params;
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(handle, params);

	// set hardware parameters
	// interleaved mode
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	// signed 16bit little endian
	snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
	// stereo
	snd_pcm_hw_params_set_channels(handle, params, 2);
	// 44.1 kb/s sampling rate
	unsigned int sampling_rate = 44100;
	snd_pcm_hw_params_set_rate_near(handle, params, &sampling_rate, &rounding_direction);
	// set period size to 32 frames
	snd_pcm_uframes_t frames = 32;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &rounding_direction);
	// write parameters
	snd_pcm_hw_params(handle, params);

	// allocate sound buffer: 2 bytes/sample, 2 channels
	const size_t size = frames * 2 * 2;
	char *buffer = (char *) malloc(size);

	// set loop time to 5s
	int period_time;
	snd_pcm_hw_params_get_period_time(params, (unsigned int*) &rounding_direction, &period_time);
	long loops = 5. * 1000. / (float) period_time;

	// play loop
	while (loops > 0){
		loops--;

		// read from stdin
		size_t bytes_read = read(0, buffer, size);
		if (0 == bytes_read){
			fprintf(stderr, "EOF\n");
		} else if (bytes_read != size){
			fprintf(stderr, "short read: read %zu bytes\n", bytes_read);
		}

		// write audio
		size_t bytes_written = snd_pcm_writei(handle, buffer, frames);
		if (-EPIPE == bytes_written){
			fprintf(stderr, "underrun occured\n");
			snd_pcm_prepare(handle);
		} else if (bytes_written < 0){
			fprintf(stderr, "writei error: %s\n", snd_strerror(bytes_written));
		} else if (bytes_written != (int) frames){
			fprintf(stderr, "short write: %zu frames written\n", bytes_written);
		}
	}

	// clean up
	snd_pcm_drain(handle); // play all pending frames
	// snd_pcm_drop(); // to stop immediately
	snd_pcm_close(handle);
	free(buffer);

	return 0;
}
