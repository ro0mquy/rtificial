#include "Project.h"

#include <regex>
#include <fstream>

#include "ProjectFileLoader.h"
#include "Rendering/PostprocPipelineLoader.h"
#include "Rendering/PostprocShader.h"
#include "Rendering/PostprocPipeline.h"

Project::~Project() = default;

static std::string loadFile(const std::string& path) {
	std::ifstream in(path);
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	return contents.str();
}

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
	std::vector<std::pair<std::string, std::string>> shaderSources;
	std::regex nameRegex("^.*/(\\w+)\\.glsl$"); // TODO
	std::smatch match;
	for(const auto& filename : projectLoader.listPostprocFiles()) {
		std::regex_match(filename, match, nameRegex);
		shaderSources.emplace_back(match[1], loadFile(filename));
	}
	PostprocPipelineLoader loader;
	return loader.load(*context, mappingSource, shaderSources);
}
