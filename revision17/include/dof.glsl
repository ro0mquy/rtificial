#include "helper.glsl"

const int numSamples = 13;

vec4 dof(sampler2D colorSampler, sampler2D depthSampler, float angle) {
	float bleedingBias = 0.02;
	float bleedingMult = 30.;

	vec4 centerPixel = textureLod(colorSampler, tc, 0);
	float centerDepth = textureLod(depthSampler, tc, 0).r;

	vec4 color = vec4(0);
	float totalWeight = 0;

	float radius = .5;
	float aspectRatio = res.x / res.y;
	vec2 pt = radius * vec2(cos(angle), sin(angle));
	pt.x /= aspectRatio;

	for (int i = 0; i < numSamples; i++) {
		float t = float(i) / (numSamples - 1.);
		vec2 offset = mix(-pt, pt, t);
		vec2 sampleCoords = tc + offset * centerPixel.a;
		vec4 samplePixel = textureLod(colorSampler, sampleCoords, 0);
		float sampleDepth = textureLod(depthSampler, sampleCoords, 0).r;

		float weight = sampleDepth < centerDepth ? samplePixel.a * bleedingMult : 1.;
		weight = (centerPixel.a > samplePixel.a + bleedingBias) ? weight : 1.;

		// karis average for better stability
		weight /= 1. + rgb2luma(samplePixel.rgb);

		color += samplePixel * weight;
		totalWeight += weight;
	}

	return color / totalWeight;
}
