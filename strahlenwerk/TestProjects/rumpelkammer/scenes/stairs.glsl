#include "march.glsl"
#line 3 "stairs"

float fObject(vec3 p, vec2 dim, float angle) {
	pRotZ(p, -angle);
	vec3 q = p;
	pTrans(q.y, 1 + dim.y);
	return f2Box(p.xy, dim.x, dim.y);
}

float fScene(vec3 p) {
	//float f1 = fBox(p, vec3(8., 4., 2.));
	//pRotZ(p, -.2);
	//float f2 = fBox(p, vec3(2., 8., 4.));
	//float f = opUnionSmooth(f2, f1, 1.);
	//float f = fConeCapped(p.yxz, 6, 2, 0.5);

	vec3 q = p;

	pMirrorTrans(p.x, 0);
	pMirrorTrans(p.y, 30);
	p.y = -p.y;
	vec2 stepDim = vec2(2.5, 1);
	float angle = stepDim.y / stepDim.x;
	pRotZ(p, angle);
	float c = sqrt(stepDim.x * stepDim.x + stepDim.y * stepDim.y) * 2;
	pTrans(p.x, c);
	pDomrepSingle(p.x, c);
	float f = fObject(p, stepDim, angle);
	float r = stepDim.y * .3;
	f = opUnionChamfer(f, fObject(p - vec3(-c,0,0), stepDim, angle), r);
	f = opUnionChamfer(f, fObject(p - vec3(c,0,0), stepDim, angle), r);

	pDomrep(q.xz, stepDim.x * 4, 80);
	float g = f2Box(q.xz, 1, 1);
	f = min(f, g);

	mUnion(f, Material(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}
