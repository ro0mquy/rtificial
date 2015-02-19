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
		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 2.))), 0.) + .1);
	}
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_fluegel = p;
	//p_fluegel.x = abs(p_fluegel.x);
	//p_fluegel.x -= 1.2;
	//p_fluegel.xy *= rot2D(radians(30.));
	//p_fluegel.xz *= rot2D(radians(20.));
	//p_fluegel.y -= .5;
	p_fluegel.y = abs(p_fluegel.y);
	p_fluegel.z /= 2.3;
	float d_fluegel = length(p_fluegel.xz) - 1.;
	float biegung = 3.;
	p_fluegel.y += biegung - .2;
	d_fluegel = max(d_fluegel, sphere(p_fluegel, biegung));

	vec3 p_filling = p;
	float phi = atan(p_filling.z, p_filling.x);
	float r = length(p_filling.xz);

	phi += pow(r, .4) * 2.;

	//float dr_phi = fluegel_domrep_rt_float;
	float dr_phi = TAU / 6.;
	phi = domrep(phi, dr_phi);

	p_filling.xz = r * vec2(cos(phi), sin(phi));
	p_filling.y -= fluegel_domrep_rt_float * .5;
	p_filling.z -= fluegel_domrep_rt_float * .5;

	p_filling.xz = domrep(p_filling.xz, vec2(fluegel_domrep_rt_float));
	//p_filling = domrep(p_filling, vec3(fluegel_domrep_rt_float));

	float f_filling = box2(p_filling.xz, 1.*vec2(.01, .05));
	//float f_filling = box(p_filling, 1.*vec3(.01, .01, .03));

	float d = max(d_fluegel, f_filling);
	//d = f_filling;

	return vec2(d, .0);
}
