#include "post.glsl"
#line 3

uniform sampler2D closest_depth; // float
uniform sampler2D max_coc; // float

out float out_closest_depth;
out float out_max_coc;

// level(6)

void main() {
	out_max_coc = -1e6;
	out_closest_depth = 1e6;

	vec2 sampleDist = 1./textureSize(closest_depth, 0);
	vec2 base = tc - sampleDist;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			vec2 sampleCoord = base + sampleDist * vec2(x, y);
			out_max_coc = max(out_max_coc, textureLod(max_coc, sampleCoord, 0).r);
			out_closest_depth = min(out_closest_depth, textureLod(closest_depth, sampleCoord, 0).r);
		}
	}
}
