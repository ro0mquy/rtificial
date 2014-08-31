#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

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

void PostprocShader::setOutputMaxLod(int index, int maxLod) {
	outputs[index].maxLod = maxLod;
}

void PostprocShader::insertBindings() {
	std::vector<int> positions;
	const std::sregex_iterator end;
	for(std::sregex_iterator it(fragmentSource.begin(), fragmentSource.end(), inputRegex); it != end; ++it) {
		positions.push_back(it->position());
	}

	size_t offset = 0;
	for(int i = 0; i < inputs.size(); i++) {
		const auto bindingString = "layout(binding = " + std::to_string(inputs[i].bindingId) + ") ";
		if(fragmentSource[positions[i] + offset] == '\n') {
			// insert after newline
			offset++;
		}
		fragmentSource.insert(positions[i] + offset, bindingString);
		offset += bindingString.size();
	}
}

void PostprocShader::bindFBO(int width, int height) {
	const int factor = std::exp2(outputLod);
	width /= factor;
	height /= factor;
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
		if(outputs[i].maxLod - outputLod > 0) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
}

int PostprocShader::getCreatedWidth() const {
	return createdWidth;
}

int PostprocShader::getCreatedHeight() const {
	return createdHeight;
}

void PostprocShader::onBeforeLoad() {
	inputs.clear();
	outputs.clear();
}

void PostprocShader::onSourceProcessed() {
	const std::sregex_iterator end;

	for(std::sregex_iterator it(fragmentSource.begin(), fragmentSource.end(), inputRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& name = match[2];
		const int components = toComponents(match[3]);
		const std::string levelString = match[5];
		const int level = levelString.empty() ? 0 : std::stoi(levelString);
		inputs.emplace_back(name, components, level);
	}

	std::vector<std::pair<size_t, int>> outputPositions;
	const std::regex outputRegex(R"regex((^|\n)[ \t]*out[ \t]+(float|vec[234])[ \t]+(\w+)[ \t]*;)regex");
	for(std::sregex_iterator it(fragmentSource.begin(), fragmentSource.end(), outputRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& name = match[3];
		const int components = toComponents(match[2]);
		outputs.emplace_back(name, components);
		outputPositions.emplace_back(match.position(), outputPositions.size());
	}

	const std::regex outputLevelRegex(R"regex((^|\n)[ \t]*//[ \t]*level\(([0-9]+)\))regex");
	std::smatch outputLevelMatch;
	if(std::regex_search(fragmentSource, outputLevelMatch, outputLevelRegex)) {
		outputLod = std::stoi(outputLevelMatch[2]);
	} else {
		outputLod = 0;
	}

	insertLocations(outputPositions);
}

void PostprocShader::onBeforeDraw() {
	for(const auto& output : outputs) {
		glActiveTexture(GL_TEXTURE0 + output.bindingId);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	for(const auto& input : inputs) {
		glActiveTexture(GL_TEXTURE0 + input.bindingId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, std::max(0, input.lod - outputLod));
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if(outputs[i].maxLod - outputLod > 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, outputs[i].maxLod - outputLod);
			glGenerateMipmap(GL_TEXTURE_2D); // allocate mipmap memory
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		ext.glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(n, drawBuffers);
	fbo_created = true;
}

void PostprocShader::deleteFBO() {
	if(fbo_created && fbo != 0) {
		glDeleteTextures(textures.size(), &textures[0]);
		context.extensions.glDeleteFramebuffers(1, &fbo);
		fbo_created = false;
	}
}
