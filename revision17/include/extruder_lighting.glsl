#include "shadow.glsl"
#line 3

vec3 applyExtruderLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = vec3(0.);

	vec3 emission = material.emission;
	result += emission;

	vec3 ambient_lighting = ambientColor(normal, -direction, material);
	//if (materialId.id == mat_id_ground) {
		result += ambient_lighting;
	//}

	SphereLight light = SphereLight(ext_light_pos_rt_vec3, ext_light_radius_rt_float, ext_light_color_rt_color, ext_light_power_rt_float*1000.);
	vec3 sphere_lighting = applySphereLight(origin, marched, direction, hit, normal, material, light) * light.color;

	vec3 dir_to_light = normalize(ext_light_pos_rt_vec3 - hit);
	float length_to_light = distance(ext_light_pos_rt_vec3, hit);
	float shadowing_value = shadowMarch(hit, dir_to_light, length_to_light, ext_shadow_softness_rt_float);
	sphere_lighting *= shadowing_value;
	result += sphere_lighting;

	SphereLight light2 = SphereLight(ext_light2_pos_rt_vec3, ext_light2_radius_rt_float, ext_light2_color_rt_color, ext_light2_power_rt_float*1000.);
	vec3 sphere_lighting2 = applySphereLight(origin, marched, direction, hit, normal, material, light2) * light2.color;

	vec3 dir_to_light2 = normalize(ext_light2_pos_rt_vec3 - hit);
	float length_to_light2 = distance(ext_light2_pos_rt_vec3, hit);
	float shadowing_value2 = shadowMarch(hit, dir_to_light2, length_to_light2, ext_shadow_softness_rt_float);
	sphere_lighting2 *= shadowing_value2;
	result += sphere_lighting2;

	SphereLight light3 = SphereLight(ext_light3_pos_rt_vec3, ext_light3_radius_rt_float, ext_light3_color_rt_color, ext_light3_power_rt_float*1000.);
	vec3 sphere_lighting3 = applySphereLight(origin, marched, direction, hit, normal, material, light3) * light3.color;

	vec3 dir_to_light3 = normalize(ext_light3_pos_rt_vec3 - hit);
	float length_to_light3 = distance(ext_light3_pos_rt_vec3, hit);
	float shadowing_value3 = shadowMarch(hit, dir_to_light3, length_to_light3, 4.* ext_shadow_softness_rt_float);
	sphere_lighting3 *= shadowing_value3;
	result += sphere_lighting3;

	return result;
}
