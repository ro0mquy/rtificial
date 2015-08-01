#include "march.glsl"
#line 3

const float id_berge_bg1 = 0.;
const float id_berge_bg2 = 1.;
const float id_berge_hugel = 2.;
const float id_berge_smooth = 3.;
const float id_pyr_spitze = 5.;
const float id_pyr_baum = 6.;
const float id_pyr_wand = 7.;
const float id_pyr_wand_top = 8.;
const float id_pyr_wand_bottom = 9.;
const float id_pyr_ceiling = 10.;
const float id_pyr_fenster = 11.;
const float id_pyr_fenster_rahmen = 12.;
const float id_pyr_fenster_saule = 13.;
const float id_pyr_fenster_saule_stairs = 14.;
const float id_pyr_wandbox = 15.;
const float id_hutten_zaun = 16.;
const float id_weg_saule = 17.;
const float id_zaun = 18.;
const float id_zaun_pfosten = 19.;
const float id_zaun_gelander = 20.;

float fSaule(vec3 p) {
	// bounding
	float f_bounding = fCylinder(p, rlx_saule_bounding_r_rt_float, rlx_saule_bounding_h_rt_float);
	if (f_bounding > 1.) {
		return f_bounding;
	}

	vec3 p_stamm = p;
	float f_stamm = fCylinder(p_stamm, rlx_saule_stamm_r_rt_float, rlx_saule_stamm_h_rt_float);

	vec3 p_top = p;
	p_top.y -= rlx_saule_stamm_h_rt_float;
	pMirrorTrans(p_top, vec3(.1));
	pMirrorAtPlane(p_top, normalize(rlx_saulen_top_mirror_dir_rt_vec3), length(rlx_saulen_top_mirror_dir_rt_vec3));
	float f_top = fBoxEdge(p_top, rlx_saulen_top_dim_rt_vec3);

	vec3 p_cut = p;
	p_cut.y -= rlx_saule_stamm_h_rt_float;
	pRotX(p_cut, Tau * rlx_saule_cut_angle_rt_float);
	pDomrepMirror(p_cut.y, .4);
	//pMirrorLoco(p_cut.zyx, vec3(.5));
	//pMirrorAtPlane(p_cut, unitVector(1., 2.), .2);
	//pMirrorTrans(p_cut, vec3(.1));
	float f_cut = abs(p_cut.y) - .1;

	float f_saule = f_stamm;
	f_saule = min(f_saule, f_top);
	f_saule = max(f_saule, -f_cut);
	return f_saule;
}

float fCylWald(vec3 p) {
	vec3 p1 = p;
	pRotY(p1, Tau * rlx_wald_rotation1_rt_float);
	pDomrep(p1.xz, rlx_wald_spacing1_rt_vec2);
	float f1 = fCylinder(p1, rlx_wald_r1_rt_float, rlx_wald_h1_rt_float);

	vec3 p2 = p;
	pRotY(p2, Tau * rlx_wald_rotation2_rt_float);
	pDomrep(p2.xz, rlx_wald_spacing2_rt_vec2);
	float f2 = fCylinder(p2, rlx_wald_r2_rt_float, rlx_wald_h2_rt_float);

	vec3 p3 = p;
	pRotY(p3, Tau * rlx_wald_rotation3_rt_float);
	pDomrep(p3.xz, rlx_wald_spacing3_rt_vec2);
	float f3 = fCylinder(p3, rlx_wald_r3_rt_float, rlx_wald_h3_rt_float);

	vec3 p4 = p;
	pRotY(p4, Tau * rlx_wald_rotation4_rt_float);
	pDomrep(p4.xz, rlx_wald_spacing4_rt_vec2);
	float f4 = fCylinder(p4, rlx_wald_r4_rt_float, rlx_wald_h4_rt_float);

	float f_cylwald = min4(f1, f2, f3, f4);
	return max(f_cylwald, -f2Sphere(p.xz, rlx_wald_free_r_rt_float));

}

