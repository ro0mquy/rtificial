#include "SceneShader.h"

#include <sstream>
#include <regex>

SceneShader::SceneShader(OpenGLContext& context) :
	context(context),
	program(context)
{
}

void SceneShader::load(std::ifstream& in) {
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	return load(contents.str());
}

void SceneShader::load(std::string source) {
	const std::regex uniformRegex(R"regex([ \t]*uniform[ \t]+(vec[234]|float)[ \t]+(\w+)[ \t]*;)regex");

	std::sregex_iterator it(source.begin(), source.end(), uniformRegex);
	const std::sregex_iterator end;
	for(; it != end; ++it) {
		const auto& match = *it;
		const auto& type = match[1];
		const auto& name = match[2];
	}

	fragmentSourceLock.lock();
	sourceChanged = true;
	fragmentSource = source;
	fragmentSourceLock.unlock();
}

/**
 * Only call this from the OpenGL thread!
 */
void SceneShader::draw() {
	fragmentSourceLock.lock();
	if(sourceChanged) {
		recompile();
	}
	fragmentSourceLock.unlock();

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

}

/**
 * Only call this from the OpenGL thread!
 */
void SceneShader::recompile() {
	program.release();
	program.addVertexShader("#version 120\nattribute vec2 c;\nvoid main() { gl_Position = vec4(c, 0., 1.); }");
	program.addFragmentShader(fragmentSource);
	program.link();

	auto coord = OpenGLShaderProgram::Attribute(program, "c");
	attributeCoord = coord.attributeID;

	sourceChanged = false;
}
