#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl_identifiers.h"

class Texture {
	public:
		Texture(unsigned int id, const unsigned char* source, unsigned int width, unsigned int height);
		void load();
		void bind();
		void destroy();

	private:
		const unsigned int id;
		const unsigned char* const source;
		const unsigned int width;
		const unsigned int height;
		GLuint textureName;
};

#endif
