#version 430

#include "blur.glsl"

layout(location = 0) uniform vec2 res;
uniform sampler2D color; // vec3 level(1)
in vec2 tc;
out vec3 out_color;

// level(1)

void main() {
	out_color = gaussian(color, tc, vec2(0., 1.), 1./res);
}
