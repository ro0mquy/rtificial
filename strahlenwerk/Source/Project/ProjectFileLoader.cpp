#include "ProjectFileLoader.h"

// TODO error handling

ProjectFileLoader::ProjectFileLoader(std::string projectRoot) :
	projectDir(projectRoot),
	postprocDir(projectDir.getChildFile("postproc")),
	sceneDir(projectDir.getChildFile("scenes"))
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

const File& ProjectFileLoader::getSceneDir() const {
	return sceneDir;
}

const File& ProjectFileLoader::getPostprocDir() const {
	return postprocDir;
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
	return files;
}
