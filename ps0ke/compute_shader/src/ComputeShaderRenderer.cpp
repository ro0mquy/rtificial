#include "ComputeShaderRenderer.h"

#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include <iostream>

ComputeShaderRenderer::ComputeShaderRenderer(unsigned int width, unsigned int height) : Renderer(width, height) {
	// generate plain example image
	const int WIDTH = 32;
	const int HEIGHT = 32;
	const int CHANNELS = 3;
	float pixels[WIDTH * HEIGHT * CHANNELS];
	for (int i = 0; i < WIDTH*HEIGHT*CHANNELS; i += CHANNELS) {
		pixels[i] = i;//float(i) / float(WIDTH*HEIGHT*CHANNELS) * 255.0f;
		pixels[i+1] = i+1;//0.0f;
		pixels[i+2] = i+2;//0.0f;
		std::cout  << pixels[i] << " " << pixels[i+1] << " " << pixels[i+2] << "\n";
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		WIDTH,
		HEIGHT,
		0,
		GL_RGB,
		GL_FLOAT,
		pixels
	);

	GLuint in_ssbo;
	glGenBuffers(1, &in_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, in_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(pixels), pixels, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, in_ssbo);

	GLuint out_ssbo;
	glGenBuffers(1, &out_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(pixels), 0, GL_STATIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, out_ssbo);

	const char* shader_src = R"(
#version 440
layout(local_size_x = 32) in;
layout(std430) buffer;
layout(binding = 0) readonly  buffer Iutput { float elements[]; } input_data;
layout(binding = 1) writeonly buffer Output { float elements[]; } output_data;

void main() {
    uint id = gl_GlobalInvocationID.x;
    output_data.elements[id] = input_data.elements[id];
}
	)";
	GLuint program = glCreateProgram();
	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, &shader_src, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	char buf[512];
	glGetShaderInfoLog(shader, 512, NULL, buf);
	std::cout << "\nStatus: " << (status == GL_TRUE) << "\n";
	if (status != GL_TRUE) {
		std::cout << "InfoLog:\n" << buf << "\n";
	}

	glAttachShader(program, shader);
	glLinkProgram(program);
	glUseProgram(program);
	glDispatchCompute(32 * 3, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	std::cout << "\n\n\n\noutput\n\n";
	float *out_data;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out_ssbo);
	out_data = (float*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	for (int i = 0; i < WIDTH*HEIGHT*CHANNELS; i += CHANNELS) {
		std::cout  << out_data[i] << " " << out_data[i+1] << " " << out_data[i+2] << "\n";
	}
}

void ComputeShaderRenderer::renderFrame(unsigned int time) {
	(void) time;
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}
