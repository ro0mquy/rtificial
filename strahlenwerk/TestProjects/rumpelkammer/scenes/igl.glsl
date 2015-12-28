#include "march.glsl"
#line 3

float opHollow(float f, float d) {
	return abs(f) - d;
}

float fScene(vec3 p) {
	vec3 p_iglu = p;
	float f_iglu = fSphere(p_iglu, igl_iglu_sphere_r_rt_float);
	f_iglu = abs(f_iglu) - igl_iglu_sphere_thick_rt_float;

	vec3 p_kamin = p_iglu;
	pTrans(p_kamin.y, igl_iglu_sphere_r_rt_float + igl_kamin_h_rt_float);
	float f_kamin_hollow = f2Sphere(p_kamin.xz, igl_kamin_r_rt_float - igl_kamin_thick_rt_float);
	float f_kamin = max(f_kamin_hollow - igl_kamin_thick_rt_float, opHollow(p_kamin.y, igl_kamin_h_rt_float));

	vec3 p_eingang = p_iglu;
	pTrans(p_eingang.z, igl_iglu_sphere_r_rt_float);
	float f_eingang = f2Sphere(p_eingang.xy, igl_eingang_r_rt_float);
	f_eingang = max(f_eingang, -p_eingang.z - igl_iglu_sphere_r_rt_float);

	vec3 p_weg = p_iglu;
	pTrans(p_weg.z, igl_weg_pos_rt_vec2.y);
	pMirrorTrans(p_weg.x, igl_weg_pos_rt_vec2.x);
	pRotY(p_weg, Tau * igl_weg_rot_rt_float);
	float f_weg = fLineX(p_weg.zyx, igl_weg_line_r_rt_float, igl_weg_line_length_rt_float);

	float f_plane = p_iglu.y;

	float f = f_iglu;
	f = opUnionRounded(f, f_kamin, igl_kamin_smooth_rt_float);
	f = max(f, -f_kamin_hollow);
	f = max(f, -f_eingang);
	f = min(f, f_weg);
	//f = min(f, f_plane);

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
