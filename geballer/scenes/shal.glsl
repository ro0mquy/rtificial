#include "march.glsl"
#line 3 "shal"

float fTorusHex(vec3 p, float rBig, float rSmall) {
	vec2 q = vec2(f2Sphere(p.xz, rBig), p.y);
	return f2Hexprism(q, rSmall);
}

float fScene(vec3 p) {
	vec2 p_decke = p.zy;
	pMirror(p_decke.x);
	p_decke.y -= shal_gang_height_rt_float;
	float f_decke = -f2PlaneAngle(p_decke, Tau * shal_gang_decke_angle_rt_float);
	float f_gang = min(p.y, f_decke);

	vec3 p_saulen = p;
	float cell_saulen = pDomrep(p_saulen.x, shal_saulen_domrep_rt_float);
	float mirror_width = shal_saulen_width_rt_float * (.75 + .25 * sin(Tau * (cell_saulen * shal_saulen_welle_freq_rt_float - shal_saulen_welle_offset_rt_float)));
	pMirrorTrans(p_saulen.z, mirror_width);
	p_saulen.y -= shal_gang_height_rt_float * .33;
	p_saulen.z -= -shal_saulen_biegung_rt_float;
	float f_saulen = fTorusHex(p_saulen.zxy, shal_saulen_biegung_rt_float, shal_saulen_radius_rt_float);

	float f = opUnionChamfer(f_gang, f_saulen, shal_gang_saulen_smooth_r_rt_float);

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
	mat.height = sin(materialId.coord.x * 2) * cos(materialId.coord.y * 2) * .1;
	return mat;
}
