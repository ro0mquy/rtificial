#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) out vec3 out_color;
layout(location = 1) out float coc;

uniform float camera_focus_dist;

uniform float camera_focal_length;
#define FOCAL_LENGTH

uniform float camera_f_stop;

void output_color(vec3 color, float dist) {
	dist = clamp(dist, 0., 1000.);
	float focus_dist = camera_focus_dist;
	float f = camera_focal_length;
	float N = camera_f_stop;
	coc = (dist - focus_dist)/dist * (f * f) / (N * (focus_dist - f)) / 0.035 * 1920.;
	if(any(isnan(color)) || any(isinf(color))) {
		color = vec3(0.);
	}
	out_color = color;
}

#line 1
