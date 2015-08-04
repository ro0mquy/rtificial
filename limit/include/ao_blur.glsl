#include "helper.glsl"
#line 3

uniform sampler2D ao; // float
uniform sampler2D depth; // float
out float out_ao;

// level(1)

void main() {
	float originalAO = textureLod(ao, tc, 0).r;
	float centerDepth = textureLod(depth, tc, 0).r;
	out_ao = originalAO;

	int n_width = 9;
	vec2 result = vec2(0);
	vec2 pixelSize = 1./res;
	float stddev = n_width / 3;
	for (int i = -n_width; i <= n_width; i++) {
		vec2 sampleCoord = tc + i * pixelSize * blurDirection;
		float sampleValue = textureLod(ao, sampleCoord, 0).r;
		float sampleDepth = textureLod(depth, sampleCoord, 0).r;
		float gaussianWeight = exp(-.5 * square(i/stddev)) + post_ao_gauss_offset_rt_float;
		float bilateralWeight = exp(-.5 * square(abs(sampleDepth - centerDepth)/post_ao_edge_blurriness_rt_float));
		result += gaussianWeight * bilateralWeight * vec2(sampleValue, 1);
	}
	out_ao = result.r / result.g;
}

