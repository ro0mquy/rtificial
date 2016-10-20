#version 430

layout(location = 0) uniform vec2 res;
vec2 tc = gl_FragCoord.xy / res;

layout(location = 64) uniform float time;

#line 3

layout(binding = 25) uniform sampler2D color; // vec3
layout(location = 0) out vec3 out_color;

void main() {
	out_color = textureLod(color, tc, 0.).rgb;
}
