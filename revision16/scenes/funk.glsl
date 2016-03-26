#include "march.glsl"
//// start ayer.glsl
#include "lights.glsl"
#line 5
const float id_layer = 101.;

uniform float lay_animation;
uniform vec2  lay_frame_dim;
uniform float lay_frame_thickness;
uniform float lay_last_layer_index;
uniform float lay_layer_dist;
uniform float lay_layer_thickness;
uniform bool lay_rotate_layers;

MatWrap wInner(vec2 p, inout float f_frame, float t);

float fGuard(vec2 p, float t);

MaterialId layerMaterialId(vec2 p, float t) {
	return MaterialId(id_layer, vec3(p, 0.), vec4(t, vec3(0)));
}

MatWrap wLayer(vec2 p, float t) {
	vec2 p_frame = p;
	if (lay_rotate_layers && t < 476 && t >= 140) {
		float rand_for_rot_ist_meine_kugel = .06 * (rand(ivec2(t+3)) * 2 - 1);
		pRot(p_frame, rand_for_rot_ist_meine_kugel);
	}
	float f_frame = max(f2BoxEdge(p.xy, lay_frame_thickness + lay_frame_dim), -f2BoxEdge(p_frame, lay_frame_dim));
	MatWrap w_inner = wInner(p, f_frame, t);
	MatWrap w_frame = MatWrap(f_frame, layerMaterialId(p_frame, t));
	w_frame.m.misc.y = f_frame;
	return mSubtract(w_inner, w_frame);
}

MatWrap wLayerEffect(vec3 p) {
	int i_layer = int(pDomrepInterval(p.z, lay_layer_dist, -lay_last_layer_index, 0));
	float t = i_layer;
	float f = Inf;
	t += (i_layer == 0) ? lay_animation : floor(lay_animation);
	if (i_layer != -int(lay_last_layer_index)) {
		float pz_after = p.z;
		pTrans(pz_after, -(lay_layer_dist - 2. * lay_layer_thickness));
		f = max(fGuard(p.xy, t - 1), pz_after);
	}

	float pz_main = p.z;
	pMirrorTrans(pz_main, lay_layer_thickness);
	MatWrap w_layer_main = wLayer(p.xy, t);
	w_layer_main.f = opIntersectChamfer(w_layer_main.f, pz_main, .05 * .25);

	w_layer_main.f = min(f, w_layer_main.f);
	return w_layer_main;
}
//// end layer.glsl

//#include "march.glsl"
//#include "layer.glsl"
#include "f16.glsl"
#include "parl.glsl"
#line 62

float fGuard(vec2 p, float t) {
	return 0;
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	float duration = 256.;
	float t_rel = t / duration;

	vec2 p_box = p;
	pMirrorLoco(p_box, vec2(0));
	pRot(p_box, Tau * funk_rot_rt_float * t_rel);
	pTrans(p_box, funk_loco_rt_vec2);
	float f_box = f2BoxEdge(p_box, funk_speed_rt_float * t_rel);
	pDomrepInterval(f_box, .5, -2, 0);
	// closing
	float width = funk_width_rt_float + funk_closing_speed_rt_float * pow(smoothstep(funk_closing_start_rt_float, 1., t_rel), funk_closing_pow_rt_float);
	f_box = abs(f_box) - width;

	// opening
	float f_myrahm = f2Quadprism(p, funk_myrahm_speed_rt_float * t_rel);

	float f = f_box;
	f = min(f, -f_myrahm);

	return MatWrap(f, layerMaterialId(p, t));
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
	return mat;
}
