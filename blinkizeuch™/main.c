#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include <AntTweakBar.h>
#include <SDL/SDL.h>

#include <libzeuch/gl.h>
#include <libzeuch/shader.h>
#include <libzeuch/vector.h>

#include "config.h"
#include "camera.h"
#include "vertex.h"
#include "flight.h"
#include "tweak_loader.h"

const double TAU = 6.28318530718;

static int init(const char fragment[]);
static void draw(void);
static void free_resources(void);
static void save_restore_camera(int i);
static void print_sdl_error(const char message[]);
static void handle_key_down(SDL_KeyboardEvent event);
static void update_state(void);
static SDL_Surface* handle_resize(bool fullscreen);
static void TW_CALL cb_set_rotation(const void* value, void* clientData);
static void TW_CALL cb_get_rotation(void* value, void* clientData);

GLuint program = 0, vbo_rectangle;
GLint attribute_coord2d, uniform_time;
GLint uniform_viewPosition, uniform_viewDirection, uniform_viewUp;
GLint uniform_someColor;
GLint uniform_res = -1;

int previousTime = 0;
int currentTime = 0;
float deltaT = 0;

bool run;
bool save_next = false;
bool is_fullscreen = false;
bool ignore_next_resize = false;

TwBar* tweakBar;
float someColor[3] = {0., 0., 0.};
unsigned int start = 0;
unsigned int end = 1;
int duration = 1000;
tweakable_t* tweakables;
int num_tweakables;

camera_t saved_positions[10];
camera_t camera;

flight_t current_flight;
bool is_flying = false;

int desktop_width, desktop_height;

int main(int argc, char *argv[]) {
	const int init_status = SDL_Init(SDL_INIT_EVERYTHING);
	if(init_status == -1) {
		print_sdl_error("Failed to initialise SDL! Error: ");
		return EXIT_FAILURE;
	}

	const SDL_VideoInfo* const videoInfo = SDL_GetVideoInfo();
	desktop_width = videoInfo->current_w;
	desktop_height = videoInfo->current_h;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	const SDL_Surface* screen = handle_resize(false);
	if (!screen) {
		print_sdl_error("Failed to create SDL window! Error: ");
		return EXIT_FAILURE;
	}

	const GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("%s\n", glewGetErrorString(err));
		return EXIT_FAILURE;
	}

	if(argc != 2) {
		fprintf(stderr, "Usage: blinkizeuch fragment.glsl");
		return EXIT_FAILURE;
	}

	if(!init(argv[1])) {
		return EXIT_FAILURE;
	}

	// initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(width, height);
	tweakBar = TwNewBar("Rumfummeldings");
	TwAddVarRO(tweakBar, "Delta time", TW_TYPE_FLOAT, &deltaT, "");
	TwAddVarCB(tweakBar, "Camera rotation", TW_TYPE_QUAT4F, cb_set_rotation, cb_get_rotation, NULL, "");
	TwAddVarRW(tweakBar, "Start", TW_TYPE_UINT32, &start, " max=9");
	TwAddVarRW(tweakBar, "End", TW_TYPE_UINT32, &end, " max=9");
	TwAddVarRW(tweakBar, "Duration", TW_TYPE_UINT32, &duration, "");

	for(int i = 0; i < num_tweakables; i++) {
		tweakable_t* t = &tweakables[i];
		TwType type;
		switch(t->type) {
			case COLOR:
				type = TW_TYPE_COLOR3F;
				t->value = malloc(3 * sizeof(float));
				break;
			case FLOAT:
			default:
				type = TW_TYPE_FLOAT;
				t->value = malloc(1 * sizeof(float));
				break;
		}
		TwAddVarRW(tweakBar, t->name, type, t->value, "");
	}

	SDL_WM_SetCaption(window_caption, NULL);
	run = true;
	while(run) {
		update_state();

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(ignore_next_resize && event.type == SDL_VIDEORESIZE) {
				ignore_next_resize = false;
				continue;
			}

			// give AntTweakBar the opportunity to handle the event
			int handled = TwEventSDL(&event, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
			if(handled) continue;

			switch(event.type){
				case SDL_KEYDOWN:
					handle_key_down(event.key);
					break;
				case SDL_QUIT:
					run = false;
					break;
				case SDL_VIDEORESIZE:
					if(!is_fullscreen) {
						width  = event.resize.w;
						height = event.resize.h;
						handle_resize(false);
					}
					break;
			}
		}
		draw();
	}
	TwTerminate();
	free_resources();
	SDL_Quit();
	return EXIT_SUCCESS;
}

