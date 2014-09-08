#include "post_head.glsl"
#include "blur.glsl"

uniform sampler2D color; // vec3 level(3)
out vec3 out_color;

// level(3)

void main() {
	out_color = gaussian(color, tc, vec2(1., 0.), 1./res, 2.);
}
