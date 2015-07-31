#include "march.glsl"
#line 3 "spimi"

float fDisc(vec3 p, float i) {
	pRotY(p, Tau / 10 * i / (5 + 3.5 * sin(time)));
	pDomrepAngle(p.xz, 10, 2 + sin(i) * .5);
	float d = fBox(p, vec3(.3, .3, 1));
	pTrans(p.x, .5 + .3);
	//float d2 = fBox(p, vec3(1, .2, .2));
	float d2 = fConeCapped(p.zxy, .3, .08, 1);
	return opUnionChamfer(d, d2, .05);
}

float fScene(vec3 p) {
	vec3 q = p;
	float c = .6;
	float i = pDomrep(q.y, c);
	//float i = 0;
	pTrans(q.y, c);
	float d1 = fDisc(q, i + 1);
	pTrans(q.y, -c);
	float d2 = fDisc(q, i);
	pTrans(q.y, -c);
	float d3 = fDisc(q, i - 1);
	float d = min(d1, min(d2, d3));

	mUnion(d, newMaterialId(0., p));
	return d;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	return mat;
}
