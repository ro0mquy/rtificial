#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#ifdef __linux__
# include <sys/inotify.h>
# include <sys/fcntl.h>
#endif

#include <AntTweakBar.h>
#include <SDL/SDL.h>
#include <IL/il.h>

#include <libzeuch/gl.h>
#include <libzeuch/shader.h>
#include <libzeuch/vector.h>
#include <libzeuch/matrix.h>

#include "config.h"
#include "camera.h"
#include "font.h"
#include "vertex.h"
#include "scene.h"
#include "timeline.h"
#include "window.h"
#include "music.h"

#define IN_EVENT_SIZE ( sizeof(struct inotify_event))
#define IN_BUF_LEN    ( 1024 * (IN_EVENT_SIZE+16))

const double TAU = 6.28318530718;
const double PAU = 4.71238898038;

static int init(void);
static void handle_sig(int);
static void load_shader(void);
static void draw(void);
static void free_resources(void);
static void post_resize_buffer(void);
static void print_sdl_error(const char message[]);
static void handle_key_down(SDL_KeyboardEvent event);
static void update_state(void);
static void TW_CALL cb_set_rotation(const void* value, void* clientData);
static void TW_CALL cb_get_rotation(void* value, void* clientData);
static void init_music(void);

GLuint program = 0, vbo_rectangle;
GLuint post_framebuffer;
GLuint vertex_shader;
GLuint post_program = 0;
GLuint post_vertex_shader;
GLuint post_tex_buffer;
GLuint post_depth_buffer;
GLint attribute_coord2d, uniform_time;
GLint uniform_view_position, uniform_view_direction, uniform_view_up;
GLint uniform_res = -1;
GLint uniform_inv_world_camera_matrix, uniform_prev_world_camera_matrix;
GLint post_attribute_coord2d;

int previousTime = 0;
int currentTime = 0;
int deltaT = 0;

mat4x4 previous_world_to_camera_matrix;

float angle_modifier = 1.; // ~ 50. / 360. * TAU
float movement_modifier = 5.;

bool run;

TwBar* tweakBar;
scene_t* scene;

camera_t camera;

char* scene_path;
char* config_path;
char* fragment_path;
char* post_path;
char* timeline_path;

timeline_t* timeline;

font_t font;

music_t music;

int main(int argc, char *argv[]) {
	// reload shader when receiving SIGUSR1
	signal(SIGHUP, handle_sig);


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
	// create path stings
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
	scene_path_length = strlen(scene_path);

	size_t fragment_name_length = strlen(fragment_name);
	fragment_path = malloc(scene_path_length + fragment_name_length + 1);
	strncpy(fragment_path, scene_path, scene_path_length);
	strncpy(fragment_path + scene_path_length, fragment_name, fragment_name_length + 1);

	size_t post_name_length = strlen(post_name);
	post_path = malloc(scene_path_length + post_name_length + 1);
	strncpy(post_path, scene_path, scene_path_length);
	strncpy(post_path + scene_path_length, post_name, post_name_length + 1);

	size_t config_name_length = strlen(config_name);
	config_path = malloc(scene_path_length + config_name_length + 1);
	strncpy(config_path, scene_path, scene_path_length);
	strncpy(config_path + scene_path_length, config_name, config_name_length + 1);

	size_t timeline_name_length = strlen(timeline_name);
	timeline_path = malloc(scene_path_length + timeline_name_length + 1);
	strncpy(timeline_path, scene_path, scene_path_length);
	strncpy(timeline_path + scene_path_length, timeline_name, timeline_name_length + 1);

#ifdef __linux__
	// inotify setup
	int in_length, in_event_i;
	int in_fd;
	int in_wd;
	char in_buffer[IN_BUF_LEN];


	in_fd = inotify_init();
	if(in_fd < 0)
		fprintf(stderr, "error: inotify: inotify_init()");
	if(fcntl(in_fd, F_SETFL, O_NONBLOCK))
		fprintf(stderr, "error: inotify: fcntl()");

	in_wd = inotify_add_watch(in_fd, fragment_path, IN_CLOSE_WRITE);
	in_wd = inotify_add_watch(in_fd, post_path, IN_CLOSE_WRITE);
	printf("watching \"%s\"\n", fragment_path);
	printf("watching \"%s\"\n", post_path);
#endif


	// initialize DevIL
	ilInit();

	if(!init()) {
		return EXIT_FAILURE;
	}

	// initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(default_width, default_height);
	tweakBar = TwNewBar("Rumfummeldings");
	TwAddVarRO(tweakBar, "Delta time", TW_TYPE_INT32, &deltaT, "");
	TwAddVarCB(tweakBar, "Camera rotation", TW_TYPE_QUAT4F, cb_set_rotation, cb_get_rotation, NULL, "");
	TwAddVarRW(tweakBar, "Movement Speed", TW_TYPE_FLOAT, &movement_modifier, "min=0.0");
	TwAddVarRW(tweakBar, "Angular Speed", TW_TYPE_FLOAT, &angle_modifier, "min=0.0 step=0.2");

	if(scene != NULL) scene_add_to_tweakbar(scene, tweakBar);

	SDL_WM_SetCaption(window_caption, NULL);
	run = true;
	while(run) {
#ifdef __linux__
		// inotify event handling
		in_length = read(in_fd, in_buffer, IN_BUF_LEN);

		in_event_i = 0;
		while(in_event_i < in_length){
			struct inotify_event *in_event = (struct inotify_event*) &in_buffer[in_event_i];
			if(in_event->mask & IN_CLOSE_WRITE){
				puts("shader file changed. reloading it for ya.");
				load_shader();
			}
			else if(in_event->mask & IN_IGNORED){
				/* this is probably really wrong, doing it anyway. for the lulz
				 * IN_IGNORED is fired when a watch is removed. during debgging
				 * I noticed, that this gets fired wehn vim wirtes to the scene
				 * file. in other code a IN_CLOSE_WRITE was fired instead.
				 * reloading and rewatching it anyway. #yolo
				 */
				in_wd = inotify_add_watch(in_fd, fragment_path, IN_CLOSE_WRITE);
				in_wd = inotify_add_watch(in_fd, post_path, IN_CLOSE_WRITE);
				puts("shader file changed. reloading it for ya.");
				load_shader();
			}
			in_event_i += IN_EVENT_SIZE + in_event->len;
		}
#endif


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
						post_resize_buffer();
					}
					break;
			}
		}
		draw();
	}
	TwTerminate();
	free_resources();
	SDL_Quit();
