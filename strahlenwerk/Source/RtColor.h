#ifndef RT_COLOR_H
#define RT_COLOR_H

#include <juce>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

class RtColor {
	public:
		RtColor();
		RtColor(float red, float green, float blue, float alpha = 1.0f);
		RtColor(Colour juceColour);
		RtColor(uint32 rgba);
		static RtColor fromHexRGBA(StringRef hexString);

		float getRed() const;
		float getGreen() const;
		float getBlue() const;
		float getAlpha() const;

		static RtColor fromHCY(float hue, float chroma, float luma);

		float getHue() const;
		float getChroma() const;
		float getLuma()const;

		RtColor withHue(float hue) const;
		RtColor withChroma(float chroma) const;
		RtColor withLuma(float luma) const;

		bool isOpaque() const;
		bool isTransparent() const;

		RtColor withAlpha(float newAlpha) const;
		RtColor withMultipliedAlpha(float alphaToMultiply) const;

		String toGLSLString(bool includeAlpha = false) const;
		String toHexString(bool includeAlpha = false) const;
		Colour toJuceColour() const;
		operator Colour() const { return toJuceColour(); }

	private:
		float red;
		float green;
		float blue;
		float alpha;

		static const glm::vec3 HCYWeights;
		static glm::vec3 hueToRGB(float hue);
		glm::vec3 toHCY() const;
};

#endif // RT_COLOR_H
