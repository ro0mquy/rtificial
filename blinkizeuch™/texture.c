#include <IL/il.h>

#include "util.h"

#include "texture.h"

bool texture_init(texture_t* texture, const char path[], const char uniform[]) {
	ILuint il_tex;
	ilGenImages(1, &il_tex);
	ilBindImage(il_tex);
	if(!ilLoadImage(path)) {
		ilDeleteImages(1, &il_tex);
		return false;
	}
	if(!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
		ilDeleteImages(1, &il_tex);
		return false;
	}

	glGenTextures(1, &texture->tex);
	glBindTexture(GL_TEXTURE_2D, texture->tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0,
			ilGetInteger(IL_IMAGE_FORMAT),
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT),
			0,
			ilGetInteger(IL_IMAGE_FORMAT),
			ilGetInteger(IL_IMAGE_TYPE),
			ilGetData());

	ilDeleteImages(1, &il_tex);

	char* const uniform_copy = util_dup_string(uniform);
	if(uniform_copy == NULL) {
		glDeleteTextures(1, &texture->tex);
	}
	texture->uniform_name = uniform_copy;

	char* const path_copy = util_dup_string(path);
	texture->path = path_copy;

	return true;
}

void texture_load_uniform(texture_t* texture, GLuint program) {
	texture->uniform_location = glGetUniformLocation(program, texture->uniform_name);
}

void texture_bind(const texture_t* texture, int index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture->tex);
	glUniform1i(texture->uniform_location, index);
}

void texture_destroy(texture_t* texture) {
	glDeleteTextures(1, &texture->tex);
	util_safe_free(texture->uniform_name);
	util_safe_free(texture->path);
}
