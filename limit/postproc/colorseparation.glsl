#include "post.glsl"
#include "helper.glsl"
#line 5

uniform sampler2D color; // vec3
out vec3 out_color;

vec2 distort(vec2 tc, vec2 offset) {
	offset.x *= pow(tc.y * .5 + .2, post_hustensaft_gamma_rt_float);
	return tc + offset;
}

vec3 filter_image_v3(vec2 offset) {
	vec3 result = vec3(0.);
	vec2 filter_width = .5 / res;
	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			vec2 filter_coord = vec2(x, y);
			float weight = exp(-(square(filter_coord.x) + square(filter_coord.y)) * .5) / (2 * Pi);
			result += weight * textureLod(color, fract(distort(tc + filter_coord * filter_width, offset)), 0).rgb;
		}
	}
	return result;
}

void main() {
	vec2 c1_offset = vec2(0.);
	vec2 c2_offset = post_hustensaft_offset_rt_vec2;

	vec3 c1_hustensaft = filter_image_v3(c1_offset);
	vec3 c2_hustensaft = filter_image_v3(c2_offset);

	out_color = mix(c1_hustensaft, c2_hustensaft, post_hustensaft_color_rt_color);
}
