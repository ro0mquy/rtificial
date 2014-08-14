#include "ProjectFileLoader.h"

// TODO error handling

ProjectFileLoader::ProjectFileLoader(std::string projectRoot) :
	projectDir(projectRoot),
	postprocDir(projectDir.getChildFile("postproc")),
	sceneDir(projectDir.getChildFile("scenes"))
{
}

std::vector<std::string> ProjectFileLoader::listPostprocFiles() {
	return listFiles(postprocDir);
}

std::vector<std::string> ProjectFileLoader::listSceneFiles() {
	return listFiles(sceneDir);
}

std::string ProjectFileLoader::getMappingFilePath() {
	return projectDir.getChildFile("mapping.txt").getFullPathName().toStdString();
}

std::vector<std::string> ProjectFileLoader::listFiles(const File& dir) {
	Array<File> results;
	dir.findChildFiles(results, dir.findFiles, false);
	std::vector<std::string> paths;
	for(const auto& file : results) {
		paths.emplace_back(file.getFullPathName().toStdString());
	}
	return paths;
}
