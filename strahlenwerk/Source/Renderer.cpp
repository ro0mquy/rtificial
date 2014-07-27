#include "Renderer.h"

Renderer::Renderer(OpenGLContext& context) :
	context(context),
	program(context),
	sourceChanged(false)
{
}

void Renderer::newOpenGLContextCreated() {
}

void Renderer::openGLContextClosing() {
}

void Renderer::renderOpenGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	fragmentSourceLock.lock();

	if(sourceChanged) {
		recompile();

		fragmentSource = "";
		sourceChanged = false;
	}

	fragmentSourceLock.unlock();

	program.use();
	context.extensions.glEnableVertexAttribArray(attribute_coord);
	const GLfloat rectangle_vertices[] = {
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	context.extensions.glVertexAttribPointer(attribute_coord, 2, GL_FLOAT, GL_FALSE, 0, rectangle_vertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	context.extensions.glDisableVertexAttribArray(attribute_coord);
}

void Renderer::setFragmentShader(std::string&& fragmentSource) {
	fragmentSourceLock.lock();

	this->fragmentSource = fragmentSource;
	sourceChanged = true;

	fragmentSourceLock.unlock();
}

void Renderer::recompile() {
	program.release();
	program.addVertexShader("#version 120\nattribute vec2 c;\nvoid main() { gl_Position = vec4(c, 0., 1.); }");
	program.addFragmentShader(fragmentSource);
	program.link();

	auto coord = OpenGLShaderProgram::Attribute(program, "c");
	attribute_coord = coord.attributeID;
}
