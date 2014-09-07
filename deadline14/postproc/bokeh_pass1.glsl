#include "post_head.glsl"
#include "bokeh.glsl"

uniform sampler2D color; // vec4

out vec4 upwards;
out vec4 both;

void main() {
	vec2 pixelSize = 1./res;
	vec4 up_accum = vec4(0.);
	vec4 down_left_accum = vec4(0.);
	vec2 down_left = vec2(cos(radians(30.)), sin(radians(30.)));
	int n1 = 0, n2 = 0;
	for(float i = 0; i < n; i++) {
		vec4 up = texture2D(color, tc + i * pixelSize * vec2(0., -1.));
		if(up.a - i > -1e-6) {
			up_accum += up;
			n1++;
		}
		vec4 down_left = texture2D(color, tc + i * pixelSize * down_left);
		if(down_left.a - i > -1e-6) {
			down_left_accum += down_left;
			n2++;
		}
	}
	upwards = up_accum / n1;
	both = up_accum / n1 + down_left_accum / n2;
}