MatWrap wPyramidStep(vec3 p, float pyramid_sidelen, float pyramid_height) {
	vec3 p_wand = p;
	pMirrorLoco(p_wand.zx, vec2(pyramid_sidelen));
	p_wand.x += pyramid_sidelen;

	// untere stufe der wand
	float f_wand_bottom = p_wand.z;
	MatWrap w_wand_bottom = MatWrap(f_wand_bottom, newMaterialId(id_pyr_wand_bottom, p_wand));

	// obere stufe der wand
	float f_wand_top = p_wand.z - rlx_pyr_wand_inset_rt_float;
	MatWrap w_wand_top = MatWrap(f_wand_top, newMaterialId(id_pyr_wand_top, p_wand));

	// decke
	float f_ceiling = p_wand.y - pyramid_height;
	MatWrap w_ceiling = MatWrap(f_ceiling, newMaterialId(id_pyr_ceiling, p_wand));

	// main wand part
	p_wand.z -= rlx_pyr_wand_inset_rt_float * .5;
	p_wand.y -= pyramid_height * .5;
	vec3 p_wand_sub = p_wand;
	pRotX(p_wand_sub, atan(rlx_pyr_wand_inset_rt_float / (rlx_pyr_wand_edge_height_rt_float * pyramid_height)));
	float f_wand_simple = p_wand_sub.z;
	float f_wand = f_wand_simple;
	MatWrap w_wand = MatWrap(f_wand_simple, newMaterialId(id_pyr_wand, p_wand_sub));


	// fenster insets
	vec3 p_fenster = p_wand;
	float cell_fenster = pDomrep(p_fenster.x, rlx_pyr_fenster_spacing_rt_float);
	float f_fenster = f2BoxEdge(p_fenster.xy, rlx_pyr_fenster_dim_rt_vec2);
	MatWrap w_fenster = MatWrap(f_fenster, newMaterialId(id_pyr_fenster, p_fenster));

	// fenster rahmen
	vec2 q_fenster_rahmen = vec2(f_wand_simple, f_fenster);
	float f_fenster_rahmen = f2BoxEdge(q_fenster_rahmen, rlx_pyr_fenster_rahmen_rt_vec2);
	MatWrap w_fenster_rahmen = MatWrap(f_fenster_rahmen, newMaterialId(id_pyr_fenster_rahmen, vec3(q_fenster_rahmen, 0.)));

	// saulen in fenstern
	vec3 p_fenster_saule = p_fenster;
	p_fenster_saule.z -= rlx_pyr_fenster_saule_offset_rt_float;
	float f_fenster_saule = fCylinder(p_fenster_saule, rlx_pyr_fenster_saule_r_rt_float, rlx_pyr_fenster_dim_rt_vec2.y);
	MatWrap w_fenster_saule = MatWrap(f_fenster_saule, newMaterialId(id_pyr_fenster_saule, p_fenster_saule));

	// rechteck zu verzierung
	vec3 p_wandbox = vec3(p_fenster.x, p_wand_sub.yz);
	pMirrorTrans(p_wandbox.x, .5 * rlx_pyr_fenster_spacing_rt_float);
	p_wandbox.y -= rlx_pyr_wandbox_pos_y_rt_float;
	float f_wandbox = fBoxEdge(p_wandbox, rlx_pyr_wandbox_dim_rt_vec3);
	MatWrap w_wandbox = MatWrap(f_wandbox, newMaterialId(id_pyr_wandbox, p_wandbox));


	MatWrap w_pyr = w_wand;
	w_pyr = mIntersect(w_pyr, w_wand_bottom);
	w_pyr = mUnion(w_pyr, w_wand_top);
	w_pyr = mSubtract(w_pyr, w_fenster);
	w_pyr = mUnionStairs(w_pyr, w_fenster_saule, rlx_pyr_fenster_saule_smooth_rt_float, 3., id_pyr_fenster_saule_stairs);
	w_pyr = mUnion(w_pyr, w_fenster_rahmen);
	w_pyr = mUnion(w_pyr, w_wandbox);
	w_pyr = mIntersect(w_pyr, w_ceiling);

	return w_pyr;

	/*
	float f = f_wand;
	f = max(f, f_wand_bottom);
	f = min(f, f_wand_top);
	f = max(f, -f_fenster);
	f = opUnionStairs(f, f_fenster_saule, rlx_pyr_fenster_saule_smooth_rt_float, 3.);
	f = min(f, f_fenster_rahmen);
	f = min(f, f_wandbox);
	f = max(f, f_ceiling);


	return f;
	// */
}

MatWrap wBasis(vec3 p) {
	// main tempel step
	p.y -= rlx_hugel_dim_rt_vec3.y;
	p.xz -= rlx_hugel_pos_rt_vec2;
	p.xz -= rlx_basis_offset_rt_vec2;
	float height = rlx_basis_height_rt_float / 3.;
	float f_bounding = fBoxEdge(vec3(p.x, p.y - height, p.z), vec3(rlx_basis_width_rt_float, height, rlx_basis_width_rt_float));
	if (f_bounding > 1.) {
		return MatWrap(f_bounding, newMaterialId(id_pyr_wand, p)); // whatever
	}
	MatWrap w_tempel = wPyramidStep(p, rlx_basis_width_rt_float, height);

	// pyramiden dach
	p.y -= height;
	float f_spitze = fPyramid(p, height, Tau * rlx_basis_spitze_angle_rt_float);
	MatWrap w_spitze = MatWrap(f_spitze, newMaterialId(id_pyr_spitze, p));

	// baum saulen auf dach
	vec3 p_baum = p;
	p_baum.y -= rlx_basis_baum_pos_y_rt_float;
	pMirrorLoco(p_baum.xz, vec2(rlx_basis_baum_pos_rt_float));
	float baum_offset = .5 * rlx_basis_baum_pos_rt_float * rlx_basis_baum_offset_rt_float;
	p_baum.z -= -baum_offset;
	pMirrorTrans(p_baum.z, baum_offset);
	float f_baum = fSaule(p_baum);
	MatWrap w_baum = MatWrap(f_baum, newMaterialId(id_pyr_baum, p_baum));

	MatWrap w_basis = mUnion(w_baum, w_spitze);
	w_basis.f = max(w_basis.f, -p.y);
	w_basis = mUnion(w_basis, w_tempel);
	return w_basis;
}

