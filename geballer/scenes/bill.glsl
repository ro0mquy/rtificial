#include "march.glsl"
#line 3 "bill"

float fScene(vec3 p) {
	float casing_r_gesamt = bill_casing_r_rt_float + bill_casing_thick_rt_float;
	vec3 p_casing = p;
	pMirrorTrans(p_casing.x, casing_r_gesamt);
	float f_casing = f2Sphere(p_casing.xy, bill_casing_r_rt_float);
	f_casing = abs(f_casing) - bill_casing_thick_rt_float;
	f_casing = max(f_casing, abs(p_casing.z) - bill_casing_width_rt_float);

	vec3 p_rillen = p_casing;
	pDomrep(p_rillen.z, bill_rillen_dist_rt_float);
	float f_rillen = f2Sphere(p_rillen.xy, casing_r_gesamt);
	f_rillen = abs(f_rillen) - bill_rillen_depth_rt_float * bill_casing_thick_rt_float;
	f_rillen = max(f_rillen, abs(p_rillen.z) - bill_rillen_width_rt_float);
	f_casing = max(f_casing, -f_rillen);


	float racer1_progress = bill_racer_orbit_progress_rt_float;
	racer1_progress = fract(racer1_progress);
	racer1_progress = linstep(0., bill_racer_pause_start_rt_float, racer1_progress);
	vec3 p_racer1 = p;
	p_racer1.x += casing_r_gesamt;
	p_racer1.x += bill_racer_orbit_r_rt_float;
	pRotY(p_racer1, Tau * racer1_progress);
	p_racer1.x -= bill_racer_orbit_r_rt_float;
	float f_racer1 = fCylinder(p_racer1.xzy, bill_racer_r_rt_float, bill_racer_h_rt_float);


	float racer2_progress = bill_racer_orbit_progress_rt_float;
	racer2_progress = 1. - fract(racer2_progress);
	racer2_progress = linstep(0., bill_racer_pause_start_rt_float, racer2_progress);
	vec3 p_racer2 = p;
	p_racer2.x -= casing_r_gesamt;
	p_racer2.x -= bill_racer_orbit_r_rt_float;
	pRotY(p_racer2, Tau * racer2_progress);
	p_racer2.x += bill_racer_orbit_r_rt_float;
	float f_racer2 = fCylinder(p_racer2.xzy, bill_racer_r_rt_float, bill_racer_h_rt_float);


	float f = f_casing;
	f = min(f, f_racer1);
	f = min(f, f_racer2);

	mUnion(f, MaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;
	return mat;
}
