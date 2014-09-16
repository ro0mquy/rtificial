#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) out vec3 out_color; // alpha = CoC
layout(location = 1) out float luminance;
layout(location = 2) out float coc;

uniform float focus_dist;

void output_color(vec3 color, float dist) {
	float focus_dist = 100. * focus_dist;
	float f = .024;
	float N = 1.;
	coc = (dist - focus_dist)/dist * (f * f) / (N * (focus_dist - f)) / 0.04 * 1920.;
	out_color = color;
	luminance = log(max(dot(color, vec3(.2126, .7152, .0722)), 1e-6)); // TODO calculate
}

#line 1
