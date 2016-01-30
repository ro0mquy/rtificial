#line 2
const float id_layer = 101.;

uniform float lay_animation;
uniform vec2  lay_frame_dim;
uniform float lay_last_layer_index;
uniform float lay_layer_dist;
uniform float lay_layer_thickness;

float fInner(vec2 p, float t);

float fLayer(vec2 p, float t) {
	float f_frame = -f2Box(p.xy, lay_frame_dim);
	float f_inner = fInner(p, t);
	float f_layer = min(f_frame, f_inner);
	return f_layer;
}

MatWrap wLayerEffect(vec3 p) {
	float i_layer = pDomrepInterval(p.z, lay_layer_dist, -lay_last_layer_index, 0);
	float t = i_layer;
	float f = Inf;
	if (i_layer == 0) {
		t += lay_animation;

		// main
		float pz_main = p.z;
		pMirrorTrans(pz_main, lay_layer_thickness);
		float f_layer_main = fLayer(p.xy, t);
		f_layer_main = max(f_layer_main, pz_main);

		// guard after
		float pz_after = p.z;
		pTrans(pz_after, -(lay_layer_dist - 2. * lay_layer_thickness));
		//float f_layer_after = fLayer(p.xy, floor(t - 1));
		//f_layer_after = max(f_layer_after, pz_after);
		float f_layer_after = pz_after;

		f = f_layer_main;
		f = min(f, f_layer_after);
	} else if (i_layer == 1) {
		float t_before = t + lay_animation + 1;
		t += floor(lay_animation);

		// main
		float pz_main = p.z;
		pMirrorTrans(pz_main, lay_layer_thickness);
		float f_layer_main = fLayer(p.xy, t);
		f_layer_main = max(f_layer_main, pz_main);

		// guard before
		float pz_before = p.z;
		pTrans(pz_before, lay_layer_dist - 2. * lay_layer_thickness);
		//float f_layer_before = fLayer(p.xy, t_before);
		//f_layer_before = max(f_layer_before, -pz_before);
		float f_layer_before = -pz_before;

		// guard after
		float pz_after = p.z;
		pTrans(pz_after, -(lay_layer_dist - 2. * lay_layer_thickness));
		//float f_layer_after = fLayer(p.xy, t - 1);
		//f_layer_after = max(f_layer_after, pz_after);
		float f_layer_after = pz_after;

		f = f_layer_main;
		f = min(f, f_layer_before);
		f = min(f, f_layer_after);
	} else if (i_layer == -lay_last_layer_index) {
		t += floor(lay_animation);

		// main
		float pz_main = p.z;
		pMirrorTrans(pz_main, lay_layer_thickness);
		float f_layer_main = fLayer(p.xy, t);
		f_layer_main = max(f_layer_main, pz_main);

		// guard before
		float pz_before = p.z;
		pTrans(pz_before, lay_layer_dist - 2. * lay_layer_thickness);
		//float f_layer_before = fLayer(p.xy, t + 1);
		//f_layer_before = max(f_layer_before, -pz_before);
		float f_layer_before = -pz_before;

		f = f_layer_main;
		f = min(f, f_layer_before);
	} else {
		t += floor(lay_animation);

		// main
		float pz_main = p.z;
		pMirrorTrans(pz_main, lay_layer_thickness);
		float f_layer_main = fLayer(p.xy, t);
		f_layer_main = max(f_layer_main, pz_main);

		// guard before
		float pz_before = p.z;
		pTrans(pz_before, lay_layer_dist - 2. * lay_layer_thickness);
		//float f_layer_before = fLayer(p.xy, t + 1);
		//f_layer_before = max(f_layer_before, -pz_before);
		float f_layer_before = -pz_before;

		// guard after
		float pz_after = p.z;
		pTrans(pz_after, -(lay_layer_dist - 2. * lay_layer_thickness));
		//float f_layer_after = fLayer(p.xy, t - 1);
		//f_layer_after = max(f_layer_after, pz_after);
		float f_layer_after = pz_after;

		f = f_layer_main;
		f = min(f, f_layer_before);
		f = min(f, f_layer_after);
	}
	MaterialId m = MaterialId(id_layer, p, vec4(t, vec3(0)));
	return MatWrap(f, m);
}
