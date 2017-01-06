#include "march.glsl"
#line 3

float pDomrepGridOnlyX(inout vec2 p, float c) {
	float i = pDomrepMirror(p.x, c);
	p.x -= .5 * c;
	if (p.x > p.y) {
		p.xy = p.yx;
	}
	return floor(.5 * i);
}

float fScene(vec3 p) {
	vec3 p_cyl = p;
	float atan_cyl = atan(p_cyl.x, p_cyl.z) + Tau * cyc_rot_global_rt_float;
	pDomrepAngleWithAtan(p_cyl.zx, cyc_domrep_angle_num_rt_float, cyc_domrep_angle_r_rt_float, atan_cyl);
	pTrans(p_cyl.x, 4.);
	pDomrepGridOnlyX(p_cyl.xz, 8.);
	pRotY(p_cyl, Tau * cyc_rot_cell_rt_float);
	pTrans(p_cyl.xz, cyc_trans_inner_rt_vec2);

	float f_cyl = f2Box(p_cyl.zx, 1., 2.*Golden_Ratio);
	pDomrepInterval(f_cyl, cyc_f_domrep_rt_float, 0., 1.);
	f_cyl = f2Sphere(vec2(f_cyl, p_cyl.y), cyc_f_thickness_rt_float);

	float f = f_cyl;
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
