#include "scene_head.glsl"
#include "rtificial.glsl"
#include "background.glsl"
#line 5

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 50., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		vec3 normal = calc_normal(o + t * d, false);
		out_color.rgb = vec3(max(dot(normal, normalize(vec3(1., .5, 2.))), 0.) + .1);
		//out_color.rgb = abs(normal);
	}
	output_color(out_color, t);
}

vec2 f(vec3 p, bool last_step) {
	float f = background(p);
	return vec2(f, 0.);
}
