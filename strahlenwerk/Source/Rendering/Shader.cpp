#include "Shader.h"

#include <sstream>
#include <regex>
#include <vector>
#include <utility>
#include <algorithm>

#include "UniformManager.h"
#include "UniformType.h"
#include "Uniform.h"
#include <StrahlenwerkApplication.h>
#include <Timeline/TimelineData.h>
#include <Timeline/Interpolator.h>

Shader::Shader(OpenGLContext& context, std::string name) :
	context(context),
	program(context),
	name(name)
{
}

Shader::~Shader() {
}

void Shader::load(std::string source) {
	uniformIds.clear();

	fragmentSource = source;

	applyIncludes();

	const std::regex uniformRegex(R"regex((^|\n)[ \t]*uniform[ \t]+(vec[234]|float|bool)[ \t]+(\w+)[ \t]*;[ \t]*(// (color))?)regex");
	const std::sregex_iterator end;

	std::vector<std::pair<size_t, int>> matches;
	for(std::sregex_iterator it(fragmentSource.begin(), fragmentSource.end(), uniformRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& typeString = match[2];
		const auto& name = match[3];

		UniformType type = UniformType::FLOAT;
		if (typeString == "bool") {
			type = UniformType::BOOL;
		}
		else if(typeString == "float") {
			type = UniformType::FLOAT;
		}
		else if(typeString == "vec2") {
			type = UniformType::VEC2;
		}
		else if(typeString == "vec3") {
			if(match[5] == "color") {
				type = UniformType::COLOR;
			} else {
				type = UniformType::VEC3;
			}
		}
		else if(typeString == "vec4") {
			type = UniformType::VEC4;
		}
		const Uniform* uniform = registerUniform(name, type);
		if(uniform == nullptr) {
			std::cerr << "Uniform with same name but different type exists: " << name << std::endl;
		} else {
			matches.emplace_back(match.position(), uniform->id);
			onUniformLoad(name, *uniform);
			uniformIds.push_back(uniform->id);
		}
	}
	insertLocations(matches);

	onSourceProcessed();

	sourceChanged = true;
}

/**
 * Only call this from the OpenGL thread!
 */
void Shader::draw(int width, int height) {
	onBeforeDraw();

	if(sourceChanged) {
		recompile();
	}
	if(!shaderOk) {
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	program.use();
	context.extensions.glEnableVertexAttribArray(attributeCoord);
	const GLfloat rectangleVertices[] = {
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	context.extensions.glVertexAttribPointer(attributeCoord, 2, GL_FLOAT, GL_FALSE, 0, rectangleVertices);
	context.extensions.glUniform2f(0, width, height);
	loadUniformValues();
	glGetError(); // clear error
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	context.extensions.glDisableVertexAttribArray(attributeCoord);
}

const std::vector<int>& Shader::getUniformIds() const {
	return uniformIds;
}

const std::string& Shader::getName() const {
	return name;
}

const std::string& Shader::getSource() const {
	return fragmentSource;
}

const Uniform* Shader::registerUniform(std::string name, UniformType type) {
	return UniformManager::Instance().registerUniform(name, type);
}

void Shader::insertLocations(const std::vector<std::pair<size_t, int>>& locations) {
	// this could be more efficient
	size_t offset = 0;
	for(const auto& location : locations) {
		const auto locationString = "layout(location = " + std::to_string(location.second) + ") ";
		if(fragmentSource[location.first + offset] == '\n') {
			// insert after newline
			offset++;
		}
		fragmentSource.insert(location.first + offset, locationString);
		offset += locationString.size();
	}
}

/**
 * Only call this from the OpenGL thread!
 */
void Shader::recompile() {
	program.release();
	const bool vertexOk = program.addVertexShader("#version 330\nin vec2 c;\nout vec2 tc;\nvoid main() { tc = c * .5 + .5;\ngl_Position = vec4(c, 0., 1.); }");
	// TODO better logging
	if(!vertexOk) {
		std::cerr << getName() + " Vertex error: " << program.getLastError() << std::endl;
	}
	const bool fragmentOk = program.addFragmentShader(fragmentSource);
	if(!fragmentOk) {
		std::cerr << getName() + " Fragment error: " << program.getLastError() << std::endl;
	}
	if(vertexOk && fragmentOk) {
		program.link();

		auto coord = OpenGLShaderProgram::Attribute(program, "c");
		attributeCoord = coord.attributeID;
		shaderOk = true;
	} else {
		shaderOk = false;
	}

	sourceChanged = false;
}

void Shader::applyIncludes() {
	const std::regex includeRegex(R"regex((^|\n)#include "(\w+.glsl)")regex");
	const std::sregex_iterator end;

	std::vector<std::tuple<std::string, std::string, int, int>> includeMatches;
	for(std::sregex_iterator it(fragmentSource.begin(), fragmentSource.end(), includeRegex); it != end; ++it) {
		const auto& match = *it;
		includeMatches.emplace_back(match[1], match[2], match.length(), match.position());
	}

	int offset = 0;
	const auto& loader = StrahlenwerkApplication::getInstance()->getProject().getLoader();
	for(const auto& match : includeMatches) {
		const auto newline = std::get<0>(match);
		const auto filename = std::get<1>(match);
		const int length = std::get<2>(match);
		const int position = std::get<3>(match);
		const auto includeFile = loader.getIncludeDir().getChildFile(String(filename));
		std::string included;
		if(includeFile.exists()) {
			included = loader.loadFile(includeFile.getFullPathName().toStdString());
		} else {
			// TODO
			std::cerr << getName() << ": Include file " << filename << " not found!" << std::endl;
			continue;
		}
		const std::string replacement = newline + included;
		fragmentSource.replace(position + offset, length, replacement);
		offset += int(replacement.length()) - length;
	}
}

/**
 * Only call this from the OpenGL thread!
 */
void Shader::loadUniformValues() {
	TimelineData& data = TimelineData::getTimelineData();
	Interpolator& interpolator = data.getInterpolator();
	const auto& uniformManager = UniformManager::Instance();
	for (const int uniformId : uniformIds) {
		const Uniform* uniform = uniformManager.getUniform(uniformId);
		const int location = uniform->id;
		ValueTree value = interpolator.getCurrentUniformState(var(uniform->name)).first;
		switch (uniform->type) {
			case UniformType::BOOL:
				{
					const float boolState = data.getValueBoolState(value);
					context.extensions.glUniform1f(location, boolState);
				}
				break;
			case UniformType::FLOAT:
				{
					const float floatX = data.getValueFloatX(value);
					context.extensions.glUniform1f(location, floatX);
				}
				break;
			case UniformType::VEC2:
				{
					const float vec2X = data.getValueVec2X(value);
					const float vec2Y = data.getValueVec2Y(value);
					context.extensions.glUniform2f(location, vec2X, vec2Y);
				}
				break;
			case UniformType::VEC3:
				{
					const float vec3X = data.getValueVec3X(value);
					const float vec3Y = data.getValueVec3Y(value);
					const float vec3Z = data.getValueVec3Z(value);
					context.extensions.glUniform3f(location, vec3X, vec3Y, vec3Z);
				}
				break;
			case UniformType::COLOR:
				{
					const float colorR = data.getValueColorR(value);
					const float colorG = data.getValueColorG(value);
					const float colorB = data.getValueColorB(value);
					context.extensions.glUniform3f(location, colorR, colorG, colorB);
				}
				break;
			case UniformType::VEC4:
				{
					const float vec4X = data.getValueVec4X(value);
					const float vec4Y = data.getValueVec4Y(value);
					const float vec4Z = data.getValueVec4Z(value);
					const float vec4W = data.getValueVec4W(value);
					context.extensions.glUniform4f(location, vec4X, vec4Y, vec4Z, vec4W);
				}
				break;
		}
	}
}

void Shader::onBeforeLoad() {
}

void Shader::onUniformLoad(const std::string& /*name*/, const Uniform& /*uniform*/) {
}

void Shader::onSourceProcessed() {
}

void Shader::onBeforeDraw() {
}
