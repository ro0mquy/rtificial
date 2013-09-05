#ifndef SHADER_H
#define SHADER_H

#include "gl.h"

GLuint shader_load_file(const char filename[], GLenum type);
GLuint shader_load_str(const char name[], const char source[], GLenum type);
GLint shader_get_uniform(GLuint program, const char name[]);
void print_log(GLuint object);
GLuint shader_link_program(GLuint vertex_shader, GLuint fragment_shader);

#endif
