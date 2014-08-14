#include "PostprocPipeline.h"

void PostprocPipeline::setShaders(std::vector<std::unique_ptr<PostprocShader>> shaders) {
	this->shaders = std::move(shaders);
}

void PostprocPipeline::render(SceneShader& shader) {
	if(shaders.size() > 2) {
		shaders[0]->bindFBO();
		shader.draw();
	}
	for(int i = 0; i < shaders.size() - 2; i++) {
		shaders[i+1]->bindFBO();
		shaders[i]->draw();
	}
	glEnable(GL_FRAMEBUFFER_SRGB);
	// bind default framebuffer again
	shaders[shaders.size() - 1]->bindFBO();
	if(shaders.size() > 2) {
		// this works becaue the last drawn shader has only one output (which should have location = 0)
		shaders[shaders.size() - 2]->draw();
	} else {
		shader.draw();
	}
	glDisable(GL_FRAMEBUFFER_SRGB);
}