MatWrap wWeg(vec3 p) {
	p.xz -= rlx_hugel_pos_rt_vec2;

	//pMirrorLoco(p.xz, vec2(0.));
	p.x -= rlx_weg_offset_first_rt_float;
	pMirrorTrans(p.z, rlx_weg_width_rt_float);
	pDomrepSingle(p.x, rlx_weg_spacing_rt_float);
	float f_saule = fSaule(p);
	MatWrap w_saule = MatWrap(f_saule, newMaterialId(id_weg_saule, p));
	return w_saule;
}

MatWrap wBerge(vec3 p) {
	// background berg 1
	vec3 p_bg = p;
	p_bg -= rlx_berg_pos_rt_vec3;
	pMirrorAtPlane(p_bg, normalize(rlx_berg_pos_rt_vec3), length(rlx_berg_pos_rt_vec3));
	float f_bg = fPyramid(p_bg, rlx_berg_h_rt_float, Tau * rlx_berg_angle_rt_float);
	MatWrap w_bg = MatWrap(f_bg, newMaterialId(id_berge_bg1, p_bg));

	// background berg 2
	vec3 p_bg2 = p;
	p_bg2 -= rlx_berg2_pos_rt_vec3;
	pMirrorAtPlane(p_bg2, normalize(rlx_berg2_pos_rt_vec3), length(rlx_berg2_pos_rt_vec3));
	float f_bg2 = fPyramid(p_bg2, rlx_berg2_h_rt_float, Tau * rlx_berg2_angle_rt_float);
	MatWrap w_bg2 = MatWrap(f_bg2, newMaterialId(id_berge_bg2, p_bg2));
	w_bg = mUnion(w_bg, w_bg2);

	// tempel hugel & boden
	vec3 p_hugel = p;
	p_hugel.xz -= rlx_hugel_pos_rt_vec2;
	float q_hugel = f2BoxRounded(p_hugel.xz, rlx_hugel_dim_rt_vec3.xz, rlx_hugel_corner_r_rt_float);
	float f_hugel = p_hugel.y - rlx_hugel_dim_rt_vec3.y * (1. - smoothstep(0., rlx_hugel_widht_rt_float, q_hugel));
	MatWrap w_hugel = MatWrap(f_hugel, newMaterialId(id_berge_hugel, vec3(p_hugel.xz, q_hugel)));

	MatWrap w_berge = mUnionSmooth(w_bg, w_hugel, rlx_hugel_smooth_rt_float, id_berge_smooth);
	return w_berge;
}

MatWrap wZaun(vec3 p) {
	p.y -= rlx_hugel_dim_rt_vec3.y;
	p.xz -= rlx_hugel_pos_rt_vec2;

	vec3 p_zaun = p;
	p_zaun.x -= rlx_hugel_dim_rt_vec3.x * rlx_zaun_pos_x_bias_rt_float;
	pMirrorTrans(p_zaun.z, rlx_hugel_dim_rt_vec3.z);
	pMirrorAtPlane(p_zaun, normalize(vec3(-1., 0., 1.)), 0.);
	float zaun_length = rlx_hugel_dim_rt_vec3.x * .5 * rlx_zaun_length_rt_float;
	p_zaun.x -= -zaun_length;
	vec3 p_gelander = p_zaun;
	pMirrorTrans(p_zaun.x, zaun_length);

	// zaun box
	vec2 q_zaun = p_zaun.yz - rlx_zaun_dim_rt_vec2;
	float f_zaun = max3(p_zaun.x, q_zaun.x, abs(q_zaun.y));
	MatWrap w_zaun = MatWrap(f_zaun, newMaterialId(id_zaun, p_zaun));

	// eckpfosten
	vec3 p_pfosten = p_zaun;
	float f_pfosten = fPyramid(p_pfosten, 1./tan(Tau * rlx_zaun_pfosten_angle_rt_float) * rlx_zaun_pfosten_dim_rt_vec3.x, Tau * rlx_zaun_pfosten_angle_rt_float);
	f_pfosten = max(f_pfosten, p_pfosten.y - rlx_zaun_pfosten_dim_rt_vec3.y);
	MatWrap w_pfosten = MatWrap(f_pfosten, newMaterialId(id_zaun_pfosten, p_pfosten));

	// gelander am berg
	p_gelander.x -= -zaun_length;
	pRotX(p_gelander, Tau * rlx_zaun_gelander_angle_rt_float);
	p_gelander.z -= rlx_zaun_gelander_dim_rt_vec3.z;
	float f_gelander = fBoxEdge(p_gelander, rlx_zaun_gelander_dim_rt_vec3);
	MatWrap w_gelander = MatWrap(f_gelander, newMaterialId(id_zaun_gelander, p_gelander));

	w_zaun = mUnion(w_zaun, w_pfosten);
	w_zaun = mUnion(w_zaun, w_gelander);
	return w_zaun;
}

