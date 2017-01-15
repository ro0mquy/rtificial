#include "RtColor.h"

RtColor::RtColor() :
	red(0.0f),
	green(0.0f),
	blue(0.0f),
	alpha(1.0f)
{
}

RtColor::RtColor(float red_, float green_, float blue_, float alpha_) :
	red(jlimit(0.0f, 1.0f, red_)),
	green(jlimit(0.0f, 1.0f, green_)),
	blue(jlimit(0.0f, 1.0f, blue_)),
	alpha(jlimit(0.0f, 1.0f, alpha_))
{
	jassert(red_ >= 0.0f && red_ <= 1.0f);
	jassert(green_ >= 0.0f && green_ <= 1.0f);
	jassert(blue_ >= 0.0f && blue_ <= 1.0f);
	jassert(alpha_ >= 0.0f && alpha_ <= 1.0f);
}

RtColor::RtColor(Colour juceColour) :
	red(juceColour.getFloatRed()),
	green(juceColour.getFloatGreen()),
	blue(juceColour.getFloatBlue()),
	alpha(juceColour.getFloatAlpha())
{
}

RtColor::RtColor(uint32 rgba) :
	red((rgba & 0xff000000) >> 24),
	green((rgba & 0x00ff0000) >> 16),
	blue((rgba & 0x0000ff00) >> 8),
	alpha(rgba & 0x000000ff)
{
}

RtColor RtColor::fromHexRGBA(StringRef hexString) {
	return RtColor(uint32(CharacterFunctions::HexParser<int>::parse(hexString.text)));
}


float RtColor::getRed() const {
	return red;
}

float RtColor::getGreen() const {
	return green;
}

float RtColor::getBlue() const {
	return blue;
}

float RtColor::getAlpha() const {
	return alpha;
}

/*
 * HCY hue/chroma/luminance color space
 * implementation after
 * http://www.chilliant.com/rgb2hsv.html
 */

//const glm::vec3 RtColor::HCYWeights = glm::vec3(0.299f, 0.587f, 0.114f);
const glm::vec3 RtColor::HCYWeights = glm::vec3(0.2126f, 0.7152f, 0.0722f);

RtColor RtColor::fromHCY(float hue, float chroma, float luma) {
	jassert(hue >= 0.0f && hue <= 1.0f);
	hue = jlimit(0.0f, 1.0f, hue);
	jassert(chroma >= 0.0f && chroma <= 1.0f);
	chroma = jlimit(0.0f, 1.0f, chroma);
	jassert(luma >= 0.0f && luma <= 1.0f);
	luma = jlimit(0.0f, 1.0f, luma);

	glm::vec3 hcy(hue, chroma, luma);
	glm::vec3 rgb = hueToRGB(hue);

	float z = glm::dot(rgb, HCYWeights);

	if (hcy.z < z) {
		hcy.y *= hcy.z / z;
	} else if (z < 1) {
		hcy.y *= (1 - hcy.z) / (1 - z);
	}

	rgb = (rgb - z) * hcy.y + hcy.z;

	return RtColor(rgb.r, rgb.g, rgb.b);
}

glm::vec3 RtColor::hueToRGB(float hue) {
	glm::vec3 rgb;
	rgb.r = jlimit(0.0f, 1.0f, glm::abs(hue * 6 - 3) - 1);
	rgb.g = jlimit(0.0f, 1.0f, 2 - glm::abs(hue * 6 - 2));
	rgb.b = jlimit(0.0f, 1.0f, 2 - glm::abs(hue * 6 - 4));
	return rgb;
}

glm::vec3 RtColor::toHCY() const {
	glm::vec3 rgb(red, green, blue);

	glm::vec4 p;
	if (rgb.g < rgb.b) {
		p = glm::vec4(rgb.b, rgb.g, -1.0f, 2.0f/3.0f);
	} else {
		p = glm::vec4(rgb.g, rgb.b, 0.0f, -1.0f/3.0f);
	}

	glm::vec4 q;
	if (rgb.r < p.x) {
		q = glm::vec4(p.x, p.y, p.w, rgb.r);
	} else {
		q = glm::vec4(rgb.r, p.y, p.z, p.x);
	}

	const float epsilon = 1e-10;
	const float c = q.x - jmin(q.w, q.y);
	const float h = glm::abs( (q.w - q.y) / (6 * c + epsilon) + q.z );

	glm::vec3 hcv(h, c, q.x);

	float y = glm::dot(rgb, HCYWeights);
	float z = glm::dot(hueToRGB(hcv.x), HCYWeights);

	if (y < z) {
		hcv.y *= z / (epsilon + y);
	} else {
		hcv.y *= (1 - z) / (epsilon + 1 - y);
	}

	return glm::vec3(hcv.x, hcv.y, y);
}

float RtColor::getHue() const {
	return toHCY().x;
}

float RtColor::getChroma() const {
	return toHCY().y;
}

float RtColor::getLuma() const {
	return toHCY().z;
}

RtColor RtColor::withHue(float hue) const {
	const glm::vec3 hcy = toHCY();
	return RtColor::fromHCY(hue, hcy.y, hcy.z);
}

RtColor RtColor::withChroma(float chroma) const {
	const glm::vec3 hcy = toHCY();
	return RtColor::fromHCY(hcy.x, chroma, hcy.z);
}

RtColor RtColor::withLuma(float luma) const {
	const glm::vec3 hcy = toHCY();
	return RtColor::fromHCY(hcy.x, hcy.y, luma);
}


bool RtColor::isOpaque() const {
	return alpha == 1.0f;
}

bool RtColor::isTransparent() const {
	return alpha == 0.0f;
}


RtColor RtColor::withAlpha(float newAlpha) const {
	jassert(newAlpha >= 0.0f && newAlpha <= 1.0f);
	return RtColor(red, green, blue, newAlpha);
}

RtColor RtColor::withMultipliedAlpha(float alphaMultiplier) const {
	jassert(alphaMultiplier >= 0);
	return RtColor(red, green, blue, alpha * alphaMultiplier);
}


String RtColor::toGLSLString(bool includeAlpha) const {
	String str = "vec";
	str += includeAlpha ? "4(" : "3(";
	str += String(red) + ", " + String(green) + ", " + String(blue);
	if (includeAlpha) {
		str += ", " + String(alpha);
	}
	str += ")";
	return str;

}

String RtColor::toHexString(bool includeAlpha) const {
	int hexColor = 0;
	hexColor += jlimit(0, 255, int(red * 255.0f)) << 16;
	hexColor += jlimit(0, 255, int(green * 255.0f)) << 8;
	hexColor += jlimit(0, 255, int(blue * 255.0f));

	String text(String::toHexString(hexColor));
	text = text.toUpperCase();
	text = text.paddedLeft('0', 6);

	if (includeAlpha) {
		hexColor <<= 8;
		hexColor += jlimit(0, 255, int(alpha * 255.0f));

		text = String::toHexString(hexColor);
		text = text.toUpperCase();
		text = text.paddedLeft('0', 8);
	}

	return '#' + text;
}

Colour RtColor::toJuceColour() const {
	return Colour::fromFloatRGBA(red, green, blue, alpha);
}

bool operator==(const RtColor &color1, const RtColor &color2) {
	return color1.red   == color2.red
		&& color1.green == color2.green
		&& color1.blue  == color2.blue
		&& color1.alpha == color2.alpha;
}

bool operator!=(const RtColor &color1, const RtColor &color2) {
	return !(color1 == color2);
}
