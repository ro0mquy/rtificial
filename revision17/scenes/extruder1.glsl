#include "march.glsl"
#include "extruder_lighting.glsl"
#line 4

const float mat_id_ext = 1.;
const float mat_id_highl = 3.;

float myTorusPartial(vec3 p, float rBig, float rSmall, float halfAngle) {
	float r = length(p.xz);
	float angle = atan(p.z, p.x);

	float clampedAngle = angle - clamp(angle, -halfAngle, halfAngle);

	p.xz = r * unitVector(clampedAngle);
	p.x -= rBig;

	pRotZ(p, angle);

	return fBoxRounded(p, rSmall, .1*rSmall);
}

float fScene(vec3 p) {
	vec3 p_ext = p;
	pTrans(p_ext.y, 10.);
	pTrans(p_ext.x, ext1_translation_rt_float);

	pTrans(p_ext.x, -ext_extrude_h_rt_float);
	float px_before = p_ext.x;
	float px_clamped = clamp(p_ext.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_ext.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	float bez_t = (-px_clamped / ext_extrude_h_rt_float + 1.) / 2.;
	pTrans(p_ext.zy, bezier(ext1_bez_p1_rt_vec2, ext1_bez_p2_rt_vec2, ext1_bez_p3_rt_vec2, ext1_bez_p4_rt_vec2, bez_t));

	pRotX(p_ext, 2. * (px_param + ext_rot_rt_float) * Tau);
	pTrans(p_ext.z, ext1_trans_amp_rt_float * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_ext,      (2. * px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_ext, 4. * (px_param + ext_rot_rt_float) * Tau);

	vec2 loco_torus = vec2(ext1_obj_loco_rt_float + ext1_obj_loco_amp_rt_float * sin(4. * (px_param + ext_rot_rt_float) * Tau));

	vec3 loco_index = pMirrorLoco(p_ext.xy, loco_torus);
	pRotY(p_ext, ext1_obj_rot_rt_float * Tau);

	float f_torus = myTorusPartial(p_ext.yxz, 3., 1., ext1_torus_angle_rt_float * Tau);

	float f_ext = f_torus;
	MatWrap w_ext = MatWrap(f_ext, MaterialId(mat_id_ext, p_ext, vec4(loco_index, px_before)));

	// golden highlight
	vec2 q_highl = vec2(f_ext, p_ext.x);
	float f_highl = f2Sphere(q_highl, ext3_color_highlight_size_rt_float);
	MatWrap w_highl = MatWrap(f_highl, newMaterialId(mat_id_highl, p_ext));

	// combine everything
	MatWrap w = w_ext;
	w = mUnion(w, w_highl);

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
		vec3 loco_index = (materialId.misc.xyz + 1.) / 2.; // {0, 1}
		float px_before = (materialId.misc.w/ext_extrude_h_rt_float + 1.) / 2.; // {0,1}

		mat.color = ext1_obj_color_rt_color;
		mat.metallic = 1.;
		mat.roughness = ext1_obj_roughness_rt_float;

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
