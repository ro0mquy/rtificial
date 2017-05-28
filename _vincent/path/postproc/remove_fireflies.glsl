#include "post.glsl"
#include "helper.glsl"
#include "median.glsl"
#line 5

uniform sampler2D color; // vec3
out vec3 out_color;

uniform bool post_disable_median;

void main() {
	if (post_disable_median) {
		out_color = textureLod(color, tc, 0).rgb;
		return;
	}

	vec2 pixelSize = 1. / textureSize(color, 0);
	vec2 base = tc - pixelSize;
	vec3 colors[9];
	float values[9];
	float avg_value = 0;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			vec3 sampleValue = textureLod(color, base + vec2(x, y) * pixelSize, 0).rgb;
			colors[y * 3 + x] = sampleValue;
			values[y * 3 + x] = rgb2luma(colors[y * 3 + x]);
			avg_value += values[y * 3 + x] * (1./9);
		}
	}

	out_color = colors[5];
	if (values[5] > avg_value * 2 || isinf(values[5]) || isnan(values[5])) {
		float median_luma = median(values);
		for (int i = 0; i < 9; i++) {
			if ((values[i] - median_luma) < 1e-6) {
				out_color = colors[i];
			}
		}
	}
}
