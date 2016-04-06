#include "march.glsl"
#include "noise.glsl"
#line 3

float id_room = 0;
float id_platform = 1;
float id_sphere1 = 2;
float id_sphere2 = 3;
float id_sphere3 = 4;
float id_sphere4 = 5;

float fScene(vec3 p) {

	vec3 p_room = p;
	float f_room = -fBox(p_room, test_room_size_rt_vec3);

	float f = f_room;
	pTrans(p_room.y, -test_room_size_rt_vec3.y);
	MatWrap w = MatWrap(f_room, newMaterialId(id_room, p_room));

	vec3 p_platform = p_room;
	pTrans(p_platform.y, test_platform_height_rt_float);
	float f_platform = fBox(p_platform,
		vec3(test_sphere_radius_rt_float * 3,
			test_platform_height_rt_float,
			test_sphere_radius_rt_float * 3
		)
	);
	MatWrap w_platform = MatWrap(f_platform, newMaterialId(id_platform, p_platform));

	w = mUnion(w, w_platform);

	vec3 p_spheres = p_platform;
	pTrans(p_spheres.y, test_platform_height_rt_float);
	vec2 sphere_cell = pMirrorTrans(p_spheres.xz, vec2(test_sphere_radius_rt_float * 1.5));
	pTrans(p_spheres.y, test_sphere_radius_rt_float);
	float f_sphere = fSphere(p_spheres, test_sphere_radius_rt_float);
	float id_sphere = dot(max(sphere_cell, 0), vec2(2, 1)) + id_sphere1;
	MatWrap w_sphere = MatWrap(f_sphere, newMaterialId(id_sphere, p_spheres));

	w  = mUnion(w, w_sphere);

	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	//return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	//return ambientColor(normal, -direction, material) + emission;
	vec3 color = emission;
	SphereLight light = SphereLight(
		test_light_position_rt_vec3,
		test_light_radius_rt_float,
		test_light_color_rt_color,
		exp2(test_light_power_rt_float)
	);
	SphereLight light2 = SphereLight(
		test_light2_position_rt_vec3,
		test_light2_radius_rt_float,
		test_light2_color_rt_color,
		exp2(test_light2_power_rt_float)
	);
	color += applySphereLight(origin, marched, direction, hit, normal, material, light);
	color += applySphereLight(origin, marched, direction, hit, normal, material, light2);
	return color;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .7;

	if (materialId.id == id_room) {
		mat.color = test_room_color_rt_color;
		vec3 p = materialId.coord;
		pDomrepGrid(p.zx, vec2(.8));
		float f_grid = f2Box(p.xy, .05);
		float depth = .01;
		mat.height = clamp(f_grid, -depth, depth);
	} else if (materialId.id == id_platform) {
		mat.color = test_platform_color_rt_color;
	} else if (materialId.id == id_sphere1) {
		mat.color = test_sphere1_color_rt_color;
	} else if (materialId.id == id_sphere2) {
		mat.color = test_sphere2_color_rt_color;
		mat.metallic = 1;
		mat.roughness = .8;
		pRotY(materialId.coord, materialId.coord.y * 3);
		mat.height = smoothFbm(materialId.coord * vec3(1, 3, 1)) * .02;
	} else if (materialId.id == id_sphere3) {
		mat.color = test_sphere3_color_rt_color;
		mat.metallic = 1;
	} else if (materialId.id == id_sphere4) {
		mat.color = test_sphere4_color_rt_color;
		mat.emission = mat.color * .08;
	}
	return mat;
}
