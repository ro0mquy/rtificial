#ifndef TEXTURE_H
#define TEXTURE_H

#include <juce>
#include <vector>

class Texture {
	public:
		Texture(const Image& image, GLenum textureUnit);
		void load();
		void bind();
		void destroy();

	private:
		// data is stored as 8 bit unsigned rgba
		std::vector<uint8> data;
		GLenum textureUnit;
		GLuint textureName;
		const size_t width;
		const size_t height;
};

#endif // TEXTURE_H
