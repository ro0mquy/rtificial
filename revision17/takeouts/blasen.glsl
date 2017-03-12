#include "march.glsl"
#line 3

const float mat_id_fabric = 0.;
const float mat_id_pattern = 1.;
const float mat_id_rotekugel = 2.;

float fScene(vec3 p) {
	float bubbles = fSphere(p - blasen_blase1_p_rt_vec3, blasen_blase1_r_rt_float);
	bubbles = opUnionRounded(bubbles, fSphere(p - blasen_blase2_p_rt_vec3, blasen_blase2_r_rt_float), blasen_blase2_smooth_rt_float);
	bubbles = opUnionRounded(bubbles, fSphere(p - blasen_blase3_p_rt_vec3, blasen_blase3_r_rt_float), blasen_blase3_smooth_rt_float);
	bubbles = opUnionRounded(bubbles, fSphere(p - blasen_blase4_p_rt_vec3, blasen_blase4_r_rt_float), blasen_blase4_smooth_rt_float);

	float plane = p.y;

	float fabric = opUnionRounded(bubbles, plane, blasen_smooth_rt_float);
	MatWrap m_fabric = MatWrap(fabric, newMaterialId(mat_id_fabric, p));


	float rotekugel = fSphere(p - blasen_rotekugel_p_rt_vec3, blasen_rotekugel_r_rt_float);
	MatWrap m_rotekugel = MatWrap(rotekugel, newMaterialId(mat_id_rotekugel, p));

	MatWrap m = mUnion(m_fabric, m_rotekugel);

	mUnion(m);
	return m.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
//	return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	vec3 ambient = ambientColor(normal, -direction, material) + emission;

	return ambient;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));

	if (materialId.id == mat_id_rotekugel) {
		mat.color = blasen_rotekugel_color_rt_color;
		mat.roughness = 0.;
		mat.metallic = 1.;
	}

	if (materialId.id == mat_id_fabric) {
		mat.roughness = blasen_roughness_rt_float;
		mat.color = blasen_color_rt_color;

		vec3 tex_hit = materialId.coord + normalize(blasen_grid_origin_dir_rt_vec3) * blasen_grid_origin_amount_rt_float;
		pDomrep(tex_hit.x, blasen_grid_dist_rt_float);
		pDomrep(tex_hit.z, blasen_grid_dist_rt_float);

		pRotY(tex_hit, blasen_grid_rot_rt_float * Tau);

		float pattern = f2Box(tex_hit.xz, blasen_grid_dist_rt_float/2, blasen_grid_width_rt_float);
		pattern = min(pattern, f2Box(tex_hit.xz, blasen_grid_width_rt_float, blasen_grid_dist_rt_float/2));

		if (pattern < 0.) {
			mat.color = vec3(1.);
			mat.roughness = 0.;
		}

	}

	return mat;
}
