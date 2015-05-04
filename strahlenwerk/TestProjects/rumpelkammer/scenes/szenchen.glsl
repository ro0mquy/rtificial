#include "march.glsl"
#line 3 "szenchen"

float fScene(vec3 p) {
	float f1 = fBox(p, vec3(8., 4., 2.));
	pRotZ(p, -.2);
	float f2 = fBox(p, vec3(2., 8., 4.));
	float f = opUnionRounded(f2, f1, 1.);

	mUnion(f, Material(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}
