#include "PostprocShader.h"

#include <regex>
#include <vector>
#include <utility>

const std::unordered_map<std::string, int>& PostprocShader::getInputs() const {
	return inputs;
}

const std::unordered_map<std::string, int>& PostprocShader::getOutputs() const {
	return outputs;
}

void PostprocShader::onBeforeLoad() {
	inputs.clear();
	outputs.clear();
}

void PostprocShader::onSourceProcessed(std::string& source) {
	const std::sregex_iterator end;

	const std::regex inputRegex(R"regex(uniform[ \t]+sampler2D[ \t]+(\w+)[ \t]*;[ \t]*//[ \t]*(float|vec[234]))regex");
	for(std::sregex_iterator it(source.begin(), source.end(), inputRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& name = match[1];
		const int components = toComponents(match[2]);
		inputs.emplace(name, components);
	}

	const std::regex outputRegex(R"regex(out[ \t]+(float|vec[234])[ \t]+(\w+)[ \t]*;)regex");
	for(std::sregex_iterator it(source.begin(), source.end(), outputRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& name = match[2];
		const int components = toComponents(match[1]);
		outputs.emplace(name, components);
	}
}

int PostprocShader::toComponents(const std::string& identifier) {
	if(identifier == "float") {
		return 1;
	} else if(identifier == "vec2") {
		return 2;
	} else if(identifier == "vec3") {
		return 3;
	} else if(identifier == "vec4") {
		return 4;
	} else {
		return 0;
	}
}