static int init(const char fragment[]) {
	for(int i = 0; i < 10; i++){
		camera_init(&saved_positions[i]);
	}
	camera_init(&camera);

	const GLuint vertex_shader = shader_load_str("vertex", vertex_source, GL_VERTEX_SHADER);
	const GLuint fragment_shader = shader_load_file(fragment, GL_FRAGMENT_SHADER);
	if(vertex_shader == 0 || fragment_shader == 0) {
		return 0;
	}
	program = shader_link_program(vertex_shader, fragment_shader);

	const GLfloat rectangle_vertices[] = {
		-1.0, -1.0,
		-1.0,  1.0,
		 1.0, -1.0,
		 1.0,  1.0,
	};
	glGenBuffers(1, &vbo_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

	const char attribute_coord2d_name[] = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_coord2d_name);
	if(attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_coord2d_name);
		return 0;
	}

	GLint uniform_aspect = shader_get_uniform(program, "aspect");
	uniform_res = shader_get_uniform(program, "res");
	uniform_time = shader_get_uniform(program, "time");
	uniform_viewPosition = shader_get_uniform(program, "viewPosition");
	uniform_viewDirection = shader_get_uniform(program, "viewDirection");
	uniform_viewUp = shader_get_uniform(program, "viewUp");
	uniform_someColor = shader_get_uniform(program, "someColor");

	glUseProgram(program);
	glUniform1f(uniform_aspect, (float) width/height);
	glUniform2f(uniform_res, width, height);

	currentTime = SDL_GetTicks();

	const char extension[] = ".json";
	char tweakable_filename[strlen(fragment) + strlen(extension) + 1];
	strncpy(tweakable_filename, fragment, strlen(fragment));
	strncpy(tweakable_filename + strlen(fragment), extension, strlen(extension));
	tweakable_filename[strlen(fragment) + strlen(extension)] = 0;
	num_tweakables = tweak_loader_load(tweakable_filename, &tweakables);

	return 1;
}


static void draw(void) {
	camera_update_uniforms(&camera, uniform_viewPosition, uniform_viewDirection, uniform_viewUp);
	glUniform3f(uniform_someColor, someColor[0], someColor[1], someColor[2]);
	glUniform1f(uniform_time, SDL_GetTicks());

	for(int i = 0; i < num_tweakables; i++) {
		tweakable_t* t = &tweakables[i];
		float* const value = (float*) t->value;
		switch(t->type) {
			case COLOR:
				glUniform3f(shader_get_uniform(program, t->uniform_name), value[0], value[1], value[2]);
				break;
			case FLOAT:
			default:
				glUniform1f(shader_get_uniform(program, t->uniform_name), value[0]);
				break;
		}
	}
	// actually we overdraw the complete screen
	// if this causes problems uncomment again
	// glClearColor(0.0, 0.0, 0.0, 1.0);
	// glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
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

	// draw AntTweakBar
	TwDraw();

	SDL_GL_SwapBuffers();
}

static void free_resources(void) {
	for(int i = 0; i < num_tweakables; i++) {
		tweakable_t* t = &tweakables[i];
		free(t->name);
		free(t->uniform_name);
		free(t->value);
	}
	free(tweakables);
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_rectangle);
}

static void save_restore_camera(int i) {
	if(save_next) {
		saved_positions[i] = camera;
		save_next = false;
	} else {
		camera = saved_positions[i];
	}
}

static void print_sdl_error(const char message[]) {
	printf("%s %s\n", message, SDL_GetError());
}

