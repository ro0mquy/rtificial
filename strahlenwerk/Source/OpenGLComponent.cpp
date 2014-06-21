#include "OpenGLComponent.h"

OpenGLComponent::OpenGLComponent() : program(context) {
	context.setRenderer(this);
	context.attachTo(*this);
	context.setContinuousRepainting(true);
}

void OpenGLComponent::newOpenGLContextCreated() {
	program.addVertexShader("#version 330\nattribute vec2 c;\nvoid main() { gl_Position = vec4(c, 0., 1.); }");
	program.addFragmentShader("#version 330\nout vec3 color; void main() { color = vec3(sin(gl_FragCoord.y * .3), cos(gl_FragCoord.x * .2), sin(gl_FragCoord.x * .23) * cos(gl_FragCoord.y * .17)) * .5 + .5; }");
	program.link();

	auto coord = OpenGLShaderProgram::Attribute(program, "c");
	attribute_coord = coord.attributeID;
}

void OpenGLComponent::openGLContextClosing() {
}

void OpenGLComponent::renderOpenGL() {
	glClear(GL_COLOR_BUFFER_BIT);
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
