#include "march.glsl"
#include "extruder_lighting.glsl"
#line 4

const float mat_id_ground = 0.;
const float mat_id_ext = 1.;

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
	pTrans(p_torus.y, 10.);
	pTrans(p_torus.x, ext1_translation_rt_float);

	pTrans(p_torus.x, -ext_extrude_h_rt_float);
	float px_before = p_torus.x;
	float px_clamped = clamp(p_torus.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_torus.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	pRotX(p_torus, 2. * (px_param + ext_rot_rt_float) * Tau);
	pTrans(p_torus.z, 3 * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_torus,      (px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_torus, 4. * (px_param + ext_rot_rt_float) * Tau);

	vec2 loco_torus = vec2(ext1_obj_loco_rt_float * sin(4. * (px_param + ext_rot_rt_float) * Tau));

	vec3 loco_index = pMirrorLoco(p_torus.xy, loco_torus);
	pRotY(p_torus, ext1_obj_rot_rt_float * Tau);

	float f_torus = myTorusPartial(p_torus.yxz, 3., 1., ext1_torus_angle_rt_float * Tau);

	float f_ext = f_torus;
	MatWrap w_ext = MatWrap(f_ext, MaterialId(mat_id_ext, p_torus, vec4(loco_index, px_before)));

	// ground plane
	float f_ground = p.y;
	MatWrap w_ground = MatWrap(f_ground, newMaterialId(mat_id_ground, p));

	// combine everything
	MatWrap w = w_ext;
	w = mUnion(w, w_ground);

	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = applyExtruderLights(origin, marched, direction, hit, normal, materialId, material);
	return result;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));

	if (materialId.id == mat_id_ext) {
		vec3 loco_index = (materialId.misc.xyz + 1.) / 2.; // {0, 1}
		float px_before = (materialId.misc.w/ext_extrude_h_rt_float + 1.) / 2.; // {0,1}

		mat.color = ext1_obj_color_rt_color;
		mat.metallic = 1.;
		mat.roughness = ext1_obj_roughness_rt_float;

	} else if (materialId.id == mat_id_ground) {
		mat.color = vec3(.3);
		mat.roughness = 0.;
	}

	return mat;
}
