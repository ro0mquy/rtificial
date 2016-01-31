#include "march.glsl"
#include "layer.glsl"
#line 4

float myTorusTwisted(vec3 p, float rBig, float rSmall, float t) {
	float r = length(p.xz);
	float angle = atan(p.z, p.x);

	p.x = r - rBig;
	p.z = 0.;

	pRotZ(p, angle + Tau * part_rot_small_offset_rt_float * t / 50.);

	return fBoxRounded(p, rSmall, part_r_roundedness_rt_float*rSmall);
}

float fInner(vec2 p, float t) {
	pTrans(p, vec2(cos(t/100. * 2 * Tau), sin(t/50. * 2 * Tau)) * 1);
	vec3 p_tor = vec3(p, 0.);
	// good values for rot_big_offset: 0.145; -0.116
	pRotY(p_tor, Tau * part_rot_big_offset_rt_float);
	pRotX(p_tor, Tau * part_rot_big_offset_rt_float);
	float f_tor = myTorusTwisted(p_tor.zxy, part_r_big_rt_float, part_r_small_rt_float, t);
	return f_tor;
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
