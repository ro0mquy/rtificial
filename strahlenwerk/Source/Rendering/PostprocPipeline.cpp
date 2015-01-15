#include "PostprocPipeline.h"

#include "SceneShader.h"
#include "PostprocShader.h"
#include <StrahlenwerkApplication.h>
#include <PropertyNames.h>

PostprocPipeline::~PostprocPipeline() = default;

void PostprocPipeline::setShaders(std::vector<std::unique_ptr<PostprocShader>> _shaders) {
	shaders = std::move(_shaders);
}

uint64_t PostprocPipeline::render(SceneShader& shader, int width, int height) {
	// TODO evaluate "double buffering" of query objects
	if(shaders.empty()) {
		return 0;
	}

	const auto requiredQueries = shaders.size(); // number of drawing operations
	if(queries.size() != requiredQueries) {
		if(!queries.empty()) {
			glDeleteQueries(queries.size(), &queries[0]);
		}
		queries.resize(requiredQueries);
		glGenQueries(requiredQueries, &queries[0]);
	}
	std::vector<std::string> queryNames;
	int queryIndex = 0;

	// render intermediate passes at a lower resolution if requested
	const int originalWidth = width;
	const int originalHeigth = height;
	if (StrahlenwerkApplication::getInstance()->getProperties().getBoolValue(PropertyNames::HalfResolutionEnabled)) {
		width /= 2;
		height /= 2;
	}

	if(shaders.size() > 1) {
		shaders[0]->bindFBO(width, height);
		queryNames.push_back(shader.getName());
		glBeginQuery(GL_TIME_ELAPSED, queries[queryIndex++]);
		shader.draw(shaders[0]->getCreatedWidth(), shaders[0]->getCreatedHeight());
		glEndQuery(GL_TIME_ELAPSED);
		shaders[0]->unbindFBO();
	}
	const int shadersSize = shaders.size() - 1;
	for(int i = 1; i < shadersSize; i++) {
		shaders[i]->bindFBO(width, height);
		queryNames.push_back(shaders[i]->getName());
		glBeginQuery(GL_TIME_ELAPSED, queries[queryIndex++]);
		shaders[i]->draw(shaders[i]->getCreatedWidth(), shaders[i]->getCreatedHeight());
		glEndQuery(GL_TIME_ELAPSED);
		shaders[i]->unbindFBO();
	}
	glEnable(GL_FRAMEBUFFER_SRGB);
	// bind default framebuffer again
	OpenGLContext::getCurrentContext()->extensions.glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, originalWidth, originalHeigth);
	glBeginQuery(GL_TIME_ELAPSED, queries[queryIndex++]);
	if(shaders.size() > 1) {
		// this works becaue the last drawn shader has only one output (which should have location = 0)
		shaders[shaders.size() - 1]->draw(originalWidth, originalHeigth);
		queryNames.push_back(shaders[shaders.size() - 1]->getName());
	} else {
		shader.draw(originalWidth, originalHeigth);
		queryNames.push_back(shader.getName());
	}
	glEndQuery(GL_TIME_ELAPSED);
	glDisable(GL_FRAMEBUFFER_SRGB);

	jassert(queryIndex == (int) queries.size());
	jassert(queryNames.size() == queries.size());

	const bool TIMINGS_ENABLED = false;
	if(TIMINGS_ENABLED) {
		for(int i = 0; i < queries.size(); i++) {
			GLuint64 time;
			glGetQueryObjectui64v(queries[i], GL_QUERY_RESULT, &time);
			std::cout << queryNames[i] << ": " << time/1000 << " µs" << std::endl;
		}
		std::cout << std::endl;
	}
	GLuint64 time;
	glGetQueryObjectui64v(queries[0], GL_QUERY_RESULT, &time);
	return uint64_t(time);
}

PostprocShader& PostprocPipeline::getShader(int n) const {
	return *shaders[n];
}

int PostprocPipeline::getNumShaders() const {
	return shaders.size();
}
