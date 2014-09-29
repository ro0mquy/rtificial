#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "gl.h"

struct Output {
	int components;
	int bindingId;
	int maxLod;
};

class Framebuffer {
	public:
		Framebuffer(int outputLod, int outputsNumber, const Output* outputs);
		~Framebuffer();

		void create(int width, int height);
		void bind();
		void unbind();
		void destroy();
		int width, height;

	private:
		const int outputLod;
		const int outputsNumber;
		const Output* const outputs;
		GLuint fbo;
		GLuint* textures;
};

#endif
