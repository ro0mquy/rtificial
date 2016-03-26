#include "march.glsl"
#include "background.glsl"
#line 4

float fScene(vec3 p) {
	float f = -fSphere(p, 150.);

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return background(direction);
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
