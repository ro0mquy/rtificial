#include "BRDFLut.h"
#include "BRDFIntegratorSource.h"
#include "Shader.h"

BRDFLut::BRDFLut() {
	const unsigned int brdfLUTWidth = 512;
	const unsigned int brdfLUTHeight = 512;

	glGenTextures(1, &brdfLUT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brdfLUT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, brdfLUTWidth, brdfLUTHeight, 0, GL_RG, GL_INT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glViewport(0, 0, brdfLUTWidth, brdfLUTHeight);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	Shader brdfIntegratorShader(brdfIntegratorSource, 0, nullptr);
	brdfIntegratorShader.compile();
	brdfIntegratorShader.bind();
	brdfIntegratorShader.draw(brdfLUTWidth, brdfLUTHeight, 0);
	brdfIntegratorShader.destroy();
	glDeleteFramebuffers(1, &fbo);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brdfLUT);
}

BRDFLut::~BRDFLut() {
	glDeleteTextures(1, &brdfLUT);
}
