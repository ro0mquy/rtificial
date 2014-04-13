#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>

#include <libzeuch/gl.h>

typedef struct {
	char* uniform_name;
	char* path;
	GLuint tex;
	GLint uniform_location;
	GLuint target;
} texture_t;

bool texture_init(texture_t* texture, const char scene_path[], const char path[], const char uniform[]);

bool texture_init_cubemap(texture_t* texture, char* const path[], const char uniform[]);

void texture_load_uniform(texture_t* texture, GLuint program);

void texture_bind(const texture_t* texture, int index);

void texture_destroy(texture_t* texture);

#endif
