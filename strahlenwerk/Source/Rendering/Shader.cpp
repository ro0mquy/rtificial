#include "Shader.h"

#include <sstream>
#include <regex>
#include <vector>
#include <utility>
#include <algorithm>

Shader::Shader(OpenGLContext& context) :
	context(context),
	program(context)
{
}

Shader::~Shader() {
}

void Shader::load(std::ifstream& in) {
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	return load(contents.str());
}

void Shader::load(std::string source) {
	uniforms.clear();

	const std::regex uniformRegex(R"regex(uniform[ \t]+(vec[234]|float)[ \t]+(\w+)[ \t]*;)regex");

	const std::sregex_iterator end;
	std::vector<std::pair<size_t, int>> matches;
	for(std::sregex_iterator it(source.begin(), source.end(), uniformRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& typeString = match[1];
		const auto& name = match[2];

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

	insertLocations(source, matches);

	onSourceProcessed(source);


	modifySource([source] (std::string& origSource) {
		origSource = source;
	});
}

/**
 * Only call this from the OpenGL thread!
 */
void Shader::draw() {
	onBeforeDraw();

	fragmentSourceLock.lock();
	if(sourceChanged) {
		recompile();
	}
	fragmentSourceLock.unlock();
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
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	context.extensions.glDisableVertexAttribArray(attributeCoord);

	onAfterDraw();
}

const std::vector<const Uniform*>& Shader::getUniforms() const {
	return uniforms;
}

const Uniform* Shader::registerUniform(std::string name, UniformType type) {
	return UniformManager::Instance().registerUniform(name, type);
}

void Shader::insertLocations(std::string& source, const std::vector<std::pair<size_t, int>>& locations) {
	// this could be more efficient
	size_t offset = 0;
	for(const auto& location : locations) {
		const auto locationString = "layout(location = " + std::to_string(location.second) + ") ";
		source.insert(location.first + offset, locationString);
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

void Shader::onBeforeLoad() {
}

void Shader::onUniformLoad(const std::string& name, const Uniform& uniform) {
}

void Shader::onSourceProcessed(std::string& source) {
}

void Shader::onBeforeDraw() {
}

void Shader::onAfterDraw() {
}
