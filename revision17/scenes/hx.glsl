#include "march.glsl"
#line 3

const float mat_id_hexarrow = 0.;
const float mat_id_bounding = 1.;
const float mat_id_hexrot = 2.;

MatWrap hexarrow(vec3 p) {
	pRotY(p, Tau/4.);
	vec3 offset = vec3(-(hx_hexarrow_outer_border_rt_float + hx_hexarrow_inner_border_rt_float) * 9./8., 0., 0.);

	float f1 = fHexprism(p, hx_hexarrow_r_rt_float, hx_hexarrow_r_rt_float/2.);
	MatWrap w1 = MatWrap(f1, newMaterialId(mat_id_hexarrow, p));

	pTrans(p, offset);
	float f2 = fHexprism(p, hx_hexarrow_r_rt_float, hx_hexarrow_r_rt_float/2.);
	MatWrap w2 = MatWrap(f2, newMaterialId(mat_id_hexarrow, p));

	pTrans(p, offset);
	float f3 = fHexprism(p, hx_hexarrow_r_rt_float, hx_hexarrow_r_rt_float/2.);
	MatWrap w3 = MatWrap(f3, newMaterialId(mat_id_hexarrow, p));

	return mUnion(w1, mUnion(w2, w3));
}

MatWrap hexrot(vec3 p) {
	pRotY(p, Tau/4.);
	pRotZ(p, -Tau/8.);

	float f = fHexprism(p, hx_hexarrow_r_rt_float, hx_hexarrow_r_rt_float/2.);
	float cutout = fHexprism(p, hx_hexarrow_r_rt_float/2., hx_hexarrow_r_rt_float);
	f = max(f, -cutout);
	MatWrap w = MatWrap(f, newMaterialId(mat_id_hexrot, p));

	return w;
}

float fScene(vec3 p) {
	vec3 p_hexarrow = p;
	pDomrepInterval(p_hexarrow, hx_hexarrow_domrep_c_rt_vec3, vec3(1), vec3(3,3,100));
	MatWrap w_hexarrow = hexarrow(p_hexarrow);
	MatWrap w = w_hexarrow;

	vec3 p_hexrot = p;
	// TODO positioning
	MatWrap w_hexrot = hexrot(p_hexrot);
	w = mUnion(w, w_hexrot);

	MatWrap w_bound = MatWrap(-fSphere(p, 100.), newMaterialId(mat_id_bounding, p));
	w = mUnion(w, w_bound);

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
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = 1.;

	if (materialId.id == mat_id_bounding) {
		mat.color = vec3(0.);
	} else if (materialId.id == mat_id_hexarrow) {
		mat.color = hx_hexarrow_color_outer_rt_color;

		vec3 tex_hit = materialId.coord;

		float border = f2Hexprism(tex_hit.xz, hx_hexarrow_r_rt_float - hx_hexarrow_outer_border_rt_float);
		float inner = f2Hexprism(tex_hit.xz, hx_hexarrow_r_rt_float - hx_hexarrow_outer_border_rt_float - hx_hexarrow_inner_border_rt_float);

		if (border < 0.) {
			mat.color = hx_hexarrow_color_border_rt_color;

			if (inner < 0.) {
				mat.color = hx_hexarrow_color_rt_color;
			}
		}

		// TODO arrows

		float edge_inner_height = hx_hexarrow_r_rt_float/2.-hx_hexarrow_outer_border_rt_float/2.;
		float edge = f2Box(tex_hit.yz, edge_inner_height, hx_hexarrow_r_rt_float);
		float stripe = f2Box(tex_hit.yz, edge_inner_height/3., hx_hexarrow_r_rt_float);

		if (edge < 0.) {
			mat.color = hx_hexarrow_color_border_rt_color;

			if (stripe < 0.) {
				mat.color = hx_hexarrow_color_rt_color;
			}
		}
	}
	// TODO hexrot

	return mat;
}