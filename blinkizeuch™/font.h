#ifndef FONT_H
#define FONT_H

#include <stdbool.h>

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
	FT_Library ft_lib;
	FT_Face face;
	GLuint program;
	GLuint tex;
	GLuint vbo;
	GLint uniform_tex;
	GLint uniform_color;
} font_t;

bool font_init(font_t* font);
void font_render_text(font_t font, const char *text, float x, float y, float sx, float sy);

static const char font_vertex_source[] =
"#version 330\n"

"in vec2 coord;"
"in vec2 texcoord;"
"out vec2 Texcoord;"

"void main(void) {"
"	gl_Position = vec4(coord, 0, 1);"
"	Texcoord = texcoord;"
"}"
;

static const char font_fragment_source[] =
"#version 330\n"

"in vec2 Texcoord;"
"uniform sampler2D tex;"
"uniform vec4 color;"
"out vec4 out_color;"

"void main(void) {"
"	out_color = vec4(1, 1, 1, texture2D(tex, Texcoord).a) * color;"
"}"
;

#endif // FONT_H
