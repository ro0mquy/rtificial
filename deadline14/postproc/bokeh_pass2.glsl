#include "post_head.glsl"
#include "bokeh.glsl"

uniform sampler2D upwards; // vec4
uniform sampler2D both; // vec4
out vec3 out_color;

vec3 calculate_color(vec2 direction, sampler2D color_coc_tex) {
	vec2 pixelSize = 1./res;
	vec3 accumulation = vec3(0.);
	int n = 0;
	float this_coc = textureLod(color_coc_tex, tc, 0.).a;

	for(float i = 0; i < kernelSize; i++) {
		vec4 other_color_coc = texture2D(color_coc_tex, tc + i * pixelSize * direction);
		if (shouldBlur(this_coc, other_color_coc.a, i)) {
			accumulation += other_color_coc.rgb;
			n++;
		}
	}
	return accumulation / n;
}

void main() {
	vec3 value_down_left = calculate_color(vec2(cos(radians(30.)), sin(radians(30.))), upwards);
	vec3 value_down_right = calculate_color(vec2(cos(radians(150.)), sin(radians(150.))), both);
	out_color = (value_down_left + value_down_right) / 3.;
}
