#include <stdlib.h>
#include <stdio.h>

#include "../lib/gl.h"
#include <SDL/SDL.h>

#include "config.h"
#include "../lib/shader.h"

static void print_sdl_error(const char message[]) {
	printf("%s %s\n", message, SDL_GetError());
}

GLuint program, vbo_rectangle;
GLint attribute_coord2d, uniform_time;

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
	printf("%dms\n", SDL_GetTicks() - last);
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
			run = event.type == SDL_QUIT ? 0 : 1;
		}
		draw();
	}
	free_resources();
	SDL_Quit();
	return EXIT_SUCCESS;
}
