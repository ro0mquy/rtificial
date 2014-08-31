#include "PostprocPipeline.h"

#include "SceneShader.h"
#include "PostprocShader.h"

PostprocPipeline::~PostprocPipeline() = default;

void PostprocPipeline::setShaders(std::vector<std::unique_ptr<PostprocShader>> _shaders) {
	shaders = std::move(_shaders);
}

void PostprocPipeline::render(SceneShader& shader, int width, int height) {
	if(shaders.empty()) {
		return;
	}

	if(shaders.size() > 1) {
		shaders[0]->bindFBO(width, height);
		shader.draw(shaders[0]->getCreatedWidth(), shaders[0]->getCreatedHeight());
		shaders[0]->unbindFBO();
	}
	for(int i = 1; i < shaders.size() - 1; i++) {
		shaders[i]->bindFBO(width, height);
		shaders[i]->draw(width, height);
		shaders[i]->unbindFBO();
	}
	glEnable(GL_FRAMEBUFFER_SRGB);
	// bind default framebuffer again
	OpenGLContext::getCurrentContext()->extensions.glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	if(shaders.size() > 1) {
		// this works becaue the last drawn shader has only one output (which should have location = 0)
		shaders[shaders.size() - 1]->draw(width, height);
	} else {
		shader.draw(width, height);
	}
	glDisable(GL_FRAMEBUFFER_SRGB);
}
