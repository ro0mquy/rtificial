#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) out vec4 out_color; // alpha = CoC
layout(location = 1) out float luminance;

void output_color(vec3 color, float dist) {
	float coc = 0.; // TODO calculate
	out_color = vec4(color, coc);
	luminance = log(max(dot(color, vec3(.2126, .7152, .0722)), 1e-6)); // TODO calculate
}

#line 1
