#include "march.glsl"
#line 3 "dosta"

float fDisc2(vec3 p, float i) {
	//float dBounding = f2Sphere(p.xy, 6/3);
	//if (dBounding > 1) {
	//	return dBounding;
	//}
	pDomrepAngle(p.yx, 10, 5/3);
	pMirrorTrans(p.xz, vec2(.5/3));
	pRotY(p, radians(45) * sin(i));
	pMirrorTrans(p.xz, vec2(.5/3));
	pRotZ(p, radians(45));
	pRotY(p, radians(45));
	return fBox(p, .5/3);
}

float fScene(vec3 p) {
	vec3 q = p;
	float j = pDomrep(q.z, 20);
	//pRotZ(q, j);
	pMirrorLoco(q.yx, vec2(2));
	pMirrorTrans(q.x, 3);
	pRotZ(q, .4);
	q = q.xzy;
	float c = 1;
	float i = pDomrep(q.z, c);
	pTrans(q.z, c);
	float d1 = fDisc2(q, i + 1);
	pTrans(q.z, -c);
	float d2 = fDisc2(q, i);
	pTrans(q.z, -c);
	float d3 = fDisc2(q, i - 1);
	float d = min(d1, min(d2, d3));
	float d4 = f2Sphere(p.xy, 3);
	pDomrepAngle(p.xy, 10, 3);
	float d5 = f2Triprism(p.xy, .5);
	d4 = max(d4, -d5);
	d = opUnionChamfer(d, d4, .2);

	mUnion(d, newMaterialId(0., q));
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