static void handle_key_down(SDL_KeyboardEvent keyEvent) {
	switch(keyEvent.keysym.sym) {
		case SDLK_t:
			save_next = true;
			break;
		case SDLK_0:
			save_restore_camera(0);
			break;
		case SDLK_1:
			save_restore_camera(1);
			break;
		case SDLK_2:
			save_restore_camera(2);
			break;
		case SDLK_3:
			save_restore_camera(3);
			break;
		case SDLK_4:
			save_restore_camera(4);
			break;
		case SDLK_5:
			save_restore_camera(5);
			break;
		case SDLK_6:
			save_restore_camera(6);
			break;
		case SDLK_7:
			save_restore_camera(7);
			break;
		case SDLK_8:
			save_restore_camera(8);
			break;
		case SDLK_9:
			save_restore_camera(9);
			break;
		case SDLK_f:
			handle_resize(!is_fullscreen);
			break;
		case SDLK_ESCAPE:
			run = false;
			break;
		case SDLK_g:
			is_flying = true;
			current_flight = flight_new(saved_positions[start], saved_positions[end], SDL_GetTicks(), duration);
		default:
			save_next = false;
			break;
	}
}

static void update_state(void) {
	previousTime = currentTime;
	currentTime = SDL_GetTicks();
	deltaT = (currentTime - previousTime) / 1000.; //convert from milliseconds to seconds

	if(is_flying) {
		if(flight_is_finished(&current_flight, currentTime)) {
			is_flying = false;
		}
		camera = flight_get_camera(&current_flight, currentTime);
	}

	const float angle_modifier = 50. / 360. * TAU;
	const float movement_modifier = 5;

	const float angle = angle_modifier * deltaT;
	const float distance = movement_modifier * deltaT;

	// get keystate map
	Uint8* keystate= SDL_GetKeyState(NULL);;

	// rotate camera
	if (keystate[SDLK_i]) {
		camera_rotate_x(&camera,  angle);
	}
	if (keystate[SDLK_k]) {
		camera_rotate_x(&camera, -angle);
	}
	if (keystate[SDLK_j]) {
		camera_rotate_y(&camera,  angle);
	}
	if (keystate[SDLK_l]) {
		camera_rotate_y(&camera, -angle);
	}
	if (keystate[SDLK_u]) {
		camera_rotate_z(&camera, -angle);
	}
	if (keystate[SDLK_o]) {
		camera_rotate_z(&camera,  angle);
	}

	// move camera
	if (keystate[SDLK_w]) {
		camera_move_z(&camera, -distance);
	}
	if (keystate[SDLK_s]) {
		camera_move_z(&camera,  distance);
	}
	if (keystate[SDLK_a]) {
		camera_move_x(&camera, -distance);
	}
	if (keystate[SDLK_d]) {
		camera_move_x(&camera,  distance);
	}
	if (keystate[SDLK_q]) {
		camera_move_y(&camera, -distance);
	}
	if (keystate[SDLK_e]) {
		camera_move_y(&camera,  distance);
	}
}

static SDL_Surface* handle_resize(bool fullscreen) {
	Uint32 flags =
		  SDL_HWSURFACE
		| SDL_DOUBLEBUF
		| SDL_OPENGL
		| SDL_RESIZABLE;
	int new_width;
	int new_height;
	if(fullscreen != is_fullscreen) {
		ignore_next_resize = true;
	}
	if(fullscreen) {
		new_width = desktop_width;
		new_height = desktop_height;
		flags |= SDL_FULLSCREEN;
	} else {
		new_width = width;
		new_height = height;
	}
	SDL_Surface* const screen = SDL_SetVideoMode(new_width, new_height, 32, flags);
	TwWindowSize(new_width, new_height);
	is_fullscreen = fullscreen;
	glViewport(0, 0, new_width, new_height);
	if(program != 0) {
		glUseProgram(program);
		glUniform2f(uniform_res, new_width, new_height);
	}
	return screen;
}

static void TW_CALL cb_set_rotation(const void* value, void* clientData) {
	(void) clientData; // unused
	const float* quat = (const float *) value;
	camera_set_rotation(&camera, quat_new(vec3_new(quat[0], quat[1], quat[2]), quat[3]));
}

static void TW_CALL cb_get_rotation(void* value, void* clientData) {
	(void) clientData; // unused
	quat rotation = camera_get_rotation(&camera);
	float* quat = (float *) value;
	quat[0] = rotation.v.x;
	quat[1] = rotation.v.y;
	quat[2] = rotation.v.z;
	quat[3] = rotation.w;
}
