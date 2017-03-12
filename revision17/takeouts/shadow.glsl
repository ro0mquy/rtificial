#include "march.glsl"
#include "shadow.glsl"
#line 4

float fScene(vec3 p) {
	float f = p.y;
	f = opUnionStairs(f, fBoxEdge(p, vec3(.2, 3., .2)), .5, 3);
	pTrans(p.y, 3.);
	f = opUnionRounded(f, fSphere(p, 1.), .1);

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;

	vec3 ambient_lighting = ambientColor(normal, -direction, material);

	SphereLight light = SphereLight(shad_light_pos_rt_vec3, .5, vec3(1.), shad_light_power_rt_float*1000.);
	vec3 sphere_lighting = applySphereLight(origin, marched, direction, hit, normal, material, light);

	vec3 dir_to_light = normalize(shad_light_pos_rt_vec3 - hit);
	float length_to_light = distance(shad_light_pos_rt_vec3, hit);
	float shadowing_value = shadowMarch(hit, dir_to_light, length_to_light, shad_softness_rt_float);

	return emission + ambient_lighting + shadowing_value * sphere_lighting;
	//return normal_light + shadowing_value * sphere_lighting;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	color = shad_invert_factor_rt_float / pow(color, vec3(shad_invert_pow_rt_float));
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.color = vec3(.5);
	mat.roughness = .0;
	return mat;
}
