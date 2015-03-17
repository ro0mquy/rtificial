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

vec2 smoothfloor(vec2 x, float k) {
	return floor(x) + smoothstep(1. - k, 1. + k, fract(x)) - (1. - smoothstep(-k, k, fract(x)));
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_sphere = p;

	float radius = length(p_sphere);
	float theta = atan(p_sphere.x, p_sphere.z) + TAU * .5;
	float phi = acos(p_sphere.y / radius);

	theta = mod(theta/TAU, .04) - .02;
	phi = mod(phi/TAU, .04) - .02;

	radius -= 1.;

	//float d = box(vec3(theta, phi, radius), vec3(.01, .01, 1.5));
	float d = torus(vec3(theta, radius, phi), vec2(.01, .005));

	d = smax(sphere(p, vec3(1.)), -d, .01 * steinchen_smoothing_rt_float);
	//d= torus(p, vec2(1., .5));
	return vec2(d, .0);
}

/*
vec2 f(vec3 p, bool last_step) {
	float radius = 10.;
	vec3 p_box = p;
	vec2 box_dim = vec2(2., .3);
	float phi = atan(p_box.y, p_box.x);
	float theta = acos(p_box.z/length(p_box));
	float r = length(p_box);
	float c = TAU / 20.;
	phi = mod(phi, c) - .5 * c;
	theta = mod(theta, c) - .5 * c;
	p_box = r * vec3(cos(phi), sin(phi), 1.) * vec3(sin(theta), sin(theta), cos(theta));
	p_box.z -= radius + box_dim.x;
	float d2 = box(p_box, box_dim.yyx);
	float d = sphere(p, radius);
	d = d2;
	return vec2(min(d, d2), 0.);
}
*/
