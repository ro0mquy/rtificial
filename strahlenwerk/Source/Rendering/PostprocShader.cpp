#include "PostprocShader.h"

#include <regex>
#include <vector>
#include <utility>

const std::vector<Input>& PostprocShader::getInputs() const {
	return inputs;
}

const std::vector<Output>& PostprocShader::getOutputs() const {
	return outputs;
}

template<class T>
static const T* findConnector(const std::vector<T>& vec, const std::string& name) {
	const auto result = std::find_if(vec.begin(), vec.end(), [name] (const T& e) { return e.name == name; });
	if(result == vec.end()) {
		return nullptr;
	} else {
		return &(*result);
	}
}

const Input* PostprocShader::findInput(const std::string& name) {
	return findConnector(inputs, name);
}

const Output* PostprocShader::findOutput(const std::string& name) {
	return findConnector(outputs, name);
}

void PostprocShader::setInputBindingId(int index, int id) {
	inputs[index].bindingId = id;
}

void PostprocShader::setOutputBindingId(int index, int id) {
	outputs[index].bindingId = id;
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
		inputs.emplace_back(name, components);
	}

	std::vector<std::pair<size_t, int>> outputPositions;
	const std::regex outputRegex(R"regex(out[ \t]+(float|vec[234])[ \t]+(\w+)[ \t]*;)regex");
	for(std::sregex_iterator it(source.begin(), source.end(), outputRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& name = match[2];
		const int components = toComponents(match[1]);
		outputs.emplace_back(name, components);
		outputPositions.emplace_back(match.position(), outputPositions.size());
	}

	insertLocations(source, outputPositions);
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
