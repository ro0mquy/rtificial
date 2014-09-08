#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) out vec3 out_color; // alpha = CoC
layout(location = 1) out float luminance;
layout(location = 2) out float coc;

void output_color(vec3 color, float dist) {
	coc = 0.; // TODO calculate
	out_color = color;
	luminance = log(max(dot(color, vec3(.2126, .7152, .0722)), 1e-6)); // TODO calculate
}

#line 1
