#include "Shader.h"
#include "DataInterpolator.h"
#include "Frontend.h" // for RT_DEBUG
#include "Constants.h"
#include "export/strahlenwerk_export_interface.h"

Shader::Shader(const char* _source, int _inputsNumber, const Input* _inputs) :
	source(_source),
	inputsNumber(_inputsNumber),
	inputs(_inputs)
{
}

void Shader::compile() {
	const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexSource = "#version 430\nvoid main() { gl_Position = vec4(gl_VertexID==1?3:-1, gl_VertexID==2?3:-1, 0.0, 1.0); }";
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glCompileShader(vertex);
	glShaderSource(fragment, 1, &source, nullptr);
	glCompileShader(fragment);
#	ifdef _DEBUG
		GLint compile_ok = GL_FALSE;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &compile_ok);
		if (compile_ok == GL_FALSE) {
			RT_DEBUG("Compile error:");
			GLint log_length;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &log_length);
			char* const log = (char* const)malloc(log_length);
			glGetShaderInfoLog(fragment, log_length, NULL, log);
			RT_DEBUG(log)
			free(log);
		}
#	endif
	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glLinkProgram(program);

#	ifdef _DEBUG
		GLint link_ok = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
		if(link_ok == GL_FALSE) {
			RT_DEBUG("Link error:");
			GLint log_length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
			char* const log =(char* const) malloc(log_length);
			glGetProgramInfoLog(program, log_length, NULL, log);
			RT_DEBUG(log)
			free(log);
		}
#	endif
}

void Shader::bind() {
	glUseProgram(program);
}

void Shader::draw(int width, int height, const int time) {
	bind();
	for(int i = 0; i < inputsNumber; i++) {
		glActiveTexture(GL_TEXTURE0 + textureOffsetPostproc + inputs[i].bindingId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, inputs[i].lod);
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glUniform2f(0, width, height);
	if (time >= 0) {
		glUniform1f(time_uniform_location, time / 1000.f); // here time should be in beats
		DataInterpolator::loadUniforms(time);
	}
//	glUniform3f(35, 0.0f, 5.0f, 7.0f);
//	glUniform4f(36, 0.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Shader::destroy() {
	glDeleteProgram(program);
}
