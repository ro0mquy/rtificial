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
	const std::regex edgeRegex(R"regex((\w+)\.(\w+)[ \t]+(\w+)\.(\w+)\n)regex");
	const std::sregex_iterator end;
	for(std::sregex_iterator it(mappingSource.begin(), mappingSource.end(), edgeRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& outputShaderName = match[1];
		const auto& inputShaderName  = match[3];
		const auto& outputName = match[2];
		const auto& inputName  = match[4];

		const auto outputId = shaderIds.find(outputShaderName);
		if(outputId == shaderIds.end()) {
			std::cerr << "Shader " << outputShaderName << " not found." << std::endl;
			continue;
		}
		const auto inputId = shaderIds.find(inputShaderName);
		if(inputId == shaderIds.end()) {
			std::cerr << "Shader " << inputShaderName << " not found." << std::endl;
			continue;
		}

		const auto& outputShader = shaders[outputId->second];
		const auto& outputs = outputShader->getOutputs();
		const auto& output = outputs.find(outputName);
		if(output == outputs.end()) {
			std::cerr << "Shader " << outputShaderName << " has no output named " << outputName << std::endl;
			continue;
		}
		const auto& inputShader = shaders[inputId->second];
		const auto& inputs = inputShader->getInputs();
		const auto& input = inputs.find(inputName);
		if(output == outputs.end()) {
			std::cerr << "Shader " << inputShaderName << " has no input named " << inputName << std::endl;
			continue;
		}

		if(input->second != output->second) {
			std::cerr << outputShaderName << "." << outputName << " and "
				<< inputShaderName << "." << inputName
				<< " don't have a matching number of components!" << std::endl;
			continue;
		}

		edges.emplace_back(outputId->second, inputId->second);
	}
	return edges;
}

void PostprocPipeline::createOrder(const std::vector<std::pair<int, int>>& mapping) {
}
