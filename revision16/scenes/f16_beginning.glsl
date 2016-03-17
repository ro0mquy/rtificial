#include "march.glsl"
#include "f16.glsl"
#line 4

float fScene(vec3 p) {
	float f = abs(p.z) - .5;
	float p_stripes = p.x;
	float c = 2. / 16;
	pDomrep(p_stripes, c);
	float f_stripes = abs(p_stripes) - c * .25;
	f_stripes = max(f, f_stripes);
	//f = max(f, fF16Ground(p.xy, 2.));
	f_stripes = max(f_stripes, -fF16Ground(p.xy, 2.));
	f = f_stripes;

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	//return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .8;
	mat.color = vec3(.5);
	return mat;
}
