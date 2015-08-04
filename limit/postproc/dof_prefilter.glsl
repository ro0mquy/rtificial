#include "post.glsl"
#include "helper.glsl"
#include "sample_disk.glsl"
#include "dof.glsl"
#include "textureNN.glsl"
#line 7

uniform sampler2D weights; // vec3
uniform sampler2D color; // vec3
uniform sampler2D max_coc; // float
uniform sampler2D depth; // float

// level(1)

out vec3 out_color;

float spreadToe(float offsetCoC, float spreadCmp) {
	float spreadToePower = 2;
	return offsetCoC <= 1. ? pow(spreadCmp, spreadToePower) : spreadCmp;
}

//float spreadCmp(float offsetCoc, float sampleCoc, float spreadScale) {
//	return spreadToe(offsetCoc, saturate(spreadScale * sampleCoc - (offsetCoc - 1.)));
//}

float spreadCmp(float offsetCoc, float sampleCoc, float spreadScale) {
	return 1. - smoothstep(sampleCoc - .01, sampleCoc + .01, offsetCoc);
	return smoothstep(offsetCoc - .2, offsetCoc, sampleCoc);
	return saturate(sampleCoc - offsetCoc + 1);
	return spreadToe(spreadScale * offsetCoc, saturate(spreadScale * (sampleCoc - offsetCoc) + .5));
}

void main() {
	vec3 centerWeights = textureLod(weights, tc, 0).rgb;
	centerWeights.yz /= sampleAlpha(centerWeights.x);
	float filterScale = max(1, textureNN(max_coc, tc).r / 3);
	float centerDepth = textureLod(depth, tc, 0).r;

	vec4 sum = vec4(0);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			float ngon = 7;
			float f = getFstopRelative();
			vec2 c = bokehTapSampleCoord(vec2(x, y) / 2., f, ngon, Pi * 0.5);

			vec2 sampleCoord = tc + c * filterScale / res;
			vec3 sampleColor = textureLod(color, sampleCoord, 0).rgb;
			vec3 sampleWeights = textureLod(weights, sampleCoord, 0).rgb;
			float sampleDepth = textureLod(depth, sampleCoord, 0).r;

			float offsetCoC = length(c);
			float spreadScale = 1 / filterScale;
			//float spread = spreadCmp(offsetCoC, sampleWeights.x, spreadScale);
			float spread = spreadCmp(offsetCoC, sampleWeights.x / filterScale, spreadScale);
			//spread = 1; // TODO

			float bilateralScale = post_dof_bilateral_scale_rt_float;
			float depthDifference = max(1 - abs(centerDepth - sampleDepth) * bilateralScale, 0);
			float backgroundDifference = max(1 - abs(centerWeights.y - sampleWeights.y), 0);
			float bilateralWeight = depthDifference * backgroundDifference;
			float sharpness = post_dof_sharpness_rt_float;
			float karisWeight = 1/(1 + (1 - sharpness) * rgb2luma(sampleColor));

			float weight = spread * bilateralWeight * karisWeight;
			sum += weight * vec4(sampleColor, 1);
		}
	}

	out_color = sum.rgb / max(1e-3, sum.w);
}
