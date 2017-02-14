#include "EnvironmentMap.h"
#include "strahlenwerk_export_interface.h"

EnvironmentMap::EnvironmentMap(GLenum textureUnit_) : textureUnit(textureUnit_) {}

void EnvironmentMap::create(unsigned int width, unsigned int height, unsigned int levels, Shader& shader) {
	glGenTextures(1, &texture);

	// TODO why this unit?
	glActiveTexture(GL_TEXTURE4 + textureOffsetAmbient);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, levels - 1);

	// allocate cubemap faces textures
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

	// allocate mipmaps
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// render
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	shader.bind();
	unsigned int currentWidth = width;
	unsigned int currentHeight = height;

	for (unsigned int level = 0; level < levels; level++) {
		glViewport(0, 0, currentWidth, currentHeight);

		// bind each of the 6 faces to one color attachment
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, texture, level);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture, level);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, texture, level);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, texture, level);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, texture, level);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, texture, level);

		// render each of the 6 faces
		// uniform 35 is camera direction (?)
		// uniform 36 is up vector
		// uniform 37 is level
		// TODO use some other uniform location and/or document the use of it!
		glUniform1i(37, level);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glUniform3f(35, -1,  0,  0);
		glUniform3f(36,  0,  1,  0);
		shader.draw(currentWidth, currentHeight, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT1);
		glUniform3f(35,  1,  0,  0);
		glUniform3f(36,  0,  1,  0);
		shader.draw(currentWidth, currentHeight, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT2);
		glUniform3f(35,  0, -1,  0);
		glUniform3f(36,  0,  0,  1);
		shader.draw(currentWidth, currentHeight, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		glUniform3f(35,  0,  1,  0);
		glUniform3f(36,  0,  0, -1);
		shader.draw(currentWidth, currentHeight, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		glUniform3f(35,  0,  0, -1);
		glUniform3f(36,  0,  1,  0);
		shader.draw(currentWidth, currentHeight, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT5);
		glUniform3f(35,  0,  0,  1);
		glUniform3f(36,  0,  1,  0);
		shader.draw(currentWidth, currentHeight, 0);

		currentWidth /= 2;
		currentHeight /= 2;
	}

	glDeleteFramebuffers(1, &fbo);
}

void EnvironmentMap::destroy() {
	glDeleteTextures(1, &texture);
}

void EnvironmentMap::bind() {
	glActiveTexture(textureUnit + textureOffsetAmbient);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}
