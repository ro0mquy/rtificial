#include "shadow.glsl"
#line 3

vec3 applyPointLightWithShadow(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, PointLight light, float shadow_softness) {
	vec3 light_result = applyPointLight(origin, marched, direction, hit, normal, material, light);

	// shadows
	vec3 lightVector = normalize(light.position - hit);
	float lightDistance = distance(light.position, hit);
	float shadowing_value = shadowMarch(hit, lightVector, lightDistance, shadow_softness);
	light_result *= shadowing_value;

	return light_result;
}

vec3 applySphereLightWithShadow(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, SphereLight light, float shadow_softness) {
	vec3 light_result = applySphereLight(origin, marched, direction, hit, normal, material, light);

	// shadows
	vec3 lightVector = normalize(light.position - hit);
	float lightDistance = distance(light.position, hit);
	float shadowing_value = shadowMarch(hit, lightVector, lightDistance, shadow_softness);
	light_result *= shadowing_value;

	return light_result;
}

vec3 applyExtruderLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = vec3(0.);

	vec3 emission = material.emission;
	result += emission;

	vec3 ambient_lighting = ambientColor(normal, -direction, material);
	//if (materialId.id == mat_id_ground) {
		result += ambient_lighting;
	//}

	SphereLight light1 = SphereLight(extlight_1_pos_rt_vec3, extlight_1_radius_rt_float, extlight_1_color_rt_color, extlight_1_power_rt_float*1000.);
	vec3 sphere_lighting1 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light1, extlight_shadow_softness_rt_float);
	result += sphere_lighting1;

	SphereLight light2 = SphereLight(extlight_2_pos_rt_vec3, extlight_2_radius_rt_float, extlight_2_color_rt_color, extlight_2_power_rt_float*1000.);
	vec3 sphere_lighting2 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light2, extlight_shadow_softness_rt_float);
	result += sphere_lighting2;

	SphereLight light3 = SphereLight(extlight_3_pos_rt_vec3, extlight_3_radius_rt_float, extlight_3_color_rt_color, extlight_3_power_rt_float*1000.);
	vec3 sphere_lighting3 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light3, extlight_shadow_softness_rt_float);
	result += sphere_lighting3;

	return result;
}
