float lumaaaaa(vec3 color) {
	return dot(color, vec3(.2126, .7152, .0722));
}

vec3 karis_average(vec3 a, vec3 b, vec3 c, vec3 d) {
	float weight = 0.;
	weight += 1./(1.+lumaaaaa(a));
	weight += 1./(1.+lumaaaaa(b));
	weight += 1./(1.+lumaaaaa(c));
	weight += 1./(1.+lumaaaaa(d));
	return (a + b + c + d) / weight;
}

vec3 average(vec3 a, vec3 b, vec3 c, vec3 d, bool karis) {
	if (karis) {
		karis_average(a, b, c, d);
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
