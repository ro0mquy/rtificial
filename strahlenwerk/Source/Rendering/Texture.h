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
		unsigned int id() const;
		const std::vector<uint8>& getData() const;
		size_t getWidth() const;
		size_t getHeight() const;

	private:
		// data is stored as 8 bit unsigned rgba
		std::vector<uint8> data;
		GLenum textureUnit;
		GLuint textureName;
		size_t width;
		size_t height;
		bool textureAllocated = false;
};

#endif // TEXTURE_H
