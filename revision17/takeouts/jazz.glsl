#include "march.glsl"
#line 3

const float mat_id_penta = 0.;
const float mat_id_torus = 1.;
const float mat_id_bounding = 2.;

float fScene(vec3 p) {
	vec3 p_penta = p;
	pRotZ(p_penta, jazz_inner_rot_rt_float*Tau);

	if (jazz_penta_outside_rt_float > 0.) {
		float angle = atan(p_penta.x, p_penta.y) / Tau * jazz_domrep1_rt_vec2.x * jazz_circle_num_rt_float;
		float radius = length(p_penta.xy) - jazz_circle_r_rt_float;
		p_penta.yx = vec2(radius, angle);

		pDomrep(p_penta.x, jazz_domrep1_rt_vec2.x);
		pDomrepInterval(p_penta.z, jazz_domrep1_rt_vec2.y, -1., 1.);
	}

	pMirrorLoco(p_penta, jazz_loco_offset_rt_vec3);
	pRotY(p_penta, Tau*jazz_rot_rt_float);
	float f_penta = fPentaprismEdge(p_penta, jazz_penta_r_rt_float, jazz_penta_h_rt_float);
	MatWrap m_penta = MatWrap(f_penta, newMaterialId(mat_id_penta, p_penta));

	vec3 p_torus = p;
	//pRotZ(p_torus, jazz_inner_rot_rt_float*Tau);
	float f_torus = fTorusBox(p_torus, jazz_torus_r_big_rt_float, jazz_torus_r_small_rt_float);
	pRotZ(p_torus, 1./4.*Tau);
	f_torus = min(f_torus, fTorusBox(p_torus, jazz_torus_r_big_rt_float, jazz_torus_r_small_rt_float));
	MatWrap m_torus = MatWrap(f_torus, newMaterialId(mat_id_torus, p_torus));

	MatWrap m = mUnion(m_penta, m_torus);

	MatWrap w_bound = MatWrap(-fSphere(p, 100.), newMaterialId(mat_id_bounding, p));
	m = mUnion(m, w_bound);

	mUnion(m);
	return m.f;
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
	Material mat = defaultMaterial(vec3(0));

	if (materialId.id == mat_id_penta) {
		mat.color = jazz_penta_color_rt_color;
		mat.emission = 1000. * jazz_penta_color_rt_color * jazz_penta_emission_rt_float;
	} else if (materialId.id == mat_id_torus) {
		mat.color = vec3(1.);
		mat.metallic = 1.;
		mat.roughness = jazz_torus_roughness_rt_float;
	}
	return mat;
}
