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

	if(shaders.size() > 2) {
		shaders[0]->bindFBO(width, height);
		shader.draw(width, height);
		shaders[0]->unbindFBO();
	}
	for(int i = 1; i < shaders.size() - 2; i++) {
		shaders[i]->bindFBO(width, height);
		shaders[i]->draw(width, height);
		shaders[i]->unbindFBO();
	}
	glEnable(GL_FRAMEBUFFER_SRGB);
	// bind default framebuffer again
	// TODO bind the default FBO without accessing the shaders array
	shaders[shaders.size() - 1]->bindFBO(width, height);
	if(shaders.size() > 2) {
		// this works becaue the last drawn shader has only one output (which should have location = 0)
		shaders[shaders.size() - 2]->draw(width, height);
	} else {
		shader.draw(width, height);
	}
	glDisable(GL_FRAMEBUFFER_SRGB);
}
