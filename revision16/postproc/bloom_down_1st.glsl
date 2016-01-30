#include "post.glsl"

#include "bloom_downsample.glsl"
#line 5

uniform sampler2D color; // vec3
out vec3 out_color;

// level(1)

void main() {
	out_color = downsample(color, tc, .5/res, true);
}
