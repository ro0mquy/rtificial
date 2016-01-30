#include "march.glsl"
#include "layer.glsl"
#line 4


float fFlame(vec2 p, float t, float r, float curve, float freq_t, float freq_y, float amp_x, float amp_curve) {
	float y = clamp(p.y, -r, r) / r;
	p.x += sin(t/20 * freq_t * Tau + y * freq_y * Tau) * pow(abs(y * amp_curve), curve) * amp_x * 2 / r;
	return f2Triprism(p, r);
}

float fInner(vec2 p, float t) {
	pTrans(p, 0, -lay_frame_dim.y);
	return fFlame(p, t, 2, 0.9, 2, 0.25, 1, 1);
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
