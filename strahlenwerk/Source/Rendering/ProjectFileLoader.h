#ifndef PROJECTFILELOADER_H
#define PROJECTFILELOADER_H

#include <string>
#include <vector>

class ProjectFileLoader {
	public:
		ProjectFileLoader(std::string projectRoot);

		using File = int; // TODO
		std::vector<File> getPostprocFiles();
		std::vector<File> getSceneFiles();
		File getMappingFile();

};
#endif
