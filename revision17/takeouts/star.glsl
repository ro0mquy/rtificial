#include "march.glsl"
#line 3

float fScene(vec3 p) {
	vec3 p_star = p;
	pMirrorLoco(p_star, star_loco_rt_vec3);
	pMirrorAtPlane(p_star, normalize(star_mirror_rt_vec3), length(star_mirror_rt_vec3));
	pRotY(p_star, star_rot_rt_float * Tau);

	#define func_box_1(q) f2Box(q, .2)
	float f_box = fBoxGeneric(p_star.xy, 1.);
	#undef func_box_1

	#define func_torus_1(q) f2Sphere(q, 4.)
	#define func_box_1(q) f2Box(q, .2)
	#define func_torus_2(q) fBoxGeneric(q, 1.)
	float f_torus = fTorusGeneric(p_star);
	#undef func_torus_1
	#undef func_box_1
	#undef func_torus_2

	vec3 p_krist = p_star;
	pMirrorTrans(p_krist.z, 15.);
	pMirrorLoco(p_krist, vec3(1.));
	pRotY(p_krist, star_krist_rot_rt_float * Tau);
	pRotZ(p_krist, 3. * star_krist_rot_rt_float * Tau);
	#define func_box_1(q) f2Box(q, .2)
	float f_krist = fBoxGeneric(p_krist.xy, 1.);
	#undef func_box_1

	float f_star = min(f_box, f_torus);
	//f_star = min(f_star, f_krist);

	float f = f_star;

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	//return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;
	return mat;
}
