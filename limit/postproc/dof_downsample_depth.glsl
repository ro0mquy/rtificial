#include "post.glsl"
#line 4

uniform sampler2D depth; // float
out float out_depth;

// level(1)

void main() {
	vec4 sampleDepth = textureGather(depth, tc, 0);
	out_depth = max(max(sampleDepth.x, sampleDepth.y), max(sampleDepth.z, sampleDepth.w));
}
