#include "Texture.h"

Texture::Texture(const Image& image, GLenum textureUnit_)
	: textureUnit(textureUnit_), width(image.getWidth()), height(image.getHeight())
{
	const size_t components = 4;
	data.reserve(components * width * height);

	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			Colour pixel = image.getPixelAt(x, y);
			data.push_back(pixel.getRed());
			data.push_back(pixel.getGreen());
			data.push_back(pixel.getBlue());
			data.push_back(pixel.getAlpha());
		}
	}
}

void Texture::load() {
	glGenTextures(1, &textureName);
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
}

void Texture::bind() {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureName);
}

void Texture::destroy() {
	glDeleteTextures(1, &textureName);
}
