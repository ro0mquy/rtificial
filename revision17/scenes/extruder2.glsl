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

	float ext_2_mirror_plane_dist_rt_float = sin(3. * (px_param + ext_rot_rt_float) * Tau) + ext_2_mirror_plane_dist_rt_float;
	float ext_2_mirror_loco_rot_rt_float = 2. * (px_param + ext_rot_rt_float) + ext_2_mirror_loco_rot_rt_float;

	// assemble object
	pMirrorLoco(p_ext.xz, vec2(ext_2_mirror_loco_rt_float));
	pTrans(p_ext.x, ext_2_mirror_loco_trans_rt_float);
	pRotY(p_ext, ext_2_mirror_loco_rot_rt_float * Tau);

	vec3 mirror_normal = vec3(0., 0., -1.);
	pQuatRotate(mirror_normal, ext_2_mirror_plane_dir_rt_quat);
	pMirrorAtPlane(p_ext, mirror_normal, ext_2_mirror_plane_dist_rt_float);

	float f_ext = fBox(p_ext, ext_2_box_r_rt_float * vec3(Golden_Ratio, 1., 1.));

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
