#include <SDL/SDL.h>
#include <pthread.h>

#include <libzeuch/shader.h>
#include <libzeuch/matrix.h>
#include <libzeuch/vector.h>
#include <libzeuch/quaternion.h>
#include <libzeuch/gl.h>

#define true 1
#define false 0

//#define WIDTH 1920
//#define HEIGHT 1080
//#define FULLSCREEN

#define WIDTH 853
#define HEIGHT 480

typedef struct {
	GLint view_position;
	GLint view_up;
	GLint view_direction;
	GLint res;
	GLint time;
	GLint notes;
	GLint envelopes;
	GLint aenvelopes;
	GLint senvelopes;
} uniforms_t;

#include "camera.h"
#include "timeline.h"

// functions that need to be called from scenes
static void draw_quad(GLint attribute_coord2d);
static void update_uniforms(const uniforms_t* uniforms, timeline_t* timeline);
static void get_uniforms(uniforms_t* uniforms, GLuint program);

#include "libblink.h"
#include "shader_sources.h"
#include "4klang.inh"
static camera_t camera;
#include "postproc.h"

typedef unsigned char ILubyte;
#include "drb.df.h"
#include "vincent.df.h"
#include "ro0mquy.df.h"
#include "ps0ke.df.h"

// scenes go here
#include "scene_blank.h"
#include "scene_test.h"
#include "scene_pythagorean.h"


static SAMPLE_TYPE audio_buffer[MAX_SAMPLES * 2];
static int playback_position = 0;

static void fill_audio(void* userdata, Uint8* stream, int len);
static void draw(void);

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

	glGenBuffers(1, &vbo_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	const GLfloat rectangle_vertices[] = {
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

	camera = (camera_t) {
		.position = vec3_new(0, 0, 10),
		.rotation = quat_new(vec3_new(0, 0, 0), 1),
	};


	int run = true;
	int alt = false;
	while(run) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					run = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							return 0;
						case SDLK_LALT:
						case SDLK_RALT:
							alt = true;
							break;
						case SDLK_F4:
							if(alt == true) return 0;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_LALT:
						case SDLK_RALT:
							alt = false;
							break;
						default:
							break;
					}
				default:
					break;
			}
		}

		draw();
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

static int initialized = false;

static GLuint vertex;

static void draw(void) {
	if(!initialized) {
		vertex = shader_load_strings(1, "vertex", (const GLchar* []) { vertex_source }, GL_VERTEX_SHADER);
		postproc_init(vertex);
		blank_init(vertex);
		test_init(vertex);
		pythagorean_init(vertex);
		initialized = true;
	} else {
		postproc_before_draw();
		//blank_draw();
		//test_draw();
		pythagorean_draw();
		postproc_after_draw();
	}
}

static float smooth_envelopes[32];
static float accum_envelopes[32];

static void update_uniforms(const uniforms_t* const uniforms, timeline_t* timeline) {
	glUniform2f(uniforms->res, WIDTH, HEIGHT);
	SDL_LockAudio();
	int position = playback_position;
	SDL_UnlockAudio();
	int index = (position / 256) * 32;
	const float factor = .3;
	for(int i = 0; i < 32; i++) {
		smooth_envelopes[i] = (1. - factor) * (&__4klang_envelope_buffer)[index + i] + factor * smooth_envelopes[i];
		accum_envelopes[i] += (&__4klang_envelope_buffer)[index + i];
	}
	glUniform1fv(uniforms->envelopes, 32, &(&__4klang_envelope_buffer)[index]);
	glUniform1fv(uniforms->senvelopes, 32, smooth_envelopes);
	glUniform1fv(uniforms->aenvelopes, 32, accum_envelopes);
	glUniform1iv(uniforms->notes, 32, &(&__4klang_note_buffer)[index]);
	int time = (int64_t) position * 1000 / 44100;
	glUniform1f(uniforms->time, (double) position / 44100);
	camera = timeline_get_camera(timeline, time);
	camera_update_uniforms(&camera, uniforms->view_position, uniforms->view_direction, uniforms->view_up);
}

static void get_uniforms(uniforms_t* uniforms, GLuint program) {
	uniforms->view_position = shader_get_uniform(program, "view_position");
	uniforms->view_up = shader_get_uniform(program, "view_up");
	uniforms->view_direction = shader_get_uniform(program, "view_direction");
	uniforms->res = shader_get_uniform(program, "res");
	uniforms->time = shader_get_uniform(program, "time");
	uniforms->notes = shader_get_uniform(program, "notes");
	uniforms->envelopes = shader_get_uniform(program, "envelopes");
	uniforms->aenvelopes = shader_get_uniform(program, "aenvelopes");
	uniforms->senvelopes = shader_get_uniform(program, "senvelopes");
}
