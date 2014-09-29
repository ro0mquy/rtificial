#ifndef SHADER_H
#define SHADER_H

#include "gl.h"

class Shader {
	public:
		Shader(const char* source);
		void compile();
		void bind();
		void draw(int width, int height);
		void destroy();

	private:
		const char* source;
		GLuint program;
};

#endif
