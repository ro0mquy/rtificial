#include "march.glsl"
#line 3 "szenchen"

float fScene(vec3 p) {
	vec3 p_rahmen = p;
	pMirrorLoco(p_rahmen, ldb_rahmen_loco_trans_rt_vec3);
	float f_rahmen = f2Box(p_rahmen.yz, vec2(ldb_rahmen_r_rt_float));


	float cnt_h = ldb_content_progress_rt_float;

	vec3 p_content = p;
	p_content.y -= -ldb_rahmen_loco_trans_rt_vec3.y + ldb_rahmen_loco_trans_rt_vec3.y * cnt_h;
	float f_content = fBox(p_content, ldb_rahmen_loco_trans_rt_vec3 * vec3(1., cnt_h, 1.) - vec3(1. - ldb_content_r_rt_float));

	float f = f_rahmen;
	f = min(f, f_content);

	mUnion(f, MaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;
	//mat.height = sin(materialId.coord.x * 2) * cos(materialId.coord.y * 2) * .1;
	return mat;
}
