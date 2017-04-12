#include "march.glsl"
#line 3

float fScene(vec3 p) {
	vec3 p_pyr = p;
	pMirrorLoco(p_pyr.zyx, vec3(1));
	pDomrepGrid(p_pyr.xz, vec2(jibjab_grid_width_rt_float));
	pRotY(p_pyr, jibjab_rot_rt_float * Tau);
	float f = fPyramid(p_pyr, jibjab_pyramid_h_rt_float, jibjab_pyramid_phi_rt_float);
	f = max(f, fSphere(p, jibjab_grid_width_rt_float * 10));

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	//return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;
	return mat;
}
