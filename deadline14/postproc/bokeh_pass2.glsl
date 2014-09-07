#include "post_head.glsl"
#include "bokeh.glsl"

uniform sampler2D upwards; // vec4
uniform sampler2D both; // vec4
out vec3 out_color;

void main() {
	vec2 down_left = vec2(cos(radians(30.)), sin(radians(30.)));
	vec2 down_right = vec2(cos(radians(150.)), sin(radians(150.)));
	vec3 left_accum = vec3(0.);
	vec3 right_accum = vec3(0.);
	vec2 pixelSize = 1./res;
	int n1 = 0, n2 = 0;
	for(float i = 0; i < n; i++) {
		vec4 left = texture2D(upwards, tc + i * pixelSize * down_left).rgb;
		if(left.a - i > 1e-6) {
			left_accum += left.rgb;
		}
		vec4 right = texture2D(both, tc + i * pixelSize * down_right).rgb;
		if(right.a - i > 1e-6) {
			right_accum += right.rgb;
		}
	}
	left_accum /= n1;
	right_accum /= n2;
	out_color = (left_accum + right_accum) / 3.;
}
