#include "credits_keyframes.h"
#include "credits_source.h"

#include "drb.df.h"
#include "vincent.df.h"
#include "ro0mquy.df.h"
#include "ps0ke.df.h"

static GLuint credits_program;
static GLuint credits_attrib_c2d;
static uniforms_t credits_uniforms;
static timeline_t credits_timeline;

static GLuint
	tex_vincent,
	tex_drb,
	tex_ps0ke,
	tex_ro0mquy;

static void credits_init(GLuint vertex) {
	const GLuint fragment = shader_load_strings(2, "test", (const GLchar* []) { libblink_source, scene_credits_source }, GL_FRAGMENT_SHADER);
	credits_program = shader_link_program(vertex, fragment);
	credits_attrib_c2d = glGetAttribLocation(credits_program, "c");
	get_uniforms(&credits_uniforms, credits_program);
	init_timeline(&credits_timeline, &credits_keyframe_list);
	glUseProgram(credits_program);
	const GLint uniform_tex_vincent = shader_get_uniform(credits_program, "tex_vincent");
	const GLint uniform_tex_drb = shader_get_uniform(credits_program, "tex_drb");
	const GLint uniform_tex_ps0ke = shader_get_uniform(credits_program, "tex_ps0ke");
	const GLint uniform_tex_ro0mquy = shader_get_uniform(credits_program, "tex_ro0mquy");
	glUniform1i(uniform_tex_vincent, 0);
	glUniform1i(uniform_tex_drb, 1);
	glUniform1i(uniform_tex_ps0ke, 2);
	glUniform1i(uniform_tex_ro0mquy, 3);
	load_texture(&tex_vincent, VINCENT_IL_IMAGE, VINCENT_IMAGE_WIDTH, VINCENT_IMAGE_HEIGHT);
	load_texture(&tex_drb, DRB_IL_IMAGE, DRB_IMAGE_WIDTH, DRB_IMAGE_HEIGHT);
	load_texture(&tex_ps0ke, PS0KE_IL_IMAGE, PS0KE_IMAGE_WIDTH, PS0KE_IMAGE_HEIGHT);
	load_texture(&tex_ro0mquy, RO0MQUY_IL_IMAGE, RO0MQUY_IMAGE_WIDTH, RO0MQUY_IMAGE_HEIGHT);
}

static void credits_draw() {
	glUseProgram(credits_program);
	glActiveTexture(GL_TEXTURE_0);
	glBindTexture(GL_TEXTURE_2D, tex_vincent);
	glActiveTexture(GL_TEXTURE_1);
	glBindTexture(GL_TEXTURE_2D, tex_drb);
	glActiveTexture(GL_TEXTURE_2);
	glBindTexture(GL_TEXTURE_2D, tex_ps0ke);
	glActiveTexture(GL_TEXTURE_3);
	glBindTexture(GL_TEXTURE_2D, tex_ro0mquy);
	update_uniforms(&credits_uniforms, &credits_timeline);
	draw_quad(credits_attrib_c2d);
}
