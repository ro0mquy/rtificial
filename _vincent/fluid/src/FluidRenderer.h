#ifndef FLUID_RENDERER_H
#define FLUID_RENDERER_H

#include "Renderer.h"

#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include <string>

class FluidRenderer : Renderer {
	public:
		FluidRenderer(unsigned int width, unsigned int height);
		~FluidRenderer();

		void renderFrame(unsigned int time) override;

	private:
		void renderQuad();
		std::string loadFile(const std::string& path);
		void shaderSourceFromFile(GLuint shader, const std::string& path);
		GLuint loadShader(const std::string& path);
		GLuint createTexture(GLsizei width, GLsizei height);
		GLuint createFramebuffer(GLuint texture);
		void swapU();

		GLuint vertexShader;
		GLuint uTexture, uFramebuffer;
		GLuint uOutTexture, uOutFramebuffer;
};

#endif
