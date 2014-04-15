#include <SDL/SDL.h>

#include <libzeuch/shader.h>
#include <libzeuch/gl.h>

#include "vertex_source.h"
#include "scene_blank.h"

//#define WIDTH 1366
//#define HEIGHT 768
//#define FULLSCREEN

#define WIDTH 800
#define HEIGHT 600

#define true 1
#define false 0

static void draw_quad(GLint attribute_coord2d);

GLuint vbo_rectangle;

void main() {
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
		.callback = NULL,
		.userdata = NULL
	};

	const GLuint vertex = shader_load_strings(1, "vertex", (const GLchar* []) { vertex_source }, GL_VERTEX_SHADER);
	const GLuint fragment = shader_load_strings(1, "fragment", (const GLchar* []) { scene_blank_source }, GL_FRAGMENT_SHADER);
	const GLuint program = shader_link_program(vertex, fragment);
	const GLint attribute_coord2d = glGetAttribLocation(program, "c");
	glGenBuffers(1, &vbo_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_rectangle);
	const GLfloat rectangle_vertices[] = {
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
	glUseProgram(program);

	int run = true;
	while(run) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					run = false;
					break;
			}

			draw_quad(attribute_coord2d);

			SDL_GL_SwapBuffers();
		}
	}

	// probably not gonna need that
	// SDL_Quit();
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
