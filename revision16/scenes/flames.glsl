#include "march.glsl"
#include "layer.glsl"
#line 4


float fFlame(vec2 p, float t, float r, float freq_t, float freq_y, float phase, float amp_x, float curve) {
	pTrans(p.y, .5*r);
	float h = 1.5*r;
	float y = clamp((p.y + .5*r) / h, 0, 1);
	p.x += sin(t/20 * freq_t * Tau  +  y * freq_y * Tau + phase) * pow(y, curve) * amp_x * r * sqrt(3)/2;
	return f2Triprism(p, r);
}

float fInner(vec2 p, inout float f_frame, float t) {
	pTrans(p, 0, -lay_frame_dim.y-.5);
	pTrans(p, vec2(cos(t/50. * 1.5 * Tau) * 0.7, sin(t/50. * 2 * Tau)) * 0.4);

	vec2 p1 = p;
	float f1 = fFlame(p1, t, 3*flame_scale_rt_float, 2*flame_speed_rt_float, 0.3, 0, 0.8, 1.5);

	vec2 p2 = p;
	pTrans(p2, 7, 0);
	float f2 = fFlame(p2, t, 4*flame_scale_rt_float, 3*flame_speed_rt_float, 0.2, 1, 0.8, 1.2);

	vec2 p3 = p;
	pTrans(p3, -6, 0);
	float f3 = fFlame(p3, t, 3.8*flame_scale_rt_float, 2.5*flame_speed_rt_float, 0.4, 1.5, 0.7, 1.8);

	vec2 p4 = p;
	pTrans(p4, -3, 0);
	float f4 = fFlame(p4, t, 4.1*flame_scale_rt_float, 2.8*flame_speed_rt_float, 0.42, 0.3, 0.9, 1.9);

	vec2 p5 = p;
	pTrans(p5, 3.5, 0);
	float f5 = fFlame(p5, t, 2.8*flame_scale_rt_float, 2.2*flame_speed_rt_float, 0.38, 0.28, 0.85, 1.8);

	vec2 p6 = p;
	pTrans(p6, 2.5, 0);
	float f6 = fFlame(p6, t, 1.2*flame_scale_rt_float, 3.2*flame_speed_rt_float, 0.2, 0.28, 0.9, 2);

	vec2 p7 = p;
	pTrans(p7, .7, 0);
	float f7 = fFlame(p7, t, 1.5*flame_scale_rt_float, 2.2*flame_speed_rt_float, 0.3, 0.342, 0.85, 2);

	vec2 p8 = p;
	pTrans(p8, -3.8, 0);
	float f8 = fFlame(p8, t, 2.3*flame_scale_rt_float, 1.6*flame_speed_rt_float, 0.4, 1.2, 0.8, 2.1);

	vec2 p9 = p;
	pTrans(p9, -5.2, 0);
	float f9 = fFlame(p9, t, 1.4*flame_scale_rt_float, 1.9*flame_speed_rt_float, 0.6, 0.8, 0.75, 2.3);

	return max(-min(min(f6, f7), min(f8, f9)), min(min(min(f1, f2), min(f3, f4)), f5));
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
