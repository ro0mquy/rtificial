#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include "gl_identifiers.h"
#include "Shader.h"

class EnvironmentMap {
	public:
		EnvironmentMap(GLenum textureUnit);
		void create(unsigned int width, unsigned int height, unsigned int levels, Shader& shader);
		void destroy();
		void bind();

	private:
		GLenum textureUnit;
		GLuint texture;
};

#endif
