#version 430

#include "bloom_upsample.glsl"
#line 5

in vec2 tc;

layout(location = 0) uniform vec2 res;

uniform sampler2D color; // vec3
uniform sampler2D previous; // vec3
out vec3 out_color;

uniform float post_bloom_amount;

// level(0)

void main() {
	//out_color = mix(textureLod(color, tc, 0.).rgb, textureLod(previous, tc, 0.).rgb, post_bloom_amount);
	out_color = textureLod(color, tc, 0.).rgb + textureLod(previous, tc, 0.).rgb / 7 * post_bloom_amount;
}
