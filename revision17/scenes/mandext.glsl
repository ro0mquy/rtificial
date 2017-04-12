#include "march.glsl"
#include "extruder_lighting.glsl"
#line 4

const float mat_id_ground = 0.;
const float mat_id_ext = 1.;

float pDomrepGridOnlyX(inout vec2 p, float c) {
	float i = pDomrepMirror(p.x, c);
	p.x -= .5 * c;
	if (p.x > p.y) {
		p.xy = p.yx;
	}
	return floor(.5 * i);
}

float rot_cell = mand_rot_cell_rt_float;
vec2 trans_inner = mand_trans_inner_rt_vec2;

MatWrap wMandala(vec3 p) {
	// domain geschwurbel
	vec3 p_cyl = p;

	pRotY(p_cyl, Tau * mand_rot_global_rt_float);

	// domrep angle
	float c_circle = pDomrepAngle(p_cyl.zx, mand_domrep_angle_num_rt_float, mand_domrep_angle_r_rt_float);

	pTrans(p_cyl.x, 4.);

	// grid domrep
	float c_grid = pDomrepGridOnlyX(p_cyl.xz, 8.);

	pRotY(p_cyl, Tau * rot_cell);
	pTrans(p_cyl.xz, trans_inner);

	// distance function geschwurbel
	pRotX(p_cyl, 2. * mand_rot_3D_rt_float * Tau);
	pRotZ(p_cyl,      mand_rot_3D_rt_float * Tau);
	float f_cyl = f2Box(p_cyl.zx, 1., 2.*Golden_Ratio);
	float c_distfunc = pDomrepInterval(f_cyl, mand_f_domrep_rt_float, 0., 1.);
	f_cyl = f2Box(vec2(f_cyl, p_cyl.y), mand_f_thickness_rt_float);

	MatWrap w_cyl = MatWrap(f_cyl, MaterialId(mat_id_ext, p_cyl, vec4(c_circle, c_grid, c_distfunc, 0.)));
	return w_cyl;
}

float fScene(vec3 p) {
	vec3 p_ext = p;
	//pTrans(p_ext.y, 10.);

	// extruder geschwurbel
	pTrans(p_ext.x, -ext_extrude_h_rt_float);
	float px_before = p_ext.x;
	float px_clamped = clamp(p_ext.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_ext.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	pRotX(p_ext, .5 * (px_param + ext_rot_rt_float) * Tau);
	/*
	pTrans(p_ext.z, 10 * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_ext,      (px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_ext, 4. * (px_param + ext_rot_rt_float) * Tau);
	// */

	rot_cell = .3 * sin(1. * (px_param + ext_rot_rt_float) * Tau) + mand_rot_cell_rt_float;
	trans_inner = vec2(1., 3.6) * sin(1. * (px_param + ext_rot_rt_float) * Tau) + mand_trans_inner_rt_vec2;


	// assemble object
	MatWrap w_ext = wMandala(p_ext.yxz);

	// ground plane
	float f_ground = p.x - mandext_plane_offset_rt_float;
	MatWrap w_ground = MatWrap(f_ground, newMaterialId(mat_id_ground, p));

	// combine everything
	MatWrap w = w_ext;
	w = mUnion(w, w_ground);

	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = vec3(0.);

	vec3 emission = material.emission;
	result += emission;

	vec3 ambient_lighting = ambientColor(normal, -direction, material);
	//if (materialId.id == mat_id_ground) {
		result += ambient_lighting;
	//}


	PointLight light1 = PointLight(
			mandlight_light1_pos_rt_vec3,
			normalize(mandlight_light1_target_rt_vec3 - mandlight_light1_pos_rt_vec3),
			Tau * mandlight_light1_angle_rt_float,
			mandlight_light1_color_rt_color,
			mandlight_light1_power_rt_float*1000.
	);
	vec3 sphere_lighting1 = applyPointLightWithShadow(origin, marched, direction, hit, normal, material, light1, mandlight_shadow_softness_rt_float);
	result += sphere_lighting1;

	PointLight light2 = PointLight(
			mandlight_light2_pos_rt_vec3,
			normalize(mandlight_light2_target_rt_vec3 - mandlight_light2_pos_rt_vec3),
			Tau * mandlight_light2_angle_rt_float,
			mandlight_light2_color_rt_color,
			mandlight_light2_power_rt_float*1000.
	);
	vec3 sphere_lighting2 = applyPointLightWithShadow(origin, marched, direction, hit, normal, material, light2, mandlight_shadow_softness_rt_float);
	result += sphere_lighting2;

	PointLight light3 = PointLight(
			mandlight_light3_pos_rt_vec3,
			normalize(mandlight_light3_target_rt_vec3 - mandlight_light3_pos_rt_vec3),
			Tau * mandlight_light3_angle_rt_float,
			mandlight_light3_color_rt_color,
			mandlight_light3_power_rt_float*1000.
	);
	vec3 sphere_lighting3 = applyPointLightWithShadow(origin, marched, direction, hit, normal, material, light3, mandlight_shadow_softness_rt_float);
	result += sphere_lighting3;

	return result;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));

	if (materialId.id == mat_id_ext) {
		float c_circle = (materialId.misc.x + floor(mand_domrep_angle_num_rt_float/2.));// / (mand_domrep_angle_num_rt_float-1);
		float c_grid = materialId.misc.y / 2. + .5; // 0,1
		float c_distfunc = materialId.misc.z; // null oder 1 nicer wert

		if (c_distfunc > 0) {
			mat.color = 2* mand_color_circle_rt_color;
		} else {
			mat.color = mand_color_2_rt_color;
		}
		if (c_grid > 0) {
			mat.color += 0.5;
		}

		mat.metallic = 1.;
		mat.roughness = mandlight_obj_roughness_rt_float;
	} else if (materialId.id == mat_id_ground) {
		mat.color = extbg_ground_color_rt_color;
		mat.metallic = 0.;
		mat.roughness = extbg_ground_roughness_rt_float;
	}

	return mat;
}
