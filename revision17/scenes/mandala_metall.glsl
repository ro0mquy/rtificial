#include "march.glsl"
#include "extruder_lighting.glsl"
#line 4

const float mat_id_plane = 0.;
const float mat_id_cyl = 1.;

float pDomrepGridOnlyX(inout vec2 p, float c) {
	float i = pDomrepMirror(p.x, c);
	p.x -= .5 * c;
	if (p.x > p.y) {
		p.xy = p.yx;
	}
	return floor(.5 * i);
}

float fScene(vec3 p) {
	// domain geschwurbel
	vec3 p_cyl = p.yxz;

	pRotY(p_cyl, Tau * mand_rot_global_rt_float);

	// domrep angle
	if (mand_mix_circle_enable_rt_bool) {
		pTrans(p_cyl.z, - mand_domrep_angle_r_rt_float / mand_mix_circle_rt_float);
	}
	float c_circle = pDomrepAngle(p_cyl.zx, mand_domrep_angle_num_rt_float * mand_mix_circle_rt_float, mand_domrep_angle_r_rt_float / mand_mix_circle_rt_float);

	pTrans(p_cyl.x, 4.);

	// grid domrep
	vec3 q_grid = p_cyl;
	float c_grid = pDomrepGridOnlyX(p_cyl.xz, 8.);
	p_cyl = mix(q_grid, p_cyl, mand_mix_grid_rt_float);

	pRotY(p_cyl, Tau * mand_rot_cell_rt_float);
	pTrans(p_cyl.xz, mand_trans_inner_rt_vec2);

	// distance function geschwurbel
	pRotX(p_cyl, 2. * mand_rot_3D_rt_float * Tau);
	pRotZ(p_cyl,      mand_rot_3D_rt_float * Tau);
	float f_cyl = f2Box(p_cyl.zx, 1., 2.*Golden_Ratio);
	float c_distfunc = pDomrepInterval(f_cyl, mand_f_domrep_rt_float, 0., 1.);
	f_cyl = f2Box(vec2(f_cyl, p_cyl.y), mand_f_thickness_rt_float);

	// material
	MatWrap w_cyl = MatWrap(f_cyl, MaterialId(mat_id_cyl, p_cyl, vec4(c_circle, c_grid, c_distfunc, 0.)));

	// ground plane
	float f_plane = p.x;
	MatWrap w_plane = MatWrap(f_plane, newMaterialId(mat_id_plane, p));

	MatWrap w = mUnion(w_cyl, w_plane);
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


	//SphereLight light1 = SphereLight(mand_lighting_light1_pos_rt_vec3, mand_lighting_light1_radius_rt_float, mand_lighting_light1_color_rt_color, mand_lighting_light1_power_rt_float*1000.);
	//vec3 sphere_lighting1 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light1, ext_shadow_softness_rt_float);
	PointLight light1 = PointLight(
			mand_lighting_light1_pos_rt_vec3,
			normalize(mand_lighting_light1_target_rt_vec3 - mand_lighting_light1_pos_rt_vec3),
			Tau * mand_lighting_light1_angle_rt_float,
			mand_lighting_light1_color_rt_color,
			mand_lighting_light1_power_rt_float*1000.
	);
	vec3 sphere_lighting1 = applyPointLightWithShadow(origin, marched, direction, hit, normal, material, light1, ext_shadow_softness_rt_float);
	result += sphere_lighting1;

	//SphereLight light2 = SphereLight(mand_lighting_light2_pos_rt_vec3, mand_lighting_light2_radius_rt_float, mand_lighting_light2_color_rt_color, mand_lighting_light2_power_rt_float*1000.);
	//vec3 sphere_lighting2 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light2, ext_shadow_softness_rt_float);
	PointLight light2 = PointLight(
			mand_lighting_light2_pos_rt_vec3,
			normalize(mand_lighting_light2_target_rt_vec3 - mand_lighting_light2_pos_rt_vec3),
			Tau * mand_lighting_light2_angle_rt_float,
			mand_lighting_light2_color_rt_color,
			mand_lighting_light2_power_rt_float*1000.
	);
	vec3 sphere_lighting2 = applyPointLightWithShadow(origin, marched, direction, hit, normal, material, light2, ext_shadow_softness_rt_float);
	result += sphere_lighting2;

	//SphereLight light3 = SphereLight(mand_lighting_light3_pos_rt_vec3, mand_lighting_light3_radius_rt_float, mand_lighting_light3_color_rt_color, mand_lighting_light3_power_rt_float*1000.);
	//vec3 sphere_lighting3 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light3, ext_shadow_softness_rt_float);
	PointLight light3 = PointLight(
			mand_lighting_light3_pos_rt_vec3,
			normalize(mand_lighting_light3_target_rt_vec3 - mand_lighting_light3_pos_rt_vec3),
			Tau * mand_lighting_light3_angle_rt_float,
			mand_lighting_light3_color_rt_color,
			mand_lighting_light3_power_rt_float*1000.
	);
	vec3 sphere_lighting3 = applyPointLightWithShadow(origin, marched, direction, hit, normal, material, light3, ext_shadow_softness_rt_float);
	result += sphere_lighting3;

	return result;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1.));
	mat.roughness = 0.1;

	if (materialId.id == mat_id_plane) {
		mat.color = mand_color_ground_rt_color;
		mat.roughness = 1.;
	} else if (materialId.id == mat_id_cyl) {
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
		mat.roughness = mand_lighting_obj_roughness_rt_float;
	}

	return mat;
}
