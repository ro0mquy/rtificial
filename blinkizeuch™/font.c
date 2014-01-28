#include <libzeuch/shader.h>
#include <libzeuch/gl.h>
#include "font.h"

const char standard_font[] = "FreeMono.ttf";
const int standard_size = 48;

void font_init() {
	// init FreeType library
	FT_Library ft;
	if(FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return 1;
	}

	// load standard font
	FT_Face face;
	if(FT_New_Face(ft, standard_font, 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return 1;
	}

	// set standard font size
	FT_Set_Pixel_Sizes(face, 0, standard_size);

		const GLuint vertex_shader = shader_load_strings(1, "font_vertex", (const GLchar* []) { font_vertex_source }, GL_VERTEX_SHADER);
		const GLuint fragment_shader = shader_load_strings(1, "font_fragment", (const GLchar* []) { font_fragment_source }, GL_FRAGMENT_SHADER);
		const GLuint program = shader_link_program(vertex_shader, fragment_shader);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &(font->tex));
	glBindTexture(GL_TEXTURE_2D, font->tex);
	glUniform1i(uniform_tex, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenBuffers(1, &(font->vbo));
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void render_text(font_t* font, const char *text, float x, float y, float sx, float sy) {
	FT_GlyphSlot g = font->face->glyph;

	for(const char* p = text; *p; p++) {
		if(FT_Load_Char(face, *p, FT_LOAD_RENDER)) continue;

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
}
