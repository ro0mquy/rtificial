#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) out vec3 out_color; // alpha = CoC
layout(location = 1) out float coc;

uniform float focus_dist;

uniform float focal_length;
#define FOCAL_LENGTH

uniform float f_stop;

void output_color(vec3 color, float dist) {
	float focus_dist = focus_dist;
	float f = focal_length;
	float N = f_stop;
	coc = (dist - focus_dist)/dist * (f * f) / (N * (focus_dist - f)) / 0.03 * res.x;
	out_color = color;
}

#line 1
