#include "Scenes.h"

#include "SceneShader.h"

Scenes::Scenes(std::vector<std::unique_ptr<SceneShader>> _shaders) :
	shaders(std::move(_shaders))
{
	int i = 0;
	for(const auto& shader : shaders) {
		shaderLookup.emplace(shader->getName(), i);
		i++;
	}
}

Scenes::~Scenes() = default;

SceneShader& Scenes::getShader(int n) const {
	return *shaders[n];
}

int Scenes::getShaderId(const std::string& name) const {
	const auto it = shaderLookup.find(name);
	if(it == shaderLookup.end()) {
		return -1;
	} else {
		return it->second;
	}
}
