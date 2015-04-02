#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "v2mplayer.h"
#include "libv2.h"

static V2MPlayer player;
extern "C" const sU8 soundtrack[];

#define CHANNELS 2
#define LENGTH 180

void main(){
	player.Init();
	player.Open(soundtrack);
	player.Play();

	sU32 a_len = CHANNELS * player.m_samplerate * LENGTH;// player.m_base.maxtime;
	printf("samplerate: %i\nmaxtime: %i\na_len: %i\n", player.m_samplerate, player.m_base.maxtime, a_len);
	sF32 *a_buffer = (sF32*) malloc(a_len * sizeof(sF32));
	if (a_buffer == NULL){
		puts("malloc: a_buffer == NULL");
		ExitProcess(666);
	}
	puts("rendering...");
	player.Render(a_buffer, a_len / CHANNELS, 0);
  
	FILE* const audio_file = fopen("audio.out", "wb");
	if (audio_file != NULL){
		puts("writing file...");
		int bytes = fwrite(a_buffer, sizeof(sF32), a_len, audio_file);
		fclose(audio_file);
		printf("bytes written: %i\n", bytes);
	}

	puts("press any key to quit");
	getchar();

	player.Close();
	free(a_buffer);
	ExitProcess(0);
}
