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
#include "Rendering/Scenes.h"

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
	renderMutex.lock();
	postproc = std::move(defaultPostproc);
	renderMutex.unlock();

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
	renderMutex.lock();
	if(scenes == nullptr) {
		postproc->render(defaultShader, width, height);
	} else {
		postproc->render(scenes->getShader(0), width, height);
	}
	renderMutex.unlock();
}

void Renderer::postprocChanged() {
	reloadPostproc();
}

void Renderer::scenesChanged() {
	reloadScenes();
}

void Renderer::setSize(int _width, int _height) {
	renderMutex.lock();
	width = _width;
	height = _height;
	renderMutex.unlock();
}

void Renderer::reloadPostproc() {
	auto newPostproc = StrahlenwerkApplication::getInstance()->getProject().getPostproc();
	renderMutex.lock();
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
	renderMutex.unlock();
	context.triggerRepaint();
}

void Renderer::reloadScenes() {
	auto newScenes = StrahlenwerkApplication::getInstance()->getProject().getScenes();
	renderMutex.lock();
	scenes = std::move(newScenes);
	renderMutex.unlock();
	context.triggerRepaint();
}
