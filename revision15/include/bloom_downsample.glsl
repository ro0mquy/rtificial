vec3 downsample(sampler2D inputTexture, vec2 bigPixelCenter, vec2 smallPixelSize) {
	vec3 topLeftOuter = .25 * textureLod(inputTexture, bigPixelCenter - 2. * smallPixelSize, 0.).rgb;
	vec3 topRightOuter = .25 * textureLod(inputTexture, bigPixelCenter + vec2(2., -2.) * smallPixelSize, 0.).rgb;
	vec3 bottomLeftOuter = .25 * textureLod(inputTexture, bigPixelCenter + vec2(-2., 2.) * smallPixelSize, 0.).rgb;
	vec3 bottomRightOuter = .25 * textureLod(inputTexture, bigPixelCenter + 2. * smallPixelSize, 0.).rgb;
	vec3 top = .25 * textureLod(inputTexture, bigPixelCenter - vec2(0., 1.) * smallPixelSize, 0.).rgb;
	vec3 right = .25 * textureLod(inputTexture, bigPixelCenter + vec2(1., 0.) * smallPixelSize, 0.).rgb;
	vec3 left = .25 * textureLod(inputTexture, bigPixelCenter - vec2(1., 0.) * smallPixelSize, 0.).rgb;
	vec3 bottom = .25 * textureLod(inputTexture, bigPixelCenter + vec2(0., 1.) * smallPixelSize, 0.).rgb;
	vec3 center = .25 * textureLod(inputTexture, bigPixelCenter, 0.).rgb;
	vec3 topLeftInner = .25 * textureLod(inputTexture, bigPixelCenter - smallPixelSize, 0.).rgb;
	vec3 topRightInner = .25 * textureLod(inputTexture, bigPixelCenter + vec2(1., -1.) * smallPixelSize, 0.).rgb;
	vec3 bottomLeftInner = .25 * textureLod(inputTexture, bigPixelCenter + vec2(-1., 1.) * smallPixelSize, 0.).rgb;
	vec3 bottomRightInner = .25 * textureLod(inputTexture, bigPixelCenter + smallPixelSize, 0.).rgb;

	vec3 middleResult = topLeftInner + topRightInner + bottomLeftInner + bottomRightInner;
	vec3 topLeftResult = topLeftOuter + top + left + center;
	vec3 topRightResult = topRightOuter + top + right + center;
	vec3 bottomLeftResult = bottomLeftOuter + bottom + left + center;
	vec3 bottomRightResult = bottomRightOuter + bottom + right + center;

	return middleResult * .5 + .125 * topLeftResult + .125 * topRightResult + .125 * bottomLeftResult + .125 * bottomRightResult;
}
