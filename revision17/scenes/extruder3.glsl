#include "march.glsl"
#include "extruder_lighting.glsl"
#line 4

const float mat_id_ground = 0.;
const float mat_id_ext = 1.;
const float mat_id_bg = 2.;
const float mat_id_highl = 3.;

float fScene(vec3 p) {
	vec3 p_ext = p;
	pTrans(p_ext.y, 15.);

	// extruder geschwurbel
	pTrans(p_ext.x, -ext_extrude_h_rt_float);
	float px_before = p_ext.x;
	float px_clamped = clamp(p_ext.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_ext.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	float bez_t = (-px_clamped / ext_extrude_h_rt_float + 1.) / 2.;
	pTrans(p_ext.zy, bezier(ext3_bez_p1_rt_vec2, ext3_bez_p2_rt_vec2, ext3_bez_p3_rt_vec2, ext3_bez_p4_rt_vec2, bez_t));

	pRotX(p_ext, 2. * (px_param + ext_rot_rt_float) * Tau);
	//pTrans(p_ext.z, 10 * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_ext,      (px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_ext, 4. * (px_param + ext_rot_rt_float) * Tau);

	//float long_rot = .5 * (px_param + ext_rot_rt_float) + ext3_long_rot_rt_float;
	float long_rot = 0.;

	// assemble object
	vec3 p_long = p_ext;
	pRotZ(p_long, long_rot * Tau);
	pTrans(p_long.x, ext3_long_trans_rt_float);
	float f_long = fSpheroid(p_long, ext3_long_l_rt_float * ext3_long_r_rt_float, ext3_long_r_rt_float);

	float f_ext = f_long;
	MatWrap w_ext = MatWrap(f_ext, MaterialId(mat_id_ext, p_ext, vec4(0.)));

	// golden highlight
	vec2 q_highl = vec2(f_long, p_long.y);
	float f_highl = f2Sphere(q_highl, ext3_color_highlight_size_rt_float);
	MatWrap w_highl = MatWrap(f_highl, newMaterialId(mat_id_highl, p_ext));

	// ground plane
	vec3 p_ground = p;
	float f_ground = opUnionRounded(p_ground.x - extbg_ground_offset_rt_float, p_ground.y, extbg_ground_round_r_rt_float);
	MatWrap w_ground = MatWrap(f_ground, newMaterialId(mat_id_ground, p));

	// background objects
	vec3 p_bg = p;
	float f_bg = f_ext_background(p_bg);
	MatWrap w_bg = MatWrap(f_bg, newMaterialId(mat_id_bg, p_bg));

	// combine everything
	MatWrap w = w_ext;
	w = mUnion(w, w_highl);
	w = mUnion(w, w_ground);
	//w = mUnion(w, w_bg);

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

	if (materialId.id == mat_id_ext || materialId.id == mat_id_bg) {
		vec3 p_ext = materialId.coord;
		//float px_before = (materialId.misc.w/ext_extrude_h_rt_float + 1.) / 2.; // {0,1}

		mat.color = ext3_color_spheroid_rt_color;
		mat.metallic = 1.;
		mat.roughness = ext3_color_roughness_rt_float;

	} else if (materialId.id == mat_id_highl) {
		mat.color = ext3_color_highlight_rt_color;
		mat.metallic = 1.;
		mat.roughness = ext3_color_highlight_roughness_rt_float;

	} else if (materialId.id == mat_id_ground) {
		mat.color = extbg_ground_color_rt_color;
		mat.metallic = 0.;
		mat.roughness = extbg_ground_roughness_rt_float;

		vec3 pos = materialId.coord;
		pTrans(pos.zy, extbg_stripes_offset_rt_vec2);
		float i_mirror = pMirrorTrans(pos.z, extbg_stripes_apart_rt_vec2.x);
		pos.y *= i_mirror;
		pTrans(pos.y, extbg_stripes_apart_rt_vec2.y);
		pMirrorTrans(pos.y, extbg_stripes_width_rt_float);
		float stripe = step(pos.y, 0.) * step(-pos.z, 0.);

		mat.color = mix(mat.color, extbg_stripes_color_rt_color, srgb2lin(extbg_stripes_indensity_rt_float) * stripe);
	}

	return mat;
}
