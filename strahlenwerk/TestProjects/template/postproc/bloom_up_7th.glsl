#include "post.glsl"

#include "bloom_upsample.glsl"
#line 5

uniform sampler2D color; // vec3
out vec3 out_color;

// level(6)

void main() {
	out_color = upsample(color, tc, 1./res);
}
