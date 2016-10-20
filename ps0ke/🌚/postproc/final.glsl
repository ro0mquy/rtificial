#include "post.glsl"
#line 3

uniform sampler2D color; // vec3
out vec3 out_color;

void main() {
	out_color = textureLod(color, tc, 0.).rgb;
}
