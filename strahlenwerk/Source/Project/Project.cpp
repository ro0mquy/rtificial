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

	// create header
	const File& shadersHeader = buildDir.getChildFile("strahlenwerk_export.h");
	std::string shadersHeaderContent = R"source(#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Keyframe.h"
)source";

	std::string postprocArrayDeclaration = "Shader postproc[" + std::to_string(postprocShaders - 1) + "] = {\n";

	std::string inputsDeclaration;
	// export shaders
	for(int i = 1; i < postprocShaders; i++) {
		const PostprocShader& shader = postproc.getShader(i);
		const File& shaderFile = buildDir.getChildFile(String(shader.getName())).withFileExtension("glsl");
		shaderFile.replaceWithText(std::regex_replace(shader.getSource(), search, replacement));
		shadersHeaderContent += "#include \"shaders/" + shader.getName() + ".h\"\n";

		auto& inputs = shader.getInputs();
		const std::string number = std::to_string(inputs.size());
		inputsDeclaration += "Input " + shader.getName() + "_inputs[" + number + "] = {\n";
		for(Input input : inputs) {
			inputsDeclaration += "\t{" +
				std::to_string(input.bindingId) + ", " +
				std::to_string(input.lod) +
				"},\n";
		}
		inputsDeclaration += "};\n";


		postprocArrayDeclaration += "\tShader(" + shader.getName() + "_source, " + number + ", " + shader.getName() + "_inputs),\n";
	}


	postprocArrayDeclaration += "};\n";

	std::string scenesArrayDeclaration = "Shader scenes[" + std::to_string(sceneShaders) + "] = {\n";

	for(int i = 0; i < sceneShaders; i++) {
		const Shader& shader = scenes.getShader(i);
		const File& shaderFile = buildDir.getChildFile(String(shader.getName())).withFileExtension("glsl");
		shaderFile.replaceWithText(std::regex_replace(shader.getSource(), search, replacement));
		shadersHeaderContent += "#include \"shaders/" + shader.getName() + ".h\"\n";
		scenesArrayDeclaration += "\tShader(" + shader.getName() + "_source, 0, nullptr),\n";
	}
	scenesArrayDeclaration += "};\n";

	shadersHeaderContent += inputsDeclaration;
	shadersHeaderContent += postprocArrayDeclaration;
	shadersHeaderContent += scenesArrayDeclaration;

	std::string fboDeclaration = "Framebuffer fbos[" + std::to_string(postprocShaders - 1) + "] = {\n";
	// export FBOs
	for(int i = 0; i < postprocShaders - 1; i++) {
		const PostprocShader& shader = postproc.getShader(i);
		auto& outputs = shader.getOutputs();
		const std::string number = std::to_string(outputs.size());
		shadersHeaderContent += "Output " + shader.getName() + "_outputs[" + number + "] = {\n";
		for(Output output : outputs) {
			shadersHeaderContent += "\t{" +
				std::to_string(output.components) + ", " +
				std::to_string(output.bindingId) + ", " +
				std::to_string(output.maxLod) +
				"},\n";
		}
		shadersHeaderContent += "};\n";
		const std::string lod = std::to_string(shader.getOutputLod());
		fboDeclaration += "\tFramebuffer(" + lod + ", " + number + ", " + shader.getName()+ "_outputs),\n";
	}
	fboDeclaration += "};\n";

	shadersHeaderContent += fboDeclaration;

	TimelineData& data = TimelineData::getTimelineData();

	std::string scenesArray = "Scene scenes_data[" + std::to_string(data.getNumScenes()) + "] = {\n";

	for(int i = 0; i < data.getNumScenes(); i++) {
		auto scene = data.getScene(i);

		const int start = data.getSceneStart(scene);
		const int duration = data.getSceneDuration(scene);
		const String shaderSource = data.getSceneShaderSource(scene);

		int shaderId = -1;
		for(int i = 0; i < sceneShaders; i++) {
			if(scenes.getShader(i).getName() == shaderSource.toStdString()) {
				shaderId = i;
				break;
			}
		}

		scenesArray += "\t{" + std::to_string(start) + ", " + std::to_string(start + duration) + ", " + std::to_string(shaderId) + "},\n";
	}

	scenesArray += "};\n";
	shadersHeaderContent += scenesArray;

	int keyframeDataEntries = 0;
	int sequences = 0;
	const int nUniforms = data.getNumUniforms();
	for(int i = 0; i < nUniforms; i++) {
		const auto uniform = data.getUniform(i);
		const String type = data.getUniformType(uniform);
		int components = 0;
		if(type == "float") {
			components = 1;
		} else if(type == "vec2") {
			components = 2;
		} else if(type == "vec3") {
			components = 3;
		} else if(type == "vec4") {
			components = 4;
		} else if(type == "color") {
			components = 3;
		} else if(type == "bool") {
			components = 1;
		}

		// count keyframes
		int keyframes = 0;
		sequences += data.getNumSequences(uniform);
		for(int j = 0; j < data.getNumSequences(uniform); j++) {
			keyframes += data.getNumKeyframes(data.getSequence(uniform, j));
		}

		keyframeDataEntries += (keyframes + 1) * components;
	}

	std::string keyframeDataArray = "float keyframe_data["  + std::to_string(keyframeDataEntries) + "] = {\n";
	std::string sequenceDataArray = "Sequence sequence_data[" + std::to_string(sequences) + "] = {\n";
	for(int i = 0; i < nUniforms; i++) {
		const auto uniform = data.getUniform(i);
		const String type = data.getUniformType(uniform);
		auto standardValue = data.getUniformStandardValue(uniform);
		keyframeDataArray += "\t";
		if(type == "float") {
			keyframeDataArray += std::to_string(float(data.getValueFloatX(standardValue))) + ",";
		} else if(type == "vec2") {
			keyframeDataArray += std::to_string(float(data.getValueVec2X(standardValue))) + ", ";
			keyframeDataArray += std::to_string(float(data.getValueVec2Y(standardValue))) + ",";
		} else if(type == "vec3") {
			keyframeDataArray += std::to_string(float(data.getValueVec3X(standardValue))) + ", ";
			keyframeDataArray += std::to_string(float(data.getValueVec3Y(standardValue))) + ", ";
			keyframeDataArray += std::to_string(float(data.getValueVec3Z(standardValue))) + ",";
		} else if(type == "vec4") {
			keyframeDataArray += std::to_string(float(data.getValueVec4X(standardValue))) + ", ";
			keyframeDataArray += std::to_string(float(data.getValueVec4Y(standardValue))) + ", ";
			keyframeDataArray += std::to_string(float(data.getValueVec4Z(standardValue))) + ", ";
			keyframeDataArray += std::to_string(float(data.getValueVec4W(standardValue))) + ",";
		} else if(type == "color") {
			keyframeDataArray += std::to_string(float(data.getValueColorR(standardValue))) + ", ";
			keyframeDataArray += std::to_string(float(data.getValueColorG(standardValue))) + ", ";
			keyframeDataArray += std::to_string(float(data.getValueColorB(standardValue))) + ",";
		} else if(type == "bool") {
			keyframeDataArray += std::to_string(bool(data.getValueBoolState(standardValue)) ? 1. : 0.) + ",";
		}
		keyframeDataArray += "\n";

		for(int j = 0; j < data.getNumSequences(uniform); j++) {
			auto sequence = data.getSequence(uniform, j);
			const int start = float(data.getAbsoluteStartForSequence(sequence));
			const int end = start + float(data.getSequenceDuration(sequence));
			int interpolation = 0;
			const auto interpolationString = data.getSequenceInterpolation(sequence);
			if(interpolationString == "step") {
				interpolation = 0;
			} else if(interpolationString == "linear") {
				interpolation = 1;
			} else if(interpolationString == "ccrSpline") {
				interpolation = 2;
			}
			sequenceDataArray += "\t{" + std::to_string(start) + ", " + std::to_string(end) + ", " +  std::to_string(interpolation) + "},\n";
			for(int k = 0; k < data.getNumKeyframes(sequence); k++) {
				keyframeDataArray += "\t";
				auto keyframe = data.getKeyframe(sequence, k);
				auto keyframeValue = data.getKeyframeValue(keyframe);
				if(type == "float") {
					keyframeDataArray += std::to_string(float(data.getValueFloatX(keyframeValue))) + ",";
				} else if(type == "vec2") {
					keyframeDataArray += std::to_string(float(data.getValueVec2X(keyframeValue))) + ", ";
					keyframeDataArray += std::to_string(float(data.getValueVec2Y(keyframeValue))) + ",";
				} else if(type == "vec3") {
					keyframeDataArray += std::to_string(float(data.getValueVec3X(keyframeValue))) + ", ";
					keyframeDataArray += std::to_string(float(data.getValueVec3Y(keyframeValue))) + ", ";
					keyframeDataArray += std::to_string(float(data.getValueVec3Z(keyframeValue))) + ",";
				} else if(type == "vec4") {
					keyframeDataArray += std::to_string(float(data.getValueVec4X(keyframeValue))) + ", ";
					keyframeDataArray += std::to_string(float(data.getValueVec4Y(keyframeValue))) + ", ";
					keyframeDataArray += std::to_string(float(data.getValueVec4Z(keyframeValue))) + ", ";
					keyframeDataArray += std::to_string(float(data.getValueVec4W(keyframeValue))) + ",";
				} else if(type == "color") {
					keyframeDataArray += std::to_string(float(data.getValueColorR(keyframeValue))) + ", ";
					keyframeDataArray += std::to_string(float(data.getValueColorG(keyframeValue))) + ", ";
					keyframeDataArray += std::to_string(float(data.getValueColorB(keyframeValue))) + ",";
				} else if(type == "bool") {
					keyframeDataArray += std::to_string(bool(data.getValueBoolState(keyframeValue)) ? 1. : 0.) + ",";
				}
				keyframeDataArray += "\n";
			}
		}
	}
	keyframeDataArray += "};\n";
	sequenceDataArray += "};\n";
	shadersHeaderContent += keyframeDataArray;
	shadersHeaderContent += sequenceDataArray;

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
