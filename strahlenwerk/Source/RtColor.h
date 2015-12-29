#ifndef RT_COLOR_H
#define RT_COLOR_H

#include <juce>
#define GLM_SWIZZLE
#include <glm/glm.hpp>

class RtColor {
	public:
		RtColor();
		RtColor(float red, float green, float blue, float alpha = 1.0f);
		RtColor(Colour juceColour);
		RtColor(uint32 rgba);
		static RtColor fromHexRGBA(StringRef hexString);

		float getRed();
		float getGreen();
		float getBlue();
		float getAlpha();

		static RtColor fromHCY(float hue, float chroma, float luma);

		float getHue();
		float getChroma();
		float getLuma();

		RtColor withHue(float hue);
		RtColor withChroma(float chroma);
		RtColor withLuma(float luma);

		bool isOpaque();
		bool isTransparent();

		RtColor withAlpha(float newAlpha);
		RtColor withMultipliedAlpha(float alphaToMultiply);

		String toGLSLString(bool includeAlpha = false);
		String toHexString(bool includeAlpha = false);
		Colour toJuceColour();

	private:
		float red;
		float green;
		float blue;
		float alpha;

		static const glm::vec3 HCYWeights;
		static glm::vec3 hueToRGB(float hue);
		glm::vec3 toHCY();
};

#endif // RT_COLOR_H
