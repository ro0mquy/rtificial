#include "Texture.h"
#include "TextureUnits.h"

#define GL_GLEXT_PROTOTYPES
#include "glcorert.h"

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
	// set this to true before calling bind to avoid infinite mutual recursion
	textureAllocated = true;
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Texture::bind() {
	if (!textureAllocated) {
		load();
	}
	glActiveTexture(textureUnit + TextureUnitOffset::UserTextures);
	glBindTexture(GL_TEXTURE_2D, textureName);
}

void Texture::destroy() {
	if (textureAllocated) {
		glDeleteTextures(1, &textureName);
	}
}

unsigned int Texture::id() const {
	return textureUnit - GL_TEXTURE0;
}

const std::vector<uint8>& Texture::getData() const {
	return data;
}
size_t Texture::getWidth() const {
	return width;
}

size_t Texture::getHeight() const {
	return height;
}
