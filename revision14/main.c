#include <SDL/SDL.h>
#include <pthread.h>

#include <libzeuch/shader.h>
#include <libzeuch/gl.h>

#include "vertex_source.h"
#include "scene_blank.h"
#include "4klang.inh"

//#define WIDTH 1366
//#define HEIGHT 768
//#define FULLSCREEN

#define WIDTH 800
#define HEIGHT 600

#define true 1
#define false 0

static SAMPLE_TYPE audio_buffer[MAX_SAMPLES * 2];
static int playback_position = 0;

static void draw_quad(GLint attribute_coord2d);
static void fill_audio(void* userdata, Uint8* stream, int len);

static GLuint vbo_rectangle;

int main() {
	pthread_t synth_thread;
	pthread_create(&synth_thread, NULL, __4klang_render, audio_buffer);

	// "Usually you initialize SDL with SDL_Init, but it also works if we leave this out." (TODO: try this)
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL
#ifdef FULLSCREEN
	| SDL_FULLSCREEN
#endif
	);
	SDL_ShowCursor(SDL_DISABLE);
	glewInit();

	SDL_AudioSpec wanted = {
		.freq = 44100,
		.format = AUDIO_S16SYS,
		.channels = 2,
		.samples = 1024,
		.callback = fill_audio,
		.userdata = NULL
	};
	SDL_OpenAudio(&wanted, NULL);


	SDL_PauseAudio(0);

	const GLuint vertex = shader_load_strings(1, "vertex", (const GLchar* []) { vertex_source }, GL_VERTEX_SHADER);
	const GLuint fragment = shader_load_strings(1, "fragment", (const GLchar* []) { scene_blank_source }, GL_FRAGMENT_SHADER);
	const GLuint program = shader_link_program(vertex, fragment);
	const GLint attribute_coord2d = glGetAttribLocation(program, "c");
	glGenBuffers(1, &vbo_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	const GLfloat rectangle_vertices[] = {
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
	glUseProgram(program);

	int run = true;
	while(run) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					run = false;
					break;
			}
		}

		draw_quad(attribute_coord2d);
		SDL_GL_SwapBuffers();
	}


	// SDL_CloseAudio
	// probably not gonna need that
	// SDL_Quit();
	//
	// also, we could kill the audio thread TODO
	return 0;
}

static void draw_quad(GLint attribute_coord2d) {
	glEnableVertexAttribArray(attribute_coord2d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	glVertexAttribPointer(
		attribute_coord2d,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(attribute_coord2d);
}

static void fill_audio(void* userdata, Uint8* stream, int len) {
	int samples = len / 4; // samples are 2 bytes, 2 samples for stereo
	int write_len = len;
	if(samples + playback_position > MAX_SAMPLES) {
		write_len = 4 * (MAX_SAMPLES - playback_position);
	}
	memcpy(stream, &audio_buffer[playback_position * 2], write_len);
	memset(stream + write_len, 0, len - write_len);
	playback_position += write_len / 4;
}
