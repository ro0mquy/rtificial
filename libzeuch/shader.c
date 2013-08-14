#include <stdlib.h>
#include <stdio.h>

#include "shader.h"

static char* file_read(const char filename[]);

GLuint shader_load(const char filename[], GLenum type) {
	GLchar* const source = file_read(filename);
	if(source == NULL) {
		fprintf(stderr, "Error reading %s: ", filename);
		perror("");
		return 0;
	}
	const GLuint res = glCreateShader(type);
	const GLchar* sources[1] = {
		source
	};
	glShaderSource(res, 1, sources, NULL);
	free(source);

	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if(compile_ok == GL_FALSE) {
		fprintf(stderr, "%s: ", filename);
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
/*
static char* file_read(const char filename[]) {
	FILE* const input = fopen(filename, "r");
	if(input == NULL) return NULL;

	size_t size = 4096;
	size_t content_length = 0;
	char* content = malloc(size + 1);
	if(content == NULL) return NULL;

	while(!feof(input) && !ferror(input)) {
		size_t read = fread(content, 1, size - content_length, input);
		content_length += read;
		if(size - content_length == 0) {
			char* new_content = realloc(content, size * 2);
			if(new_content == NULL) {
				fclose(input);
				return NULL;
			}
			content = new_content;
			size *= 2;
		}
	}
	fclose(input);
	if(ferror(input)) {
		fprintf(stderr, "Error reading file");
		return NULL;
	} else {
		content[content_length] = '\0';
		return content;
	}
}
*/

static char* file_read(const char filename[]) {
	FILE* const input = fopen(filename, "rb");
	if(input == NULL) return NULL;

	if(fseek(input, 0, SEEK_END) == -1) return NULL;
	const long int size = ftell(input);
	if(size == -1) return NULL;
	if(fseek(input, 0, SEEK_SET) == -1) return NULL;

	char* const content = malloc(size + 1);
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

	char* const log = malloc(log_length);

	if(glIsShader(object)) {
		glGetShaderInfoLog(object, log_length, NULL, log);
	} else if(glIsProgram(object)) {
		glGetProgramInfoLog(object, log_length, NULL, log);
	}

	fprintf(stderr, "%s", log);
	free(log);
}
