#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include <juce>

class Shader;

class EnvironmentMap {
	public:
		~EnvironmentMap();

		void create(unsigned int width, unsigned int height);
		void render(Shader& shader);
		void bind(GLenum textureUnit);

	private:
		GLuint texture;
		bool deleteTexture = false;
		unsigned int width, height;
};

#endif  // ENVIRONMENTMAP_H
