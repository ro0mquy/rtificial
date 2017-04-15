#version 430

layout(location = 0) uniform vec2 res;
vec2 tc = gl_FragCoord.xy / res;

layout(location = 64) uniform float time;


layout(binding = 25) uniform sampler2D color; // vec3
layout(binding = 26) uniform sampler2D depth; // float
layout(location = 0) out vec3 out_color;
layout(location = 1) out float out_depth;

void main() {
	out_color = texture(color, tc, 0.).rgb;
	out_depth = textureLod(depth, tc, 0.).r;
}
