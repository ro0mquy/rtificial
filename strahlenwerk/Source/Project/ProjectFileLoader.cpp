#include "ProjectFileLoader.h"

#include <sstream>
#include <fstream>

// TODO error handling

ProjectFileLoader::ProjectFileLoader(std::string projectRoot) :
	projectDir(projectRoot),
	postprocDir(projectDir.getChildFile("postproc")),
	sceneDir(projectDir.getChildFile("scenes")),
	includeDir(projectDir.getChildFile("include")),
	buildDir(projectDir.getChildFile("build")),
	environmentsDir(projectDir.getChildFile("environments")),
	texturesDir(projectDir.getChildFile("textures"))
{
}

std::vector<File> ProjectFileLoader::listPostprocFiles() const {
	return listFiles(postprocDir);
}

std::vector<File> ProjectFileLoader::listSceneFiles() const {
	return listFiles(sceneDir);
}

std::vector<File> ProjectFileLoader::listEnvironmentFiles() const {
	return listFiles(environmentsDir);
}

std::vector<File> ProjectFileLoader::listTextureFiles() const {
	Array<File> results;
	texturesDir.findChildFiles(results, texturesDir.findFiles, false);
	std::vector<File> files;
	for(const auto& file : results) {
		files.push_back(file);
	}
	return files;
}

File ProjectFileLoader::getMappingFile() const {
	return projectDir.getChildFile("mapping.txt");
}

File ProjectFileLoader::getTimelineDataFile() const {
	return projectDir.getChildFile("timeline.json");
}

File ProjectFileLoader::getAudioFile() const {
	return projectDir.getChildFile("music.ogg");
}

File ProjectFileLoader::getEnvelopeFile() const {
	return projectDir.getChildFile("envelopes.out");
}

File ProjectFileLoader::getBpmFile() const {
	return projectDir.getChildFile("bpm.bpm");
}

File ProjectFileLoader::getBakeFile() const {
	return projectDir.getChildFile("uniforms.baked");
}

const File& ProjectFileLoader::getProjectDir() const {
	return projectDir;
}

const File& ProjectFileLoader::getSceneDir() const {
	return sceneDir;
}

const File& ProjectFileLoader::getPostprocDir() const {
	return postprocDir;
}

const File& ProjectFileLoader::getIncludeDir() const {
	return includeDir;
}

const File& ProjectFileLoader::getBuildDir() const {
	return buildDir;
}

const File& ProjectFileLoader::getEnvironmentsDir() const {
	return environmentsDir;
}

const File& ProjectFileLoader::getTexturesDir() const {
	return texturesDir;
}

std::string ProjectFileLoader::loadFile(const std::string& path) {
	std::ifstream in(path);
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	return contents.str();
}

std::vector<File> ProjectFileLoader::listFiles(const File& dir) {
	Array<File> results;
	dir.findChildFiles(results, dir.findFiles, false);
	std::vector<File> files;
	for(const auto& file : results) {
		if(file.hasFileExtension("glsl")) {
			files.push_back(file);
		}
	}
	std::sort(files.begin(), files.end(),
	[](const File& a, const File& b) -> bool {
		return a.getFileName() < b.getFileName();
	});
	return files;
}
