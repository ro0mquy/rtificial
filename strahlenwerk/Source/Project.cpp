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
		postproc = std::make_shared<PostprocPipeline>();
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

std::shared_ptr<PostprocPipeline> Project::getPostproc() {
	return postproc;
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

std::vector<std::pair<std::string, std::string>> Project::listShaderSources(std::vector<std::string> files) {
	std::vector<std::pair<std::string, std::string>> shaderSources;
	std::regex nameRegex("^.*/(\\w+)\\.glsl$"); // TODO
	std::smatch match;
	for(const auto& filename : files) {
		std::regex_match(filename, match, nameRegex);
		shaderSources.emplace_back(match[1], loadFile(filename));
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
