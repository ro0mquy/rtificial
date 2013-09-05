#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <AntTweakBar.h>
#include <SDL/SDL.h>

#include <libzeuch/gl.h>
#include <libzeuch/shader.h>
#include <libzeuch/vector.h>

#include "config.h"
#include "camera.h"

const double TAU = 6.28318530718;

static void print_sdl_error(const char message[]) {
	printf("%s %s\n", message, SDL_GetError());
}

GLuint program, vbo_rectangle;
GLint attribute_coord2d, uniform_time;
GLint uniform_viewPosition, uniform_viewDirection, uniform_viewUp;
GLint uniform_someColor;

int previousTime = 0;
int currentTime = 0;
float deltaT = 0;

Uint8 *keystate;

int save_next = 0;

TwBar* tweakBar;
float someColor[3] = {0., 0., 0.};

camera_t saved_positions[10];
camera_t camera;

static void save_restore_camera(int i);

static int init(const char fragment[]) {
	for(int i = 0; i < 10; i++){
		camera_init(&saved_positions[i]);
	}
	camera_init(&camera);

	const GLuint vertex_shader = shader_load("vertex.glsl", GL_VERTEX_SHADER);
	const GLuint fragment_shader = shader_load(fragment, GL_FRAGMENT_SHADER);
	if(vertex_shader == 0 || fragment_shader == 0) {
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(link_ok == GL_FALSE) {
		fprintf(stderr, "glLinkProgram: ");
		print_log(program);
		return 0;
	}

	printf("Program complete\n");
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
	GLint uniform_res = shader_get_uniform(program, "res");
	uniform_time = shader_get_uniform(program, "time");
	uniform_viewPosition = shader_get_uniform(program, "viewPosition");
	uniform_viewDirection = shader_get_uniform(program, "viewDirection");
	uniform_viewUp = shader_get_uniform(program, "viewUp");
	uniform_someColor = shader_get_uniform(program, "someColor");

	glUseProgram(program);
	if(uniform_aspect != -1) {
		glUniform1f(uniform_aspect, (float) width/height);
	}
	if(uniform_res != -1) {
		glUniform2f(uniform_res, width, height);
	}

	printf("initialized\n");
	currentTime = SDL_GetTicks();
	return 1;
}


static void draw(void) {
	camera_update_uniforms(&camera, uniform_viewPosition, uniform_viewDirection, uniform_viewUp);
	glUniform3f(uniform_someColor, someColor[0], someColor[1], someColor[2]);

	if(uniform_time != -1) {
		glUniform1f(uniform_time, SDL_GetTicks());
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
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

	TwDraw();

	SDL_GL_SwapBuffers();
}

static void free_resources(void) {
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_rectangle);
}

int main(int argc, char *argv[]) {
	const int init_status = SDL_Init(SDL_INIT_EVERYTHING);
	if(init_status == -1) {
		print_sdl_error("Failed to initialise SDL! Error: ");
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	const SDL_Surface* const screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
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
	TwAddVarRW(tweakBar, "Some color", TW_TYPE_COLOR3F, &someColor, "");

	// getting keystate map
	keystate = SDL_GetKeyState(NULL);

	SDL_WM_SetCaption(window_caption, NULL);
	int run = 1;
	while(run) {
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaT = (currentTime - previousTime) / 1000.; //convert from milliseconds to seconds
		printf("\b\b\b\b\b\b%.3fs", deltaT);

		const float angle_modifier = 50. / 360. * TAU;
		const float movement_modifier = 5;

		const float angle = angle_modifier * deltaT;
		const float distance = movement_modifier * deltaT;

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

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			int handled = TwEventSDL(&event, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
			if(handled) continue;
			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_t:
							save_next = 1;
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
						default:
							save_next = 0;
							break;
					}
					break;
				case SDL_QUIT:
					run = 0;
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

static void save_restore_camera(int i) {
	if(save_next) {
		saved_positions[i] = camera;
		save_next = 0;
	} else {
		camera = saved_positions[i];
	}
}
