#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define TAU 6.28318530718

#include "../lib/gl.h"
#include <SDL/SDL.h>

#include "config.h"
#include "../lib/shader.h"

static void print_sdl_error(const char message[]) {
	printf("%s %s\n", message, SDL_GetError());
}

GLuint program, vbo_rectangle;
GLint attribute_coord2d, uniform_time;
GLint uniform_viewPosition, uniform_viewDirection, uniform_viewUp;

float angle_x = 0;
float angle_y = 0;
float angle_z = 0;

float position_x = 0;
float position_y = 0;
float position_z = 0;

int last = 0;

static int init(const char fragment[]) {
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

	glUseProgram(program);
	if(uniform_aspect != -1) {
		glUniform1f(uniform_aspect, (float) width/height);
	}
	if(uniform_res != -1) {
		glUniform2f(uniform_res, width, height);
	}
	
	printf("initialized\n");
	last = SDL_GetTicks();
	return 1;
}


static void draw(void) {
	float direction_x = sin(angle_x);
	float direction_y = sin(angle_y);
	float direction_z = -cos(angle_x) * cos(angle_y);

	float up_x = sin(angle_x);
	float up_y = sin(angle_y + TAU/4);
	float up_z = -cos(angle_x) * cos(angle_y + TAU/4);

	glUniform3f(uniform_viewPosition, position_x, position_y, position_z);
	glUniform3f(uniform_viewDirection, direction_x, direction_y, direction_z);
	glUniform3f(uniform_viewUp, up_x, up_y, up_z);

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

	SDL_GL_SwapBuffers();
	//printf("%dms\n", SDL_GetTicks() - last);
	last = SDL_GetTicks();
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

	SDL_WM_SetCaption(window_caption, NULL);
	int run = 1;
	while(run) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			const float angle_modifier = 5. / 360. * TAU;
			const float movement_modifier = 0.5;
			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						// rotate camera
						case SDLK_w:
							angle_y += angle_modifier;
							break;
						case SDLK_s:
							angle_y -= angle_modifier;
							break;
						case SDLK_a:
							angle_x -= angle_modifier;
							break;
						case SDLK_d:
							angle_x += angle_modifier;
							break;

						// move camera
						case SDLK_i:
							position_z -= movement_modifier;
							break;
						case SDLK_k:
							position_z += movement_modifier;
							break;
						case SDLK_j:
							position_x -= movement_modifier;
							break;
						case SDLK_l:
							position_x += movement_modifier;
							break;
						case SDLK_u:
							position_y -= movement_modifier;
							break;
						case SDLK_o:
							position_y += movement_modifier;
							break;
						default:
							break;
					}
					break;
				case SDL_QUIT:
					run = 0;
			}
		}
		draw();
	}
	free_resources();
	SDL_Quit();
	return EXIT_SUCCESS;
}