#ifdef __linux__
	inotify_rm_watch(in_fd, in_wd);
	close(in_fd);
#endif
	return EXIT_SUCCESS;
}

static int init(void) {
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	camera_init(&camera);

	// set up vertex shader for main scene rendering and postprocessing
	vertex_shader = shader_load_strings(1, "vertex", (const GLchar* []) { vertex_source }, GL_VERTEX_SHADER);
	post_vertex_shader = shader_load_strings(1, "post_vertex", (const GLchar* []) { post_vertex_source }, GL_VERTEX_SHADER);

	const GLfloat rectangle_vertices[] = {
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	glGenBuffers(1, &vbo_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

	// create target framebuffer and texture for postprocessing
	glGenTextures(1, &post_tex_buffer);
	glBindTexture(GL_TEXTURE_2D, post_tex_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// depth buffer
	glGenTextures(1, &post_depth_buffer);
	glBindTexture(GL_TEXTURE_2D, post_depth_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// make both textures as big as the window
	post_resize_buffer();

	glGenFramebuffers(1, &post_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, post_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, post_tex_buffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, post_depth_buffer, 0);

	// specify the attachments to be drawn to
	glDrawBuffers(2, (GLenum[]) { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });

	/// zomg error checking!1!elf1
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "Framebuffer not complete: error %X\n", status);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	scene = scene_load(scene_path, config_path);

	load_shader();

	timeline = timeline_new();

	timeline_load(timeline, timeline_path);
	camera = timeline_get_camera(timeline);

	init_music();

	currentTime = SDL_GetTicks();

	return 1;
}

static void handle_sig(int signum) {
	(void)signum;
	load_shader();
}

static void load_shader(void) {
	// compile main raymarching program
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

	// compile postprocessing program
	GLuint post_fragment_shader = shader_load_files(1, (const char* []) { post_path }, GL_FRAGMENT_SHADER);
	// if post.glsl file doesn't exist, load default shader which does nothing
	if (post_fragment_shader == 0) {
		post_fragment_shader = shader_load_strings(1, post_path, (const GLchar* []) { post_default_fragment_source }, GL_FRAGMENT_SHADER);
	}

	if(post_program != 0) glDeleteProgram(post_program);
	post_program = shader_link_program(post_vertex_shader, post_fragment_shader);
	glDeleteShader(post_fragment_shader);

	const char post_attribute_coord2d_name[] = "coord2d";
	post_attribute_coord2d = glGetAttribLocation(post_program, post_attribute_coord2d_name);
	if(post_attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s for postprocessing\n", post_attribute_coord2d_name);
		return;
	}

	glUseProgram(post_program);
	uniform_prev_world_camera_matrix = shader_get_uniform(post_program, "previous_world_to_camera_matrix");
	uniform_inv_world_camera_matrix = shader_get_uniform(post_program, "inverse_world_to_camera_matrix");
	GLuint post_uniform_tex = shader_get_uniform(post_program, "tex");
	GLuint post_uniform_depth = shader_get_uniform(post_program, "tex_depth");
	glUniform1i(post_uniform_tex, /*GL_TEXTURE*/0);
	glUniform1i(post_uniform_depth, /*GL_TEXTURE*/1);
}


static void draw(void) {
	// block until everything from the last frame is drawn
	glFinish();
	int start_time = SDL_GetTicks();

	glDisable(GL_BLEND);
	glUseProgram(program);

	glUniform2f(uniform_res, window_get_width(), window_get_height());
	camera_update_uniforms(&camera, uniform_view_position, uniform_view_direction, uniform_view_up);
	glUniform1f(uniform_time, timeline_get_time(timeline));

	if(scene != NULL) scene_bind(scene);

	// actually we overdraw the complete screen
	// if this causes problems uncomment again
	// glClearColor(0.0, 0.0, 0.0, 1.0);
	// glClear(GL_COLOR_BUFFER_BIT);

	// draw into framebuffer, so we can do some postprocessing stuff
	glBindFramebuffer(GL_FRAMEBUFFER, post_framebuffer);
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
	// switch back to normal screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// apply postprocessing
	glUseProgram(post_program);
	glEnableVertexAttribArray(post_attribute_coord2d);

	glUniformMatrix4fv(uniform_prev_world_camera_matrix, 1, GL_TRUE, previous_world_to_camera_matrix.a);
	previous_world_to_camera_matrix = camera_world_to_camera_matrix(&camera);
	mat4x4 inverse_world_to_camera_matrix = mat4x4_invert(previous_world_to_camera_matrix);
	glUniformMatrix4fv(uniform_inv_world_camera_matrix, 1, GL_TRUE, inverse_world_to_camera_matrix.a);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, post_tex_buffer);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, post_depth_buffer);

	// we reuse the vbo from the last draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(post_attribute_coord2d);

	// block until the main program is drawn
	glFinish();
	deltaT = SDL_GetTicks() - start_time;

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
	music_destroy(&music);
	timeline_destroy(timeline);
	free(timeline);
	free(scene_path);
	free(config_path);
	free(fragment_path);
	free(post_path);
	free(timeline_path);
	glDeleteShader(vertex_shader);
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_rectangle);
	glDeleteTextures(1, &post_tex_buffer);
	glDeleteTextures(1, &post_depth_buffer);
	glDeleteFramebuffers(1, &post_framebuffer);
	glDeleteProgram(post_program);
}

