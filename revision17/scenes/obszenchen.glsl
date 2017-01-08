#include "march.glsl"
#line 3

// spiral starting at the orgin
// dist is distance between tubes, r is radius of tubes
vec2 pSpiral(vec3 p, float dist) {
	float radius = length(p.xz);
	float angle = atan(p.z, p.x);
	radius -= angle * dist / Tau;
	vec2 q = vec2(radius, p.y);
	pDomrep(q.x, dist);
	return q;
}
/*
float fScene(vec3 p) {
	//float f = fSpiral(p, 1 + obszenchen_radius_rt_float, 1);
	float box_width = .5;
	box_width = mix(box_width, box_width * sqrt(2), 2. * abs(.5 - mod(obszenchen_rot_rt_float, .25) / .25));
	vec2 q = pSpiral(p, box_width * 3);
	pRot(q, Tau * obszenchen_rot_rt_float);
	float f = f2Box(q, box_width);

	f = max(p.y, -f);

	mUnion(f, newMaterialId(0., p));
	return f;
}
*/

float fScene(vec3 p) {
	
	pRotY(p, .125 * Tau);
	pRotX(p, .125 * Tau);
	vec3 loco3 = pMirrorLoco(p, vec3(1.6));
	pTrans(p.y, 1);
	pRotZ(p, obszenchen_rot_rt_float);
	pRotY(p, .125 * Tau);
	pRotX(p, .125 * Tau);
	vec3 loco = pMirrorLoco(p, vec3(.5));
	pRotX(p, obszenchen_rot_rt_float * Tau);
	pRotY(p, (obszenchen_rot_rt_float + .25) * Tau);
	//vec3 loco2 = pMirrorLoco(p, vec3(.5 + .3 * (.5 + .5 * sin(3. * obszenchen_rot_rt_float * Tau))));
	vec3 loco2 = pMirrorLoco(p, vec3(.7));
	float f = fSphere(p, 1);

/*
	float size = 1.;
	vec3 loco = vec3(0);
	for (int i = 0; i < 5; i++) {
		pRotY(p, obszenchen_rot_rt_float * Tau);
		pRotX(p, acos(sqrt(2./3)));
		pRotZ(p, radians(45));
		loco += 1./5. * (1. - size) * pMirrorLoco(p, vec3(size));
		size *= .7;
	}
	float f = fSphere(p, 1);

	mUnion(f, newMaterialId(0., loco));
	*/
	mUnion(f, newMaterialId(0., (loco + loco2 + loco3 + .5) / 4.));
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
	mat.roughness = .5;
	mat.color = materialId.coord * .5 + .5;
	return mat;
}
