#ifndef SCENES_H
#define SCENES_H

#include <vector>
#include <string>
#include <string>
#include <memory>

class SceneShader;

class Scenes {
	public:
		Scenes(std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> shaders);
		~Scenes();

		SceneShader& getShader(int n) const;

	private:
		std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> shaders;
};

#endif
