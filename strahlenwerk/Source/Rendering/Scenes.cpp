#include "Scenes.h"

#include "SceneShader.h"

Scenes::Scenes(std::vector<std::pair<std::string, std::unique_ptr<SceneShader>>> shaders) :
	shaders(std::move(shaders))
{
}

Scenes::~Scenes() = default;

SceneShader& Scenes::getShader(int n) const {
	return *shaders[n].second;
}