MatWrap wHutten(vec3 p) {
	p.xz -= rlx_hugel_pos_rt_vec2;

	vec3 p_hutte = p;
	pMirrorLoco(p_hutte.xz, vec2(0.));
	p_hutte.x -= rlx_hutte_pos_rt_vec2.x;
	p_hutte.z -= .5 * sum(rlx_hutte_pos_rt_vec2);
	float cell_hutte = pMirror(p_hutte.z);
	p_hutte.z -= .5 * (rlx_hutte_pos_rt_vec2.x - rlx_hutte_pos_rt_vec2.y);

	float hutte_width = cell_hutte > 0. ? rlx_hutte_outer_width_rt_float : rlx_hutte_inner_width_rt_float;
	float hutte_height = cell_hutte > 0. ? rlx_hutte_outer_height_rt_float : rlx_hutte_inner_height_rt_float;
	float f_bounding = fBoxEdge(p_hutte, vec3(hutte_width, hutte_height, hutte_width));
	MatWrap w_hutte = MatWrap(f_bounding, newMaterialId(id_pyr_wand, p_hutte)); // whatever
	if (f_bounding < 1.) {
		w_hutte = wPyramidStep(p_hutte, hutte_width, hutte_height);
	}

	vec3 p_zaun = p_hutte;
	pMirrorTrans(p_zaun.z, rlx_hutte_pos_rt_vec2.y * .5);
	float f_zaun = fBoxEdge(p_zaun, rlx_hutte_zaun_dim_rt_vec3);
	MatWrap w_zaun = MatWrap(f_zaun, newMaterialId(id_hutten_zaun, p_zaun));

	return mUnion(w_hutte, w_zaun);
}

float fScene(vec3 p) {
	mUnion(wBasis(p));
	mUnion(wWeg(p));
	mUnion(wBerge(p));
	mUnion(wZaun(p));
	mUnion(wHutten(p));

	/*
	float f = Inf;
	f = min(f, fCylWald(p));
	f = min(f, fBasis(p));
	//f = min(f, fVorbau(p));
	//f = min(f, fFluegel(p));
	f = min(f, fWeg(p));
	//f = min(f, fFeld(p));
	//f = min(f, fTurm(p));
	f = min(f, fBerge(p));
	f = min(f, fZaun(p));
	f = min(f, fHutten(p));
	// */

	//mUnion(f, newMaterialId(0., p));
	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;

	vec3 faded_barn = vec3(165,87,69)/255.;
	vec3 end_is_near = vec3(100,90,81)/255.;
	vec3 lightness = vec3(185,164,136)/255.;
	vec3 like_clockwork = vec3(199,190,174)/255.;
	vec3 nolonger_smells = vec3(133,131,119)/255.;

	mat.color = like_clockwork;

	if (materialId.id == id_pyr_spitze || materialId.id == id_pyr_wand) {
		mat.color = lightness;
	} else if (materialId.id == id_pyr_wand_top || materialId.id == id_pyr_wand_bottom) {
		mat.color = end_is_near;
	} else if (materialId.id == id_pyr_wandbox) {
		mat.color = faded_barn;
	} else if (materialId.id == id_hutten_zaun) {
		mat.color = nolonger_smells;
	} else if (materialId.id == id_pyr_fenster) {
		mat.color = nolonger_smells;
	} else if (materialId.id == id_pyr_fenster_rahmen) {
		mat.color = end_is_near;
	} else if (materialId.id == id_pyr_fenster_saule || materialId.id == id_pyr_fenster_saule_stairs) {
		mat.color = rlx_col_pyr_saule_rt_color;
	} else if (materialId.id == id_pyr_baum) {
		mat.color = end_is_near;
	} else if (materialId.id == id_weg_saule) {
		mat.color = rlx_col_saule_rt_color;
	}


	return mat;
}
