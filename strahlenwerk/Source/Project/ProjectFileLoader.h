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
		File getMappingFile();
		File getTimelineDataFile();
		File getAudioFile();

		const File& getSceneDir() const;
		const File& getPostprocDir() const;
		const File& getIncludeDir() const;

		static std::string loadFile(const std::string& path);

	private:
		static std::vector<File> listFiles(const File& dir);

		File projectDir;
		File postprocDir;
		File sceneDir;
		File includeDir;
};
#endif
