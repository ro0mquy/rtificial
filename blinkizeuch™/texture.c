#include <IL/il.h>

#include "util.h"

#include "texture.h"
#include "urs.h"

static bool load_texture(const char path[], GLenum target);

bool texture_init(texture_t* texture, const char scene_path[], const char path[], const char uniform[]) {
	texture->path = NULL;
	texture->uniform_name = NULL;

	glGenTextures(1, &texture->tex);
	glBindTexture(GL_TEXTURE_2D, texture->tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// assemble texture path; "<scene_path><path_json>\0"
	const size_t scene_path_length = strlen(scene_path);
	const size_t path_length = strlen(path);
	char* path_texture = malloc(scene_path_length + path_length + 1);
	strncpy(path_texture, scene_path, scene_path_length);
	strncpy(path_texture + scene_path_length, path, path_length + 1);

	const bool load_success = load_texture(path_texture, GL_TEXTURE_2D);
	util_safe_free(path_texture);
	if(!load_success) {
		texture_destroy(texture);
		return false;
	}
	char* const uniform_copy = util_dup_string(uniform);
	if(uniform_copy == NULL) {
		texture_destroy(texture);
		return false;
	}
	texture->uniform_name = uniform_copy;

	char* const path_copy = util_dup_string(path);
	texture->path = path_copy;
	texture->target = GL_TEXTURE_2D;

	return true;
}

bool texture_init_cubemap(texture_t* texture, char* const path[], const char uniform[]) {
	texture->path = NULL;
	texture->uniform_name = NULL;

	glGenTextures(1, &texture->tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture->tex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	bool loaded = true;
	loaded &= load_texture(path[0], GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	loaded &= load_texture(path[1], GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	loaded &= load_texture(path[2], GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	loaded &= load_texture(path[3], GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	loaded &= load_texture(path[4], GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	loaded &= load_texture(path[5], GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	if(!loaded) {
		texture_destroy(texture);
		return false;
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	char* const uniform_copy = util_dup_string(uniform);
	if(uniform_copy == NULL) {
		texture_destroy(texture);
		return false;
	}
	texture->uniform_name = uniform_copy;
	texture->target = GL_TEXTURE_CUBE_MAP;

	return true;
}

void texture_load_uniform(texture_t* texture, GLuint program) {
	texture->uniform_location = glGetUniformLocation(program, texture->uniform_name);
}

void texture_bind(const texture_t* texture, int index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(texture->target, texture->tex);
	glUniform1i(texture->uniform_location, index);
}

void texture_destroy(texture_t* texture) {
	glDeleteTextures(1, &texture->tex);
	util_safe_free(texture->uniform_name);
	util_safe_free(texture->path);
}

static bool load_texture(const char path[], GLenum target) {
	ILuint il_tex;
	ilGenImages(1, &il_tex);
	ilBindImage(il_tex);
	if(!ilLoadImage(path)) {
		ilDeleteImages(1, &il_tex);
		return false;
	}
	if(!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
		ilDeleteImages(1, &il_tex);
		return false;
	}

	int width = URS_IMAGE_WIDTH;
	int height = URS_IMAGE_HEIGHT;
	ILubyte new_data[URS_IMAGE_WIDTH * URS_IMAGE_HEIGHT * 4];
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			new_data[4 * (y * width + x) + 0] = URS_IL_IMAGE[y * width + x];
			new_data[4 * (y * width + x) + 1] = URS_IL_IMAGE[y * width + x];
			new_data[4 * (y * width + x) + 2] = URS_IL_IMAGE[y * width + x];
			new_data[4 * (y * width + x) + 3] = 255;
		}
	}

	glTexImage2D(target, 0,
			GL_RGBA,
			URS_IMAGE_WIDTH,
			URS_IMAGE_HEIGHT,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			new_data);
	/*
	puts(path);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			printf("%3d ", *(ilGetData() + 4 * (y * width + x)));
			//printf("%3d ", *(ilGetData() + 1 + 4 * (y * width + x)));
			//printf("%3d ", *(ilGetData() + 2 + 4 * (y * width + x)));
			//printf("%3d\t", *(ilGetData() + 3 + 4 * (y * width + x)));
		}
		puts("");
	}*/

	ilDeleteImages(1, &il_tex);
	return true;
}