static void print_sdl_error(const char message[]) {
	printf("%s %s\n", message, SDL_GetError());
}

static void post_resize_buffer(void) {
	glBindTexture(GL_TEXTURE_2D, post_tex_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			window_get_width(),
			window_get_height(),
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, post_depth_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
			window_get_width(),
			window_get_height(),
			0, GL_RED, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

static void handle_key_down(SDL_KeyboardEvent keyEvent) {
	switch(keyEvent.keysym.sym) {
		case SDLK_f:
			window_handle_resize(!window_is_fullscreen(), 0, 0);
			post_resize_buffer();
			break;
		case SDLK_ESCAPE:
			run = false;
			break;
		case SDLK_F5:
			load_shader();
			break;
		case SDLK_p:
			timeline_add_frame(timeline, camera);
			break;
		case SDLK_F7:
			scene_save(scene, config_path);
		case SDLK_F8:
			timeline_save(timeline, timeline_path);
		default:
			break;
	}
}

static void update_state(void) {
	previousTime = currentTime;
	currentTime = SDL_GetTicks();
	int deltaTime = currentTime - previousTime;

	timeline_update(timeline, deltaTime);

	if(timeline_camera_changed(timeline)) {
		camera = timeline_get_camera(timeline);
	}

	float angle = angle_modifier * deltaTime / 1000.;
	float distance = movement_modifier * deltaTime / 1000.;

	// get keystate map
	Uint8* keystate = SDL_GetKeyState(NULL);

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

static void init_music(void) {
	music_load(&music);
}
