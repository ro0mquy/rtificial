#include "march.glsl"
#line 3 "spoid"

float fScene(vec3 p) {

	vec3 p_spoid = p;
	pRotY(p_spoid, Tau * spoid_rot_rt_float);
	p_spoid.x -= spoid_outer_length_rt_float;

	float f_spoid = f2Spheroid(p_spoid.xz, spoid_outer_length_rt_float, spoid_outer_radius_rt_float);
	vec2 q_spoid = vec2(f_spoid, p_spoid.y);
	float f_spoid_torus = f2Pentaprism(q_spoid, spoid_inner_radius_rt_float);

	vec2 p_cutout = q_spoid;
	p_cutout.x -= spoid_inner_radius_rt_float;
	float f_cutout = f2Box(p_cutout, spoid_cutout_radius_rt_float);
	f_spoid_torus = opIntersectChamfer(f_spoid_torus, -f_cutout, spoid_cutout_chamfer_radius_rt_float);

	vec3 p_cutout2 = p_spoid;
	pRotX(p_cutout2, Tau * spoid_cutout2_rot_rt_float);
	pRotY(p_cutout2, Tau * spoid_cutout2_rot_rt_float);
	pDomrep(p_cutout2, vec3(spoid_cutout2_domrep_c_rt_float));
	float f_cutout2 = fOctahedron(p_cutout2, spoid_cutout2_radius_rt_float);
	f_spoid_torus = opSubtractChamfer(f_spoid_torus, f_cutout2, spoid_cutout2_chamfer_radius_rt_float * spoid_cutout2_radius_rt_float);

	float f = f_spoid_torus;

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
