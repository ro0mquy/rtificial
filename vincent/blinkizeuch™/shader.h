#ifndef SHADER_H
#define SHADER_H

#include "gl.h"

GLuint shader_load(const char filename[], GLenum type);
GLint shader_get_uniform(GLuint program, const char name[]);
void print_log(GLuint object);

#endif
