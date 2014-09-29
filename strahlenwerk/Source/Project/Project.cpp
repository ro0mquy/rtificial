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
#include "AudioManager.h"

Project::Project(const std::string& dir, AudioManager& _audioManager) :
	loader(dir),
	timelineData(loader.getTimelineDataFile()),
	fileListener(*this),
	audioManager(_audioManager)
{
	watchFiles(dir);
	reloadAudio();
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

void Project::reloadShaders() {
	reloadPostproc();
	reloadScenes();
}

void Project::saveTimelineData() {
	timelineData.writeTimelineDataToFile(loader.getTimelineDataFile());
}

void Project::contextChanged(OpenGLContext& _context) {
	context = &_context;
	reloadShaders();
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
	timelineData.readTimelineDataFromFile(loader.getTimelineDataFile());
	reloadShaders();
}

const ProjectFileLoader& Project::getLoader() const {
	return loader;
}

void Project::makeDemo(Scenes& scenes, PostprocPipeline& postproc) {
	const File& buildDir = loader.getBuildDir();
	buildDir.deleteRecursively();
	buildDir.createDirectory();

	const int postprocShaders = postproc.getNumShaders();
	const int sceneShaders = scenes.getNumShaders();

	const std::regex search(R"regex(layout\((.*)\))regex");
	const std::string replacement = "//[\nlayout($1)\n//]\n";

	// create shaders.h
	const File& shadersHeader = buildDir.getChildFile("shaders.h");
	std::string shadersHeaderContent = "#include \"Shader.h\"\n";

	std::string postprocArrayDeclaration = "Shader postproc[" + std::to_string(postprocShaders - 1) + "] = {\n";

	// export shaders
	for(int i = 1; i < postprocShaders; i++) {
		const Shader& shader = postproc.getShader(i);
		const File& shaderFile = buildDir.getChildFile(String(shader.getName())).withFileExtension("glsl");
		shaderFile.replaceWithText(std::regex_replace(shader.getSource(), search, replacement));
		shadersHeaderContent += "#include \"shaders/" + shader.getName() + ".h\"\n";
		postprocArrayDeclaration += "\tShader(" + shader.getName() + "_source),\n";
	}

	postprocArrayDeclaration += "};\n";

	std::string scenesArrayDeclaration = "Shader scenes[" + std::to_string(sceneShaders) + "] = {\n";

	for(int i = 0; i < sceneShaders; i++) {
		const Shader& shader = scenes.getShader(i);
		const File& shaderFile = buildDir.getChildFile(String(shader.getName())).withFileExtension("glsl");
		shaderFile.replaceWithText(std::regex_replace(shader.getSource(), search, replacement));
		shadersHeaderContent += "#include \"shaders/" + shader.getName() + ".h\"\n";
		scenesArrayDeclaration += "\tShader(" + shader.getName() + "_source),\n";
	}
	scenesArrayDeclaration += "};\n";

	shadersHeaderContent += postprocArrayDeclaration;
	shadersHeaderContent += scenesArrayDeclaration;

	shadersHeaderContent += "Framebuffer fbos[" + std::to_string(postprocShaders - 1) + "] = {\n";
	// export FBOs
	for(int i = 0; i < postprocShaders - 1; i++) {
		const PostprocShader& shader = postproc.getShader(i);
		const std::string lod = std::to_string(shader.getOutputLod());
		shadersHeaderContent += "\tFramebuffer(" + lod + ", 0, nullptr),\n";
	}
	shadersHeaderContent += "};\n";

	shadersHeader.replaceWithText(shadersHeaderContent);
}

void Project::handleFileAction(
		efsw::WatchID /*watchid*/,
		const std::string& dir,
		const std::string& filename,
		efsw::Action /*action*/,
		std::string /*oldFilename*/)
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
		reloadShaders();
	} else if(changedFile == loader.getAudioFile()) {
		reloadAudio();
	}
}

std::vector<std::unique_ptr<PostprocShader>> Project::loadPostprocShaders() {
	std::string mappingSource = loader.loadFile(loader.getMappingFile().getFullPathName().toStdString());
	auto shaderSources = listShaderSources(loader.listPostprocFiles());
	PostprocPipelineLoader loader;
	return loader.load(*context, mappingSource, shaderSources);
}

std::vector<std::unique_ptr<SceneShader>> Project::loadSceneShaders() {
	auto shaderSources = listShaderSources(loader.listSceneFiles());
	std::vector<std::unique_ptr<SceneShader>> shaders;
	for(auto& shader : shaderSources) {
		shaders.emplace_back(new SceneShader(*context, shader.first));
		shaders.back()->load(shader.second);
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
		if (false == timelineData.getInterpolator().shouldAddUniformToTimlineData(uniform->name)) {
			// special uniform controller handles this uniform
			continue;
		}

		// TODO handle same uniform with different type
		if(timelineData.getUniform(var(uniform->name)).isValid()) {
			// uniform already exists
			continue;
		}

		var type;
		// TODO
		switch(uniform->type) {
			case UniformType::BOOL:
				type = "bool";
				break;
			case UniformType::FLOAT:
				type = "float";
				break;
			case UniformType::VEC2:
				type = "vec2";
				break;
			case UniformType::VEC3:
				type = "vec3";
				break;
			case UniformType::COLOR:
				type = "color";
				break;
			case UniformType::VEC4:
				type = "vec4";
				break;
		}
		timelineData.addUniform(var(uniform->name), type);
	}
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
	for(const auto& shader : shaders) {
		addUniforms(*shader);
	}
	if(!shaders.empty()) {
		scenes = std::unique_ptr<Scenes>(new Scenes(std::move(shaders)));
	} else {
		std::cerr << "No shaders loaded" << std::endl;
	}
	scenesChanged();
}

void Project::reloadAudio() {
	audioManager.loadFile(loader.getAudioFile());
}
