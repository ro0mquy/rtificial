#include "post.glsl"
#include "dof.glsl"
#line 4

uniform sampler2D depth; // float
out float closest_depth;
out float max_coc;

// level(6)

void main() {
	max_coc = 0;

	vec2 sampleDistance = 2./textureSize(depth, 0);
	// 2^(6 - 1) / 2 (depth input is level(1) and we fetch 2 pixels per direction per tap)
	int kernel_width = 16;
	vec2 base = tc - sampleDistance * (kernel_width / 2 - .5);

	closest_depth = 1e6;
	for (int y = 0; y < kernel_width; y++) {
		for (int x = 0; x < kernel_width; x++) {
			vec4 sampleDepth = textureGather(depth, base + vec2(x, y) * sampleDistance, 0);
			closest_depth = min(closest_depth, min(min(sampleDepth.x, sampleDepth.y), min(sampleDepth.z, sampleDepth.w)));
			max_coc = max(max_coc, max(max(calcCoC(sampleDepth.x), calcCoC(sampleDepth.y)), max(calcCoC(sampleDepth.z), calcCoC(sampleDepth.w))));
		}
	}
}
