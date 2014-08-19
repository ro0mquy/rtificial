#include "PostprocPipeline.h"

void PostprocPipeline::setShaders(std::vector<std::unique_ptr<PostprocShader>> _shaders) {
	shaders = std::move(_shaders);
}

void PostprocPipeline::render(SceneShader& shader) {
	if(shaders.empty()) {
		return;
	}

	if(shaders.size() > 2) {
		shaders[0]->bindFBO();
		shader.draw();
	}
	for(int i = 1; i < shaders.size() - 2; i++) {
		shaders[i]->bindFBO();
		shaders[i]->draw();
	}
	glEnable(GL_FRAMEBUFFER_SRGB);
	// bind default framebuffer again
	// TODO bind the default FBO without accessing the shaders array
	shaders[shaders.size() - 1]->bindFBO();
	if(shaders.size() > 2) {
		// this works becaue the last drawn shader has only one output (which should have location = 0)
		shaders[shaders.size() - 2]->draw();
	} else {
		shader.draw();
	}
	glDisable(GL_FRAMEBUFFER_SRGB);
}
