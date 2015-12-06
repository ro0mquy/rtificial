#include "FluidRenderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

void debugOutput(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		void *userParam)
{
	(void) source;
	(void) type;
	(void) id;
	(void) length;
	(void) userParam;
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
		// ignore notifications
		return;
	}

	std::cerr << message << std::endl;

	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		exit(1);
	}
}

FluidRenderer::FluidRenderer(unsigned int width, unsigned int height)
	: Renderer(width, height)
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Might be a good idea to do that!
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE); // Check specs.
	glDebugMessageCallback((GLDEBUGPROC)&debugOutput, nullptr); // Go wild.

	const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexSource = "#version 130\nvoid main() { gl_Position = vec4(gl_VertexID==1?3:-1, gl_VertexID==2?3:-1, 0.0, 1.0); }";
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glCompileShader(vertex);

	//std::string source = loadFile("src/shader.glsl");

	//uTexture = createTexture(width, height);
	//uOutTexture = createTexture(width, height);
	//uFramebuffer = createFramebuffer(uTexture);
	//uOutFramebuffer = createFramebuffer(uOutFramebuffer);

}

FluidRenderer::~FluidRenderer() {
	glDeleteShader(vertexShader);
}

void FluidRenderer::renderFrame(unsigned int time) {
//u = advect(u);
//u = diffuse(u);
//u = addForces(u);
//p = computePressure(u);
//u = subtractPressureGradient(u, p);
}

void FluidRenderer::renderQuad() {
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

std::string FluidRenderer::loadFile(const std::string& path) {
	std::ifstream in(path);
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	return contents.str();
}

void FluidRenderer::shaderSourceFromFile(GLuint shader, const std::string& path) {
	std::string source = loadFile(path);
	const char* source_c_str = source.c_str();
	const GLint source_length = source.size();
	glShaderSource(shader, 1, &source_c_str, &source_length);
}

GLuint FluidRenderer::loadShader(const std::string& path) {
	GLuint shaderProgram = glCreateProgram();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	shaderSourceFromFile(fragmentShader, path);
	glCompileShader(fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

GLuint FluidRenderer::createTexture(GLsizei width, GLsizei height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	return texture;
}

GLuint FluidRenderer::createFramebuffer(GLuint texture) {
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FluidRenderer::swapU() {
	std::swap(uTexture, uOutTexture);
	std::swap(uFramebuffer, uOutFramebuffer);
}
