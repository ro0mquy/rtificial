#include "post.glsl"
#include "helper.glsl"
#line 5

uniform sampler2D color; // vec3
out vec3 out_color;

vec2 distort(vec2 tc, vec2 offset) {
	offset.x *= pow(tc.y * .5 + .2, post_hustensaft_gamma_rt_float);
	return tc + offset;
}

float filter_image(vec2 offset, uint channel) {
	float result = 0;
	vec2 filter_width = .5 / res;
	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			vec2 filter_coord = vec2(x, y);
			float weight = exp(-(square(filter_coord.x) + square(filter_coord.y)) * .5) / (2 * Pi);
			result += weight * textureLod(color, fract(distort(tc + filter_coord * filter_width, offset)), 0)[channel];
		}
	}
	return result;
}

void main() {
	vec2 red_offset = post_sep_red_offset_rt_vec2;
	vec2 green_offset = post_sep_green_offset_rt_vec2;
	vec2 blue_offset = post_sep_blue_offset_rt_vec2;
	float red_offsetted = filter_image(red_offset, 0);
	float green_offsetted = filter_image(green_offset, 1);
	float blue_offsetted = filter_image(blue_offset, 2);
	out_color = vec3(red_offsetted, green_offsetted, blue_offsetted);
}
