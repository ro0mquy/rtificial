#ifndef SHADER_H
#define SHADER_H

#include "gl.h"

GLuint shader_load_files(int n, const char* filenames[n], GLenum type);
GLuint shader_load_strings(int n, const char* name, const GLchar* sources[n], GLenum type);
GLint shader_get_uniform(GLuint program, const char name[]);
void print_log(GLuint object);
GLuint shader_link_program(GLuint vertex_shader, GLuint fragment_shader);

#endif
