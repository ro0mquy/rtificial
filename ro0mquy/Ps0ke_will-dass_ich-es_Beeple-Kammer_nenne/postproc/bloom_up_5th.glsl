#version 430

#include "bloom_upsample.glsl"
#line 5

in vec2 tc;

layout(location = 0) uniform vec2 res;

uniform sampler2D color; // vec3
out vec3 out_color;

// level(4)

void main() {
	out_color = upsample(color, tc, 1./res);
}
