#include "Project.h"

#include <regex>
#include <fstream>

#include "ProjectFileLoader.h"
#include "Rendering/PostprocPipelineLoader.h"
#include "Rendering/PostprocShader.h"
#include "Rendering/PostprocPipeline.h"
#include "Rendering/SceneShader.h"

Project::~Project() = default;

void Project::registerPostprocListener(ChangeListener* listener) {
	postprocChangeBroadcaster.addChangeListener(listener);
}

void Project::unregisterPostprocListener(ChangeListener* listener) {
	postprocChangeBroadcaster.removeChangeListener(listener);
}

void Project::triggerPostprocReload() {
	auto shaders = loadPostprocShaders();
	if(!shaders.empty()) {
		postproc = std::unique_ptr<PostprocPipeline>(new PostprocPipeline());
		postproc->setShaders(std::move(shaders));
	} else {
		std::cerr << "No shaders loaded" << std::endl;
	}
	postprocChangeBroadcaster.sendChangeMessage();
}

void Project::contextChanged(OpenGLContext& _context) {
	context = &_context;
	triggerPostprocReload();
}

std::unique_ptr<PostprocPipeline> Project::getPostproc() {
	return std::move(postproc);
}

std::vector<std::unique_ptr<PostprocShader>> Project::loadPostprocShaders() {
	ProjectFileLoader projectLoader("./test_project"); // TODO

	std::string mappingSource = loadFile(projectLoader.getMappingFilePath());
	auto shaderSources = listShaderSources(projectLoader.listPostprocFiles());
	PostprocPipelineLoader loader;
	return loader.load(*context, mappingSource, shaderSources);
}

std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> Project::loadSceneShaders() {
	ProjectFileLoader projectLoader("./test_project"); // TODO

	auto shaderSources = listShaderSources(projectLoader.listSceneFiles());
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

std::string Project::loadFile(const std::string& path) {
	std::ifstream in(path);
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	return contents.str();
}
