#include "Shader.h"

#include <sstream>
#include <regex>
#include <vector>
#include <utility>
#include <algorithm>

#include "UniformManager.h"
#include "UniformType.h"
#include "Uniform.h"
#include "StrahlenwerkApplication.h"

Shader::Shader(OpenGLContext& context) :
	context(context),
	program(context)
{
}

Shader::~Shader() {
}

void Shader::load(std::string source) {
	uniforms.clear();

	fragmentSource = source;

	applyIncludes();

	const std::regex uniformRegex(R"regex((^|\n)[ \t]*uniform[ \t]+(vec[234]|float)[ \t]+(\w+)[ \t]*;)regex");
	const std::sregex_iterator end;

	std::vector<std::pair<size_t, int>> matches;
	for(std::sregex_iterator it(fragmentSource.begin(), fragmentSource.end(), uniformRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& typeString = match[2];
		const auto& name = match[3];

		UniformType type = UniformType::FLOAT;
		if(typeString == "float") {
			type = UniformType::FLOAT;
		}
		else if(typeString == "vec2") {
			type = UniformType::VEC2;
		}
		else if(typeString == "vec3") {
			type = UniformType::VEC3;
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
			uniforms.push_back(uniform);
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
	glGetError(); // clear error
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	context.extensions.glDisableVertexAttribArray(attributeCoord);
}

const std::vector<const Uniform*>& Shader::getUniforms() const {
	return uniforms;
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
		std::cerr << "Vertex error: " << program.getLastError() << std::endl;
	}
	const bool fragmentOk = program.addFragmentShader(fragmentSource);
	if(!fragmentOk) {
		std::cerr << "Fragment error: " << program.getLastError() << std::endl;
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

	std::vector<std::smatch> includeMatches;
	for(std::sregex_iterator it(fragmentSource.begin(), fragmentSource.end(), includeRegex); it != end; ++it) {
		includeMatches.push_back(*it);
	}

	size_t offset = 0;
	const auto& loader = StrahlenwerkApplication::getInstance()->getProject().getLoader();
	for(const auto& match : includeMatches) {
		const auto includeFile = loader.getIncludeDir().getChildFile(String(match[2]));
		std::string included;
		if(includeFile.exists()) {
			included = loader.loadFile(includeFile.getFullPathName().toStdString());
		} else {
			// TODO
			std::cerr << "Include file " << match[2] << " not found!" << std::endl;
		}
		const std::string replacement = match[1].str() + included;
		fragmentSource.replace(match.position() + offset, match.length(), replacement);
		offset += replacement.length() - match.length();
	}
}

void Shader::onBeforeLoad() {
}

void Shader::onUniformLoad(const std::string& name, const Uniform& uniform) {
}

void Shader::onSourceProcessed() {
}

void Shader::onBeforeDraw() {
}
