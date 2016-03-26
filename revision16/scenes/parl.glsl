#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#include "parl.glsl"
#include "materials.glsl"
#line 7

float fGuard(vec2 p, float t) {
	return 0;
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	pTrans(p.x, 256 * .3);
	f_frame = p.y + lay_frame_dim.y;
	vec2 p_f16 = p;
	pTrans(p_f16.x, -48 * .3);
	pF16Parl(p_f16, t);
	MatWrap w_f16 = wF16Air(p_f16, 4);
	w_f16.m.misc.x = t;
	w_f16.m.misc.y = abs(w_f16.f);

	pTrans(p.x, 68);
	float f_outline;
	float f = f2Parl(p, f_outline);

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
	mat.color *= mix(lay_texture_intesity_rt_float, 1., (smoothFbm(.2 * materialId.coord.xy + materialId.misc.x * .1) * .5 + .5));
	if (materialId.id == id_f16) {
		mOutline(mat, materialId, f16_outline_color_rt_color, f16_outline_intensity_rt_float);
	} else if (materialId.id == id_f16_flame) {
		mOutline(mat, materialId, f16_flame_color_rt_color, f16_flame_intensity_rt_float);
	}
	return mat;
}
