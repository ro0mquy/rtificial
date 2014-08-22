#include <fstream>
#include <sstream>
#include <regex>

#include "Renderer.h"

#include "Rendering/PostprocShader.h"
#include "StrahlenwerkApplication.h"
#include "Rendering/PostprocPipeline.h"
#include "Rendering/PostprocPipelineLoader.h"
#include "Project.h"
#include "Rendering/DefaultShader.h"

Renderer::Renderer(OpenGLContext& context) :
	context(context),
	defaultShader(context),
	defaultPostproc(new PostprocPipeline),
	width(0),
	height(0)
{
	defaultShader.load(defaultShaderSource);
	PostprocPipelineLoader loader;
	defaultPostproc->setShaders(loader.load(context, "input.color output.color\n", {}));
	postprocMutex.lock();
	postproc = std::move(defaultPostproc);
	postprocMutex.unlock();

	auto& project = StrahlenwerkApplication::getInstance()->getProject();
	project.registerListener(this);
	project.contextChanged(context);
}

Renderer::~Renderer() {
	StrahlenwerkApplication::getInstance()->getProject().unregisterListener(this);
}

void Renderer::newOpenGLContextCreated() {
}

void Renderer::openGLContextClosing() {
}

void Renderer::renderOpenGL() {
	postprocMutex.lock();
	postproc->render(defaultShader, width, height);
	postprocMutex.unlock();
}

void Renderer::postprocChanged() {
	reloadPostproc();
}

void Renderer::scenesChanged() {
}

void Renderer::setSize(int _width, int _height) {
	postprocMutex.lock();
	width = _width;
	height = _height;
	postprocMutex.unlock();
}

void Renderer::reloadPostproc() {
	auto newPostproc = StrahlenwerkApplication::getInstance()->getProject().getPostproc();
	postprocMutex.lock();
	if(newPostproc != nullptr) {
		if(defaultPostproc == nullptr) {
			defaultPostproc = std::move(postproc);
		}
		postproc = std::move(newPostproc);
	} else {
		if(defaultPostproc != nullptr) {
			postproc = std::move(defaultPostproc);
		}
	}
	postprocMutex.unlock();
	context.triggerRepaint();
}
