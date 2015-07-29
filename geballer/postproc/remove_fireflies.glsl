#include "post.glsl"
#include "helper.glsl"
#include "median.glsl"
#line 5

uniform sampler2D color; // vec3
out vec3 out_color;

uniform bool post_disable_median;

float lumaaaaa(vec3 color) {
	return dot(color, vec3(.2126, .7152, .0722));
}

void main() {
	vec2 pixelSize = 1. / textureSize(color, 0);
	vec2 base = tc - pixelSize;
	vec3 colors[9];
	float values[9];
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			vec3 sampleValue = textureLod(color, base + vec2(x, y) * pixelSize, 0).rgb;
			colors[y * 3 + x] = sampleValue;
			values[y * 3 + x] = lumaaaaa(colors[y * 3 + x]);
		}
	}

	float median_luma = median(values);
	for (int i = 0; i < 9; i++) {
		if ((values[i] - median_luma) < 1e-6) {
			out_color = colors[i];
		}
	}
	if (post_disable_median) {
		out_color = colors[5];
	}
}
