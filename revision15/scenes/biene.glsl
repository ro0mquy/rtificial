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
		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 0.))), 0.) + .1);
	}
}

float expStep( float x, float k, float n ) {
    return exp( -k*pow(x,n) );
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_body = p;
	float t = clamp(p_body.z, -1. / .4, 1. / .4);
	float dellen = expStep(abs(fract(t * 3.) * 2. - 1.), 20., 5.);
	p_body.z -= (1. - pow(min(length(p_body.xy), 1.), 1.47)) * .6;
	p_body.z *= .4;
	float d_body = sphere(p_body, 1. - dellen * .03);

	vec3 p_fluegel = p;
	p_fluegel.x = abs(p_fluegel.x);
	p_fluegel.x -= 1.2;
	p_fluegel.xy *= rot2D(radians(30.));
	p_fluegel.xz *= rot2D(radians(20.));
	p_fluegel.y -= .5;
	p_fluegel.z /= 2.3;
	float d_fluegel = length(p_fluegel.xz) - 1.;
	p_fluegel.y = abs(p_fluegel.y);
	float biegung = 3.;
	p_fluegel.y += biegung - .2;
	d_fluegel = max(d_fluegel, sphere(p_fluegel, biegung));

	vec3 p_kopf = p;
	p_kopf.z += 2.2;
	float kopf_radius = .7;
	float d_kopf = sphere(p_kopf, kopf_radius);
	vec3 p_fuelerchen = p_kopf; // ordentlich fuel
	p_fuelerchen.z += .4;
	p_fuelerchen.y -= .35;
	float t_fueler = clamp(-p_fuelerchen.z, 0., 1.);
	p_fuelerchen.yz *= rot2D(radians(20.) * p_fuelerchen.z);
	p_fuelerchen.x = abs(p_fuelerchen.x);
	p_fuelerchen.x -= .3;
	p_fuelerchen.xz *= rot2D(radians(-30.) * t_fueler);
	float d_fuelerchen = line(p_fuelerchen, vec3(0., 0., -1.), .1);
	d_kopf = smin(d_kopf, d_fuelerchen, .1);

	float d = min(d_body, d_fluegel);
	d = min(d, d_kopf);

	return vec2(d, .0);
}
