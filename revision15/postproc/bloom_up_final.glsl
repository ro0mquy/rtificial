#version 430

#include "bloom_upsample.glsl"
#line 5

in vec2 tc;

layout(location = 0) uniform vec2 res;

uniform sampler2D color; // vec3
uniform sampler2D previous; // vec3
out vec3 out_color;

uniform float bloom_amount;

// level(0)

void main() {
	out_color = .5 * upsample(color, tc, 1./res) + .5 * textureLod(previous, tc, 0.).rgb;
	out_color = textureLod(color, tc, 0.).rgb + out_color * bloom_amount;
}
