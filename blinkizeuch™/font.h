#ifndef FONT_H
#def FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
	FT_Face face;
	GLuint tex;
	GLuint vbo;
} font_t;

static const char font_fragment_source[] =
"#version 120"

"varying vec2 texcoord;"
"uniform sampler2D tex;"
"uniform vec4 color;"

"void main(void) {"
"	gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).a) * color;"
"}"
;

static const char font_vertex_source[] =
"#version 120"

"attribute vec4 coord;"
"varying vec2 texcoord;"

"void main(void) {"
"	gl_Position = vec4(coord.xy, 0, 1);"
"	texcoord = coord.zw;"
"}"
;

#endif
