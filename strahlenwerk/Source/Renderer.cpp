#include <fstream>
#include <sstream>
#include <regex>

#include "Renderer.h"

#include "Rendering/PostprocShader.h"
#include "StrahlenwerkApplication.h"
#include "Rendering/PostprocPipeline.h"
#include "Rendering/PostprocPipelineLoader.h"
#include "Project/Project.h"
#include "Rendering/DefaultShader.h"
#include "Rendering/Scenes.h"
#include <MainWindow.h>
#include <PropertyNames.h>

Renderer::Renderer(OpenGLContext& context) :
	context(context),
	defaultShader(context, "default"),
	defaultPostproc(new PostprocPipeline),
	width(0),
	height(0)
{
	MainWindow::getApplicationCommandManager().addListener(this);

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
#ifdef _WINDOWS
	initializeOpenGLFunctions();
#endif
}

void Renderer::openGLContextClosing() {
	scenesDeletionQueue.push_back(std::move(scenes));
	postprocDeletionQueue.push_back(std::move(defaultPostproc));
	postprocDeletionQueue.push_back(std::move(postproc));
	scenesDeletionQueue.clear();
	postprocDeletionQueue.clear();
}

void Renderer::renderOpenGL() {
	renderMutex.lock();
	scenesDeletionQueue.clear();
	postprocDeletionQueue.clear();

	if(scenes == nullptr) {
		lastFrameDuration = postproc->render(defaultShader, width, height);
	} else {
		auto& data = TimelineData::getTimelineData();
		const String shaderName = data.getSceneShaderSource(data.getCurrentScene());
		const int shaderId = scenes->getObjectId(shaderName.toStdString());
		if(shaderId == -1) {
			lastFrameDuration = defaultPostproc->render(defaultShader, width, height);
		} else {
			lastFrameDuration = postproc->render(scenes->getObject(shaderId), width, height);
		}
	}
	renderMutex.unlock();
}

void Renderer::postprocChanged() {
	reloadPostproc();
}

void Renderer::scenesChanged() {
	reloadScenes();
}

void Renderer::ambientLightsChanged() {
	reloadAmbientLights();
}

void Renderer::setSize(int _width, int _height) {
	renderMutex.lock();
	width = _width;
	height = _height;
	renderMutex.unlock();
}

uint64_t Renderer::getLastFrameDuration() {
	std::lock_guard<std::mutex> lock(renderMutex);
	return lastFrameDuration;
}

void Renderer::performMakeDemo() {
	std::lock_guard<std::mutex> lock(renderMutex);
	StrahlenwerkApplication::getInstance()->getProject().makeDemo(*scenes, *postproc);
}

void Renderer::performToggleHalfResolution() {
	auto& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool previous = properties.getBoolValue(PropertyNames::HalfResolutionEnabled);
	properties.setValue(PropertyNames::HalfResolutionEnabled, !previous);
	context.triggerRepaint();
}


void Renderer::reloadPostproc() {
	auto newPostproc = StrahlenwerkApplication::getInstance()->getProject().getPostproc();
	{
		std::lock_guard<std::mutex> lock(renderMutex);
		if(newPostproc != nullptr) {
			if(defaultPostproc == nullptr) {
				defaultPostproc = std::move(postproc);
			} else {
				postprocDeletionQueue.push_back(std::move(postproc));
			}
			postproc = std::move(newPostproc);
		} else {
			if(defaultPostproc != nullptr) {
				postproc = std::move(defaultPostproc);
			}
		}
	}
	context.triggerRepaint();
}

void Renderer::reloadScenes() {
	auto newScenes = StrahlenwerkApplication::getInstance()->getProject().getScenes();
	{
		std::lock_guard<std::mutex> lock(renderMutex);
		scenesDeletionQueue.push_back(std::move(scenes));
		scenes = std::move(newScenes);
	}
	context.triggerRepaint();
}

void Renderer::reloadAmbientLights() {
	{
		std::lock_guard<std::mutex> lock(renderMutex);
	}
}

void Renderer::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch(info.commandID) {
		case Renderer::makeDemo:
			performMakeDemo();
			break;
		case Renderer::toggleHalfResolution:
			performToggleHalfResolution();
		default:
			break;
	}
}

void Renderer::applicationCommandListChanged() {
}
