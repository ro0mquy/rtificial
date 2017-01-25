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
		RtColor(const glm::vec3 rgb);
		RtColor(const glm::vec4 rgba);
		static RtColor fromHexRGBA(StringRef hexString);

		float getRed() const;
		float getGreen() const;
		float getBlue() const;
		float getAlpha() const;

		void setRed(const float r_);
		void setGreen(const float g_);
		void setBlue(const float b_);
		void setAlpha(const float a_);

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

		RtColor toLinear() const;
		RtColor toSrgb() const;

		glm::vec3 toVec3() const;
		glm::vec4 toVec4() const;
		String toGLSLString(bool includeAlpha = false) const;
		String toHexString(bool includeAlpha = false) const;
		Colour toJuceColour() const;
		operator Colour() const { return toJuceColour(); }

		friend bool operator==(const RtColor &color1, const RtColor &color2);
		friend bool operator!=(const RtColor &color1, const RtColor &color2);

	private:
		float red;
		float green;
		float blue;
		float alpha;

		static const glm::vec3 HCYWeights;
		static glm::vec3 hueToRGB(float hue);
		glm::vec3 toHCY() const;

		static float linearToSrgb(const float linear);
		static glm::vec3 linearToSrgb(const glm::vec3 linear);
		static float srgbToLinear(const float srgb);
		static glm::vec3 srgbToLinear(const glm::vec3 srgb);
};

#endif // RT_COLOR_H
