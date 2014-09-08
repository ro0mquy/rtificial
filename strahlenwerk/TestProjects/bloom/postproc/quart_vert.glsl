#version 430

#include "blur.glsl"

layout(location = 0) uniform vec2 res;
uniform sampler2D color; // vec3 level(3)
in vec2 tc;
out vec3 out_color;

// level(2)

void main() {
	out_color = gaussian(color, tc, vec2(1., 0.), 1./res, .5);
}
