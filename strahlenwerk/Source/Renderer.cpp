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
#include "MainWindow.h"
#include "PropertyNames.h"
#include "Rendering/AmbientLight.h"

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
	project.addListener(this);
	project.contextChanged(context);
}

Renderer::~Renderer() {
	StrahlenwerkApplication::getInstance()->getProject().removeListener(this);
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
	ambientLightsDeletionQueue.clear();
}

void Renderer::renderOpenGL() {
	renderMutex.lock();
	scenesDeletionQueue.clear();
	postprocDeletionQueue.clear();
	ambientLightsDeletionQueue.clear();
	for (Texture& textureToDestroy : texturesDeletionQueue) {
		textureToDestroy.destroy();
	}
	texturesDeletionQueue.clear();

	for (Texture& texture : textures) {
		texture.bind();
	}

	if(scenes == nullptr) {
		lastFrameDuration = postproc->render(defaultShader, width, height, shouldGetImage);
	} else {
		TimelineData& data = TimelineData::getTimelineData();
		const ValueTree currentScene = data.getCurrentScene();
		const String shaderSourceName = data.getSceneShaderSource(currentScene);
		const String environmentSourceName = data.getSceneEnvironmentSource(currentScene);

		if (ambientLights != nullptr) {
			const int ambientId = ambientLights->getObjectId(environmentSourceName.toStdString());
			if (ambientId != -1) {
				ambientLights->getObject(ambientId).bind();
			}
		}

		const int shaderId = scenes->getObjectId(shaderSourceName.toStdString());
		if(shaderId == -1) {
			lastFrameDuration = defaultPostproc->render(defaultShader, width, height, shouldGetImage);
		} else {
			lastFrameDuration = postproc->render(scenes->getObject(shaderId), width, height, shouldGetImage);
		}
	}

	if (shouldGetImage) {
		shouldGetImage = false;
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

void Renderer::texturesChanged() {
	std::vector<Texture> newTextures = StrahlenwerkApplication::getInstance()->getProject().getTextures();
	{
		std::lock_guard<std::mutex> lock(renderMutex);
		texturesDeletionQueue.insert(texturesDeletionQueue.end(), textures.begin(), textures.end());
		textures = newTextures;
	}
	context.triggerRepaint();
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
	StrahlenwerkApplication::getInstance()->getProject().makeDemo(*scenes, *postproc, *ambientLights, textures);
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
	auto newAmbientLights = StrahlenwerkApplication::getInstance()->getProject().getAmbientLights();
	{
		std::lock_guard<std::mutex> lock(renderMutex);
		ambientLightsDeletionQueue.push_back(std::move(ambientLights));
		ambientLights = std::move(newAmbientLights);
	}
	context.triggerRepaint();
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

PostprocPipeline& Renderer::getPostproc() {
	return *postproc;
}

void Renderer::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	postproc->removeChangeListener(this);
	sendChangeMessage();
}

void Renderer::requestRenderedImage() {
	postproc->addChangeListener(this);
	shouldGetImage = true;
	context.triggerRepaint();
}

const RtImage& Renderer::getRenderedImage() {
	return postproc->getRenderedImage();
}
