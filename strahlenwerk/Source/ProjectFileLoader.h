#ifndef PROJECTFILELOADER_H
#define PROJECTFILELOADER_H

#include <string>
#include <vector>
#include <juce>

class ProjectFileLoader {
	public:
		ProjectFileLoader(std::string projectRoot);

		std::vector<std::string> listPostprocFiles();
		std::vector<std::string> listSceneFiles();
		std::string getMappingFilePath();

	private:
		static std::vector<std::string> listFiles(const File& dir);

		const File projectDir;
		const File postprocDir;
		const File sceneDir;
};
#endif
