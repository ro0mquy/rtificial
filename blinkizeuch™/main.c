#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include <AntTweakBar.h>
#include <SDL/SDL.h>
#include <IL/il.h>

#include <libzeuch/gl.h>
#include <libzeuch/shader.h>
#include <libzeuch/vector.h>

#include "config.h"
#include "camera.h"
#include "vertex.h"
#include "flight.h"
#include "scene.h"
#include "timeline.h"
#include "window.h"

const double TAU = 6.28318530718;

static int init(void);
static void load_shader(void);
static void draw(void);
static void free_resources(void);
static void save_restore_camera(int i);
static void print_sdl_error(const char message[]);
static void handle_key_down(SDL_KeyboardEvent event);
static void update_state(void);
static void TW_CALL cb_set_rotation(const void* value, void* clientData);
static void TW_CALL cb_get_rotation(void* value, void* clientData);

GLuint program = 0, vbo_rectangle;
GLuint vertex_shader;
GLint attribute_coord2d, uniform_time;
GLint uniform_view_position, uniform_view_direction, uniform_view_up;
GLint uniform_res = -1;

int previousTime = 0;
int currentTime = 0;
float deltaT = 0;

bool run;
bool save_next = false;

TwBar* tweakBar;
unsigned int start = 0;
unsigned int end = 1;
int duration = 1000;
scene_t* scene;

camera_t saved_positions[10];
camera_t camera;

flight_t current_flight;
bool is_flying = false;

char* scene_path;

timeline_t* timeline;

int main(int argc, char *argv[]) {
	const int init_status = SDL_Init(SDL_INIT_EVERYTHING);
	if(init_status == -1) {
		print_sdl_error("Failed to initialise SDL! Error: ");
		return EXIT_FAILURE;
	}

	const SDL_VideoInfo* const video_info = SDL_GetVideoInfo();
	window_set_desktop_dim(video_info->current_w, video_info->current_h);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	const SDL_Surface* screen = window_handle_resize(false, default_width, default_height);
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
		fprintf(stderr, "Usage: blinkizeuch scenedir/");
		return EXIT_FAILURE;
	}
	size_t scene_path_length = strlen(argv[1]);
	if(argv[1][scene_path_length - 1] != '/') {
		scene_path = malloc(scene_path_length + 2);
		memcpy(scene_path, argv[1], scene_path_length);
		scene_path[scene_path_length] = '/';
		scene_path[scene_path_length + 1] = '\0';
	} else {
		scene_path = malloc(scene_path_length + 1);
		memcpy(scene_path, argv[1], scene_path_length + 1);
	}
	// initialize DevIL
	ilInit();

	if(!init()) {
		return EXIT_FAILURE;
	}

	// initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(default_width, default_height);
	tweakBar = TwNewBar("Rumfummeldings");
	TwAddVarRO(tweakBar, "Delta time", TW_TYPE_FLOAT, &deltaT, "");
	TwAddVarCB(tweakBar, "Camera rotation", TW_TYPE_QUAT4F, cb_set_rotation, cb_get_rotation, NULL, "");
	TwAddVarRW(tweakBar, "Start", TW_TYPE_UINT32, &start, " max=9");
	TwAddVarRW(tweakBar, "End", TW_TYPE_UINT32, &end, " max=9");
	TwAddVarRW(tweakBar, "Duration", TW_TYPE_UINT32, &duration, "");

	if(scene != NULL) scene_add_to_tweakbar(scene, tweakBar);

	SDL_WM_SetCaption(window_caption, NULL);
	run = true;
	while(run) {
		update_state();

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(window_should_ignore_next_resize() && event.type == SDL_VIDEORESIZE) {
				window_dont_ignore_next_resize();
				continue;
			}

			// give AntTweakBar the opportunity to handle the event
			int handled = TwEventSDL(&event, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
			if(handled) continue;

			if(timeline_handle_sdl_event(timeline, &event)) continue;

			switch(event.type){
				case SDL_KEYDOWN:
					handle_key_down(event.key);
					break;
				case SDL_QUIT:
					run = false;
					break;
				case SDL_VIDEORESIZE:
					if(!window_is_fullscreen()) {
						window_handle_resize(false, event.resize.w, event.resize.h);
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

static int init(void) {
	for(int i = 0; i < 10; i++){
		camera_init(&saved_positions[i]);
	}
	camera_init(&camera);

	const size_t scene_path_length = strlen(scene_path);
	const size_t config_name_length = strlen(config_name);
	char config_path[scene_path_length + config_name_length + 1];
	strncpy(config_path, scene_path, scene_path_length);
	strncpy(config_path + scene_path_length, config_name, config_name_length + 1);

	vertex_shader = shader_load_strings(1, "vertex", (const GLchar* []) { vertex_source }, GL_VERTEX_SHADER);

	const GLfloat rectangle_vertices[] = {
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	glGenBuffers(1, &vbo_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

	scene = scene_load(config_path);

	load_shader();

	timeline = timeline_new();

	currentTime = SDL_GetTicks();

	return 1;
}

static void load_shader(void) {
	const size_t scene_path_length = strlen(scene_path);
	const size_t fragment_name_length = strlen(fragment_name);
	char fragment_path[scene_path_length + fragment_name_length + 1];
	strncpy(fragment_path, scene_path, scene_path_length);
	strncpy(fragment_path + scene_path_length, fragment_name, fragment_name_length + 1);

	const GLuint fragment_shader = shader_load_files(2, (const char* []) { libblink_path, fragment_path }, GL_FRAGMENT_SHADER);
	if(program != 0) glDeleteProgram(program);
	program = shader_link_program(vertex_shader, fragment_shader);
	glDeleteShader(fragment_shader);

	const char attribute_coord2d_name[] = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_coord2d_name);
	if(attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_coord2d_name);
		return;
	}

	uniform_res = shader_get_uniform(program, "res");
	uniform_time = shader_get_uniform(program, "time");
	uniform_view_position = shader_get_uniform(program, "view_position");
	uniform_view_direction = shader_get_uniform(program, "view_direction");
	uniform_view_up = shader_get_uniform(program, "view_up");

	if(scene != NULL) scene_load_uniforms(scene, program);
}


static void draw(void) {
	glUseProgram(program);
	glUniform2f(uniform_res, window_get_width(), window_get_height());
	camera_update_uniforms(&camera, uniform_view_position, uniform_view_direction, uniform_view_up);
	glUniform1f(uniform_time, SDL_GetTicks());

	if(scene != NULL) scene_bind(scene);

	// actually we overdraw the complete screen
	// if this causes problems uncomment again
	// glClearColor(0.0, 0.0, 0.0, 1.0);
	// glClear(GL_COLOR_BUFFER_BIT);

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

	timeline_draw(timeline);

	// draw AntTweakBar
	TwDraw();

	SDL_GL_SwapBuffers();
}

static void free_resources(void) {
	if(scene != NULL) {
		scene_destroy(scene);
		free(scene);
	}
	timeline_destroy(timeline);
	free(timeline);
	free(scene_path);
	glDeleteShader(vertex_shader);
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
			window_handle_resize(!window_is_fullscreen(), 0, 0);
			break;
		case SDLK_ESCAPE:
			run = false;
			break;
		case SDLK_g:
			is_flying = true;
			current_flight = flight_new(saved_positions[start], saved_positions[end], SDL_GetTicks(), duration);
			break;
		case SDLK_r:
			load_shader();
			break;
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
