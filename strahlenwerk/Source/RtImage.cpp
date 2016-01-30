#include "RtImage.h"

RtImage::RtImage() :
	width(0),
	height(0),
	numChannels(3)
{
}

RtImage::RtImage(int width_, int height_, bool hasAlphaChannel) :
	width(width_),
	height(height_),
	numChannels(hasAlphaChannel ? 4 : 3),
	pixelData(width * height * numChannels)
{
}

RtImage::RtImage(Image juceImage) :
	width(juceImage.getWidth()),
	height(juceImage.getHeight()),
	numChannels(juceImage.hasAlphaChannel() ? 4 : 3),
	pixelData(width * height * numChannels)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			setPixelAt(x, y, RtColor(juceImage.getPixelAt(x, y)));
		}
	}
}


int RtImage::getWidth() const {
	return width;
}

int RtImage::getHeight() const {
	return height;
}

Rectangle<int> RtImage::getBounds() const {
	return Rectangle<int>(width, height);
}

bool RtImage::hasAlphaChannel() const {
	return numChannels == 4;
}


void RtImage::clear() {
	pixelData.clear();
}


RtColor RtImage::getPixelAt(int x, int y) const {
	jassert(isPositiveAndNotGreaterThan(x, width));
	jassert(isPositiveAndNotGreaterThan(y, height));

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
	jassert(isPositiveAndNotGreaterThan(x, width));
	jassert(isPositiveAndNotGreaterThan(y, height));

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


Image RtImage::toJuceImage() const {
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
