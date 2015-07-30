#include "march.glsl"
#line 3

float fSaule(vec3 p) {
	vec3 p_stamm = p;
	float f_stamm = fCylinder(p_stamm, rlx_saule_stamm_r_rt_float, rlx_saule_stamm_h_rt_float);

	vec3 p_top = p;
	p_top.y -= rlx_saule_stamm_h_rt_float;
	pMirrorTrans(p_top, vec3(.1));
	pMirrorAtPlane(p_top, normalize(rlx_saulen_top_mirror_dir_rt_vec3), length(rlx_saulen_top_mirror_dir_rt_vec3));
	float f_top = fBox(p_top, rlx_saulen_top_dim_rt_vec3);

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

	/*
	p.y -= rlx_saule_knick_height_rt_float;
	pMirror(p.y);
	pRotX(p, Tau * rlx_saule_rotation_rt_float);
	float f_saule = fBox(p, rlx_saule_dim_rt_vec3);
	return f_saule;
	// */
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

float fPyramidStep(vec3 p, float pyramid_sidelen, float pyramid_height) {
	vec3 p_wand = p;
	pMirrorLoco(p_wand.zx, vec2(pyramid_sidelen));
	p_wand.x += pyramid_sidelen;

	float f_wand_bottom = p_wand.z;
	float f_wand_top = p_wand.z - rlx_pyr_wand_inset_rt_float;
	float f_ceiling = p_wand.y - pyramid_height;

	p_wand.z -= rlx_pyr_wand_inset_rt_float * .5;
	p_wand.y -= pyramid_height * .5;
	vec3 p_wand_sub = p_wand;
	pRotX(p_wand_sub, atan(rlx_pyr_wand_inset_rt_float / (rlx_pyr_wand_edge_height_rt_float * pyramid_height)));
	float f_wand_simple = p_wand_sub.z;
	/*
	pRotZ(p_wand_sub, Tau * rlx_pyr_wand_sub_schief_rt_float);
	pDomrepMirror(p_wand_sub.x, rlx_pyr_wand_sub_spacing_rt_float);
	pRotY(p_wand_sub, Tau * rlx_pyr_wand_sub_angle_rt_float);
	float f_wand = p_wand_sub.z;
	// */
	float f_wand = f_wand_simple;


	vec3 p_fenster = p_wand;
	float cell_fenster = pDomrep(p_fenster.x, rlx_pyr_fenster_spacing_rt_float);
	float f_fenster = f2BoxEdge(p_fenster.xy, rlx_pyr_fenster_dim_rt_vec2);
	vec2 q_fenster_rahmen = vec2(f_wand_simple, f_fenster);
	float f_fenster_rahmen = f2Box(q_fenster_rahmen, rlx_pyr_fenster_rahmen_rt_vec2);

	vec3 p_fenster_saule = p_fenster;
	p_fenster_saule.z -= rlx_pyr_fenster_saule_offset_rt_float;
	float f_fenster_saule = fCylinder(p_fenster_saule, rlx_pyr_fenster_saule_r_rt_float, rlx_pyr_fenster_dim_rt_vec2.y);

	vec3 p_wandbox = vec3(p_fenster.x, p_wand_sub.yz);
	pMirrorTrans(p_wandbox.x, .5 * rlx_pyr_fenster_spacing_rt_float);
	p_wandbox.y -= rlx_pyr_wandbox_pos_y_rt_float;
	float f_wandbox = fBox(p_wandbox, rlx_pyr_wandbox_dim_rt_vec3);


	float f = f_wand;
	f = max(f, f_wand_bottom);
	f = min(f, f_wand_top);
	f = max(f, -f_fenster);
	f = opUnionStairs(f, f_fenster_saule, rlx_pyr_fenster_saule_smooth_rt_float, 3.);
	f = min(f, f_fenster_rahmen);
	f = min(f, f_wandbox);
	f = max(f, f_ceiling);


	return f;
}

float fBasis(vec3 p) {
	p.y -= rlx_hugel_dim_rt_vec3.y;
	p.xz -= rlx_hugel_pos_rt_vec2;
	p.xz -= rlx_basis_offset_rt_vec2;
	float height = rlx_basis_height_rt_float / 3.;
	float f1 = fPyramidStep(p, rlx_basis_width_rt_float, height);

	//*
	p.y -= height;
	float f_spitze = fPyramid(p, height, Tau * rlx_basis_spitze_angle_rt_float);

	vec3 p_saule = p;
	p_saule.y -= rlx_basis_saule_pos_y_rt_float;
	pMirrorLoco(p_saule.xz, vec2(rlx_basis_saule_pos_rt_float));
	float saule_offset = .5 * rlx_basis_saule_pos_rt_float * rlx_basis_saule_offset_rt_float;
	p_saule.z -= -saule_offset;
	pMirrorTrans(p_saule.z, saule_offset);
	float f_saule = fSaule(p_saule);
	f_spitze = min(f_spitze, f_saule);

	f_spitze = max(f_spitze, -p.y);
	return min(f1, f_spitze);
	// */

	/*
	vec3 p_dach = p;
	p_dach.y -= height;
	p_dach.x -= rlx_basis_dach_width_rt_float;
	pMirrorTrans(p_dach.z, rlx_basis_dach_width_rt_float);
	float f_bigblock = fBox(p_dach, rlx_basis_bigblock_rt_vec3);

	vec3 p_bogen = p_dach;
	p_bogen.z -= rlx_basis_bogen_pos_z_rt_float;
	p_bogen.y -= rlx_basis_bogen_height_rt_float;
	float f_bogen = fCylinder(p_bogen, rlx_basis_bogen_r_rt_float, rlx_basis_bogen_height_rt_float);
	p_bogen.y -= rlx_basis_bogen_height_rt_float;
	p_bogen.z -= rlx_basis_bogen_offset_rt_float;
	float f_bogen_dach = f2PlaneAngle(p_bogen.zy, Tau * rlx_basis_bogen_angle_rt_float);
	f_bogen_dach = max3(f_bogen_dach, -p_bogen.y, abs(p_bogen.x) - rlx_basis_bogen_depth_rt_float);
	f_bogen = min(f_bogen, f_bogen_dach);

	float f_dach = f_bigblock;
	f_dach = min(f_dach, f_bogen);

	return min(f1, f_dach);
	// */

	/*
	p.y -= height;
	float f2 = fPyramidStep(p, 2./3. * rlx_basis_width_rt_float, height);
	p.y -= height;
	float f3 = fPyramidStep(p, 1./3. * rlx_basis_width_rt_float, height);
	return min3(f1, f2, f3);
	// */
}

float fVorbau(vec3 p) {
	float height = rlx_vorbau_height_rt_float * .5;
	vec3 p_vorbau = p;
	pTrans(p_vorbau.xz, rlx_vorbau_pos_rt_vec2);

	vec3 p1 = p_vorbau;
	float f1 = fPyramidStep(p1, rlx_vorbau_width_rt_float, height);

	vec3 p2 = p_vorbau;
	p2.y -= height;
	float f2 = fPyramidStep(p2, .5 * rlx_vorbau_width_rt_float, height);


	vec3 p_klein = p;
	pMirrorTrans(p_klein.x, rlx_vorbau_klein_offset_rt_vec2.x);
	float cell_klein = pMirrorTrans(p_klein.z, rlx_vorbau_pos_rt_vec2.y + rlx_vorbau_klein_offset_rt_vec2.y);
	float klein_width = rlx_vorbau_klein_width_rt_float;
	if (cell_klein < 0.) {
		p_klein.xz -= rlx_vorbau_klein_smaller_offset_rt_vec2;
		klein_width *= rlx_vorbau_klein_smaller_rt_float;
	}
	float f_klein = fPyramidStep(p_klein, klein_width, rlx_vorbau_klein_height_rt_float);


	return min3(f1, f2, f_klein);
}

float fFluegel(vec3 p) {
	pMirrorTrans(p.x, rlx_fluegel_pos_rt_vec2.x);
	p.z -= rlx_fluegel_pos_rt_vec2.y;
	//pDomrepInterval(p.z, );
	return 0;
}

float fWeg(vec3 p) {
	p.xz -= rlx_hugel_pos_rt_vec2;

	//pMirrorLoco(p.xz, vec2(0.));
	p.x -= rlx_weg_offset_first_rt_float;
	pMirrorTrans(p.z, rlx_weg_width_rt_float);
	pDomrepSingle(p.x, rlx_weg_spacing_rt_float);
	float f_saule = fSaule(p);
	return f_saule;
}

float fFeld(vec3 p) {
	pMirrorLoco(p.xz, vec2(0.));
	p.z -= rlx_feld_spacing_rt_vec2.y * .5;
	p.x -= rlx_feld_offset_first_rt_float;
	pDomrepInterval(p.xz, rlx_feld_spacing_rt_vec2, vec2(0.), rlx_feld_interval_end_rt_vec2);
	float f_saule = fSaule(p);
	return f_saule;
}

float fTurm(vec3 p) {
	pMirrorTrans(p.xz, rlx_turm_pos_rt_vec2);
	float f_basis = f2Sphere(p.xz, rlx_turm_r_rt_float);
	p.y -= rlx_turm_h_rt_float;
	float f_spitze = fConeAngle(p, Tau * rlx_turm_angle_rt_float);
	float f_turm = max(f_basis, f_spitze);
	return f_turm;
}

float fBerge(vec3 p) {
	vec3 p_bg = p;
	p_bg -= rlx_berg_pos_rt_vec3;
	pMirrorAtPlane(p_bg, normalize(rlx_berg_pos_rt_vec3), length(rlx_berg_pos_rt_vec3));
	float f_bg = fPyramid(p_bg, rlx_berg_h_rt_float, Tau * rlx_berg_angle_rt_float);

	vec3 p_bg2 = p;
	p_bg2 -= rlx_berg2_pos_rt_vec3;
	pMirrorAtPlane(p_bg2, normalize(rlx_berg2_pos_rt_vec3), length(rlx_berg2_pos_rt_vec3));
	float f_bg2 = fPyramid(p_bg2, rlx_berg2_h_rt_float, Tau * rlx_berg2_angle_rt_float);
	f_bg = min(f_bg, f_bg2);

	vec3 p_hugel = p;
	p_hugel.xz -= rlx_hugel_pos_rt_vec2;
	float q_hugel = f2BoxRounded(p_hugel.xz, rlx_hugel_dim_rt_vec3.xz, rlx_hugel_corner_r_rt_float);
	float f_hugel = p_hugel.y - rlx_hugel_dim_rt_vec3.y * (1. - smoothstep(0., rlx_hugel_widht_rt_float, q_hugel));

	float f_berge = opUnionSmooth(f_bg, f_hugel, rlx_hugel_smooth_rt_float);
	return f_berge;
}

float fZaun(vec3 p) {
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

	vec2 q_zaun = p_zaun.yz - rlx_zaun_dim_rt_vec2;
	float f_zaun = max3(p_zaun.x, q_zaun.x, abs(q_zaun.y));

	vec3 p_pfosten = p_zaun;
	/*
	float f_pfosten = fBox(p_pfosten, rlx_zaun_pfosten_dim_rt_vec3);
	p_pfosten.y -= rlx_zaun_pfosten_dim_rt_vec3.y;
	float f_spitze = fPyramid(p_pfosten, rlx_zaun_spitze_h_rt_float, atan(rlx_zaun_pfosten_dim_rt_vec3.x / rlx_zaun_spitze_h_rt_float));
	f_spitze = max(f_spitze, -p_pfosten.y);
	f_pfosten = min(f_pfosten, f_spitze);
	// */
	float f_pfosten = fPyramid(p_pfosten, 1./tan(Tau * rlx_zaun_pfosten_angle_rt_float) * rlx_zaun_pfosten_dim_rt_vec3.x, Tau * rlx_zaun_pfosten_angle_rt_float);
	f_pfosten = max(f_pfosten, p_pfosten.y - rlx_zaun_pfosten_dim_rt_vec3.y);

	p_gelander.x -= -zaun_length;
	pRotX(p_gelander, Tau * rlx_zaun_gelander_angle_rt_float);
	p_gelander.z -= rlx_zaun_gelander_dim_rt_vec3.z;
	float f_gelander = fBox(p_gelander, rlx_zaun_gelander_dim_rt_vec3);

	return min3(f_zaun, f_pfosten, f_gelander);
}

float fHutten(vec3 p) {
	p.xz -= rlx_hugel_pos_rt_vec2;

	vec3 p_hutte = p;
	pMirrorLoco(p_hutte.xz, vec2(0.));
	p_hutte.x -= rlx_hutte_pos_rt_vec2.x;
	p_hutte.z -= .5 * sum(rlx_hutte_pos_rt_vec2);
	float cell_hutte = pMirror(p_hutte.z);
	p_hutte.z -= .5 * (rlx_hutte_pos_rt_vec2.x - rlx_hutte_pos_rt_vec2.y);

	float hutte_width = cell_hutte > 0. ? rlx_hutte_outer_width_rt_float : rlx_hutte_inner_width_rt_float;
	float hutte_height = cell_hutte > 0. ? rlx_hutte_outer_height_rt_float : rlx_hutte_inner_height_rt_float;
	float f_hutte = fPyramidStep(p_hutte, hutte_width, hutte_height);

	vec3 p_zaun = p_hutte;
	pMirrorTrans(p_zaun.z, rlx_hutte_pos_rt_vec2.y * .5);
	float f_zaun = fBox(p_zaun, rlx_hutte_zaun_dim_rt_vec3);

	return min(f_hutte, f_zaun);
}

float fScene(vec3 p) {
	float f = p.y;
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

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;
	return mat;
}
