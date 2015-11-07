#include "PostprocPipeline.h"

#include "SceneShader.h"
#include "PostprocShader.h"
#include <StrahlenwerkApplication.h>
#include <PropertyNames.h>
#include <MainWindow.h>
#include <OpenGLComponent.h>
#include <MainContentComponent.h>

PostprocPipeline::~PostprocPipeline() = default;

void PostprocPipeline::handleAsyncUpdate() {
	std::lock_guard<std::mutex> lock(pixelDataMutex);

	jassert(pixelDataChannels >= 3);
	renderedImage = Image(Image::PixelFormat::RGB, pixelDataWidth, pixelDataHeight, true);
	//Image::BitmapData imgBitmapData(rendered, Image::BitmapData::writeOnly);
	for (int y = 0; y < pixelDataHeight; y++) {
		for (int x = 0; x < pixelDataWidth; x++) {
			const Colour color = Colour::fromFloatRGBA(
				pixelData[(y * pixelDataWidth + x) * pixelDataChannels + 0],
				pixelData[(y * pixelDataWidth + x) * pixelDataChannels + 1],
				pixelData[(y * pixelDataWidth + x) * pixelDataChannels + 2],
				1.0f
			);
			// mirror horizontally
			//imgBitmapData.setPixelColour(x, pixelDataHeight -y, color);
			renderedImage.setPixelAt(x, pixelDataHeight - y, color);
		}
	}

	sendChangeMessage();
}

void PostprocPipeline::setShaders(std::vector<std::unique_ptr<PostprocShader>> _shaders) {
	shaders = std::move(_shaders);
}

uint64_t PostprocPipeline::render(SceneShader& scene, int width, int height, bool shouldGetImage) {
	// TODO evaluate "double buffering" of query objects
	if(shaders.empty()) {
		return 0;
	}

	jassert(shaders.size() != 1);

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

	shaders[0]->bindFBO(width, height);
	queryNames.push_back(scene.getName());
	glBeginQuery(GL_TIME_ELAPSED, queries[queryIndex++]);
	scene.draw(shaders[0]->getCreatedWidth(), shaders[0]->getCreatedHeight());
	glEndQuery(GL_TIME_ELAPSED);
	shaders[0]->unbindFBO();

	for (size_t i = 1; i < shaders.size() - 1; i++) {
		// perform postproc at full resolution
		// may be completely broken (sorry!)
		// TODO check if this is sane
		shaders[i]->bindFBO(1280, 720);
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
	// this works becaue the last drawn shader has only one output (with location = 0)
	shaders[shaders.size() - 1]->draw(originalWidth, originalHeigth);

	if (shouldGetImage) {
		readPixels(originalWidth, originalHeigth);
	}

	queryNames.push_back(shaders[shaders.size() - 1]->getName());
	glEndQuery(GL_TIME_ELAPSED);
	glDisable(GL_FRAMEBUFFER_SRGB);

	jassert(queryIndex == (int) queries.size());
	jassert(queryNames.size() == queries.size());

	const bool TIMINGS_ENABLED = false;
	if(TIMINGS_ENABLED) {
		for(size_t i = 0; i < queries.size(); i++) {
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

void PostprocPipeline::readPixels(int width, int height) {
	std::lock_guard<std::mutex> lock(pixelDataMutex);
	pixelDataWidth = width;
	pixelDataHeight = height;
	pixelData = new float[pixelDataWidth * pixelDataHeight * pixelDataChannels];
	glReadPixels(0, 0, pixelDataWidth, pixelDataHeight, GL_RGB, GL_FLOAT, pixelData);
	triggerAsyncUpdate();
}

const Image& PostprocPipeline::getRenderedImage(){
	return renderedImage;
}

