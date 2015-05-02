#ifndef COMPUTE_SHADER_RENDERER_H
#define COMPUTE_SHADER_RENDERER_H

#include "Renderer.h"

#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

class ComputeShaderRenderer : Renderer {
	public:
		ComputeShaderRenderer(unsigned int width, unsigned int height);
		void renderFrame(unsigned int time) override;
};

#endif
