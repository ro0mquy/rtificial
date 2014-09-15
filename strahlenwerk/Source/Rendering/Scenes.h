#ifndef SCENES_H
#define SCENES_H

#include <vector>
#include <string>
#include <string>
#include <memory>
#include <unordered_map>

class SceneShader;

class Scenes {
	public:
		Scenes(std::vector<std::unique_ptr<SceneShader>> shaders);
		~Scenes();

		SceneShader& getShader(int n) const;
		int getShaderId(const std::string& name) const;

	private:
		std::vector<std::unique_ptr<SceneShader>> shaders;
		std::unordered_map<std::string, int> shaderLookup;
};

#endif
