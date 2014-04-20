#include "ladebalken_source.h"
static GLuint balken_program;
static GLuint balken_attrib_c2d;
static GLint uniform_progress;

void ladebalken_init(GLuint vertex) {
	const GLchar* kram[] = { scene_ladebalken_source };
	const GLuint fragment = shader_load_strings(1, "balken", kram, GL_FRAGMENT_SHADER);
	balken_program = shader_link_program(vertex, fragment);
	balken_attrib_c2d = glGetAttribLocation(balken_program, "c");
	GLint res = shader_get_uniform(balken_program, "res");
	uniform_progress = shader_get_uniform(balken_program, "progress");
	glUseProgram(balken_program);
	glUniform2f(res, WIDTH, HEIGHT);
}

void ladebalken_draw(float progress) {
	glUseProgram(balken_program);
	glUniform1f(uniform_progress, progress);
	draw_quad(balken_attrib_c2d);
	SDL_GL_SwapBuffers();
}
