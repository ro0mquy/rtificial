#include <string.h>
#include <stdlib.h>

#include <libzeuch/quaternion.h>
#include <libzeuch/matrix.h>
#include "util.h"
#include "tweakable.h"

static size_t get_value_size(tweakable_type_t type);
static void init_value(void* const value, tweakable_type_t type, void* defaults);
static void update_rotation_uniform(GLint uniform_location, const float value[4]);

bool tweakable_init(
	tweakable_t* const tweakable,
	const char name[],
	const char uniform_name[],
	const tweakable_type_t type,
	const char varparam[],
	void* const defaults
) {
	char* const name_buffer = util_dup_string(name);
	char* const uniform_name_buffer = util_dup_string(uniform_name);
	void* const value_buffer = malloc(get_value_size(type));
	char* const varparam_buffer = util_dup_string(varparam);
	*tweakable = (tweakable_t) {
		.name = name_buffer,
		.uniform_name = uniform_name_buffer,
		.type = type,
		.value = value_buffer,
		.varparam = varparam_buffer,
		.uniform_location = -1,
	};
	if(name_buffer == NULL || uniform_name_buffer == NULL || value_buffer == NULL || varparam_buffer == NULL) {
		tweakable_destroy(tweakable);
		return false;
	}
	init_value(value_buffer, type, defaults);
	return true;
}

void tweakable_load_uniform(tweakable_t* const tweakable, const GLuint program) {
	tweakable->uniform_location = glGetUniformLocation(program, tweakable->uniform_name);
}

void tweakable_update_uniform(const tweakable_t* const tweakable) {
	const float* const value = tweakable->value;
	const unsigned char* const bool_value = tweakable->value;
	const GLint uniform_location = tweakable->uniform_location;
	switch(tweakable->type) {
		case COLOR:
			glUniform3f(uniform_location, value[0], value[1], value[2]);
			break;
		case BOOL:
			glUniform1ui(uniform_location, *bool_value);
			break;
		case DIR3:
			glUniform3f(uniform_location, value[0], value[1], value[2]);
			break;
		case ROTATION:
			update_rotation_uniform(uniform_location, value);
			break;
		case FLOAT:
		default:
			glUniform1f(uniform_location, value[0]);
			break;
	}
}

void tweakable_destroy(tweakable_t* const tweakable) {
	if(tweakable == NULL) return;
	util_safe_free(tweakable->name);
	util_safe_free(tweakable->uniform_name);
	util_safe_free(tweakable->value);
	util_safe_free(tweakable->varparam);
}

bool tweakable_get_type(const char name[], tweakable_type_t* const out_type) {
	if(strcmp(name, "color") == 0) {
		*out_type = COLOR;
	} else if(strcmp(name, "float") == 0) {
		*out_type = FLOAT;
	} else if(strcmp(name, "bool") == 0) {
		*out_type = BOOL;
	} else if(strcmp(name, "direction") == 0) {
		*out_type = DIR3;
	} else if(strcmp(name, "rotation") == 0) {
		*out_type = ROTATION;
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
		case BOOL:
			type = TW_TYPE_BOOL8;
			break;
		case DIR3:
			type = TW_TYPE_DIR3F;
			break;
		case ROTATION:
			type = TW_TYPE_QUAT4F;
			break;
		case FLOAT:
		default:
			type = TW_TYPE_FLOAT;
			break;
	}
	TwAddVarRW(tweak_bar, tweakable->name, type, tweakable->value, tweakable->varparam);
}


static size_t get_value_size(const tweakable_type_t type) {
	switch(type) {
		case COLOR:
			return 3 * sizeof(float);
		case BOOL:
			return sizeof(bool);
		case DIR3:
			return 3 * sizeof(float);
		case ROTATION:
			return 4 * sizeof(float);
		case FLOAT:
		default:
			return 1 * sizeof(float);
	}
}

static void init_value(void* const value, const tweakable_type_t type, void* defaults) {
	float* const float_value = (float*) value;
	float* const float_defaults = (float*) defaults;
	bool* const bool_value = (bool*) value;
	bool* const bool_defaults = (bool*) defaults;
	switch(type) {
		case COLOR:
			if (defaults == NULL) {
				float_value[0] = 1.;
				float_value[1] = 1.;
				float_value[2] = 1.;
			} else {
				float_value[0] = float_defaults[0];
				float_value[1] = float_defaults[1];
				float_value[2] = float_defaults[2];
			}
			break;
		case BOOL:
			if (defaults == NULL) {
				*bool_value = false;
			} else {
				*bool_value = *bool_defaults;
			}
			break;
		case DIR3:
			if(defaults == NULL) {
				float_value[0] = 1.;
				float_value[1] = 0.;
				float_value[2] = 0.;
			} else {
				float_value[0] = float_defaults[0];
				float_value[1] = float_defaults[1];
				float_value[2] = float_defaults[2];
			}
			break;
		case ROTATION:
			if(defaults == NULL) {
				float_value[0] = 0.;
				float_value[1] = 0.;
				float_value[2] = 0.;
				float_value[3] = 1.;
			} else {
				float_value[0] = float_defaults[0];
				float_value[1] = float_defaults[1];
				float_value[2] = float_defaults[2];
				float_value[3] = float_defaults[3];
			}
			break;
		case FLOAT:
		default:
			if (defaults == NULL) {
				float_value[0] = 0.;
			} else {
				float_value[0] = float_defaults[0];
			}
			break;
	}
}

static void update_rotation_uniform(GLint uniform_location, const float value[4]) {
	const quat q = quat_new(vec3_new(value[0], value[1], value[2]), value[3]);
	const mat4x4 mat4 = quat_to_mat4x4(q);
	// transposed for inverse rotation
	const GLfloat mat3[9] = {
		mat4.a[0][0],
		mat4.a[0][1],
		mat4.a[0][2],
		mat4.a[1][0],
		mat4.a[1][1],
		mat4.a[1][2],
		mat4.a[2][0],
		mat4.a[2][1],
		mat4.a[2][2],
	};
	glUniformMatrix3fv(uniform_location, 1, GL_FALSE, mat3);
}
