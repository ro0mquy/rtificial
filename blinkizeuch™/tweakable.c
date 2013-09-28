#include <string.h>
#include <stdlib.h>

#include "tweakable.h"

static char* duplicate_string(const char string[]);
static void free_if_not_null(void* pointer);
static size_t get_value_size(tweakable_type_t type);
static void init_value(void* const value, tweakable_type_t type);

bool tweakable_init(
	tweakable_t* const tweakable,
	const char name[],
	const char uniform_name[],
	const tweakable_type_t type
) {
	char* const name_buffer = duplicate_string(name);
	char* const uniform_name_buffer = duplicate_string(uniform_name);
	void* const value_buffer = malloc(get_value_size(type));
	*tweakable = (tweakable_t) {
		.name = name_buffer,
		.uniform_name = uniform_name_buffer,
		.type = type,
		.value = value_buffer,
		.uniform_location = -1,
	};
	if(name_buffer == NULL || uniform_name == NULL || value_buffer == NULL) {
		tweakable_destroy(tweakable);
		return false;
	}
	init_value(value_buffer, type);
	return true;
}

void tweakable_load_uniform(tweakable_t* const tweakable, const GLuint program) {
	tweakable->uniform_location = glGetUniformLocation(program, tweakable->uniform_name);
}

void tweakable_update_uniform(const tweakable_t* const tweakable) {
	const float* const value = tweakable->value;
	const GLint uniform_location = tweakable->uniform_location;
	switch(tweakable->type) {
		case COLOR:
			glUniform3f(uniform_location, value[0], value[1], value[2]);
			break;
		case FLOAT:
		default:
			glUniform1f(uniform_location, value[0]);
			break;
	}
}

void tweakable_destroy(tweakable_t* const tweakable) {
	if(tweakable == NULL) return;
	free_if_not_null(tweakable->name);
	free_if_not_null(tweakable->uniform_name);
	free_if_not_null(tweakable->value);
}

bool tweakable_get_type(const char name[], tweakable_type_t* const out_type) {
	if(strcmp(name, "color") == 0) {
		*out_type = COLOR;
	} else if(strcmp(name, "float") == 0) {
		*out_type = FLOAT;
	} else {
		return false;
	}
	return true;
}

void tweakable_add_to_bar(const tweakable_t* const tweakable, TwBar* const tweak_bar) {
	TwType type;
	switch(tweakable->type) {
		case COLOR:
			type = TW_TYPE_COLOR3F;
			break;
		case FLOAT:
		default:
			type = TW_TYPE_FLOAT;
			break;
	}
	TwAddVarRW(tweak_bar, tweakable->name, type, tweakable->value, "");
}

static char* duplicate_string(const char string[]) {
	size_t length = strlen(string) + 1; // length including 0 byte
	char* const buffer = malloc(sizeof(char) * length);
	if(buffer == NULL) return NULL;
	return strncpy(buffer, string, length);
}

static void free_if_not_null(void* const pointer) {
	if(pointer != NULL) {
	   free(pointer);
	}
}

static size_t get_value_size(const tweakable_type_t type) {
	switch(type) {
		case COLOR:
			return 3 * sizeof(float);
		case FLOAT:
		default:
			return 1 * sizeof(float);
	}
}

static void init_value(void* const value, const tweakable_type_t type) {
	float* const float_value = (float*) value;
	switch(type) {
		case COLOR:
			float_value[0] = 0.;
			float_value[1] = 0.;
			float_value[2] = 0.;
			break;
		case FLOAT:
		default:
			float_value[0] = 0.;
			break;
	}
}
