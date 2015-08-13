#include "FlareRenderer.h"

#include <fstream>
#include <sstream>
#include <iostream>

FlareRenderer::FlareRenderer(unsigned int width, unsigned int height) :
	Renderer(width, height)
{
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const std::string vertexSource = loadFile("src/vertex.glsl");
	const std::string geometrySource = loadFile("src/geometry.glsl");
	const std::string fragmentSource = loadFile("src/fragment.glsl");

	const char* vertexSourceCstr = vertexSource.c_str();
	const char* geometrySourceCstr = geometrySource.c_str();
	const char* fragmentSourceCstr = fragmentSource.c_str();

	const GLint vertexSourceLength = vertexSource.size();
	const GLint geometrySourceLength = geometrySource.size();
	const GLint fragmentSourceLength = fragmentSource.size();

	glShaderSource(vertexShader, 1, &vertexSourceCstr, &vertexSourceLength);
	glShaderSource(geometryShader, 1, &geometrySourceCstr, &geometrySourceLength);
	glShaderSource(fragmentShader, 1, &fragmentSourceCstr, &fragmentSourceLength);

	glCompileShader(vertexShader);
	glCompileShader(geometryShader);
	glCompileShader(fragmentShader);

	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
	glAttachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	glLinkProgram(program);

	GLint programLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &programLinked);
	if (programLinked != GL_TRUE) {
		GLsizei logLength = 0;
		GLchar message[4096];
		glGetProgramInfoLog(program, 4096, &logLength, message);
		std::cerr << message << std::endl;
	}

	glUseProgram(program);
	glUniform2f(glGetUniformLocation(program, "resolution"), width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
}

FlareRenderer::~FlareRenderer() {
	glDeleteProgram(program);
}

void FlareRenderer::renderFrame(unsigned int time) {
	glUseProgram(program);

	glClear(GL_COLOR_BUFFER_BIT);

	const GLint position = glGetAttribLocation(program, "position");
	const GLint interface12 = glGetAttribLocation(program, "interface12");

	const float positions[] = {
		-1.0,  1.0, // 1
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 2
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 3
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 4
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 5
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 6
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 7
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 8
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 9
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 10
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 11
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 12
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
		-1.0,  1.0, // 13
		-1.0, -1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};
	const int interfaces[] = {
		4, 1, // 1
		4, 1,
		4, 1,
		4, 1,
		4, 2, // 2
		4, 2,
		4, 2,
		4, 2,
		4, 3, // 3
		4, 3,
		4, 3,
		4, 3,
		3, 2, // 4
		3, 2,
		3, 2,
		3, 2,
		3, 1, // 5
		3, 1,
		3, 1,
		3, 1,
		2, 1, // 6
		2, 1,
		2, 1,
		2, 1,
		5, 1, // 7
		5, 1,
		5, 1,
		5, 1,
		5, 2, // 8
		5, 2,
		5, 2,
		5, 2,
		5, 3, // 9
		5, 3,
		5, 3,
		5, 3,
		5, 4, // 10
		5, 4,
		5, 4,
		5, 4,
		9, 8, // 11
		9, 8,
		9, 8,
		9, 8,
		9, 7, // 12
		9, 7,
		9, 7,
		9, 7,
		8, 7, // 13
		8, 7,
		8, 7,
		8, 7,
	};
	glUniform1f(glGetUniformLocation(program, "time"), time / 1000.);

	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, positions);
	glEnableVertexAttribArray(interface12);
	glVertexAttribIPointer(interface12, 2, GL_INT, 0, interfaces);
	glUniform1i(glGetUniformLocation(program, "index"), 0);
	glDrawArrays(GL_LINES_ADJACENCY, 0, 52);
	glUniform1i(glGetUniformLocation(program, "index"), 1);
	glDrawArrays(GL_LINES_ADJACENCY, 0, 52);
	glUniform1i(glGetUniformLocation(program, "index"), 2);
	glDrawArrays(GL_LINES_ADJACENCY, 0, 52);
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(interface12);

}

std::string FlareRenderer::loadFile(const std::string& path) const {
	std::ifstream in(path);
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	return contents.str();
}
