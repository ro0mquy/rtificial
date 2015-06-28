#include "march.glsl"
#line 3

float fCylWald(vec3 p) {
	vec3 p1 = p;
	pRotY(p1, Tau * relaxo_wald_rotation1_rt_float);
	pDomrep(p1.xz, relaxo_wald_spacing1_rt_vec2);
	float f1 = fCylinder(p1, relaxo_wald_r1_rt_float, relaxo_wald_h1_rt_float);

	vec3 p2 = p;
	pRotY(p2, Tau * relaxo_wald_rotation2_rt_float);
	pDomrep(p2.xz, relaxo_wald_spacing2_rt_vec2);
	float f2 = fCylinder(p2, relaxo_wald_r2_rt_float, relaxo_wald_h2_rt_float);

	vec3 p3 = p;
	pRotY(p3, Tau * relaxo_wald_rotation3_rt_float);
	pDomrep(p3.xz, relaxo_wald_spacing3_rt_vec2);
	float f3 = fCylinder(p3, relaxo_wald_r3_rt_float, relaxo_wald_h3_rt_float);

	vec3 p4 = p;
	pRotY(p4, Tau * relaxo_wald_rotation4_rt_float);
	pDomrep(p4.xz, relaxo_wald_spacing4_rt_vec2);
	float f4 = fCylinder(p4, relaxo_wald_r4_rt_float, relaxo_wald_h4_rt_float);

	return min4(f1, f2, f3, f4);
}

float fPyramidStep(vec3 p, float pyramid_sidelen, float pyramid_height) {
	vec3 p1 = p;
	float f1 = fPyramid(p1, 6./3. * relaxo_pyramid_height_rt_float, Tau * relaxo_pyramid_phi_rt_float);
	f1 = max(f1, p1.y - 1./3. * relaxo_pyramid_height_rt_float);

	vec3 p_wand = p;
	pMirrorLoco(p_wand.xz, vec2(pyramid_sidelen));
	vec3 p_deckel_furche = p_wand;
	pRotY(p_wand, Tau * relaxo_pyramid_loco_rot_y_rt_float);

	float f_ruckwand = p_wand.x + relaxo_pyramid_ruckwand_depth_rt_float;
	float f_deckel = p_wand.y - pyramid_height;

	pTrans(p_deckel_furche.z, -pyramid_sidelen + relaxo_pyramid_furche_width_rt_float);
	float f_deckel_furche = p_deckel_furche.z;
	f_deckel_furche = max(f_deckel_furche, -p_deckel_furche.y + pyramid_height - relaxo_pyramid_furche_depth_rt_float);
	f_deckel = opSubtractChamfer(f_deckel, f_deckel_furche, relaxo_pyramid_furche_smooth_rt_float);

	float wand_angle = Tau * relaxo_pyramid_wand_angle_rt_float;
	pRotZ(p_wand, wand_angle);
	pDomrep(p_wand.z, relaxo_pyramid_wand_domrep_rt_float);

	vec3 p_saulen = p_wand;
	pTrans(p_saulen.y, pyramid_height);
	pMirrorTrans(p_saulen.z, relaxo_pyramid_saulen_trans_big_rt_float);
	vec3 p_kreise = p_saulen;
	float saulen_angle = -Tau * relaxo_pyramid_saulen_angle_rt_float;
	pRotX(p_saulen, saulen_angle);
	vec3 p_streben = p_saulen;
	pMirrorTrans(p_saulen.z, relaxo_pyramid_saulen_trans_small_rt_float);
	float f_saulen = f2Box(p_saulen.xz, relaxo_pyramid_saulen_dim_rt_vec2);

	pDomrep(p_streben.y, relaxo_pyramid_streben_domrep_rt_float);
	pRotX(p_streben, -saulen_angle);
	float f_streben = fBoxEdge(p_streben, vec3(relaxo_pyramid_streben_dim_rt_vec2, relaxo_pyramid_saulen_trans_small_rt_float));

	float f_kreise_schnitt = p_kreise.x - relaxo_pyramid_saulen_dim_rt_vec2.x;
	pRotZ(p_kreise, -wand_angle);
	pTrans(p_kreise.z, relaxo_pyramid_keise_pos_z_rt_float);
	float f_kreise = f2Sphere(p_kreise.xz, relaxo_pyramid_keise_r_rt_float);
	f_kreise = max(f_kreise, f_kreise_schnitt);

	vec3 p_schnitte = p;
	pTrans(p_schnitte.y, .5 * pyramid_height);
	float f_schnitte = abs(p_schnitte.y) - relaxo_pyramid_schnitte_height_rt_float;

	float f_pyramid = f_saulen;
	f_pyramid = opUnionChamfer(f_pyramid, f_streben, relaxo_pyramid_streben_smooth_rt_float);
	f_pyramid = min(f_pyramid, f_kreise);
	f_pyramid = min(f_pyramid, f_ruckwand);
	f_pyramid = max(f_pyramid, f_deckel);
	f_pyramid = max(f_pyramid, -f_schnitte);
	return f_pyramid;
}

