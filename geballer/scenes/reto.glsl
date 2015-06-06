#include "march.glsl"
#line 3 "reto"

float fScene(vec3 p) {
	vec3 p_big = p;
	pRotY(p_big, Tau * reto_big_rot_rt_float);
	float f_big_outer = f2Pentaprism(p_big.xz, reto_big_outer_r_rt_float);
	vec2 q_big = vec2(f_big_outer, p_big.y);
	float f_big = f2Sphere(q_big, reto_big_inner_r_rt_float);

	vec3 p_small = p_big;
	pRotY(p_small, Tau / 5. / 4.);
	pDomrepAngle(p_small.xz, 5., 0.);
	p_small.x -= reto_big_outer_r_rt_float * cos(Tau / 5. / 2.);
	pRotZ(p_small, Tau * reto_small_rot_rt_float);

	float s_o_r = reto_big_inner_r_rt_float * reto_small_outer_r_rt_float;
	float f_small_outer = f2Sphere(p_small.xy, s_o_r);
	vec2 q_small = vec2(p_small.z, f_small_outer);
	float f_small = f2Pentaprism(q_small, reto_small_inner_r_rt_float);

	vec3 p_cutout = p_small;
	pDomrepAngle(p_cutout.xy, 5., 0.);
	pMirrorTrans(p_cutout.y, reto_cutout_r_rt_float);
	float f_cutout = p_cutout.y;
	f_small = opSubtractStairs(f_small, f_cutout, reto_cutout_smooth_rt_float, 5.);

	float f = min(f_big, f_small);

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
