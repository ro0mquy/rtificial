#include <libzeuch/shader.h>
#include <libzeuch/gl.h>
#include "window.h"
#include "font.h"

bool font_init(font_t* font, const char* const fontname, int fontsize) {
	// init FreeType library
	if(FT_Init_FreeType(&(font->ft_lib))) {
		fprintf(stderr, "Could not init freetype library\n");
		return false;
	}

	// load standard font
	if(FT_New_Face(font->ft_lib, fontname, 0, &(font->face))) {
		fprintf(stderr, "Could not open font\n");
		return false;
	}

	// set standard font size
	FT_Set_Pixel_Sizes(font->face, 0, fontsize);

	// compile shaders
	const GLuint vertex_shader = shader_load_strings(1, "font_vertex", (const GLchar* []) { font_vertex_source }, GL_VERTEX_SHADER);
	const GLuint fragment_shader = shader_load_strings(1, "font_fragment", (const GLchar* []) { font_fragment_source }, GL_FRAGMENT_SHADER);
	font->program = shader_link_program(vertex_shader, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// bind uniforms and attributes
	font->attribute_coord2d = glGetAttribLocation(font->program, "coord");
	font->uniform_tex = glGetUniformLocation(font->program, "tex");
	font->uniform_color = glGetUniformLocation(font->program, "color");

	glGenBuffers(1, &(font->vbo));
	return true;
}

void font_print(font_t font, const char* text, float x, float y) {
	// the width and height of a pixel expressed in opengl's -1. to 1. range
	float sx = 2. / window_get_width();
	float sy = 2. / window_get_height();

	FT_GlyphSlot g = font.face->glyph;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(font.program);
	glUniform4f(font.uniform_color, 1., 1., 1., 1.);

	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(font.uniform_tex, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnableVertexAttribArray(font.attribute_coord2d);
	glBindBuffer(GL_ARRAY_BUFFER, font.vbo);
	glVertexAttribPointer(font.attribute_coord2d, 4, GL_FLOAT, GL_FALSE, 0, 0);

	for(const char* p = text; *p; p++) {
		if(FT_Load_Char(font.face, *p, FT_LOAD_RENDER)) continue;

		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_ALPHA,
				g->bitmap.width,
				g->bitmap.rows,
				0,
				GL_ALPHA,
				GL_UNSIGNED_BYTE,
				g->bitmap.buffer
			    );

		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{x2,     -y2    , 0, 0},
			{x2 + w, -y2    , 1, 0},
			{x2,     -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(font.attribute_coord2d);
	glDeleteTextures(1, &tex);
}

void font_destroy(font_t* font) {
	glDeleteProgram(font->program);
	FT_Done_Face(font->face);
	FT_Done_FreeType(font->ft_lib);
}
