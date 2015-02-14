#define GL_GLEXT_PROTOTYPES
#include "glcorert.h"

#include "EnvironmentMap.h"
#include "Shader.h"

EnvironmentMap::~EnvironmentMap() {
	if (deleteTexture) {
		glDeleteTextures(1, &texture);
	}
}

void EnvironmentMap::create(unsigned int width_, unsigned int height_) {
	width = width_;
	height = height_;

	if (deleteTexture) {
		glDeleteTextures(1, &texture);
	}
	glGenTextures(1, &texture);
	deleteTexture = true;

	// could just use the current active texture unit TODO
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// allocate cubemap faces textures
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

	// clear texture (OpenGL 4.4 required)
	GLfloat black[] = {0, 0, 0};
	glClearTexImage(texture, 0, GL_RGB, GL_FLOAT, black);

	// generate mipmaps, just to be safe
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void EnvironmentMap::render(Shader& shader) {
	// create temporary fbo
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	// probably we don't have to bind the texture
	// glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	// could just use the current active texture unit TODO
	glActiveTexture(GL_TEXTURE0);

	// bind each of the 6 faces to one color attachment
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, texture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, texture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, texture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, texture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, texture, 0);

	shader.use();
	// render each of the 6 faces
	// uniform 35 is camera direction (?)
	// TODO use some other uniform location and/or document the use of it!
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glUniform3f(35, -1,  0,  0);
	shader.drawWithoutRecompile(width, height);
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glUniform3f(35,  1,  0,  0);
	shader.drawWithoutRecompile(width, height);
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glUniform3f(35,  0, -1,  0);
	shader.drawWithoutRecompile(width, height);
	glDrawBuffer(GL_COLOR_ATTACHMENT3);
	glUniform3f(35,  0,  1,  0);
	shader.drawWithoutRecompile(width, height);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glUniform3f(35,  0,  0, -1);
	shader.drawWithoutRecompile(width, height);
	glDrawBuffer(GL_COLOR_ATTACHMENT5);
	glUniform3f(35,  0,  0,  1);
	shader.drawWithoutRecompile(width, height);

	glDeleteFramebuffers(1, &fbo);

	// update mipmaps
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void EnvironmentMap::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}
