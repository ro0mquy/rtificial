#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#include "materials.glsl"
#include "noise.glsl"
#line 7

float id_f16 = 1;
float id_lighthouse = 2;
float id_lighthouse_lamp = 3;
float id_tree = 4;
float id_mountain = 5;

float fGuard(vec2 p, float t) {
	return 0;
}

float saw(float p) {
	float y = p;
	float k = .8;
	for (int i = 0; i < 3; i++) {
		y = sin(p + k*y );
	}
	return y;
}

float mountainNoise(float p) {
	int index = int(floor(p));
	float d = smoothstep(0, 1, fract(p));
	float v0 = rand(ivec2(index, 0));
	float v1 = rand(ivec2(index + 1, 0));
	return mix(saw(p * 2), mix(v0, v1, d) * 2 - 1, .5);
}

float mountainFbm(float p) {
	float result = 0.;
	for (int i = 0; i < 3; i++) {
		result += exp2(-i) * mountainNoise(p);
		p *= 2.;
	}
	return result / 1.75;
}

MatWrap f2Lighthouse(vec2 p) {
	float f_tower = f2ConeCapped(p, 1, .4, 2.5);
	vec2 p_stripes = p;
	float c = 5/3.;
	pTrans(p_stripes.y, c * .25);
	pDomrep(p_stripes.y, c);
	float f_stripes = abs(p_stripes.y) - c * .25;
	f_stripes = max(f_tower, -f_stripes);
	f_tower = min(abs(f_tower) -.05, f_stripes);
	pTrans(p.y, 2.5 + .5);
	pTrans(p.y, .7);
	float f_top = f2ConeCapped(p, .5, 0, .3);
	pTrans(p.y, -.7);
	vec2 p_unmirrored = p;
	pMirrorTrans(p.y, .4);
	f_top = min(f_top, f2Box(p, vec2(.52, .08)));
	p = p_unmirrored;
	pMirrorTrans(p.x, .4);
	f_top = min(f_top, f2Box(p, vec2(.03, .5)));
	p = p_unmirrored;
	pTrans(p.y, -.15);
	float f_lamp = f2BoxRounded(p, vec2(.13, .25), .1);
	f_top = min(f_top, f_lamp);
	f_tower = min(f_top, f_tower);
	MatWrap w_tower = MatWrap(f_tower, newMaterialId(id_lighthouse, vec3(p, 0)));
	w_tower.m.misc.y = abs(f_tower);
	MatWrap w_lamp = MatWrap(f_lamp, newMaterialId(id_lighthouse_lamp, vec3(p, 0)));
	w_tower = mUnion(w_tower, w_lamp);
	return w_tower;
}

MatWrap f2LighthouseWrapper(vec2 p, float t) {
	pTrans(p.y, -lay_frame_dim.y + 1.95);
	//pTrans(p.x, (lay_frame_dim.x + 1) * (1 - 2 * t));
	pTrans(p.x, land_lighthouse_pos_rt_float);
	MatWrap w = f2Lighthouse(p*1.3);
	w.f /= 1.3;
	w.m.misc.y /= 1.3;
	return w;
}

float pSplit(inout vec2 p, vec2 c, float angle, inout float scale) {
	float s2 = sign(p.y);
	c.y *= .9;
	pTrans(p.y, c.y);
	float s = pMirrorTrans(p.y, c.y);
	if (s > 0.) {
		float side = pMirror(p.x);
		pFlip(c.y);
		pTrans(p, c);
		pRot(p, -angle * Tau);
		float decay = tree_size_decay_rt_float;
		scale *= decay;
		p *= decay;
		pTrans(p, -c);
	}
	return s2;
}

float f2Tree(vec2 p, float t) {
	float tb = tree_bounce_rt_float;
	// two jumps
	t = min(1., square(t/tb)) + min(0., (square( (t - .5*(tb+1.)) / (.5*(1.-tb)) ) - 1.) * .7);
	// one and a half jump
	//t = min(1., square(t/tb)) + min(0., (square( (t - 1.) / (1.-tb) ) - 1.) * tree_bounce_h_rt_float);

	float big_scale = mix(2.6, 4., t);
	//pTrans(p.x, -(2 * t - 1) * (lay_frame_dim.x + 2.2));
	pTrans(p.x, 2.2);
	pTrans(p.x, land_tree_pos_rt_float);
	pTrans(p.y, -lay_frame_dim.y - .1);
	float bounding = f2Box(p, vec2(3,4));
	if (bounding > .5) {
		return bounding;
	}
	p *= big_scale;
	vec2 pTree = p;
	vec2 c = vec2(.4, 2.);
	float scale = 1.;
	float angle = tree_branch_angle_rt_float + .04 * sin(t * Pi - Pi*.5);
	float s = pSplit(pTree, c, angle, scale);
	for (int i = 0; i < 6; i++) {
		pSplit(pTree, c, angle, scale);
	}
	float fTree = f2Box(pTree, c);
	fTree *= s;
	fTree /= scale;
	fTree = max(fTree, -p.y + .1);

	return fTree / big_scale;
}

