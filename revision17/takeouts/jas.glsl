#include "march.glsl"
#line 3

const float mat_id_roid = 0.;

float fScene(vec3 p) {
	vec3 p_roid = p;
	pMirrorLoco(p_roid.zxy, vec3(jas_loco_radius_rt_float));
	pDomrepAngle(p_roid.xy, 7, jas_repeat_radius_rt_float);
	pRotY(p_roid, jas_cone_angle_rt_float * Tau);
	float f_roid = f2Spheroid(p_roid.xz, jas_roid_l_rt_float, jas_roid_r_rt_float);

	vec3 p_ray = p;
	pMirrorGrid(p_ray.zyx, jas_ray_grid_rt_float);
	pMirrorTrans(p_ray.xy, jas_ray_mirror_offset_rt_vec2);
	float f_ray = f2Hexprism(p_ray.xy, jas_ray_r_rt_float);
	pDomrepInterval(f_ray, jas_ray_r_rt_float * .12, -3, 1);
	//f_ray = 1000;

	vec3 p_hex = p;
	pQuatRotate(p_hex, jas_hex_rot_rt_quat);
	pMirrorGrid(p_hex.yxz, jas_hex_grid_rt_float);
	float f_hex = fHexprism(p_hex, jas_ray_r_rt_float, jas_ray_r_rt_float/2);

	float f = max(f_roid, -f_ray);
	f = min(f, f_hex);

	mUnion(f, newMaterialId(mat_id_roid, p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
//	return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;
	if (materialId.id == 0.) {
		mat.color = jas_roid_color_rt_color;
		mat.metallic = 1.;
	}
	return mat;
}
