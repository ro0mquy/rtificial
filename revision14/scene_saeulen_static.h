#include "saeulen_static_keyframes.h"
#include "saeulen_static_source.h"

static GLuint saeulen_static_program;
static GLuint saeulen_static_attrib_c2d;
static uniforms_t saeulen_static_uniforms;
static timeline_t saeulen_static_timeline;

static void saeulen_static_init(GLuint vertex) {
	const GLchar* dinge[] =  { libblink_source, scene_saeulen_static_source };
	const GLuint fragment = shader_load_strings(2, "test", dinge, GL_FRAGMENT_SHADER);
	saeulen_static_program = shader_link_program(vertex, fragment);
	saeulen_static_attrib_c2d = glGetAttribLocation(saeulen_static_program, "c");
	get_uniforms(&saeulen_static_uniforms, saeulen_static_program);
	init_timeline(&saeulen_static_timeline, &saeulen_static_keyframe_list);
}

static void saeulen_static_draw() {
	glUseProgram(saeulen_static_program);
	update_uniforms(&saeulen_static_uniforms, &saeulen_static_timeline);
	draw_quad(saeulen_static_attrib_c2d);
}

