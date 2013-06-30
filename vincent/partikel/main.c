#include <stdlib.h>
#include <stdio.h>

#include "../lib/gl.h"
#include <SDL/SDL.h>

#include "config.h"
#include "../lib/shader.h"

static void print_sdl_error(const char message[]) {
	printf("%s %s\n", message, SDL_GetError());
}

static GLuint program;
static GLuint vbo_vertices, vbo_colors, vbo_velocities, vbo_start_times;

static GLint attrib_vertex;
static GLint attrib_color;
static GLint attrib_velocity;
static GLint attrib_start_time;

static GLint uniform_time;

static GLint array_width, array_height;

static GLfloat random_glfloat() {
	return (GLfloat) rand() / RAND_MAX;
}

static GLfloat* create_vertex(GLfloat* ptr, GLfloat i, GLfloat j) {
	*(ptr++) = i;
	*(ptr++) = 0.;
	*(ptr++) = j;
	return ptr;
}

static GLfloat* create_color(GLfloat* ptr, GLfloat i, GLfloat j) {
	*(ptr++) = random_glfloat() * 0.5;
	*(ptr++) = random_glfloat() * 0.5;
	*(ptr++) = random_glfloat() * 0.5 + 0.5;
	*(ptr++) = random_glfloat() * 0.5 + 0.5;
	return ptr;
}

static GLfloat* create_velocity(GLfloat* ptr, GLfloat i, GLfloat j) {
	*(ptr++) = random_glfloat() * 2. - 1.;
	*(ptr++) = random_glfloat() * 2. - 1.;
	*(ptr++) = random_glfloat() * 2. - 1.;
	return ptr;
}

static GLfloat* create_start_time(GLfloat* ptr, GLfloat i, GLfloat j) {
	*(ptr++) = random_glfloat() * 10;
	return ptr;
}

static void creation_loop(GLfloat* buffer, int w, int h, GLfloat* (*func)(GLfloat*, GLfloat, GLfloat j)) {
	for(GLfloat i = 0.5 / w - 0.5; i < 0.5; i += 1.0/w) {
		for(GLfloat j = 0.5 / h - 0.5; j < 0.5; j += 1.0/h) {
			buffer = func(buffer, i, j);
		}
	}
}

static void create_points(int w, int h) {
	const size_t size = w * h * sizeof(GLfloat);
	GLfloat* buffer = malloc(4 * size);

	creation_loop(buffer, w, h, create_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, size * 3, buffer, GL_STATIC_DRAW);
	creation_loop(buffer, w, h, create_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
	glBufferData(GL_ARRAY_BUFFER, size * 4, buffer, GL_STATIC_DRAW);
	creation_loop(buffer, w, h, create_velocity);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_velocities);
	glBufferData(GL_ARRAY_BUFFER, size * 3, buffer, GL_STATIC_DRAW);
	creation_loop(buffer, w, h, create_start_time);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_start_times);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);

	array_width = w;
	array_height = h;
	free(buffer);
}

static int init() {
	const GLuint vertex_shader = shader_load("vertex.glsl", GL_VERTEX_SHADER);
	const GLuint fragment_shader = shader_load("fragment.glsl", GL_FRAGMENT_SHADER);
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

	attrib_vertex = glGetAttribLocation(program, "position");
	attrib_color = glGetAttribLocation(program, "color");
	attrib_velocity = glGetAttribLocation(program, "velocity");
	attrib_start_time = glGetAttribLocation(program, "start_time");

	uniform_time = shader_get_uniform(program, "time");

	glGenBuffers(1, &vbo_vertices);
	glGenBuffers(1, &vbo_colors);
	glGenBuffers(1, &vbo_velocities);
	glGenBuffers(1, &vbo_start_times);

	create_points(500, 500);

	return 1;
}


static void draw(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glUniform1f(uniform_time, (GLfloat) (SDL_GetTicks() % 12000)/1000);

	glEnableVertexAttribArray(attrib_vertex);
	glEnableVertexAttribArray(attrib_color);
	glEnableVertexAttribArray(attrib_velocity);
	glEnableVertexAttribArray(attrib_start_time);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glVertexAttribPointer(
		attrib_vertex,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
	glVertexAttribPointer(
		attrib_color,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_velocities);
	glVertexAttribPointer(
		attrib_velocity,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_start_times);
	glVertexAttribPointer(
		attrib_start_time,
		1,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glPointSize(2.0);
	glDrawArrays(GL_POINTS, 0, array_width * array_height);

	glDisableVertexAttribArray(attrib_vertex);
	glDisableVertexAttribArray(attrib_color);
	glDisableVertexAttribArray(attrib_velocity);
	glDisableVertexAttribArray(attrib_start_time);

	SDL_GL_SwapBuffers();
}

static void free_resources(void) {
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &vbo_colors);
	glDeleteBuffers(1, &vbo_velocities);
	glDeleteBuffers(1, &vbo_start_times);
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

	if(!init()) {
		return EXIT_FAILURE;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
