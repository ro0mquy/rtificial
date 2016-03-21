#include "march.glsl"
#include "f16.glsl"
#line 4

float fScene(vec3 p) {
	float f = abs(p.z) - .02;
	f = opIntersectRounded(f, fF16Ground(p.xy, 2.), .01);

	float f_background_layer = p.z + 5;
	float f_ground = p.y + .47;
	f_ground = max(f_ground, -p.z - .3);

	f = min(f, f_background_layer);
	f = min(f, f_ground);

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	//return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	SphereLight light1 = SphereLight(
		vec3(.7, 0, -3),
		2,
		begin_light1_color_rt_color,
		50000
	);
	SphereLight light2 = SphereLight(
		vec3(-1.2, 1, -2.5),
		1.,
		begin_light2_color_rt_color,
		2000000
	);
	vec3 sphereLight = applySphereLight(origin, marched, direction, hit, normal, material, light1);
	sphereLight += begin_light_rt_float * applySphereLight(origin, marched, direction, hit, normal, material, light2);
	return ambientColor(normal, -direction, material) + emission + sphereLight;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = 1;
	mat.color = vec3(.0);
	return mat;
}
