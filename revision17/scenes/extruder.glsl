#include "march.glsl"
#line 3

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

	//*
	pTrans(p_torus.x, -ext_extrude_h_rt_float);
	float px_before = p_torus.x * ext_extrude_freq_rt_float;
	p_torus.x -= clamp(p_torus.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);

	pRotX(p_torus, 2. * (px_before + ext_rot_rt_float) * Tau);
	pRotY(p_torus,      (px_before + ext_rot_rt_float) * Tau);
	pRotZ(p_torus, 4. * (px_before + ext_rot_rt_float) * Tau);

	float ext_obj_loco_rt_float = ext_obj_loco_rt_float * sin(4. * (px_before + ext_rot_rt_float) * Tau);
	// */

	pMirrorLoco(p_torus.xy, vec2(ext_obj_loco_rt_float));
	pRotY(p_torus, ext_obj_rot_rt_float * Tau);
	float f_torus = myTorusPartial(p_torus.yxz, 3., 1., ext_torus_angle_rt_float * Tau);


	float f = f_torus;

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
