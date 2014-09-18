#include "ProjectFileLoader.h"

#include <sstream>
#include <fstream>

// TODO error handling

ProjectFileLoader::ProjectFileLoader(std::string projectRoot) :
	projectDir(projectRoot),
	postprocDir(projectDir.getChildFile("postproc")),
	sceneDir(projectDir.getChildFile("scenes")),
	includeDir(projectDir.getChildFile("include"))
{
}

std::vector<File> ProjectFileLoader::listPostprocFiles() {
	return listFiles(postprocDir);
}

std::vector<File> ProjectFileLoader::listSceneFiles() {
	return listFiles(sceneDir);
}

File ProjectFileLoader::getMappingFile() {
	return projectDir.getChildFile("mapping.txt");
}

File ProjectFileLoader::getTimelineDataFile() {
	return projectDir.getChildFile("timeline.xml");
}

File ProjectFileLoader::getAudioFile() {
	return projectDir.getChildFile("music.flac");
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
