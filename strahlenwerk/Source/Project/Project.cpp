#include "Project.h"

#include <regex>

#include "Rendering/PostprocPipelineLoader.h"
#include "Rendering/PostprocShader.h"
#include "Rendering/PostprocPipeline.h"
#include "Rendering/SceneShader.h"
#include "StrahlenwerkApplication.h"
#include "PropertyNames.h"
#include "ProjectListener.h"
#include "Rendering/Scenes.h"
#include "Rendering/Shader.h"
#include "Rendering/Uniform.h"
#include "Rendering/UniformManager.h"

Project::Project(const std::string& dir) :
	loader(dir),
	fileListener(*this)
{
	watchFiles(dir);
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
	for(auto& shader : shaders) {
		addUniforms(*shader);
	}
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
	for(auto& p : shaders) {
		addUniforms(*p.second);
	}
	if(!shaders.empty()) {
		scenes = std::unique_ptr<Scenes>(new Scenes(std::move(shaders)));
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

std::unique_ptr<Scenes> Project::getScenes() {
	return std::move(scenes);
}

TimelineData& Project::getTimelineData() {
	return timelineData;
}

void Project::loadDirectory(const std::string& dir) {
	loader = ProjectFileLoader(dir);
	watchFiles(dir);
	StrahlenwerkApplication::getInstance()->getProperties().setValue(PropertyNames::PROJECT_DIR, var(dir));
	reload();
}

const ProjectFileLoader& Project::getLoader() const {
	return loader;
}

void Project::handleFileAction(
		efsw::WatchID watchid,
		const std::string& dir,
		const std::string& filename,
		efsw::Action action,
		std::string oldFilename)
{
	const auto changedFile = File(dir + filename);
	if(   (changedFile.getParentDirectory() == loader.getPostprocDir() && changedFile.hasFileExtension("glsl"))
	   || (File(dir + filename) == loader.getMappingFile())
	) {
		reloadPostproc();
	} else if(changedFile.getParentDirectory() == loader.getSceneDir() && changedFile.hasFileExtension("glsl")) {
		// TODO find a way for reloading only changed scenes
		reloadScenes();
	} else if(changedFile.getParentDirectory() == loader.getIncludeDir() && changedFile.hasFileExtension("glsl")) {
		reload();
	}
}

std::vector<std::unique_ptr<PostprocShader>> Project::loadPostprocShaders() {
	std::string mappingSource = loader.loadFile(loader.getMappingFile().getFullPathName().toStdString());
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
			loader.loadFile(file.getFullPathName().toStdString())
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

void Project::watchFiles(const std::string& dir) {
	fileWatcher = std::unique_ptr<efsw::FileWatcher>(new efsw::FileWatcher);
	fileWatcher->addWatch(dir, &fileListener, true);
	fileWatcher->watch();
}

void Project::addUniforms(const Shader& shader) {
	auto uniformsArray = timelineData.getUniformsArray();
	const auto& uniformManager = UniformManager::Instance();
	for(const int uniformId : shader.getUniformIds()) {
		const Uniform* uniform = uniformManager.getUniform(uniformId);
		// TODO handle same uniform with different type
		if(!timelineData.getUniform(var(uniform->name)).isValid()) {
			var type;
			// TODO
			switch(uniform->type) {
				case UniformType::FLOAT:
					type = var("float");
					break;
				case UniformType::VEC2:
					type = var("vec2");
					break;
				case UniformType::VEC3:
					type = var("vec3");
					break;
				case UniformType::COLOR:
					type = var("color");
					break;
				case UniformType::VEC4:
					type = var("vec4");
					break;
			}
			timelineData.addUniform(var(uniform->name), type);
		}
	}
}

