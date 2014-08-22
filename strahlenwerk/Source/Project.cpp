#include "Project.h"

#include <regex>
#include <fstream>

#include "Rendering/PostprocPipelineLoader.h"
#include "Rendering/PostprocShader.h"
#include "Rendering/PostprocPipeline.h"
#include "Rendering/SceneShader.h"
#include "StrahlenwerkApplication.h"
#include "PropertyNames.h"
#include "ProjectListener.h"

Project::Project(const std::string& dir) : loader(dir)
{
}

Project::~Project() = default;

void Project::registerListener(ProjectListener* listener) {
	listeners.push_back(listener);
}

void Project::unregisterListener(ProjectListener* listener) {
	const auto pos = std::find(listeners.begin(), listeners.end(), listener);
	if(pos != listeners.end()) {
		listeners.erase(pos);
	}
}

void Project::reload() {
	reloadPostproc();
	reloadScenes();
}

void Project::reloadPostproc() {
	auto shaders = loadPostprocShaders();
	if(!shaders.empty()) {
		postproc = std::unique_ptr<PostprocPipeline>(new PostprocPipeline());
		postproc->setShaders(std::move(shaders));
	} else {
		std::cerr << "No shaders loaded" << std::endl;
	}
	postprocChanged();
}

void Project::reloadScenes() {
	auto shaders = loadSceneShaders();
	if(!shaders.empty()) {
	} else {
		std::cerr << "No shaders loaded" << std::endl;
	}
	scenesChanged();
}

void Project::contextChanged(OpenGLContext& _context) {
	context = &_context;
	reload();
}

std::unique_ptr<PostprocPipeline> Project::getPostproc() {
	return std::move(postproc);
}

void Project::loadDirectory(const std::string& dir) {
	loader = ProjectFileLoader(dir);
	StrahlenwerkApplication::getInstance()->getProperties().setValue(PropertyNames::PROJECT_DIR, var(dir));
	reload();
}

std::vector<std::unique_ptr<PostprocShader>> Project::loadPostprocShaders() {
	std::string mappingSource = loadFile(loader.getMappingFilePath());
	auto shaderSources = listShaderSources(loader.listPostprocFiles());
	PostprocPipelineLoader loader;
	return loader.load(*context, mappingSource, shaderSources);
}

std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> Project::loadSceneShaders() {
	auto shaderSources = listShaderSources(loader.listSceneFiles());
	std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> shaders;
	for(auto& shader : shaderSources) {
		shaders.emplace_back(shader.first, std::unique_ptr<SceneShader>(new SceneShader(*context)));
		shaders.back().second->load(shader.second);
	}
	return shaders;
}

std::vector<std::pair<std::string, std::string>> Project::listShaderSources(const std::vector<File>& files) {
	std::vector<std::pair<std::string, std::string>> shaderSources;
	for(const auto& file : files) {
		shaderSources.emplace_back(
			file.getFileNameWithoutExtension().toStdString(),
			loadFile(file.getFullPathName().toStdString())
		);
	}
	return shaderSources;
}

void Project::postprocChanged() {
	for(auto listener : listeners) {
		listener->postprocChanged();
	}
}

void Project::scenesChanged() {
	for(auto listener : listeners) {
		listener->scenesChanged();
	}
}

std::string Project::loadFile(const std::string& path) {
	std::ifstream in(path);
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	return contents.str();
}
