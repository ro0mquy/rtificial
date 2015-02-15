#version 430

#include "rtificial.glsl"
#line 5

out vec4 out_color;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 100., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 0.))), 0.) + .1);
	}
}

vec2 f(vec3 p, bool last_step) {
	//p.z += 10.;
	//return vec2(sphere(p, 1.), 0.);
	float kuhle_radius = 100.;
	p.y += 10. * pow((1. - smoothstep(0., kuhle_radius, length(p.xz))), spiral_kuhle_power_rt_float);
	float d;
	d = p.y;
	vec3 p_grosse_dinger = p;
	float alpha = atan(p_grosse_dinger.z, p_grosse_dinger.x);
	float r2 = length(p_grosse_dinger.xz);
	float r3 = r2;
	float c5 = 20. * spiral_ringe_rt_float;
	float cell2 = floor(r2 / c5);
	r2 = mod(r2, c5);
	p_grosse_dinger.xz = r2 * vec2(cos(alpha), sin(alpha));
	float radius = 1. * (1.8 - spiral_ringe_rt_float);// + cell2 * .2;
	p_grosse_dinger.y -= .8 * radius;
	vec3 p_loecher = p;
	float c3 = TAU / (10. + cell2 * 15.);
	alpha = mod(alpha, c3) - .5 * c3;
	p_loecher.xz = r3 * vec2(cos(alpha), sin(alpha));
	p_loecher.y -= .8 * radius;
	d = smin(d, max(torus82(p_grosse_dinger, vec2(3., radius)), -(length(p_loecher.zy) - radius * .3)), clamp(spiral_ringe_rt_float * 4., .2, 2.));

	vec3 p_kleine_dinger = p;
	float r = length(p_kleine_dinger.xz);
	float phi = atan(p_kleine_dinger.z, p_kleine_dinger.x);
	float c = TAU / 30.;
	float cell = floor(phi / c) / 30. * .5 + .5;
	phi += pow(r, .4) * .4;
	phi = mod(phi, c) - .5 * c;
	p_kleine_dinger.xz = r * vec2(cos(phi), sin(phi));
	float c2 = 3.;
	p_kleine_dinger.x -= .5 * c2;
	p_kleine_dinger.x = mod(p_kleine_dinger.x, c2) - .5 * c2;
	//p_kleine_dinger.y -= cell * 5.;
	p_kleine_dinger.yz *= rot2D(radians(20.));
	p_kleine_dinger.xy *= rot2D(radians(-20.));
	d = min(d, torus(p_kleine_dinger, vec2(.5, .1)));
	return vec2(d, 0.);
}
