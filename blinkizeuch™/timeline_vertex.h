#ifndef TIMELINE_VERTEX_H
#define TIMELINE_VERTEX_H

static const char timeline_vertex_source[] =
"#version 330\n"
"in vec2 coord2d;"
"void main(void) {"
"	gl_Position = vec4(coord2d * 2. - 1., 0., 1.);"
"}"
;

#endif
