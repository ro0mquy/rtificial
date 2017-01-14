#include "march.glsl"
#line 3

const float mat_id_bounding = 0.;
const float mat_id_ext = 1.;

float myTorusTwisted(vec3 p, float rBig, float rSmall) {
	vec2 q = vec2(f2Sphere(p.xz, rBig), p.y);
	float angle = atan(p.z, p.x);
	pRot(q, angle);
	return f2Box(q, rSmall);
}

float myTorusPartial(vec3 p, float rBig, float rSmall, float halfAngle) {
	float r = length(p.xz);
	float angle = atan(p.z, p.x);

	float clampedAngle = angle - clamp(angle, -halfAngle, halfAngle);

	p.xz = r * unitVector(clampedAngle);
	p.x -= rBig;

	pRotZ(p, angle);

	return fBoxRounded(p, rSmall, .1*rSmall);
}

float fScene(vec3 p) {
	vec3 p_torus = p;

	pTrans(p_torus.x, -ext_extrude_h_rt_float);
	float px_before = p_torus.x;
	float px_clamped = clamp(p_torus.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_torus.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	pRotX(p_torus, 2. * (px_param + ext_rot_rt_float) * Tau);
	pTrans(p_torus.z, 10 * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_torus,      (px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_torus, 4. * (px_param + ext_rot_rt_float) * Tau);

	vec2 loco_torus = vec2(ext_obj_loco_rt_float * sin(4. * (px_param + ext_rot_rt_float) * Tau));

	vec3 loco_index = pMirrorLoco(p_torus.xy, loco_torus);
	pRotY(p_torus, ext_obj_rot_rt_float * Tau);

	float f_torus = myTorusPartial(p_torus.yxz, 3., 1., ext_torus_angle_rt_float * Tau);

	MatWrap w_ext = MatWrap(f_torus, MaterialId(mat_id_ext, p_torus, vec4(loco_index, px_before)));

	MatWrap w_bound = MatWrap(-fSphere(p, 100.), newMaterialId(mat_id_bounding, p));
	MatWrap w = mUnion(w_ext, w_bound);

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

	if (materialId.id == mat_id_bounding) {
		mat.color = ext_color_background_rt_color;
	} else if (materialId.id == mat_id_ext) {
		vec3 loco_index = (materialId.misc.xyz + 1.) / 2.; // {0, 1}
		float px_before = (materialId.misc.w/ext_extrude_h_rt_float + 1.) / 2.; // {0,1}

		mat.color = ext_color_torus_rt_color;

		if (loco_index.x > 0) {
			mat.color = ext_color_torus_hightlight_rt_color;
		}
		if (loco_index.y > 0) {
			mat.color += 0.5;
		}
		mat.color = mix(ext_color_fade_rt_color,mat.color, pow(px_before, ext_fade_pow_rt_float));
	}

	return mat;
}
