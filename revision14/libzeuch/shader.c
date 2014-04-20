#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "shader.h"

#define COLOR_RED_BOLD "\033[1;31m"
#define COLOR_RED      "\033[0;31m"
#define COLOR_RESET    "\033[0m"

static char* file_read(const char filename[]);

GLuint shader_load_files(int n, const char* filenames[], GLenum type) {
	GLchar** sources = (GLchar**) _malloca(n * sizeof(GLchar*));
	for(int i = 0; i < n; i++) {
		sources[i] = file_read(filenames[i]);
		if(sources[i] == NULL) {
			fprintf(stderr, "Error reading %s: ", filenames[i]);
			for(int j = 0; j < i; j++) {
				free(sources[j]);
			}
			perror("");
			return 0;
		}
	}
	GLuint res = shader_load_strings(n, filenames[n - 1], (const GLchar**) sources, type);
	for(int i = 0; i < n; i++) {
		free(sources[i]);
	}
	return res;
}

GLuint shader_load_strings(int n, const char* name, const GLchar* sources[], GLenum type) {
	const GLuint res = glCreateShader(type);
	glShaderSource(res, n, sources, NULL);

	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if(compile_ok == GL_FALSE) {
		fprintf(stderr, "%s: ", name);
		print_log(res);
		glDeleteShader(res);
		return 0;
	}
	return res;
}

GLint shader_get_uniform(GLuint program, const char name[]) {
	GLint uniform = glGetUniformLocation(program, name);
	if(uniform == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", name);
		return -1;
	}
	return uniform;
}

static char* file_read(const char filename[]) {
	FILE* const input = fopen(filename, "rb");
	if(input == NULL) return NULL;

	if(fseek(input, 0, SEEK_END) == -1) return NULL;
	const long int size = ftell(input);
	if(size == -1) return NULL;
	if(fseek(input, 0, SEEK_SET) == -1) return NULL;

	char* const content = (char *const) malloc(size + 1);
	if(content == NULL) return NULL;

	fread(content, 1, size, input);
	if(ferror(input)) {
		free(content);
		return NULL;
	}

	fclose(input);
	content[size] = '\0';
	return content;
}


void print_log(GLuint object) {
	GLint log_length = 0;
	if(glIsShader(object)) {
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} else if(glIsProgram(object)) {
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} else {
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* const log = (char *const) malloc(log_length);

	if(glIsShader(object)) {
		glGetShaderInfoLog(object, log_length, NULL, log);
	} else if(glIsProgram(object)) {
		glGetProgramInfoLog(object, log_length, NULL, log);
	}

	fprintf(stderr, "%s%s%s", COLOR_RED_BOLD, log, COLOR_RESET);
	free(log);
}

GLuint shader_link_program(GLuint vertex_shader, GLuint fragment_shader) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(link_ok == GL_FALSE) {
		fprintf(stderr, "glLinkProgram: ");
		print_log(program);
		return 0;
	}
	return program;

}
