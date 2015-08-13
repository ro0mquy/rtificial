#ifndef EXAMPLE_RENDERER_H
#define EXAMPLE_RENDERER_H

#include "Renderer.h"

#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#include <string>

class FlareRenderer : Renderer {
	public:
		FlareRenderer(unsigned int width, unsigned int height);
		~FlareRenderer();

		void renderFrame(unsigned int time) override;

	private:
		std::string loadFile(const std::string& path) const;

		GLuint program;
};

#endif
