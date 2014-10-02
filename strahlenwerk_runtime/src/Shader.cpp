#include "Shader.h"
#include "DataInterpolator.h"

#define DEBUG
#ifdef DEBUG
#	include <iostream>
#	ifdef BUILD_WINDOWS
#		include <Windows.h>
#	endif
#endif

Shader::Shader(const char* _source, int _inputsNumber, const Input* _inputs) :
	source(_source),
	inputsNumber(_inputsNumber),
	inputs(_inputs)
{
}

void Shader::compile() {
	const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexSource = "#version 410\nlayout(location = 0) in vec2 c;\nlayout(location = 0) out vec2 tc;\nvoid main() { tc = c * .5 + .5;\ngl_Position = vec4(c, 0., 1.);}";
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glCompileShader(vertex);
	glShaderSource(fragment, 1, &source, nullptr);
	glCompileShader(fragment);
	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glLinkProgram(program);

#ifdef DEBUG
	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(link_ok == GL_FALSE) {
		std::cerr << "Link error:" << std::endl;
		GLint log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		char* const log =(char* const) malloc(log_length);
		glGetProgramInfoLog(program, log_length, NULL, log);
#		ifdef BUILD_WINDOWS
			OutputDebugString(log);
#		else
			std::cerr << log;
#		endif
		free(log);
	}
#endif
}

void Shader::bind() {
	glUseProgram(program);
}

void Shader::draw(int width, int height) {
	bind();
	for(int i = 0; i < inputsNumber; i++) {
		glActiveTexture(GL_TEXTURE0 + inputs[i].bindingId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, inputs[i].lod);
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	const GLfloat rectangleVertices[] = {
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, rectangleVertices);
	glUniform2f(0, width, height);
	DataInterpolator::loadUniforms();
	glUniform3f(3, 0.0f, 5.0f, 7.0f);
	glUniform4f(4, 0.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
}

void Shader::destroy() {
	glDeleteProgram(program);
}
