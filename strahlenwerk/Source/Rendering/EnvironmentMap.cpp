#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include "EnvironmentMap.h"

EnvironmentMap::~EnvironmentMap() {
	if (deleteTexture) {
		glDeleteTextures(1, &texture);
	}
}
void EnvironmentMap::create(unsigned int width, unsigned int height) {
	if (deleteTexture) {
		glDeleteTextures(1, &texture);
	}
	glGenTextures(1, &texture);
	deleteTexture = true;
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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

void EnvironmentMap::render(SceneShader& shader) {
	// TODO
}

void EnvironmentMap::bind(GLenum textureUnit) {
	// TODO
}
