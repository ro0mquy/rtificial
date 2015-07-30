#include "post.glsl"
#include "helper.glsl"
#include "sample_disk.glsl"
#include "dof.glsl"
#include "textureNN.glsl"
#include "noise.glsl"
#line 7

// TODO downsampling
uniform sampler2D weights; // vec3 level(0)
uniform sampler2D color; // vec3 level(0)
uniform sampler2D max_coc; // float

out vec4 out_color;

// level(1)

float spreadToe(float offsetCoC, float spreadCmp) {
	float spreadToePower = 6;
	return offsetCoC <= 1. ? pow(spreadCmp, spreadToePower) : spreadCmp;
}

float spreadCmp(float offsetCoc, float sampleCoc, float spreadScale) {
	float difference = (sampleCoc - offsetCoc) / 3;
	//return difference >= .0 ? 1 : 0;
	return smoothstep(-1./3 * .5, 0., sampleCoc - offsetCoc);
	//return 1. - smoothstep(sampleCoc - .01, sampleCoc + .01, offsetCoc);
	return smoothstep(offsetCoc - .2, offsetCoc, sampleCoc);
	//return saturate(sampleCoc - offsetCoc + 1);
	sampleCoc *= 3.;
	offsetCoc *= 3;
	return spreadToe(offsetCoc, saturate(sampleCoc - offsetCoc + 1));
}

void main() {
	vec3 centerWeights = textureLod(weights, tc, 0).rgb;
	float filterScale = textureNN(max_coc, tc).r * .5;
	vec2 pixelSize = 1./textureSize(color, 0);

	vec4 background_sum = vec4(0);
	vec4 foreground_sum = vec4(0);

	int taps = 11;
	for (int y = 0; y < taps; y++) {
		for (int x = 0; x < taps; x++) {
			float ngon = 7;
			float f = getFstopRelative();
			vec2 c = bokehTapSampleCoord(vec2(x, y) / (taps - 1), f, ngon, Pi * 0.5);

			vec2 jitter = .5 * vec2(
				valueNoise(res * c + gl_FragCoord.xy),
				valueNoise((res * c + 837) + gl_FragCoord.xy)
			);
			c += jitter / filterScale;
			vec2 sampleCoord = tc + c * filterScale * pixelSize;
			vec3 sampleColor = textureNN(color, sampleCoord).rgb; //textureLod(color, sampleCoord, 0).rgb;
			vec3 sampleWeights = textureNN(weights, sampleCoord).rgb; //textureLod(weights, sampleCoord, 0).rgb;
			sampleWeights.x *= .5;

			float offsetCoC = length(c);
			float spreadScale = 1. / filterScale;
			float spread = spreadCmp(offsetCoC, sampleWeights.x / filterScale, spreadScale);

			background_sum += sampleWeights.y * spread * vec4(sampleColor, 1);
			foreground_sum += sampleWeights.z * spread * vec4(sampleColor, 1);
		}
	}

	background_sum.rgb /= max(1e-3, background_sum.w);
	foreground_sum.rgb /= max(1e-3, foreground_sum.w);
	float normalization = 1./(square(taps) * sampleAlpha(filterScale * 2));
	float alpha = saturate(2. * normalization * foreground_sum.a);
	// TODO
	//float background_factor = saturate(.5 * (2 - centerWeights.x));
	//float foreground_factor = saturate(.5 * (2 - filterScale));
	//combinedAlpha = mix(background_factor, foreground_factor, background_sum.a);
	//out_color = vec3(mix(background_sum.rgb, foreground_sum.rgb, foreground_sum.a));

	//out_color = foreground_sum.rgb;
	//combinedAlpha = 0;

	float backgroundFactor = smoothstep(0.5, 1.5, centerWeights.x);
	float foregroundFactor = smoothstep(0.5, 1.5, filterScale);
	float combinedFactor = mix(backgroundFactor, foregroundFactor, alpha);
	combinedFactor = backgroundFactor;
	out_color = vec4(mix(background_sum.rgb, foreground_sum.rgb, alpha), combinedFactor);
}
