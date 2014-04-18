#include "gitter_keyframes.h"
#include "gitter_source.h"

#include "texture/alcatraz.h"
#include "texture/conspiracy.h"
#include "texture/greetings.h"
#include "texture/iq.h"
#include "texture/mercury.h"
#include "texture/nerd2nerd.h"
#include "texture/nerdarzt.h"
#include "texture/stroboholics.h"
#include "texture/urs.h"


static GLuint gitter_program;
static GLuint gitter_attrib_c2d;
static uniforms_t gitter_uniforms;
static timeline_t gitter_timeline;

static GLuint
	tex_alcatraz,
	tex_conspiracy,
	tex_greetings,
	tex_iq,
	tex_mercury,
	tex_nerd2nerd,
	tex_nerdarzt,
	tex_stroboholics,
	tex_urs;

static void gitter_init(GLuint vertex) {
	const GLuint fragment = shader_load_strings(2, "test", (const GLchar* []) { libblink_source, scene_gitter_source }, GL_FRAGMENT_SHADER);
	gitter_program = shader_link_program(vertex, fragment);
	gitter_attrib_c2d = glGetAttribLocation(gitter_program, "c");
	get_uniforms(&gitter_uniforms, gitter_program);
	glUseProgram(gitter_program);
	const GLint uniform_tex_alcatraz = shader_get_uniform(gitter_program, "tex_alcatraz");
	const GLint uniform_tex_conspiracy = shader_get_uniform(gitter_program, "tex_conspiracy");
	const GLint uniform_tex_greetings = shader_get_uniform(gitter_program, "tex_greetings");
	const GLint uniform_tex_iq = shader_get_uniform(gitter_program, "tex_iq");
	const GLint uniform_tex_mercury = shader_get_uniform(gitter_program, "tex_mercury");
	const GLint uniform_tex_nerd2nerd = shader_get_uniform(gitter_program, "tex_nerd2nerd");
	const GLint uniform_tex_nerdarzt = shader_get_uniform(gitter_program, "tex_nerdarzt");
	const GLint uniform_tex_stroboholics = shader_get_uniform(gitter_program, "tex_stroboholics");
	const GLint uniform_tex_urs = shader_get_uniform(gitter_program, "tex_urs");
	glUniform1i(uniform_tex_alcatraz, 0);
	glUniform1i(uniform_tex_conspiracy, 1);
	glUniform1i(uniform_tex_greetings, 2);
	glUniform1i(uniform_tex_iq, 3);
	glUniform1i(uniform_tex_mercury, 4);
	glUniform1i(uniform_tex_nerd2nerd, 5);
	glUniform1i(uniform_tex_nerdarzt, 6);
	glUniform1i(uniform_tex_stroboholics, 7);
	glUniform1i(uniform_tex_urs, 8);

	init_timeline(&gitter_timeline, &gitter_keyframe_list);
	load_texture(&tex_alcatraz, ALCATRAZ_IL_IMAGE, ALCATRAZ_IMAGE_WIDTH, ALCATRAZ_IMAGE_HEIGHT);
	load_texture(&tex_conspiracy, CONSPIRACY_IL_IMAGE, CONSPIRACY_IMAGE_WIDTH, CONSPIRACY_IMAGE_HEIGHT);
	load_texture(&tex_greetings, GREETINGS_IL_IMAGE, GREETINGS_IMAGE_WIDTH, GREETINGS_IMAGE_HEIGHT);
	load_texture(&tex_iq, IQ_IL_IMAGE, IQ_IMAGE_WIDTH, IQ_IMAGE_HEIGHT);
	load_texture(&tex_mercury, MERCURY_IL_IMAGE, MERCURY_IMAGE_WIDTH, MERCURY_IMAGE_HEIGHT);
	load_texture(&tex_nerd2nerd, NERD2NERD_IL_IMAGE, NERD2NERD_IMAGE_WIDTH, NERD2NERD_IMAGE_HEIGHT);
	load_texture(&tex_nerdarzt, NERDARZT_IL_IMAGE, NERDARZT_IMAGE_WIDTH, NERDARZT_IMAGE_HEIGHT);
	load_texture(&tex_stroboholics, STROBOHOLICS_IL_IMAGE, STROBOHOLICS_IMAGE_WIDTH, STROBOHOLICS_IMAGE_HEIGHT);
	load_texture(&tex_urs, URS_IL_IMAGE, URS_IMAGE_WIDTH, URS_IMAGE_HEIGHT);
}

static void gitter_draw() {
	glUseProgram(gitter_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_alcatraz);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, tex_conspiracy);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, tex_greetings);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, tex_iq);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, tex_mercury);
	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, tex_nerd2nerd);
	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_2D, tex_nerdarzt);
	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D, tex_stroboholics);
	glActiveTexture(GL_TEXTURE0 + 8);
	glBindTexture(GL_TEXTURE_2D, tex_urs);
	update_uniforms(&gitter_uniforms, &gitter_timeline);
	draw_quad(gitter_attrib_c2d);
}
