#ifndef PROJECTFILELOADER_H
#define PROJECTFILELOADER_H

#include <string>
#include <vector>
#include <juce>

class ProjectFileLoader {
	public:
		ProjectFileLoader(std::string projectRoot);

		std::vector<File> listPostprocFiles() const;
		std::vector<File> listSceneFiles() const;
		File getMappingFile() const;
		File getTimelineDataFile() const;
		File getAudioFile() const;
		File getEnvelopeFile() const;
		File getBakeFile() const;

		const File& getSceneDir() const;
		const File& getPostprocDir() const;
		const File& getIncludeDir() const;
		const File& getBuildDir() const;
		const File& getEnvironmentsDir() const;

		static std::string loadFile(const std::string& path);

	private:
		static std::vector<File> listFiles(const File& dir);

		File projectDir;
		File postprocDir;
		File sceneDir;
		File includeDir;
		File buildDir;
		File environmentsDir;
};
#endif
