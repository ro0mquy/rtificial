#ifndef VERTEX_H
#define VERTEX_H

static const char vertex_source[] = "\
#version 330\n\
in vec2 coord2d;\
\
void main(void) {\
	gl_Position = vec4(coord2d, 0.0, 1.0);\
}\
";

static const char post_vertex_source[] = "\
#version 330\n\
in vec2 coord2d;\
out vec2 texcoord;\
void main() {\
	texcoord = (coord2d + 1.) / 2.;\
	gl_Position = vec4(coord2d, 0.0, 1.0);\
}\
";

#endif
