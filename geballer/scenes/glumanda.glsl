#include "march.glsl"
#line 3

float fSpimiDisc(vec3 p, float i, float discAtan) {
	float innerRadius = 2 + sin(i) * .5;
	float rotationAngle = Tau / 10 * i / (5 + 3.5 * sin(time));
	pRotY(p, rotationAngle);
	pDomrepAngleWithAtan(p.xz, 10, innerRadius, rotationAngle + discAtan);
	float d = fBox(p, vec3(.3, .3, 1));
	pTrans(p.x, .5 + .3);
	float d2 = fConeCapped(p.zxy, .3, .08, 1);
	return opUnionChamfer(d, d2, .05);
}

float fSpimi(vec3 p, float scale) {
	float c = .6;
	float i = pDomrep(p.y, c);
	//float i = 0;
	float discAtan = atan(p.z, p.x);
	// TODO use simpler objects as guards
	pTrans(p.y, c);
	float d1 = fSpimiDisc(p, i + 1, discAtan);
	pTrans(p.y, -c);
	float d2 = fSpimiDisc(p, i, discAtan);
	pTrans(p.y, -c);
	float d3 = fSpimiDisc(p, i - 1, discAtan);
	return min(d1, min(d2, d3));
}

float fScene(vec3 p) {
	float f = fSpimi(p, 1.);

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
