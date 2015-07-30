#include "helper.glsl"

vec3 karis_average(vec3 a, vec3 b, vec3 c, vec3 d) {
	float weights = 0.;
	float weight_a = 1/(1 + rgb2luma(a));
	float weight_b = 1/(1 + rgb2luma(b));
	float weight_c = 1/(1 + rgb2luma(c));
	float weight_d = 1/(1 + rgb2luma(d));
	return (a * weight_a + b * weight_b + c * weight_c + d * weight_d)
		/ (weight_a + weight_b + weight_c + weight_d);
}

vec3 average(vec3 a, vec3 b, vec3 c, vec3 d, bool karis) {
	if (karis) {
		return karis_average(a, b, c, d);
	} else {
		return .25 * a + .25 * b + .25 * c + .25 * d;
	}
}

vec3 downsample(sampler2D inputTexture, vec2 bigPixelCenter, vec2 smallPixelSize, bool karis) {
	vec3 topLeftOuter = textureLod(inputTexture, bigPixelCenter - 2. * smallPixelSize, 0.).rgb;
	vec3 topRightOuter = textureLod(inputTexture, bigPixelCenter + vec2(2., -2.) * smallPixelSize, 0.).rgb;
	vec3 bottomLeftOuter = textureLod(inputTexture, bigPixelCenter + vec2(-2., 2.) * smallPixelSize, 0.).rgb;
	vec3 bottomRightOuter = textureLod(inputTexture, bigPixelCenter + 2. * smallPixelSize, 0.).rgb;
	vec3 top = textureLod(inputTexture, bigPixelCenter - vec2(0., 1.) * smallPixelSize, 0.).rgb;
	vec3 right = textureLod(inputTexture, bigPixelCenter + vec2(1., 0.) * smallPixelSize, 0.).rgb;
	vec3 left = textureLod(inputTexture, bigPixelCenter - vec2(1., 0.) * smallPixelSize, 0.).rgb;
	vec3 bottom = textureLod(inputTexture, bigPixelCenter + vec2(0., 1.) * smallPixelSize, 0.).rgb;
	vec3 center = textureLod(inputTexture, bigPixelCenter, 0.).rgb;
	vec3 topLeftInner = textureLod(inputTexture, bigPixelCenter - smallPixelSize, 0.).rgb;
	vec3 topRightInner = textureLod(inputTexture, bigPixelCenter + vec2(1., -1.) * smallPixelSize, 0.).rgb;
	vec3 bottomLeftInner = textureLod(inputTexture, bigPixelCenter + vec2(-1., 1.) * smallPixelSize, 0.).rgb;
	vec3 bottomRightInner = textureLod(inputTexture, bigPixelCenter + smallPixelSize, 0.).rgb;

	vec3 middleResult      = average(topLeftInner, topRightInner, bottomLeftInner, bottomRightInner, karis);
	vec3 topLeftResult     = average(topLeftOuter, top, left, center, karis);
	vec3 topRightResult    = average(topRightOuter, top, right, center, karis);
	vec3 bottomLeftResult  = average(bottomLeftOuter, bottom, left, center, karis);
	vec3 bottomRightResult = average(bottomRightOuter, bottom, right, center, karis);

	return middleResult * .5 + .125 * topLeftResult + .125 * topRightResult + .125 * bottomLeftResult + .125 * bottomRightResult;
}
