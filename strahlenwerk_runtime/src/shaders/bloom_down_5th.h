#ifndef bloom_down_5th_H
#define bloom_down_5th_H
const char bloom_down_5th_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float lumaaaaa(vec3 color) {
)shader_source"
R"shader_source(	return dot(color, vec3(.2126, .7152, .0722));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 karis_average(vec3 a, vec3 b, vec3 c, vec3 d) {
)shader_source"
R"shader_source(	float weight = 0.;
)shader_source"
R"shader_source(	weight += 1./(1.+lumaaaaa(a));
)shader_source"
R"shader_source(	weight += 1./(1.+lumaaaaa(b));
)shader_source"
R"shader_source(	weight += 1./(1.+lumaaaaa(c));
)shader_source"
R"shader_source(	weight += 1./(1.+lumaaaaa(d));
)shader_source"
R"shader_source(	return (a + b + c + d) / weight;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 average(vec3 a, vec3 b, vec3 c, vec3 d, bool karis) {
)shader_source"
R"shader_source(	if (karis) {
)shader_source"
R"shader_source(		karis_average(a, b, c, d);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		return .25 * a + .25 * b + .25 * c + .25 * d;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 downsample(sampler2D inputTexture, vec2 bigPixelCenter, vec2 smallPixelSize, bool karis) {
)shader_source"
R"shader_source(	vec3 topLeftOuter = textureLod(inputTexture, bigPixelCenter - 2. * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 topRightOuter = textureLod(inputTexture, bigPixelCenter + vec2(2., -2.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottomLeftOuter = textureLod(inputTexture, bigPixelCenter + vec2(-2., 2.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottomRightOuter = textureLod(inputTexture, bigPixelCenter + 2. * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 top = textureLod(inputTexture, bigPixelCenter - vec2(0., 1.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 right = textureLod(inputTexture, bigPixelCenter + vec2(1., 0.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 left = textureLod(inputTexture, bigPixelCenter - vec2(1., 0.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottom = textureLod(inputTexture, bigPixelCenter + vec2(0., 1.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 center = textureLod(inputTexture, bigPixelCenter, 0.).rgb;
)shader_source"
R"shader_source(	vec3 topLeftInner = textureLod(inputTexture, bigPixelCenter - smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 topRightInner = textureLod(inputTexture, bigPixelCenter + vec2(1., -1.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottomLeftInner = textureLod(inputTexture, bigPixelCenter + vec2(-1., 1.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottomRightInner = textureLod(inputTexture, bigPixelCenter + smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 middleResult      = average(topLeftInner, topRightInner, bottomLeftInner, bottomRightInner, karis);
)shader_source"
R"shader_source(	vec3 topLeftResult     = average(topLeftOuter, top, left, center, karis);
)shader_source"
R"shader_source(	vec3 topRightResult    = average(topRightOuter, top, right, center, karis);
)shader_source"
R"shader_source(	vec3 bottomLeftResult  = average(bottomLeftOuter, bottom, left, center, karis);
)shader_source"
R"shader_source(	vec3 bottomRightResult = average(bottomRightOuter, bottom, right, center, karis);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return middleResult * .5 + .125 * topLeftResult + .125 * topRightResult + .125 * bottomLeftResult + .125 * bottomRightResult;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 5
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 11) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// level(5)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	out_color = downsample(color, tc, .5/res, false);
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
