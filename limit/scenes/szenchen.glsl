#include "march.glsl"
#line 3 "szenchen"

const float id_floor = 0.;
const float id_aman = 1.;

float fAman(vec3 p) {
	float r_cube = aman_cube_r_rt_float;
	float d_space = 2. * r_cube;
	p.y -= r_cube;

	float f_foot_left1 = fBox(p, r_cube);
	p.x -= d_space;
	float f_foot_left2 = fBox(p, r_cube);
	p.x -= d_space;
	float f_foot_left3 = fBox(p, r_cube);
	p.x -= d_space;
	float f_foot_left4 = fBox(p, r_cube);
	p.x -= d_space;
	float f_foot_left5 = fBox(p, r_cube);
	p.x -= d_space;

	float f_foot_left = min4(f_foot_left1, f_foot_left2, f_foot_left3, f_foot_left4);
	f_foot_left = min(f_foot_left, f_foot_left5);

	return f_foot_left;
}

float fScene(vec3 p) {
	mUnion(p.y, newMaterialId(id_floor, p));
	mUnion(fAman(p), newMaterialId(id_aman, p));

	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return ambientColor(normal, -direction, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;

	if (materialId.id == id_floor) {
		mat.color = vec3(96., 110., 113.) / 255.;
	} else if (materialId.id == id_aman) {
		mat.color = vec3(.0);
	}
	return mat;
}
