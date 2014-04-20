#include "saeulen_keyframes.h"
#include "saeulen_source.h"

static GLuint saeulen_program;
static GLuint saeulen_attrib_c2d;
static uniforms_t saeulen_uniforms;
static timeline_t saeulen_timeline;

static void saeulen_init(GLuint vertex) {
	const GLchar* foo[] = { libblink_source, scene_saeulen_source };
	const GLuint fragment = shader_load_strings(2, "test", foo , GL_FRAGMENT_SHADER);
	saeulen_program = shader_link_program(vertex, fragment);
	saeulen_attrib_c2d = glGetAttribLocation(saeulen_program, "c");
	get_uniforms(&saeulen_uniforms, saeulen_program);
	init_timeline(&saeulen_timeline, &saeulen_keyframe_list);
}

static void saeulen_draw() {
	glUseProgram(saeulen_program);
	update_uniforms(&saeulen_uniforms, &saeulen_timeline);
	draw_quad(saeulen_attrib_c2d);
}

