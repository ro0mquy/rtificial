#ifndef SHADER_H
#define SHADER_H

#include "gl.h"

struct Input {
	int bindingId;
	int lod;
};

class Shader {
	public:
		Shader(const char* source, int inputsNumber, const Input* inputs);
		void compile();
		void bind();
		void draw(int width, int height);
		void destroy();

	private:
		const char* const source;
		GLuint program;
		const int inputsNumber;
		const Input* const inputs;
};

#endif
