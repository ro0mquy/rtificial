#include "march.glsl"
#include "noise.glsl"
#line 4 "tple"

const float id_boden = 0.;
const float id_hill = 1.;
const float id_saulen = 2.;
const float id_mauer = 3.;
const float id_nebenhill = 4.;
const float id_monolith = 5.;
const float id_bg = 6.;

float fScene(vec3 p) {
	vec3 p_boden = p;
	float f_boden = p_boden.y;

	float f_kuhle = fBoxEdge(p_boden, tple_kuhle_dim_rt_vec3);
	f_boden = opSubtractStairs(f_boden, f_kuhle, .9 * tple_kuhle_dim_rt_vec3.y, tple_kuhle_n_stairs_rt_float);
	mUnion(f_boden, newMaterialId(id_boden, p_boden));

	vec3 p_hill = p;
	float f_hill = fPyramid(p_hill, tple_hill_h_rt_float, Tau * tple_hill_angle_rt_float);
	f_hill = max(f_hill, p_hill.y - tple_hill_plateau_h_rt_float);

	vec3 p_hill_band = p_hill;
	p_hill_band.y -= tple_hill_band_y_offset_rt_float;
	float f_hill_band = fBoxEdge(p_hill_band, tple_hill_band_dim_rt_vec3);

	f_hill = opUnionChamfer(f_hill, f_hill_band, tple_hill_band_smooth_rt_float);
	mUnion(f_hill, newMaterialId(id_hill, p_hill));


	vec3 p_weg = p;
	vec2 cell_weg = pMirrorLoco(p_weg.xz, vec2(0.));
	float hash_cell_weg = dot(cell_weg, vec2(3., 7.)) * .1; // [-1, 1]
	float hash_cell_weg_nb1 = dot(vec2(-cell_weg.x, cell_weg.y), vec2(3., 7.)) * .1;
	float hash_cell_weg_nb2 = dot(vec2(cell_weg.x, -cell_weg.y), vec2(3., 7.)) * .1;

	vec3 p_saulen = p_weg;
	p_saulen.x -= tple_saulen_first_pos_rt_float * tple_saulen_dist_rt_float;
	float cell_saulen = pDomrepSingle(p_saulen.x, tple_saulen_dist_rt_float);
	p_saulen.z -= tple_weg_width_rt_float;

	// distortion
	float hash_cell_saulen = uintToFloat(hash(uint(cell_saulen + hash_cell_weg))) * 2. - 1.;
	float hash_cell_saulen_nb1 = uintToFloat(hash(uint(cell_saulen + hash_cell_weg_nb1))) * 2. - 1.;
	float hash_cell_saulen_nb2 = uintToFloat(hash(uint(cell_saulen + hash_cell_weg_nb2))) * 2. - 1.;
	float distort_offset = hash_cell_saulen * tple_weg_width_distort_rt_float;
	p_saulen.z -= distort_offset;

	float f_saulen = fCylinder(p_saulen, tple_saulen_r_rt_float, tple_saulen_h_rt_float);

	vec3 p_saulen_top = p_saulen;
	p_saulen_top.y -= tple_saulen_h_rt_float;
	float f_saulen_top = fBox(p_saulen_top, tple_saulen_top_dim_rt_vec3);
	f_saulen = opUnionChamfer(f_saulen, f_saulen_top, tple_saulen_top_smooth_rt_float);

	// guard plane
	float distort_offset_nb = min(hash_cell_weg_nb1, hash_cell_weg_nb2) * tple_weg_width_distort_rt_float;
	float f_saulen_guard = p_saulen.z + 2. * tple_weg_width_rt_float + distort_offset + distort_offset_nb - tple_saulen_top_dim_rt_vec3.z;

	f_saulen = min(f_saulen, f_saulen_guard);
	mUnion(f_saulen, newMaterialId(id_saulen, p_saulen));


	vec3 p_mauer = p_weg;
	p_mauer.xz -= tple_mauer_pos_rt_vec2;
	pRotY(p_mauer, Tau * tple_mauer_max_rot_rt_float * hash_cell_weg);

	vec3 p_mauer_dach = p_mauer;
	p_mauer_dach.y -= tple_mauer_dach_y_rt_float;
	float f_mauer_dach = fBoxEdge(p_mauer_dach, tple_mauer_dach_dim_rt_vec3);

	vec3 p_mauer_pfeiler = p_mauer;
	pMirrorTrans(p_mauer_pfeiler.z, tple_mauer_pfeiler_spacing_rt_float);
	float f_mauer_pfeiler = fBoxEdge(p_mauer_pfeiler, tple_mauer_pfeiler_dim_rt_vec3);

	float f_mauer = opUnionChamfer(f_mauer_dach, f_mauer_pfeiler, tple_mauer_smooth_rt_float);
	mUnion(f_mauer, newMaterialId(id_mauer, p_mauer));


	vec3 p_nebenhill = p;
	p_nebenhill.xz -= tple_nebenhill_pos_rt_vec2;
	float f_nebenhill = fPyramid(p_nebenhill, tple_nebenhill_h_rt_float, Tau * tple_nebenhill_angle_rt_float);
	mUnion(f_nebenhill, newMaterialId(id_nebenhill, p_nebenhill));


	vec3 p_monolith = p_hill;
	p_monolith.y -= tple_hill_plateau_h_rt_float;

	vec3 p_mono1 = p_monolith;
	p_mono1 -= tple_mono1_pos_rt_vec3;
	float f_mono1 = fBox(p_mono1, tple_mono1_dim1_rt_vec3);
	p_mono1 -= tple_mono1_offset_rt_vec3;
	f_mono1 = min(f_mono1, fBox(p_mono1, tple_mono1_dim2_rt_vec3));

	vec3 p_mono2 = p_monolith;
	p_mono2 -= tple_mono2_pos_rt_vec3;
	float f_mono2 = fBox(p_mono2, tple_mono2_dim1_rt_vec3);
	p_mono2 -= tple_mono2_offset_rt_vec3;
	f_mono2 = min(f_mono2, fBox(p_mono2, tple_mono2_dim2_rt_vec3));

	float f_monolith = min(f_mono1, f_mono2);
	mUnion(f_monolith, newMaterialId(id_monolith, p_monolith));


	vec3 p_bg = p;
	p_bg -= tple_bg_pos_rt_vec3;
	pMirrorAtPlane(p_bg, normalize(tple_bg_pos_rt_vec3), length(tple_bg_pos_rt_vec3));
	float f_bg = fPyramid(p_bg, tple_bg_h_rt_float, Tau * tple_bg_angle_rt_float);

	vec3 p_bg2 = p;
	p_bg2 -= tple_bg2_pos_rt_vec3;
	pMirrorAtPlane(p_bg2, normalize(tple_bg2_pos_rt_vec3), length(tple_bg2_pos_rt_vec3));
	float f_bg2 = fPyramid(p_bg2, tple_bg2_h_rt_float, Tau * tple_bg2_angle_rt_float);
	f_bg = min(f_bg, f_bg2);

	mUnion(f_bg, newMaterialId(id_bg, p_bg));


	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 color = .07 * ambientColor(normal, -direction, material);
	return color;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(.8));
	if (materialId.id == id_boden) {
		mat.roughness = 1.;

		vec2 p_boden = materialId.coord.xz;
		pMirrorLoco(p_boden, vec2(0.));
		p_boden.x -= tple_tex_boden_streifen_x_rt_float;
		pRot(p_boden, Tau * tple_tex_boden_streifen_rot_rt_float);
		pDomrepInterval(p_boden.y, tple_tex_boden_streifen_spacing_rt_float, 0., tple_tex_boden_streifen_num_rt_float - 1.);
		float f_streifen = abs(p_boden.y) - tple_tex_boden_streifen_width_rt_float;
		float t_streifen = 1. - smoothstep(-.05, .05, f_streifen);
		mat.color = mix(mat.color, tple_tex_boden_streifen_col_rt_color, t_streifen);
	} else if (materialId.id == id_monolith) {
		mat.roughness = .1;
		mat.color = tple_mono_col_rt_color;
	} else {
		mat.roughness = .1;
	}
	return mat;
}
