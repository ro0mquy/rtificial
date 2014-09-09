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
	float this_coc1 = textureLod(upwards, tc, 0.).a;
	float this_coc2 = textureLod(both, tc, 0.).a;
	for(float i = 0; i < n; i++) {
		vec4 left = texture2D(upwards, tc + i * pixelSize * down_left);
		float foo = left.a / this_coc1;
		if(abs(left.a) - i > -1e-6 && (left.a < 0. || (foo > 0. && foo < 2.))) {
			left_accum += left.rgb;
			n1++;
		}
		vec4 right = texture2D(both, tc + i * pixelSize * down_right);
		foo = right.a / this_coc2;
		if(abs(right.a) - i > -1e-6 && (right.a < 0. || (foo > 0. && foo < 2.))) {
			right_accum += right.rgb;
			n2++;
		}
	}
	left_accum /= n1;
	right_accum /= n2;
	out_color = (left_accum + right_accum) / 3.;
}
