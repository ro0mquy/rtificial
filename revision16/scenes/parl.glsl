#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#include "parl.glsl"
#line 5

float fGuard(vec2 p, float t) {
	return 0;
}

float fInner(vec2 p, inout float f_frame, float t) {
	pTrans(p.x, 256 * .3);
	f_frame = p.y + lay_frame_dim.y;
	vec2 p_f16 = p;
	pTrans(p_f16, land_f16_offset_rt_vec2);
	pTrans(p_f16.x, land_f16_motion_rt_float);
	pTrans(p_f16.x, t * .3);
	float f_f16 = fF16Ground(p_f16, 3);

	pTrans(p.x, 68);
	float f = f2Parl(p);
	f = min(f, f_f16);

	return f;
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
