#include "march.glsl"
#line 3

const float mat_id_plane = 0.;
const float mat_id_cyc = 1.;

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
	float c_circle = pDomrepAngleWithAtan(p_cyl.zx, cyc_domrep_angle_num_rt_float, cyc_domrep_angle_r_rt_float, atan_cyl);
	pTrans(p_cyl.x, 4.);
	float c_grid = pDomrepGridOnlyX(p_cyl.xz, 8.);
	pRotY(p_cyl, Tau * cyc_rot_cell_rt_float);
	pTrans(p_cyl.xz, cyc_trans_inner_rt_vec2);

	float f_cyl = f2Box(p_cyl.zx, 1., 2.*Golden_Ratio);
	float c_distfunc = pDomrepInterval(f_cyl, cyc_f_domrep_rt_float, 0., 1.);
	f_cyl = f2Box(vec2(f_cyl, p_cyl.y), cyc_f_thickness_rt_float);

	MatWrap w_cyl = MatWrap(f_cyl, MaterialId(mat_id_cyc, p_cyl, vec4(c_circle, c_grid, c_distfunc, 0.)));

	float f_plane = p.y;
	MatWrap w_plane = MatWrap(f_plane, newMaterialId(mat_id_plane, p));

	MatWrap w = mUnion(w_cyl, w_plane);
	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	//return applyNormalLights(origin, marched, direction, hit, normal, material) + emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1.));
	mat.roughness = 0.1;

	if (materialId.id == mat_id_plane) {
		mat.color = cyc_color_ground_rt_color;
		mat.roughness = 1.;
	} else if (materialId.id == mat_id_cyc) {
		float c_circle = (materialId.misc.x + floor(cyc_domrep_angle_num_rt_float/2.));// / (cyc_domrep_angle_num_rt_float-1);
		float c_grid = materialId.misc.y / 2. + .5; // 0,1
		float c_distfunc = materialId.misc.z; // null oder 1 nicer wert

		if (c_distfunc > 0) {
			mat.color = 2* cyc_color_circle_rt_color;
		} else {
			mat.color = cyc_color_2_rt_color;
		}
		if (c_grid > 0) {
			mat.color += 0.5;
		}
	}

	return mat;
}
