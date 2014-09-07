#include "post_head.glsl"
#include "blur.glsl"

uniform sampler2D color; // vec4 level(4)
out vec3 out_color;

// level(4)

void main() {
	out_color = textureLod(color, tc, 0.).rgb;
}
