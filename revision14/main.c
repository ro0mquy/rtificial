#include <SDL/SDL.h>
#include <pthread.h>
#include <unistd.h>

#include <libzeuch/shader.h>
#include <libzeuch/matrix.h>
#include <libzeuch/vector.h>
#include <libzeuch/quaternion.h>
#include <libzeuch/gl.h>

#define true 1
#define false 0

#define WIDTH 1920
#define HEIGHT 1080
#define FULLSCREEN

//#define WIDTH 853
//#define HEIGHT 480

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

typedef uint8_t ILubyte;

// functions that need to be called from scenes
static void draw_quad(GLint attribute_coord2d);
static void update_uniforms(const uniforms_t* uniforms, timeline_t* timeline);
static void get_uniforms(uniforms_t* uniforms, GLuint program);
static void init_timeline(timeline_t* timeline, keyframe_list_t* keyframes);
static void load_texture(GLuint* texture , ILubyte* data, int width, int height);

#include "libblink.h"
#include "shader_sources.h"
#include "4klang.inh"
static camera_t camera;
#include "postproc.h"

// scenes go here
#include "scene_pythagorean.h"
#include "scene_saeulen_static.h"
#include "scene_gitter.h"
#include "scene_ufo.h"
#include "scene_saeulen.h"
#include "scene_schwurbeltunnel.h"
#include "scene_credits.h"
#include "scene_ladebalken.h"


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
		ladebalken_init(vertex);
		ladebalken_draw(.1);
		usleep(200000);
		postproc_init(vertex);
		ladebalken_draw(.2);
		usleep(200000);
		pythagorean_init(vertex);
		ladebalken_draw(.3);
		usleep(200000);
		saeulen_static_init(vertex);
		ladebalken_draw(.4);
		usleep(200000);
		gitter_init(vertex);
		ladebalken_draw(.5);
		usleep(200000);
		ufo_init(vertex);
		ladebalken_draw(.6);
		usleep(200000);
		saeulen_init(vertex);
		ladebalken_draw(.7);
		usleep(200000);
		schwurbeltunnel_init(vertex);
		ladebalken_draw(.8);
		usleep(200000);
		credits_init(vertex);
		ladebalken_draw(.9);
		usleep(200000);
		ladebalken_draw(1.);
		usleep(200000);
		initialized = true;
		SDL_PauseAudio(0);
	} else {
		postproc_before_draw();
		SDL_LockAudio();
		int time = (int64_t) playback_position * 1000 / 44100;
		SDL_UnlockAudio();
		if(time < 28900) {
			pythagorean_draw();
		} else if(time < 44000) {
			saeulen_static_draw();
		} else if(time < 98000) {
			gitter_draw();
		} else if(time < 118800) {
			ufo_draw();
		} else if(time < 125829) {
			pythagorean_draw();
		} else if(time < 138485) {
			saeulen_draw();
		} else if(time < 150526) {
			schwurbeltunnel_draw();
		} else if(time < 159246) {
			pythagorean_draw();
		} else if(time < 164200) {
			saeulen_draw();
		} else if(time < 169272) {
			schwurbeltunnel_draw();
		} else if(time < 178709) {
			ufo_draw();
		} else if(time < 192800) {
			credits_draw();
		} else {
			exit(0);
		}
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

static void init_timeline(timeline_t* timeline, keyframe_list_t* keyframes) {
	timeline->keyframes = keyframes;
	keyframe_list_t* controlPoints = malloc(sizeof(keyframe_list_t) + sizeof(keyframe_t));
	*controlPoints = (keyframe_list_t) {
		.length = 0,
		.allocated = 1,
	};
	timeline->controlPoints = timeline_get_bezier_spline(controlPoints, timeline->keyframes, .5);
}

static void load_texture(GLuint* texture , ILubyte* data, int width, int height) {
	ILubyte rgba_data[width * height * 4];
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			rgba_data[4 * (y * width + x) + 0] = data[y * width + x];
			rgba_data[4 * (y * width + x) + 1] = data[y * width + x];
			rgba_data[4 * (y * width + x) + 2] = data[y * width + x];
			rgba_data[4 * (y * width + x) + 3] = 255;
		}
	}
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			rgba_data);
}
