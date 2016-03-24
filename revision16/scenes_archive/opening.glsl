#include "march.glsl"
#line 3

float fScene(vec3 p) {
	float side = pMirror(p.x);
	pRotY(p, opening_angle_rt_float * Tau);
	float angle = Tau * opening_angle_rt_float;
	float i = pDomrepInterval(p.z, cos(angle), -20, 0);
	pTrans(p.x,  - i * sin(angle));
	p.x *= side;
	pTrans(p.x, opening_box_trans_rt_float);
	pMirrorTrans(p.z, .01);
	float f = max(p.z, f2Box(p.xy, vec2(1, 1)));

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
