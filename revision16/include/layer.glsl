#include "lights.glsl"
#line 3
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
	if (lay_rotate_layers && t < 276) {
		float rand_for_rot_ist_meine_kugel = .1 * (sin(t+3));
		pRot(p_frame, rand_for_rot_ist_meine_kugel);
	}
	float f_frame = max(f2BoxEdge(p.xy, lay_frame_thickness + lay_frame_dim), -f2BoxEdge(p_frame, lay_frame_dim));
	MatWrap w_inner = wInner(p, f_frame, t);
	MatWrap w_frame = MatWrap(f_frame, layerMaterialId(p_frame, t));
	w_frame.m.misc.y = f_frame;
	w_inner.m.coord.xy = p_frame;
	return mUnion(w_frame, w_inner);
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
	if (lay_disable_chamfer_rt_bool) {
		w_layer_main.f = max(w_layer_main.f, pz_main);
	} else {
		w_layer_main.f = opIntersectChamfer(w_layer_main.f, pz_main, .05 * .25);
	}

	w_layer_main.f = min(f, w_layer_main.f);
	w_layer_main.m.misc[2] = i_layer;
	return w_layer_main;
}
