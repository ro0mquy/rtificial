#include "rtificial.glsl"
#line 3

out vec4 out_color;

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
		out_color.rgb = abs(normal);
	}
}

vec2 f(vec3 p, bool last_step) {
	p.y -= -5.;

	vec3 p_berg1 = trans(p, 30., 15., 0.);
	float f_berg1 = cone(p_berg1.xzy, normalize(vec2(1., 1.)));

	vec3 p_berg2 = trans(p, 20., 10., 5.);
	float f_berg2 = cone(p_berg2.xzy, normalize(vec2(2., 1.)));

	vec3 p_berg3 = trans(p, 0., 9., 30.);
	float f_berg3 = cone(p_berg3.xzy, normalize(vec2(1.5, 1.)));

	vec3 p_berg4 = trans(p, -20., 9., 30.);
	float f_berg4 = cone(p_berg4.xzy, normalize(vec2(.8, 1.)));

	vec3 p_berg5 = trans(p, -30., 15., -4.);
	float f_berg5 = cone(p_berg5.xzy, normalize(vec2(1.3, 1.)));

	vec3 p_berg6 = trans(p, -30., 8., -15.);
	float f_berg6 = cone(p_berg6.xzy, normalize(vec2(.9, 1.)));

	vec3 p_berg7 = trans(p, -3., 12., -25.);
	float f_berg7 = cone(p_berg7.xzy, normalize(vec2(1., 1.)));

	float f_ground = p.y;

	float smin_factor = 3.;

	float f = f_berg1;
	f = smin(f, f_berg2, smin_factor);
	f = smin(f, f_berg3, smin_factor);
	f = smin(f, f_berg4, smin_factor);
	f = smin(f, f_berg5, smin_factor);
	f = smin(f, f_berg6, smin_factor);
	f = smin(f, f_berg7, smin_factor);
	f = smin(f, f_ground, smin_factor);

	return vec2(f, 0.);
}
