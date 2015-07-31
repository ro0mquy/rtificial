#include "march.glsl"
#line 3 "stegr"

float fScene(vec3 p) {
	vec3 q = p;

	//pMirrorLoco(q.zx, vec2(40));
	pMirrorLoco(q.xy, vec2(20, 28));
	pRotZ(q, radians(-10));
	//pMirrorLoco(q.yzx, vec3(20));
	//pRotY(q, time);
	vec3 q2 = q;
	pMirrorTrans(q2.xz, vec2(6.5));
	float d = f2Box(q2.xz, .8);
	pMirrorTrans(q.xz, vec2(3));
	pRotY(q, radians(45));
	pDomrepMirror(q.y, 8);
	pRotX(q, radians(45));
	float d2 = f2Triprism(q.xy, .8);
	d = opUnionChamfer(d, d2, .2);

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
