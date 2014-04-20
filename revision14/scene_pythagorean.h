#include "pythagorean_keyframes.h"
#include "pythagorean_source.h"

static GLuint pythagorean_program;
static GLuint pythagorean_attrib_c2d;
static uniforms_t pythagorean_uniforms;
static timeline_t pythagorean_timeline;

static void pythagorean_init(GLuint vertex) {
	const GLchar* bla[] = { libblink_source, scene_pythagorean_source };
	const GLuint fragment = shader_load_strings(2, "test", bla, GL_FRAGMENT_SHADER);
	pythagorean_program = shader_link_program(vertex, fragment);
	pythagorean_attrib_c2d = glGetAttribLocation(pythagorean_program, "c");
	get_uniforms(&pythagorean_uniforms, pythagorean_program);
	init_timeline(&pythagorean_timeline, &pythagorean_keyframe_list);
}

static void pythagorean_draw() {
	glUseProgram(pythagorean_program);
	update_uniforms(&pythagorean_uniforms, &pythagorean_timeline);
	draw_quad(pythagorean_attrib_c2d);
}

