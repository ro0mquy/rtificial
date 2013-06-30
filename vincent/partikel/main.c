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

static GLint attrib_vertex;
static GLint attrib_color;
static GLint attrib_velocity;
static GLint attrib_start_time;

static GLint uniform_time;

static GLint array_width, array_height;
static GLfloat *vertices;
static GLfloat *colors;
static GLfloat *velocities;
static GLfloat *start_times;


static GLfloat random_glfloat() {
	return (GLfloat) rand() / RAND_MAX;
}

static void create_points(GLint w, GLint h) {
	const size_t size = w * h * sizeof(GLfloat);
	vertices = malloc(3 * size);
	colors = malloc(3 * size);
	velocities = malloc(3 * size);
	start_times = malloc(size);

	GLfloat *vertices_ptr, *colors_ptr, *velocities_ptr, *start_times_ptr;
	vertices_ptr = vertices;
	colors_ptr = colors;
	velocities_ptr = velocities;
	start_times_ptr = start_times;
	for(GLfloat i = 0.5 / w - 0.5; i < 0.5; i += 1.0/w) {
		for(GLfloat j = 0.5 / h - 0.5; j < 0.5; j += 1.0/h) {
			*(vertices_ptr++) = i;
			*(vertices_ptr++) = 0.;
			*(vertices_ptr++) = j;

			*(colors_ptr++) = random_glfloat() * 0.5;
			*(colors_ptr++) = random_glfloat() * 0.5;
			*(colors_ptr++) = random_glfloat() * 0.5 + 0.5;

			*(velocities_ptr++) = random_glfloat() * 2. - 1.;
			*(velocities_ptr++) = random_glfloat() * 2. - 1.;
			*(velocities_ptr++) = random_glfloat() * 2. - 1.;

			*(start_times_ptr++) = random_glfloat() * 10;
		}
	}
	array_width = w;
	array_height = h;
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

	create_points(500, 500);

	return 1;
}


static void draw(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glUniform1f(uniform_time, (GLfloat) SDL_GetTicks()/1000);

	glEnableVertexAttribArray(attrib_vertex);
	glEnableVertexAttribArray(attrib_color);
	glEnableVertexAttribArray(attrib_velocity);
	glEnableVertexAttribArray(attrib_start_time);

	glVertexAttribPointer(
		attrib_vertex,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		vertices
	);
	glVertexAttribPointer(
		attrib_color,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		colors
	);
	glVertexAttribPointer(
		attrib_velocity,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		velocities
	);
	glVertexAttribPointer(
		attrib_start_time,
		1,
		GL_FLOAT,
		GL_FALSE,
		0,
		start_times
	);

	glDrawArrays(GL_POINTS, 0, array_width * array_height);

	glDisableVertexAttribArray(attrib_vertex);
	glDisableVertexAttribArray(attrib_color);
	glDisableVertexAttribArray(attrib_velocity);
	glDisableVertexAttribArray(attrib_start_time);

	SDL_GL_SwapBuffers();
}

static void free_resources(void) {
	glDeleteProgram(program);
	free(vertices);
	free(colors);
	free(velocities);
	free(start_times);
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
