#ifndef PROJECTFILELOADER_H
#define PROJECTFILELOADER_H

#include <string>
#include <vector>
#include <juce>

class ProjectFileLoader {
	public:
		ProjectFileLoader(std::string projectRoot);

		std::vector<File> listPostprocFiles();
		std::vector<File> listSceneFiles();
		std::string getMappingFilePath();

	private:
		static std::vector<File> listFiles(const File& dir);

		File projectDir;
		File postprocDir;
		File sceneDir;
};
#endif
