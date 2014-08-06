#include <regex>

#include "PostprocPipeline.h"

void PostprocPipeline::loadShaders(OpenGLContext& context) {
	shaderIds.clear();
	shaderIds.emplace("input", 0);
	shaderIds.emplace("output", 1);

	UniformManager manager; // dummy
	shaders.emplace_back(new PostprocShader(context));
	shaders.back()->load("out vec3 color; void main() {}", manager);
	shaders.emplace_back(new PostprocShader(context));
	shaders.back()->load("uniform sampler2D color; // vec2\n void main() {}", manager);
}

std::vector<std::pair<int, int>> PostprocPipeline::loadMapping(const std::string& mappingSource) {
	std::vector<std::pair<int, int>> edges;
	return edges;
}

void PostprocPipeline::createOrder(const std::vector<std::pair<int, int>>& mapping) {
}
