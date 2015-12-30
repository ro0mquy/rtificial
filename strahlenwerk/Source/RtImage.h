#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <juce>

#include <RtColor.h>

class RtImage {
	public:
		RtImage(int width, int height, bool hasAlphaChannel = false);
		RtImage(Image juceImage);

		int getWidth();
		int getHeight();
		Rectangle<int> getBounds();
		bool hasAlphaChannel();

		void clear();

		RtColor getPixelAt(int x, int y);
		void setPixelAt(int x, int y, RtColor color);

		Image toJuceImage();

	private:
		const int width;
		const int height;
		const int numChannels;
		const int numValues;

		ScopedPointer<float> pixelData;
};

#endif
