#include "march.glsl"
#include "f16.glsl"
#line 4

float id_lights = 1;

float fScene(vec3 p) {
	float f = abs(p.z) - .02;
	f = opIntersectRounded(f, fF16Ground(p.xy, 2.), .01);

	float f_background_layer = p.z + 5;
	float f_ground = p.y + .47;
	f_ground = max(f_ground, -p.z - .3);

	f = min(f, f_background_layer);
	f = min(f, f_ground);

	MatWrap w_scene = MatWrap(f, newMaterialId(0, p));

	vec3 p_lights = p;
	pTrans(p_lights.y, -.47);
	pTrans(p_lights.xz, begin_lights_offset_rt_vec2);
	float i_light = pDomrepInterval(p_lights.x, 1.2, 0, 7);
	float f_lights = fCylinder(p_lights, .05, .01);
	MatWrap w_lights = MatWrap(f_lights, newMaterialId(id_lights, p));
	w_lights.m.misc.w = i_light;

	w_scene = mUnion(w_scene, w_lights);

	mUnion(w_scene);
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	//return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	PointLight light1 = PointLight(
		begin_light1_position_rt_vec3,
		begin_light1_color_rt_color,
		exp2(begin_light1_intensity_rt_float)
	);
	PointLight light2 = PointLight(
		begin_light2_position_rt_vec3,
		begin_light2_color_rt_color,
		exp2(begin_light2_intensity_rt_float)
	);
	vec3 sphereLight = applyPointLight(origin, marched, direction, hit, normal, material, light1);
	sphereLight += begin_light_rt_float * applyPointLight(origin, marched, direction, hit, normal, material, light2);
	return ambientColor(normal, -direction, material) + emission + sphereLight;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = 1;
	mat.color = vec3(.01);
	if (materialId.id == id_lights) {
		mat.emission = vec3(.5);
		if (int(materialId.misc.w) == int(begin_light_index_rt_float)) {
			mat.emission += 30 * begin_light_rt_float;
		}
	}
	return mat;
}
