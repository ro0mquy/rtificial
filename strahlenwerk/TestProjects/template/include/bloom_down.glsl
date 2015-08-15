#include "post.glsl"

#include "bloom_downsample.glsl"
#line 5

uniform sampler2D color; // vec3
out vec3 out_color;

void main() {
	out_color = downsample(color, tc, .5/res, false);
}
