#include "march.glsl"
#include "noise.glsl"
#line 3 "wiese"

float fScene(vec3 p) {
	float f = Inf;

	vec2 cell = pDomrepMirror(p.xz, vec2(5., 7.));
	pTrans(p.xz, .1, .4);
	vec3 p_stiel = p;
	float f_stiel = fCylinder(p_stiel, wiese_stiel_radius_rt_float, wiese_stiel_height_rt_float);
	f = min(f, f_stiel);

	vec3 p_bluete = p_stiel;
	p_bluete.y -= wiese_stiel_height_rt_float;

	float max_bluete_radius = wiese_bluete_radius_rt_float * (1. + wiese_bluete_radius_rt_float);
	float f_bluete_bounding = fSphere(p_bluete, max_bluete_radius);
	float f_bluete = f_bluete_bounding;

	if (f_bluete < 1.) {
		float bluete_radius = wiese_bluete_radius_rt_float;
		bluete_radius *= 1. + wiese_bluete_radius_amplitude_rt_float * valueNoise(wiese_bluete_radius_freq_rt_float * (cell + time));

		f_bluete = fSphere(p_bluete, bluete_radius);
	}
	f = min(f, f_bluete);


	mUnion(f, Material(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}
