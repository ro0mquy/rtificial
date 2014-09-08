#include "post_head.glsl"
#include "bokeh.glsl"

uniform sampler2D color; // vec3
uniform sampler2D coc; // float

out vec4 upwards;
out vec4 both;

void main() {
	vec2 pixelSize = 1./res;
	vec4 up_accum = vec4(0.);
	vec4 down_left_accum = vec4(0.);
	vec2 down_left = vec2(cos(radians(30.)), sin(radians(30.)));
	int n1 = 0, n2 = 0;
	for(float i = 0; i < n; i++) {
		vec3 up = textureLod(color, tc + i * pixelSize * vec2(0., -1.), 0.).rgb;
		float up_coc = textureLod(coc, tc + i * pixelSize * vec2(0., -1.), 0.).r;
		if(up_coc - i > -1e-6) {
			up_accum += vec4(up, up_coc);
			n1++;
		}
		vec3 down = textureLod(color, tc + i * pixelSize * down_left, 0.).rgb;
		float down_coc = textureLod(coc, tc + i * pixelSize * down_left, 0.).r;
		if(down_coc - i > -1e-6) {
			down_left_accum += vec4(down, down_coc);
			n2++;
		}
	}
	upwards = up_accum / n1;
	both = up_accum / n1 + down_left_accum / n2;
}
