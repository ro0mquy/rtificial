#version 430

#include "blur.glsl"

layout(location = 0) uniform vec2 res;
uniform sampler2D color; // vec4 level(4)
in vec2 tc;
out vec3 out_color;

// level(3)

void main() {
	out_color = gaussian(color, tc, vec2(1., 0.), 1./res, .25);
}
