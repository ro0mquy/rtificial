#include "march.glsl"
#include "extruder_lighting.glsl"
#line 4

const float mat_id_ground = 0.;
const float mat_id_ext = 1.;

float fScene(vec3 p) {
	vec3 p_ext = p;
	pTrans(p_ext.y, 10.);

	// extruder geschwurbel
	//*
	pTrans(p_ext.x, -ext_extrude_h_rt_float);
	float px_before = p_ext.x;
	float px_clamped = clamp(p_ext.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_ext.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	pRotX(p_ext, 2. * (px_param + ext_rot_rt_float) * Tau);
	pTrans(p_ext.z, 10 * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_ext,      (px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_ext, 4. * (px_param + ext_rot_rt_float) * Tau);
	// */

	// assemble object
	// standing V
	vec2 p_V = p_ext.zy;
	pMirror(p_V.x);
	float f_V = f2PlaneAngle(p_V, extlogo_V_angle_rt_float * Tau);
	pMirrorTrans(f_V, extlogo_thickness_rt_float);
	f_V = max(f_V, p_V.y - extlogo_V_height_rt_float);

	// lying V
	vec2 p_lt = p_ext.zy;
	pTrans(p_lt, extlogo_lt_origin_rt_vec2);
	pRot(p_lt, extlogo_lt_angle_global_rt_float * Tau);
	float i_lt_updown = pMirror(p_lt.x);

	float lt_thickness = extlogo_thickness_rt_float;
	if (i_lt_updown < 0.) {
		pRot(p_lt, saturate((p_lt.x - extlogo_lt_down_bend_offset_rt_float) / extlogo_lt_down_bend_scale_rt_float) * extlogo_lt_down_bend_angle_rt_float * Tau);
		lt_thickness *= extlogo_lt_down_thickness_factor_rt_float;
	}

	float f_lt = f2PlaneAngle(p_lt, extlogo_lt_angle_rt_float * Tau);
	pMirrorTrans(f_lt, lt_thickness);

	if (i_lt_updown > 0.) {
		float f_lt_cutout = p_ext.z;
		f_lt_cutout = min(-f_lt_cutout - extlogo_lt_cutout_left_rt_float, f_lt_cutout + extlogo_lt_cutout_right_rt_float);
		f_lt = max(f_lt, f_lt_cutout);

		f_lt = max(f_lt, f2PlaneAngle(p_ext.zy - extlogo_lt_up_cut_pos_rt_vec2, extlogo_lt_up_cut_angle_rt_float * Tau));
	} else {
		f_lt = max(f_lt, f2PlaneAngle(p_ext.zy - extlogo_lt_down_cut_pos_rt_vec2, extlogo_lt_down_cut_angle_rt_float * Tau));
	}

	float f_ext = min(f_V, f_lt);
	f_ext = max(f_ext, abs(p_ext.x) - extlogo_thickness_rt_float); // extrude
	MatWrap w_ext = MatWrap(f_ext, MaterialId(mat_id_ext, p_ext, vec4(0.)));

	// ground plane
	vec3 p_ground = p;
	float f_ground = opUnionRounded(p_ground.x - extbg_ground_offset_rt_float, p_ground.y, extbg_ground_round_r_rt_float);
	MatWrap w_ground = MatWrap(f_ground, newMaterialId(mat_id_ground, p));

	// combine everything
	MatWrap w = w_ext;
	w = mUnion(w, w_ground);

	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = applyExtruderLights(origin, marched, direction, hit, normal, materialId, material);
	return result;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));

	if (materialId.id == mat_id_ext) {
		vec3 p_ext = materialId.coord;
		//float px_before = (materialId.misc.w/ext_extrude_h_rt_float + 1.) / 2.; // {0,1}

		mat.color = ext2_obj_color_rt_color;
		mat.metallic = 1.;
		mat.roughness = ext2_obj_roughness_rt_float;

	} else if (materialId.id == mat_id_ground) {
		mat.color = extbg_ground_color_rt_color;
		mat.metallic = 0.;
		mat.roughness = extbg_ground_roughness_rt_float;
	}

	return mat;
}
