#include "post.glsl"
#include "helper.glsl"
#line 4

uniform sampler2D color; // vec4
out vec4 out_color;

// level(1)

void min_max(float a, float b, out float min_out, out float max_out) {
	min_out = min(a, b);
	max_out = max(a, b);
}

float median(float elements[9]) {
	float min10, max10;
	min_max(elements[1], elements[2], min10, max10);
	float min11, max11;
	min_max(elements[4], elements[5], min11, max11);
	float min12, max12;
	min_max(elements[7], elements[8], min12, max12);

	float min20, max20;
	min_max(elements[0], min10, min20, max20);
	float min21, max21;
	min_max(elements[3], min11, min21, max21);
	float min22, max22;
	min_max(elements[6], min12, min22, max22);

	float min30, max30;
	min_max(max20, max10, min30, max30);
	float min31, max31;
	min_max(max21, max11, min31, max31);
	float min32, max32;
	min_max(max22, max12, min32, max32);

	float max40 = max(min20, min21);
	float min41 = min(max31, max32);

	float min50, max50;
	min_max(min31, min32, min50, max50);

	float max60 = max(max40, min22);
	float max61 = max(min30, min50);
	float min62 = min(max30, min41);

	float min70 = min(max61, max50);

	float min80, max80;
	min_max(min70, min62, min80, max80);

	float max90 = max(max60, min80);

	return min(max90, max80);
}

float lumaaaaa(vec3 color) {
	return dot(color, vec3(.2126, .7152, .0722));
}

void main() {
	vec2 pixelSize = 1. / textureSize(color, 0);
	vec2 base = tc - pixelSize;
	vec3 colors[9];
	float values[9];
	float alphas[9];
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			vec4 sampleValue = textureLod(color, base + vec2(x, y) * pixelSize, 0);
			colors[y * 3 + x] = sampleValue.rgb;
			values[y * 3 + x] = lumaaaaa(colors[y * 3 + x]);
			alphas[y * 3 + x] = sampleValue.a;
		}
	}

	float median_luma = median(values);
	out_color.a = median(alphas);
	for (int i = 0; i < 9; i++) {
		if ((values[i] - median_luma) < 1e-6) {
			out_color.rgb = colors[i];
		}
	}
}
