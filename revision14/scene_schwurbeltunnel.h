#include "schwurbeltunnel_keyframes.h"
#include "schwurbeltunnel_source.h"

static GLuint schwurbeltunnel_program;
static GLuint schwurbeltunnel_attrib_c2d;
static uniforms_t schwurbeltunnel_uniforms;
static timeline_t schwurbeltunnel_timeline;

static void schwurbeltunnel_init(GLuint vertex) {
	const GLuint fragment = shader_load_strings(2, "test", (const GLchar* []) { libblink_source, scene_schwurbeltunnel_source }, GL_FRAGMENT_SHADER);
	schwurbeltunnel_program = shader_link_program(vertex, fragment);
	schwurbeltunnel_attrib_c2d = glGetAttribLocation(schwurbeltunnel_program, "c");
	get_uniforms(&schwurbeltunnel_uniforms, schwurbeltunnel_program);
	init_timeline(&schwurbeltunnel_timeline, &schwurbeltunnel_keyframe_list);
}

static void schwurbeltunnel_draw() {
	glUseProgram(schwurbeltunnel_program);
	update_uniforms(&schwurbeltunnel_uniforms, &schwurbeltunnel_timeline);
	draw_quad(schwurbeltunnel_attrib_c2d);
}

