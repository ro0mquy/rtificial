#version 430

// inactive uniforms lead to crashes (!)
layout(location = 0) uniform vec2 res;
uniform float red;
uniform vec3 foo; // color

out vec3 out_color;

void main() {
	out_color = mix(foo, vec3(red, gl_FragCoord.xy / res.xy), .1);
}
