#include "ufo_keyframes.h"
#include "ufo_source.h"

static GLuint ufo_program;
static GLuint ufo_attrib_c2d;
static uniforms_t ufo_uniforms;
static timeline_t ufo_timeline;

static void ufo_init(GLuint vertex) {
	const GLuint fragment = shader_load_strings(2, "test", (const GLchar* []) { libblink_source, scene_ufo_source }, GL_FRAGMENT_SHADER);
	ufo_program = shader_link_program(vertex, fragment);
	ufo_attrib_c2d = glGetAttribLocation(ufo_program, "c");
	get_uniforms(&ufo_uniforms, ufo_program);
	init_timeline(&ufo_timeline, &ufo_keyframe_list);
}

static void ufo_draw() {
	glUseProgram(ufo_program);
	update_uniforms(&ufo_uniforms, &ufo_timeline);
	draw_quad(ufo_attrib_c2d);
}

