#include "march.glsl"
#include "shadow.glsl"
#line 4

const float mat_id_ground = 0.;
const float mat_id_ext = 1.;

float fScene(vec3 p) {
	vec3 p_ext = p;
	pTrans(p_ext.y, 10.);

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

	// ground plane
	float f_ground = p.y;
	MatWrap w_ground = MatWrap(f_ground, newMaterialId(mat_id_ground, p));

	MatWrap w = mUnion(w_ext, w_ground);

	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = vec3(0.);

	vec3 emission = material.emission;
	result += emission;

	vec3 ambient_lighting = ambientColor(normal, -direction, material);
	if (materialId.id == mat_id_ground) {
		result += ambient_lighting;
	}

	SphereLight light = SphereLight(ext_light_pos_rt_vec3, ext_light_radius_rt_float, ext_light_color_rt_color, ext_light_power_rt_float*1000.);
	vec3 sphere_lighting = applySphereLight(origin, marched, direction, hit, normal, material, light) * light.color;

	vec3 dir_to_light = normalize(ext_light_pos_rt_vec3 - hit);
	float length_to_light = distance(ext_light_pos_rt_vec3, hit);
	float shadowing_value = shadowMarch(hit, dir_to_light, length_to_light);
	sphere_lighting *= shadowing_value;
	result += sphere_lighting;

	SphereLight light2 = SphereLight(ext_light2_pos_rt_vec3, ext_light2_radius_rt_float, ext_light2_color_rt_color, ext_light2_power_rt_float*1000.);
	vec3 sphere_lighting2 = applySphereLight(origin, marched, direction, hit, normal, material, light2) * light2.color;

	vec3 dir_to_light2 = normalize(ext_light2_pos_rt_vec3 - hit);
	float length_to_light2 = distance(ext_light2_pos_rt_vec3, hit);
	float shadowing_value2 = shadowMarch(hit, dir_to_light2, length_to_light2);
	sphere_lighting2 *= shadowing_value2;
	result += sphere_lighting2;

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

		if (abs(p_ext.y) < .2) {
			mat.color = ext_3_color_highlight_rt_color;
			mat.metallic = 1.;
			mat.roughness = ext_3_color_highlight_roughness_rt_float;
		} else {
			mat.color = ext_3_color_spheroid_rt_color;
			mat.metallic = 1.;
			mat.roughness = ext_3_color_roughness_rt_float;
		}

	} else if (materialId.id == mat_id_ground) {
		mat.color = vec3(1.);
		mat.roughness = 1.;
	}

	return mat;
}
