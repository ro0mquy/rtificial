#include "post_head.glsl"
#include "blur.glsl"

uniform sampler2D color; // vec3 level(2)
out vec3 out_color;

// level(2)

void main() {
	out_color = gaussian(color, tc, vec2(1., 0.), 1./res, 1.);
}
