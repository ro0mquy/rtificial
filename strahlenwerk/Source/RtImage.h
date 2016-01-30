#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <juce>

#include <RtColor.h>

class RtImage {
	public:
		RtImage();
		RtImage(int width, int height, bool hasAlphaChannel = false);
		RtImage(Image juceImage);

		int getWidth() const;
		int getHeight() const;
		Rectangle<int> getBounds() const;
		bool hasAlphaChannel() const;

		void clear();

		RtColor getPixelAt(int x, int y) const;
		void setPixelAt(int x, int y, RtColor color);

		Image toJuceImage() const;
		operator Image() const { return toJuceImage(); }

	private:
		int width;
		int height;
		int numChannels;

	public:
		std::vector<float> pixelData;
};

#endif
