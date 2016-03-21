#line 2
const float id_layer = 101.;

uniform float lay_animation;
uniform vec2  lay_frame_dim;
uniform float lay_frame_thickness;
uniform float lay_last_layer_index;
uniform float lay_layer_dist;
uniform float lay_layer_thickness;

float fInner(vec2 p, inout float f_frame, float t);

float fGuard(vec2 p, float t);

float fLayer(vec2 p, float t) {
	float f_frame = max(f2Box(p.xy, lay_frame_thickness + lay_frame_dim), -f2Box(p.xy, lay_frame_dim));
	float f_inner = fInner(p, f_frame, t);
	float f_layer = min(f_frame, f_inner);
	return f_layer;
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
	float f_layer_main = fLayer(p.xy, t);
	f_layer_main = max(f_layer_main, pz_main);

	f = min(f, f_layer_main);
	MaterialId m = MaterialId(id_layer, p, vec4(t, vec3(0)));
	return MatWrap(f, m);
}
