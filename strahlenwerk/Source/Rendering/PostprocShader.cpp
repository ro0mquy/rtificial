#include "PostprocShader.h"

#include <vector>
#include <utility>

PostprocShader::~PostprocShader() {
	deleteFBO();
}

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

void PostprocShader::insertBindings() {
	modifySource([this] (std::string& source) {
		std::vector<int> positions;
		const std::sregex_iterator end;
		for(std::sregex_iterator it(source.begin(), source.end(), inputRegex); it != end; ++it) {
			positions.push_back(it->position());
		}

		size_t offset = 0;
		for(int i = 0; i < inputs.size(); i++) {
			const auto bindingString = "layout(binding = " + std::to_string(inputs[i].bindingId) + ") ";
			source.insert(positions[i] + offset, bindingString);
			offset += bindingString.size();
		}
	});
}

void PostprocShader::bindFBO(int width, int height) {
	if(!fbo_created || (width != createdWidth) || (height != createdHeight)) {
		createFBO(width, height);
	}
	context.extensions.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
}

void PostprocShader::unbindFBO() {
	for(int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + outputs[i].bindingId);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
}

void PostprocShader::setDefaultFBO() {
	dont_create = true;
}

void PostprocShader::onBeforeLoad() {
	inputs.clear();
	outputs.clear();
}

void PostprocShader::onSourceProcessed(std::string& source) {
	const std::sregex_iterator end;

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

void PostprocShader::onBeforeDraw() {
	for(const auto& output : outputs) {
		glActiveTexture(GL_TEXTURE0 + output.bindingId);
		glBindTexture(GL_TEXTURE_2D, 0);
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

void PostprocShader::createFBO(int width, int height) {
	if(dont_create) return;
	deleteFBO();
	createdWidth = width;
	createdHeight = height;
	const auto& ext = context.extensions;
	ext.glGenFramebuffers(1, &fbo);

	const auto& outputs = getOutputs();
	const int n = outputs.size();
	GLenum drawBuffers[n];
	textures.resize(n);
	glGenTextures(n, &textures[0]);

	ext.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	for(int i = 0; i < n; i++) {
		glActiveTexture(GL_TEXTURE0 + outputs[i].bindingId);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		// GL_RGB and GL_FLOAT are actually not needed, these are just sane
		// parameters in case we actually would transfer data
		GLint internalFormat;
		GLenum format;
		switch(outputs[i].components) {
			case 1:
				internalFormat = GL_R16F;
				format = GL_RED;
				break;
			case 2:
				internalFormat = GL_RG16F;
				format = GL_RG;
				break;
			case 3:
				internalFormat = GL_RGB16F;
				format = GL_RGB;
				break;
			case 4:
			default:
				internalFormat = GL_RGBA16F;
				format = GL_RGBA;
				break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		ext.glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	ext.glDrawBuffers(n, drawBuffers);
	fbo_created = true;
}

void PostprocShader::deleteFBO() {
	if(fbo_created && fbo != 0) {
		glDeleteTextures(textures.size(), &textures[0]);
		context.extensions.glDeleteFramebuffers(1, &fbo);
		fbo_created = false;
	}
}
