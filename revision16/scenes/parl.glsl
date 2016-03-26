#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#include "parl.glsl"
#include "materials.glsl"
#line 7

float id_f16 = 1;

float fGuard(vec2 p, float t) {
	return 0;
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	pTrans(p.x, 256 * .3);
	f_frame = p.y + lay_frame_dim.y;
	vec2 p_f16 = p;
	pTrans(p_f16.x, -48 * .3);
	pF16Parl(p_f16, t);
	float f_f16 = fF16Air(p_f16, 4);
	MatWrap w_f16 = MatWrap(f_f16, newMaterialId(id_f16, vec3(p_f16, 0)));
	w_f16.m.misc.x = t;
	w_f16.m.misc.y = abs(f_f16);

	pTrans(p.x, 68);
	float f = f2Parl(p);

	MatWrap w_layer = MatWrap(f, layerMaterialId(p, t));
	return mUnion(w_layer, w_f16);
}

float fScene(vec3 p) {
	mUnion(wLayerEffect(p));

	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 front_light = layerLight(origin, marched, direction, hit, normal, material);
	return ambientColor(normal, -direction, material) + material.emission + front_light;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .5;
	float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
	mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);
	if (materialId.id == id_f16) {
		mOutline(mat, materialId, f16_outline_color_rt_color, f16_outline_intensity_rt_float);
	}
	return mat;
}
