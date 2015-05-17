#include "ComputeShaderRenderer.h"

#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include <iostream>
#include <string.h>


void APIENTRY debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
	(void) userParam;
	std::cout << source << " " << type << " " << id << " " << severity << " " << length << " " << message << "\n";
}

ComputeShaderRenderer::ComputeShaderRenderer(unsigned int width, unsigned int height) : Renderer(width, height) {
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB); // Might be a good idea to do that!
	glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE); // Check specs.
	glDebugMessageCallbackARB((GLDEBUGPROCARB)&debugOutput, NULL); // Go wild.

	// generate plain example image
	const int WIDTH = 32;
	const int HEIGHT = 32;
	const int CHANNELS = 4;
	float pixels[WIDTH * HEIGHT * CHANNELS];
	for (int i = 0; i < WIDTH*HEIGHT*CHANNELS; i += CHANNELS) {
		pixels[i] = float(i) / float(WIDTH*HEIGHT*CHANNELS);
		pixels[i+1] = 1 - float(i) / float(WIDTH*HEIGHT*CHANNELS);
		pixels[i+2] = 0.0f;
		pixels[i+3] = 0.5f;
		std::cout  << pixels[i] << " " << pixels[i+1] << " " << pixels[i+2] << " " << pixels[i+3] << "\n";
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
/*
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


// <las>
glTexImage2D(
	GL_TEXTURE_2D,
	0,
	GL_R32F,
	HG_STAMP_WIDTH,
	HG_STAMP_HEIGHT,
	0,
	GL_RED,
	GL_FLOAT,
	NULL
);

glTexImage2D(
	GL_TEXTURE_2D,
	0,
	GL_RGB32F,
	HG_STAMP_WIDTH,
	HG_STAMP_HEIGHT,
	0,
	GL_RGB,
	GL_FLOAT,
	NULL
);
glTexImage2D(
	GL_TEXTURE_2D,
	0,
	GL_RGBA32F,
	HG_STAMP_WIDTH,
	HG_STAMP_HEIGHT,
	0,
	GL_RGBA,
	GL_FLOAT,
	NULL
);
*/
glTexImage2D(
	GL_TEXTURE_2D,
	0,
	GL_RGBA32F,
	WIDTH,
	HEIGHT,
	0,
	GL_RGBA,
	GL_FLOAT,
	NULL
);

/*
   <las>
glBindImageTexture(0, m_depthReductionTextures[m_depthReducePingPongIdx], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
// Number of workgroups in X and Y
wgX = (w+WorkGroupSizeX-1)/WorkGroupSizeX;
wgY = (h+WorkGroupSizeY-1)/WorkGroupSizeY;
glUniform2i(glGetUniformLocation(m_reduceMinPRG->getGLID(), "resolution"), w, h)
glDispatchCompute(wgX, wgY, 1);
glMemoryBarrier(GL_ALL_BARRIER_BITS);
*/

	std::cout << "glgeterror: " << glGetError() << "\n";
	glBindImageTexture(
		0,
		tex,
		0,
		GL_FALSE,
		0,
		GL_READ_ONLY,
		GL_R32F
	);
	std::cout << "glgeterror: " << glGetError() << "\n";
/*
	GLuint in_ssbo;
	glGenBuffers(1, &in_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, in_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(pixels), pixels, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, in_ssbo);
*/

	GLuint out_ssbo;
	glGenBuffers(1, &out_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(pixels), 0, GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, out_ssbo);

	const char* shader_src =
R"(
#version 440
layout(local_size_x = 32, local_size_y = 32) in;

layout(std430) buffer;
//layout(binding = 0) readonly  buffer Iutput { float elements[]; } input_data;
layout(rgba32f, binding = 0) readonly uniform highp image2D in_texture;
layout(binding = 1) writeonly buffer Output { float elements[]; } output_data;

void main() {
    uvec2 id = gl_GlobalInvocationID.xy;
	vec4 color = imageLoad(in_texture, ivec2(id));
	//*
    output_data.elements[(id.y*32 + id.x)*4] = (id.y*32 + id.x)*4;
    output_data.elements[(id.y*32 + id.x)*4 + 1] = (id.y*32 + id.x)*4 + 1;
    output_data.elements[(id.y*32 + id.x)*4 + 2] = (id.y*32 + id.x)*4 + 2;
    output_data.elements[(id.y*32 + id.x)*4 + 3] = (id.y*32 + id.x)*4 + 3;
	//*/
	/*
    output_data.elements[(id.y*32 + id.x)*4] = color.r;
    output_data.elements[(id.y*32 + id.x)*4 + 1] = color.g;
    output_data.elements[(id.y*32 + id.x)*4 + 2] = color.b;
    output_data.elements[(id.y*32 + id.x)*4 + 3] = color.a;
	*/
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
	glDispatchCompute(1, 1, 1);
//	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
glMemoryBarrier(GL_ALL_BARRIER_BITS);

	std::cout << "\n\n\n\noutput\n\n";
	float *out_data;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out_ssbo);
	out_data = (float*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	for (int i = 0; i < WIDTH*HEIGHT*CHANNELS; i += CHANNELS) {
		std::cout  << out_data[i] << " " << out_data[i+1] << " " << out_data[i+2] << " " << out_data[i+3] << "\n";
	}
}

void ComputeShaderRenderer::renderFrame(unsigned int time) {
	(void) time;
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}
