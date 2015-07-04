#version 430

in vec2 tc;
layout(location = 0) uniform vec2 res;

uniform sampler2D color; // vec3
uniform sampler2D depth; // float
out vec3 out_color;
out float out_depth;

void main() {
	out_color = texture(color, tc, 0.).rgb;
	out_depth = textureLod(depth, tc, 0.).r;
}
