#include "march.glsl"
#line 3

const float id_layer = 1.;

float fInner(vec2 p, float t) {
	pTrans(p, vec2(cos(t/50. * 2 * Tau), sin(t/50. * 2 * Tau)) * 3);
	return f2Box(p, 1);
}

float fLayer(vec2 p, float t) {
	float f_frame = -f2Box(p.xy, lay_frame_dim_rt_vec2);
	float f_inner = fInner(p, t);
	float f_layer = min(f_frame, f_inner);
	return f_layer;
}

MatWrap wLayerEffect(vec3 p) {
	float i_layer = pDomrepInterval(p.z, lay_layer_dist_rt_float, -lay_last_layer_index_rt_float, 0);
	float t = i_layer;
	if (i_layer == 0) {
		t += lay_animation_rt_float;
	} else {
		t += floor(lay_animation_rt_float);
	}
	float f_layer = fLayer(p.xy, t);

	pMirrorTrans(p.z, lay_layer_thickness_rt_float);
	float f = max(f_layer, p.z);
	MaterialId m = MaterialId(id_layer, p, vec4(t, vec3(0)));
	return MatWrap(f, m);
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
