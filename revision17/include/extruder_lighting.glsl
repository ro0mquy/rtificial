#include "shadow.glsl"
#line 3

vec3 applySphereLightWithShadow(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, SphereLight light, float shadow_softness) {
	vec3 L = light.position - hit;
	float lightDistance2 = dot(L, L);
	float lightDistance = sqrt(lightDistance2);
	L /= lightDistance;
	float NoL = pdot(L, normal);

	// diffuse part
	float incomingLight = light.power / (4 * square(Pi) * square(light.radius));
	incomingLight *= diffuseSphereLight(hit, normal, light);
	vec3 diffuse = material.color * (max(0, NoL * incomingLight) / Pi);

	// specular part
	vec3 closestPoint = specularSphereLightClosestPoint(hit, normal, -direction, material.roughness, light);
	float roughnessAlpha = saturate(material.roughness + light.radius / (2 * sqrt(lightDistance2)));
	float closestPointDistance2 = dot(closestPoint, closestPoint);
	closestPoint /= sqrt(closestPointDistance2);
	float normalization = square(material.roughness / roughnessAlpha);
	float incomingLightSpecular = light.power * normalization / (4 * Pi * closestPointDistance2);
	vec3 F_metal = fresnel(-direction, closestPoint, material.color);
	float specular = incomingLightSpecular * NoL * brdfSpecularEpicSmithWithoutFresnel(-direction, closestPoint, normal, material.roughness);

	vec3 F_dielectric = fresnel(-direction, closestPoint, vec3(.04));
	vec3 light_result = mix(diffuse + F_dielectric * specular, specular * F_metal, material.metallic);
	light_result *= light.color;

	// shadows
	float shadowing_value = shadowMarch(hit, L, lightDistance, shadow_softness);
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

	SphereLight light1 = SphereLight(ext_light1_pos_rt_vec3, ext_light1_radius_rt_float, ext_light1_color_rt_color, ext_light1_power_rt_float*1000.);
	vec3 sphere_lighting1 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light1, ext_shadow_softness_rt_float);
	result += sphere_lighting1;

	SphereLight light2 = SphereLight(ext_light2_pos_rt_vec3, ext_light2_radius_rt_float, ext_light2_color_rt_color, ext_light2_power_rt_float*1000.);
	vec3 sphere_lighting2 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light2, ext_shadow_softness_rt_float);
	result += sphere_lighting2;

	SphereLight light3 = SphereLight(ext_light3_pos_rt_vec3, ext_light3_radius_rt_float, ext_light3_color_rt_color, ext_light3_power_rt_float*1000.);
	vec3 sphere_lighting3 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light2, ext_shadow_softness_rt_float);
	result += sphere_lighting3;

	return result;
}

float f_ext_background(vec3 p) {
	vec3 p_bg = p;
	pTrans(p_bg, extbg_offset_rt_vec3);
	vec2 i_bg = pDomrep(p_bg.xz, extbg_domrep_rt_vec2);
	pRotY(p_bg, 9781 * i_bg.y + 31 + extbg_rot_anim_rt_float);
	pRotX(p_bg, 4514 * i_bg.x + 89 + extbg_rot_anim_rt_float);
	float f_bg = fTetrahedron(p_bg, extbg_tetra_r_rt_float);
	return f_bg;
}
