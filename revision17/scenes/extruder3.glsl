#include "march.glsl"
#line 3

const float mat_id_bounding = 0.;
const float mat_id_ext = 1.;

float fScene(vec3 p) {
	vec3 p_ext = p;

	// extruder geschwurbel
	pTrans(p_ext.x, -ext_extrude_h_rt_float);
	float px_before = p_ext.x;
	float px_clamped = clamp(p_ext.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_ext.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	pRotX(p_ext, 2. * (px_param + ext_rot_rt_float) * Tau);
	pTrans(p_ext.z, 10 * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_ext,      (px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_ext, 4. * (px_param + ext_rot_rt_float) * Tau);

	float long_rot = .5 * (px_param + ext_rot_rt_float) + ext_3_long_rot_rt_float;

	// assemble object
	vec3 p_long = p_ext;
	pRotZ(p_long, long_rot * Tau);
	pTrans(p_long.x, ext_3_long_trans_rt_float);
	float f_long = fSpheroid(p_long, ext_3_long_l_rt_float * ext_3_long_r_rt_float, ext_3_long_r_rt_float);

	float f_ext = f_long;

	// material stuff
	MatWrap w_ext = MatWrap(f_ext, MaterialId(mat_id_ext, p_ext, vec4(0.)));

	MatWrap w_bound = MatWrap(-fSphere(p, 100.), newMaterialId(mat_id_bounding, p));
	MatWrap w = mUnion(w_ext, w_bound);

	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	//return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));

	if (materialId.id == mat_id_bounding) {
		mat.color = ext_color_background_rt_color;
	} else if (materialId.id == mat_id_ext) {
		vec3 loco_index = (materialId.misc.xyz + 1.) / 2.; // {0, 1}
		float px_before = (materialId.misc.w/ext_extrude_h_rt_float + 1.) / 2.; // {0,1}

		mat.color = ext_color_torus_rt_color;

		if (loco_index.x > 0) {
			mat.color = ext_color_torus_hightlight_rt_color;
		}
		if (loco_index.y > 0) {
			mat.color += 0.5;
		}
		mat.color = mix(ext_color_fade_rt_color,mat.color, pow(px_before, ext_fade_pow_rt_float));
	}

	return mat;
}
