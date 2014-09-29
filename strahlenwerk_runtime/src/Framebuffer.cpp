#include "Framebuffer.h"

Framebuffer::Framebuffer(int _outputLod, int _outputsNumber, const Output* _outputs) :
	outputLod(_outputLod),
	outputsNumber(_outputsNumber),
	outputs(_outputs)
{
	textures = new GLuint[outputsNumber];
}

Framebuffer::~Framebuffer() {
	delete[] textures;
}

void Framebuffer::create(int _width, int _height) {
	int factor = 1;
	for(int i = 0; i < outputLod; i++) {
		factor *= 2;
	}

	width = _width / factor;
	height = _height / factor;

	glGenFramebuffers(1, &fbo);
	glGenTextures(outputsNumber, textures);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	GLenum drawBuffers[outputsNumber];
	for(int i = 0; i < outputsNumber; i++) {
		glActiveTexture(GL_TEXTURE0 + outputs[i].bindingId);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		GLint internalFormat;
		GLenum format;
		switch(outputs[i].components) {
			case 1:
				internalFormat = GL_R16F;
				format = GL_RED;
				break;
			case 2:
				internalFormat = GL_RG16F;
				format = GL_RG;
				break;
			case 3:
				internalFormat = GL_RGB16F;
				format = GL_RGB;
				break;
			case 4:
			default:
				internalFormat = GL_RGBA16F;
				format = GL_RGBA;
				break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if(outputs[i].maxLod - outputLod > 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, outputs[i].maxLod - outputLod);
			glGenerateMipmap(GL_TEXTURE_2D); // allocate mipmap memory
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(outputsNumber, drawBuffers);
}

void Framebuffer::bind() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
}

void Framebuffer::unbind() {
	for(int i = 0; i < outputsNumber; i++) {
		glActiveTexture(GL_TEXTURE0 + outputs[i].bindingId);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		if(outputs[i].maxLod - outputLod > 0) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
}

void Framebuffer::destroy() {
	glDeleteTextures(outputsNumber, textures);
	glDeleteFramebuffers(1, &fbo);
}
