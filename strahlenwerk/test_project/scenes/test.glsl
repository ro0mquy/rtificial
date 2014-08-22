#version 430

// inactive uniforms lead to crashes (!)
layout(location = 0) uniform vec2 res;

out vec3 out_color;

void main() {
	out_color = vec3(1., gl_FragCoord.xy / res.xy);
}
