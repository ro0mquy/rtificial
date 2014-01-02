#ifndef TWEAKABLE_H
#define TWEAKABLE_H

#include <stdbool.h>

#include <AntTweakBar.h>

#include <libzeuch/gl.h>

typedef enum tweakable_type_t {
	COLOR, FLOAT
} tweakable_type_t;

typedef struct tweakable_t {
	char* name;
	char* uniform_name;
	tweakable_type_t type;
	void* value;
	GLint uniform_location;
} tweakable_t;

bool tweakable_init(
	tweakable_t* tweakable,
	const char name[],
	const char uniform_name[],
	tweakable_type_t type_name,
	const void* defaults
);

void tweakable_load_uniform(tweakable_t* tweakable, GLuint program);

void tweakable_update_uniform(const tweakable_t* tweakable);

void tweakable_destroy(tweakable_t* tweakable);

bool tweakable_get_type(const char name[], tweakable_type_t* out_type);

void tweakable_add_to_bar(const tweakable_t* tweakable, TwBar* tweak_bar);

#endif
