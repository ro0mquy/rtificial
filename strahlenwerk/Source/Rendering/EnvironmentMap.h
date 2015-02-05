#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include <juce>

class SceneShader;

class EnvironmentMap {
	public:
		~EnvironmentMap();

		void create(unsigned int width, unsigned int height);
		void render(SceneShader& shader);
		void bind(GLenum textureUnit);

	private:
		GLuint texture;
		bool deleteTexture = false;
};

#endif  // ENVIRONMENTMAP_H
