#include "RtImage.h"

#include <cstring>

RtImage::RtImage(int width_, int height_, bool hasAlphaChannel) :
	width(width_),
	height(height_),
	numChannels(hasAlphaChannel ? 4 : 3),
	numValues(width * height * numChannels)
{
	pixelData = new float[numValues];
	clear();
}

RtImage::RtImage(Image juceImage) :
	width(juceImage.getWidth()),
	height(juceImage.getHeight()),
	numChannels(juceImage.hasAlphaChannel() ? 4 : 3),
	numValues(width * height * numChannels)
{
	pixelData = new float[numValues];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			setPixelAt(x, y, RtColor(juceImage.getPixelAt(x, y)));
		}
	}
}


int RtImage::getWidth() {
	return width;
}

int RtImage::getHeight() {
	return height;
}

Rectangle<int> RtImage::getBounds() {
	return Rectangle<int>(width, height);
}

bool RtImage::hasAlphaChannel() {
	return numChannels == 4;
}


void RtImage::clear() {
	std::memset(pixelData, 0, numValues);
}


RtColor RtImage::getPixelAt(int x, int y) {
	const int pixelOffset = (y * width + x) * numChannels;

	jassert(numChannels >= 3);
	return RtColor(
		pixelData[pixelOffset + 0],
		pixelData[pixelOffset + 1],
		pixelData[pixelOffset + 2],
		( hasAlphaChannel() ? pixelData[pixelOffset + 3] : 1.0f )
	);
}

void RtImage::setPixelAt(int x, int y, RtColor color) {
	const int pixelOffset = (y * width + x) * numChannels;

	jassert(numChannels >= 3);
	pixelData[pixelOffset + 0] = color.getRed();
	pixelData[pixelOffset + 1] = color.getGreen();
	pixelData[pixelOffset + 2] = color.getBlue();

	if (hasAlphaChannel()) {
		jassert(numChannels >= 4);
		pixelData[pixelOffset + 3] = color.getAlpha();
	}
}


Image RtImage::toJuceImage() {
	Image juceImage(
		( hasAlphaChannel() ? Image::PixelFormat::ARGB : Image::PixelFormat::RGB ),
		width,
		height,
		false
	);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			juceImage.setPixelAt(x, y, getPixelAt(x, y).toJuceColour());
		}
	}

	return juceImage;
}
