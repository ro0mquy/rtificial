#include "post_head.glsl"
#include "blur.glsl"

uniform sampler2D color; // vec3 level(4)
out vec3 out_color;

// level(4)

void main() {
	out_color = gaussian(color, tc, vec2(1., 0.), 1./res, 4.);
}
