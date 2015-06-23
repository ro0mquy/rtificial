#include "march.glsl"
#line 3 "tweto"

float fScene(vec3 p) {
	vec3 p_tor = p;
	float r_tor = length(p_tor.xz) - tweto_tor_r_big_rt_float;
	float angle_tor = atan(p_tor.z, p_tor.x);
	vec2 q_tor = vec2(r_tor, p_tor.y);
	pRot(q_tor, angle_tor / 4. * tweto_tor_num_twists_rt_float + Tau * tweto_tor_rotation_rt_float);
	float f_torus = f2Box(q_tor, tweto_tor_r_small_rt_float);

	vec3 p_glider = p;
	pRotY(p_glider, Tau * tweto_glider_rotation_rt_float);
	p_glider.x -= tweto_tor_r_big_rt_float;
	pRotZ(p_glider, Tau / 8. + Tau * tweto_tor_rotation_rt_float - Tau * tweto_glider_rotation_rt_float / 4. * tweto_tor_num_twists_rt_float);
	pMirrorTrans(p_glider.xy, vec2(tweto_glider_r_big_rt_float * tweto_tor_r_small_rt_float * sqrt(.5)));
	float f_glider = fSphere(p_glider, tweto_glider_r_small_rt_float);

	float f = f_torus;
	f = min(f, f_glider);

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
