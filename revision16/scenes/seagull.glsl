#include "march.glsl"
#include "layer.glsl"
#line 4

float f2Line(vec2 p, float r, vec2 v) {
	float h = saturate(dot(p, v)/ dot(v, v));
	return f2Sphere(p - v*h, r);
}

float fSeagull(vec2 p, float t) {
	pRot(p, Tau / 12);
	pTrans(p.y, (t - 20) * .2);
	float f;
	vec2 seagull_d = vec2(seagull_body_width_rt_float, seagull_body_height_rt_float);
	float t3 = p.y / seagull_d.y;
	pTrans(p.y, -seagull_d.y);
	float t1 = t3 - 1;
	float head = -exp(-square(t1) * 20);
	float middle = exp(-t3 * t3 * 8);
	float t2 = t3 + 1;
	float end = exp(-square(t2) * 15);
	f = f2Line(p, seagull_d.x * (1. + .5 * middle + .6 * end + .4 * head), vec2(0, 2 * seagull_d.y));
	pTrans(p.y, seagull_d.y);

	vec2 p_wings = p;
	vec2 wings_d = vec2(seagull_wings_length_rt_float, seagull_wings_width_rt_float);
	pTrans(p_wings.y, seagull_wings_position_rt_float * seagull_d.y);
	pMirrorTrans(p_wings.x, seagull_d.x);
	pTrans(p_wings.y, -wings_d.y);
	pRot(p_wings, seagull_wings_rotation_rt_float * Tau + sin(t * Tau * .04) * .4);
	pTrans(p_wings.y, wings_d.y);
	pTrans(p_wings.x, wings_d.x);
	float f_wings_inner = f2Box(p_wings, wings_d);
	vec2 wings_outer_d = vec2(seagull_wings_outer_length_rt_float, seagull_wings_width_rt_float);
	vec2 p_wings_outer = p_wings;
	pTrans(p_wings_outer.x, wings_d.x);
	pTrans(p_wings_outer.y, wings_outer_d.y);
	pRot(p_wings_outer, seagull_wings_outer_rotation_rt_float * Tau + sin(t * Tau * .04) * .1);
	pTrans(p_wings_outer.y, -wings_outer_d.y);
	pTrans(p_wings_outer.x, wings_outer_d.x);
	float f_wings_outer = f2Box(p_wings_outer, wings_outer_d);
	vec2 p_wings_outer_cut = p_wings_outer;
	pTrans(p_wings_outer_cut, wings_outer_d);
	float f_wings_outer_cut = f2Plane(p_wings_outer_cut, unitVector(-Tau / 8));
	f_wings_outer = opIntersectSmooth(f_wings_outer, f_wings_outer_cut, .3);
	float f_wings = opUnionSmooth(f_wings_inner, f_wings_outer, .0);
	//float f_wings = min(f_wings_inner, f_wings_outer);

	f = opUnionRounded(f, f_wings, .2);

	return f;
}

float fInner(vec2 p, float t) {
	return fSeagull(p, t);
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
