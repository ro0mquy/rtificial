#version 430

#include "rtificial.glsl"
#line 5

out vec4 out_color;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 50., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		out_color.rgb = vec3(1.);
	}
}

vec2 f(vec3 p, bool last_step) {
	p.z += 10.;
	return vec2(sphere(p, 1.), 0.);
}
