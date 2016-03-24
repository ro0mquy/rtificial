#include "march.glsl"
#include "layer.glsl"
#line 4

float fGuard(vec2 p, float t) {
	return 0;
}

float f2Line(vec2 p, float r, vec2 v) {
	float h = saturate(dot(p, v)/ dot(v, v));
	return f2Sphere(p - v*h, r);
}

float f2Torus(vec2 p, float rBig, float rSmall) {
	 float d = f2Sphere(p, rBig);
	 pMirrorTrans(d, rSmall);
	 return d;
}

float f2TorusBox(vec2 p, float rBig, float rSmall) {
	 float d = f2BoxEdge(p, rBig);
	 pMirrorTrans(d, rSmall);
	 return d;
}

float f2TorusTorus(vec2 p, float rBig, float rMiddle, float rSmall) {
	float d = f2Torus(p, rBig, rMiddle);
	pMirrorTrans(d, rSmall);
	return d;
}

float f2TorusSpheroid(vec2 p, float l, float rBig, float rSmall) {
	float d = f2Spheroid(p, l, rBig);
	pDomrepSingle(d, rSmall * 40);
	pMirrorTrans(d, rSmall);
	return d;
}

float f2TorusTorusSpheroid(vec2 p, float l, float rBig, float rMiddle, float rSmall) {
	float d = f2TorusSpheroid(p, l, rBig, rMiddle);
	pMirrorTrans(d, rSmall);
	return d;
}

float fInner(vec2 p, inout float f_frame, float t) {
	pTrans(p, vec2(cos(t/50. * 1 * Tau), sin(t/50. * 1 * Tau)) * 3);
	pRot(p, t * Tau * .01);
	pMirrorGrid(p, 1.3);
	pRot(p, t * Tau * .01);
	pMirrorLoco(p, vec2(.8));
	return f2TorusSpheroid(p, .25 + .25 * cos(t * Tau * .03), 1 + .3 * sin(t * Tau * .07), .1);
}

float fScene(vec3 p) {
	mUnion(wLayerEffect(p));
	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return ambientColor(normal, -direction, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .5;
	float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
	mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);
	return mat;
}