float fBasis(vec3 p) {
	float height = relaxo_basis_height_rt_float / 3.;
	float f1 = fPyramidStep(p, relaxo_basis_width_rt_float, height);
	p.y -= height;
	float f2 = fPyramidStep(p, 2./3. * relaxo_basis_width_rt_float, height);
	p.y -= height;
	float f3 = fPyramidStep(p, 1./3. * relaxo_basis_width_rt_float, height);
	return min3(f1, f2, f3);
}

float fVorbau(vec3 p) {
	float height = relaxo_vorbau_height_rt_float * .5;
	vec3 p_vorbau = p;
	pTrans(p_vorbau.xz, relaxo_vorbau_pos_rt_vec2);

	vec3 p1 = p_vorbau;
	float f1 = fPyramidStep(p1, relaxo_vorbau_width_rt_float, height);

	vec3 p2 = p_vorbau;
	p2.y -= height;
	float f2 = fPyramidStep(p2, .5 * relaxo_vorbau_width_rt_float, height);


	vec3 p_klein = p;
	pMirrorTrans(p_klein.x, relaxo_vorbau_klein_offset_rt_vec2.x);
	float cell_klein = pMirrorTrans(p_klein.z, relaxo_vorbau_pos_rt_vec2.y + relaxo_vorbau_klein_offset_rt_vec2.y);
	float klein_width = relaxo_vorbau_klein_width_rt_float;
	if (cell_klein < 0.) {
		p_klein.xz -= relaxo_vorbau_klein_smaller_offset_rt_vec2;
		klein_width *= relaxo_vorbau_klein_smaller_rt_float;
	}
	float f_klein = fPyramidStep(p_klein, klein_width, relaxo_vorbau_klein_height_rt_float);


	return min3(f1, f2, f_klein);
}

float fFluegel(vec3 p) {
	pMirrorTrans(p.x, relaxo_fluegel_pos_rt_vec2.x);
	p.z -= relaxo_fluegel_pos_rt_vec2.y;
	//pDomrepInterval(p.z, );
	return 0;
}

float fSaule(vec3 p) {
	vec3 p_stamm = p;
	float f_stamm = fCylinder(p_stamm, relaxo_saule_stamm_r_rt_float, relaxo_saule_stamm_h_rt_float);

	vec3 p_top = p;
	p_top.y -= relaxo_saule_stamm_h_rt_float;
	pMirrorTrans(p_top, vec3(.1));
	pMirrorAtPlane(p_top, normalize(relaxo_saulen_top_mirror_dir_rt_vec3), length(relaxo_saulen_top_mirror_dir_rt_vec3));
	float f_top = fBox(p_top, relaxo_saulen_top_dim_rt_vec3);

	vec3 p_cut = p;
	p_cut.y -= relaxo_saule_stamm_h_rt_float;
	pRotX(p_cut, Tau * relaxo_saule_cut_angle_rt_float);
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
	p.y -= relaxo_saule_knick_height_rt_float;
	pMirror(p.y);
	pRotX(p, Tau * relaxo_saule_rotation_rt_float);
	float f_saule = fBox(p, relaxo_saule_dim_rt_vec3);
	return f_saule;
	// */
}

float fWeg(vec3 p) {
	pMirrorLoco(p.xz, vec2(0.));
	p.z -= relaxo_weg_width_rt_float;
	p.x -= relaxo_weg_offset_first_rt_float;
	pDomrepSingle(p.x, relaxo_weg_spacing_rt_float);
	float f_saule = fSaule(p);
	return f_saule;
}

float fFeld(vec3 p) {
	pMirrorLoco(p.xz, vec2(0.));
	p.z -= relaxo_feld_spacing_rt_vec2.y * .5;
	p.x -= relaxo_feld_offset_first_rt_float;
	pDomrepInterval(p.xz, relaxo_feld_spacing_rt_vec2, vec2(0.), relaxo_feld_interval_end_rt_vec2);
	float f_saule = fSaule(p);
	return f_saule;
}

float fTurm(vec3 p) {
	pMirrorTrans(p.xz, relaxo_turm_pos_rt_vec2);
	float f_basis = f2Sphere(p.xz, relaxo_turm_r_rt_float);
	p.y -= relaxo_turm_h_rt_float;
	float f_spitze = fConeAngle(p, Tau * relaxo_turm_angle_rt_float);
	float f_turm = max(f_basis, f_spitze);
	return f_turm;
}

float fScene(vec3 p) {
	float f_boden = p.y;

	//float f_cylwald = fCylWald(p);
	//f_cylwald = max(f_cylwald, -f2Sphere(p.xz, relaxo_wald_free_r_rt_float));

	float f_basis = fBasis(p);
	float f_vorbau = fVorbau(p);
	//float f_fluegel = fFluegel(p);

	float f_weg = fWeg(p);

	//float f_feld = fFeld(p);

	//float f_turm = fTurm(p);


	float f = f_boden;
	//f = min(f, f_cylwald);
	f = min(f, f_basis);
	f = min(f, f_vorbau);
	//f = min(f, f_fluegel);
	f = min(f, f_weg);
	//f = min(f, f_feld);
	//f = min(f, f_turm);

	mUnion(f, MaterialId(0., p));
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
