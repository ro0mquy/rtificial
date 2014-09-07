#include "post_head.glsl"

uniform sampler2D color; // vec3
out vec3 out_color;

void main() {
	// TODO
	out_color = textureLod(color, tc, 0.).rgb;
}