float f2Mountain(vec2 p, float t) {
	vec2 p_mountain = p;
	pTrans(p_mountain.y, -.5);
	//pTrans(p_mountain.x, (lay_frame_dim.x + 4.5) * (1 - 2 * t));
	pTrans(p_mountain.x, land_mountain_pos_rt_float);
	// mountain
	float n = mountainFbm(p_mountain.x) * .5;
	pRot(p_mountain, radians(5));
	float s = pMirror(p_mountain.x);
	pRot(p_mountain, -radians(50));
	return p_mountain.y - n;
}

float f2RandomStuff(vec2 p, float t) {
	if (p.y +3.5 > 1.) {
		return p.y + 3.5;
	}
	vec2 p_busch = p;
	pTrans(p_busch.y, -lay_frame_dim.y + land_busch_pos_x_rt_float);
	pDomrepMirror(p_busch.x, land_busch_domrep_rt_float);
	// erster busch
	float f_busch = f2Sphere(p_busch, land_busch_r_rt_float);
	// zweiter busch
	pTrans(p_busch.x, 3.);
	f_busch = min(f_busch, f2Pentaprism(p_busch.yx, 1.4*land_busch_r_rt_float));
	pTrans(p_busch.x, -.3);
	f_busch = opUnionRounded(f_busch, f2Pentaprism(p_busch, 2.*land_busch_r_rt_float), .1);

	vec2 p_haus = p;
	pTrans(p_haus.x, -10.);
	float c_haus = pDomrepMirror(p_haus.x, land_haus_domrep_rt_float);
	pTrans(p_haus.y, -lay_frame_dim.y);
	// haus base
	vec2 dim_haus = 1.5*vec2(.2, .4);
	float f_haus = f2BoxEdge(p_haus, dim_haus);
	// haus dach
	float tri_r = dim_haus.x / cos(radians(30));
	vec2 p_dach = p_haus;
	pTrans(p_dach.y, dim_haus.y + .5 * tri_r);
	float f_dach = f2Triprism(p_dach, tri_r);
	f_haus = min(f_haus, f_dach);
	// hütte
	vec2 p_hutte = p_haus;
	pTrans(p_hutte.x, land_hutte_pos_rt_float);
	vec2 dim_hutte = 1.5*vec2(.15, .13);
	float f_hutte = f2BoxEdge(p_hutte, dim_hutte);
	f_haus = min(f_haus, f_hutte);

	/*
	// bezier for vogel
	float bez_t = sin(.08 * t) * .5 + .5;
	vec2 bez1 = land_vogel_bez1_rt_vec2;
	vec2 bez2 = land_vogel_bez2_rt_vec2;
	vec2 bez3 = land_vogel_bez3_rt_vec2;
	vec2 bez12 = mix(bez1, bez2, bez_t);
	vec2 bez23 = mix(bez2, bez3, bez_t);
	vec2 bez_vogel = mix(bez12, bez23, bez_t);
	vec2 p_vogel = p;
	pTrans(p_vogel.x, land_vogel_pos_offset_rt_float);
	pTrans(p_vogel, bez_vogel);
	float c_vogel = pDomrepInterval(p_vogel.x, land_vogel_domrep_rt_float, 1., 2.);
	p_vogel.x *= mod(c_vogel, 2.) * 2. - 1.;
	pMirrorTrans(p_vogel.x, .0);
	float flugel_rot = (sin(1.*t) + 1.) / 2. * .1;
	pRot(p_vogel, Tau * flugel_rot);
	pTrans(p_vogel.x, .05);
	float f_vogel = f2Sphere(p_vogel, land_vogel_r_small_rt_float);
	pTrans(p_vogel.y, land_vogel_offset_rt_float);
	f_vogel = max(f_vogel, -f2Sphere(p_vogel, land_vogel_r_big_rt_float));
	// */

	vec2 p_kerbe = p;
	float f_kerbe = Inf;
	//int c_kerbe_big = int(pDomrep(p_kerbe.x, land_kerbe_domrep_big_rt_float));
	//if (c_kerbe_big % 2 == 0) {
		float r_kerbe = land_kerbe_r_rt_float;
		//r_kerbe *= -square(abs(p_kerbe.x) / land_kerbe_domrep_big_rt_float * 2.) + 1.;
		pTrans(p_kerbe.y, -lay_frame_dim.y + r_kerbe);
		int c_kerbe = int(pDomrep(p_kerbe.x, land_kerbe_domrep_rt_float));
		float rand_kerbe = cheapHash(ivec2(c_kerbe, c_kerbe + 28323)) * 2. - 1.;
		pRot(p_kerbe, Tau * (.125 + .05 * rand_kerbe));
		f_kerbe = f2CornerEdge(p_kerbe);
	//}

	float f_rand = f_busch;
	f_rand = min(f_rand, f_haus);
	//f_rand = min(f_rand, f_vogel);
	f_rand = min(f_rand, f_kerbe);
	return f_rand;
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	//float f_tree = f2Tree(p, t);
	//f_frame = max(min(f_frame, f_tree), -max(f_frame, f_tree));
	//return f_frame;
	//return max(f2Tree(p, t);
	float start_width = 13 * lay_frame_dim.y / lay_frame_dim.x;;
	float zoomout_duration = 64;

	vec2 p_f16 = p;
	pF16Landscape(p_f16, t);
	float exhaust = saturate(t/64);
	float wheel_rotation = mix(.056, 1., saturate(t / 40));
	float f_f16 = fF16Ground(p_f16, mix(start_width, 4, saturate(t / zoomout_duration)), exhaust, wheel_rotation);
	MatWrap w_frame = MatWrap(f_frame, layerMaterialId(p, t));
	MatWrap w_f16 = MatWrap(f_f16, newMaterialId(id_f16, vec3(p_f16, 0)));
	w_f16.m.misc.x = t;
	w_f16.m.misc.y = abs(f_f16);
	f_frame = Inf;
	w_f16 = mSubtract(mUnion(w_frame, w_f16), mIntersect(w_frame, w_f16));
	float f = Inf;

	float tree_start = 8 + zoomout_duration;
	float tree_duration = 134;
	float lighthouse_start = tree_start + tree_duration;
	float lighthouse_duration = 96;
	float mountain_start = lighthouse_start + lighthouse_duration;
	float mountain_duration = 76;

	float total_duration = 576;
	float t_rel = t / total_duration;
	float x_offset = .5 * land_a_rt_float * square(t_rel) + land_v0_rt_float * t_rel;

	vec2 p_offset = p;
	pTrans(p_offset.x, -x_offset);

	MatWrap w_layer = MatWrap(f, layerMaterialId(p_offset, t));

	float f_ground = p_offset.y + lay_frame_dim.y;
	w_layer.f = min(w_layer.f, f_ground);

	float f_random_stuff = f2RandomStuff(p_offset, t);
	w_layer.f = min(w_layer.f, f_random_stuff);

	if (t >= lighthouse_start && t < lighthouse_start + lighthouse_duration) {
		MatWrap w_lighthouse = f2LighthouseWrapper(p_offset, (t - lighthouse_start) / lighthouse_duration);
		w_layer = mUnion(w_layer, w_lighthouse);
	}
	if (t >= tree_start && t < tree_start + tree_duration) {
		float f_tree = f2Tree(p_offset, (t - tree_start) / tree_duration);
		w_layer.f = min(w_layer.f, f_tree);
		MatWrap w_tree = MatWrap(f_tree, newMaterialId(id_tree, vec3(p, 0)));
		w_tree.m.misc.y = abs(f_tree);
		w_layer = mUnion(w_layer, w_tree);
	}
	if (t >= mountain_start && t < mountain_start + mountain_duration) {
		float f_mountain = f2Mountain(p_offset, (t - mountain_start) / mountain_duration);
		MatWrap w_mountain = MatWrap(f_mountain, newMaterialId(id_mountain, vec3(p, 0)));
		w_mountain.m.misc.x = t;
		w_mountain.m.misc.y = abs(f_mountain);
		w_layer = mUnion(w_layer, w_mountain);
	}

	w_layer = mUnion(w_layer, w_f16);
	return w_layer;
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
	} else if (materialId.id == id_lighthouse) {
		mOutline(mat, materialId, part_glow_color_rt_color, part_glow_intensity_rt_float);
	} else if (materialId.id == id_lighthouse_lamp) {
		mat.color = land_lighthouse_lamp_color_rt_color;
		mat.emission = vec3(land_lighthouse_lamp_intensity_rt_float * land_lighthouse_lamp_glow_rt_float * 1e3);
		mat.emission *= mat.color;
	} else if (materialId.id == id_tree) {
		mOutline(mat, materialId, land_tree_color_rt_color, land_tree_glow_intensity_rt_float);
	} else if (materialId.id == id_mountain) {
		mOutline(mat, materialId, rain_drops_color_rt_color, land_mountain_glow_intensity_rt_float);
	}
	return mat;
}
