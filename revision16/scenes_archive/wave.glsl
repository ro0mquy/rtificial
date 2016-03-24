#include "march.glsl"
#include "layer.glsl"
#line 4

float fGuard(vec2 p, float t) {
	return 0;
}

float fInner(vec2 p, inout float f_frame, float t) {
	/*
	float radiusWave = 1.5;
	float radiusNoWave = wave_nowave_radius_rt_float;
	vec2 pWave = p;
	float fWave = f2Sphere(pWave, 1.5);
	vec2 pNoWave = p;
	float alpha = wave_t_rt_float * Pi;
	pTrans(pNoWave, unitVector(Pi - alpha) * radiusWave);
	float beta = wave_beta_rt_float * Pi;
	pTrans(pNoWave, unitVector(Pi - alpha - Pi * .5 - beta) * radiusNoWave);
	//pTrans(pNoWave, wave_trans_rt_vec2);
	float fNoWave = f2Sphere(pNoWave, radiusNoWave);
	float f = max(fWave, -fNoWave);
	f = max(f, -p.y);
	return f;
	*/
	float f = f2Box(p, vec2(1., .5));
	float g = f2Box(p, vec2(.5, .5));
	return opSubtractRounded(f, g, .2);
	return opSymmetricDifference(f, g);
}

float fScene(vec3 p) {
	mUnion(wLayerEffect(p));

	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return ambientColor(normal, -direction, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .5;
	float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
	mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);
	return mat;
}
