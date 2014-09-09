#include "post_head.glsl"
#include "bokeh.glsl"

uniform sampler2D color; // vec3
uniform sampler2D coc; // float

out vec4 upwards;
out vec4 both;

vec4 calculate_color_coc(vec2 direction) {
	vec2 pixelSize = 1./res;
	vec4 accumulation = vec4(0.);
	int n = 0;
	float this_coc = textureLod(coc, tc, 0.).r;

	for(float i = 0; i < kernelSize; i++) {
		vec3 other_color = textureLod(color, tc + i * pixelSize * direction, 0.).rgb;
		float other_coc = textureLod(coc, tc + i * pixelSize * direction, 0.).r;
		if (shouldBlur(this_coc, other_coc, i)) {
			accumulation += vec4(other_color, other_coc);
			n++;
		}
	}
	return accumulation / n;
}

void main() {
	vec4 value_up = calculate_color_coc(vec2(0., -1.));
	vec4 value_down_left = calculate_color_coc(vec2(cos(radians(30.)), sin(radians(30.))));
	upwards = value_up;
	both = value_up + value_down_left;
}
