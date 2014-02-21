#ifndef FONT_H
#define FONT_H

#include <stdbool.h>

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
	FT_Library ft_lib;
	FT_Face face;
	GLuint program;
	GLuint vbo;
	GLint attribute_coord2d;
	GLint uniform_tex;
	GLint uniform_color;
} font_t;

bool font_init(font_t* font, const char* const fontname, int fontsize);
void font_print(font_t font, const char* text, float x, float y);
void font_destroy(font_t font);

static const char font_vertex_source[] =
"#version 330\n"

"in vec4 coord;"
//"in vec2 texcoord;"
"out vec2 Texcoord;"

"void main(void) {"
"	gl_Position = vec4(coord.xy, 0, 1);"
"	Texcoord = coord.zw;"
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
