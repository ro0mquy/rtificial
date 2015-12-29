#include "march.glsl"
#line 3

float myTorusPartial(vec3 p, float rBig, float rSmall, float halfAngle) {
	float r = length(p.xz);
	float angle = atan(p.z, p.x);

	float clampedAngle = angle - clamp(angle, -halfAngle, halfAngle);

	p.xz = r * unitVector(clampedAngle);
	p.x -= rBig;

	pRotZ(p, angle + Tau * part_rot_small_offset_rt_float);

	return fBoxRounded(p, rSmall, .1*rSmall);
}

float myTorusPartialBig(vec3 p, float rBig, float rSmall, float halfAngle) {
	float r = length(p.xz);
	float angle = atan(p.z, p.x);

	float clampedAngle = angle - clamp(angle, -halfAngle, halfAngle);

	p.xz = r * unitVector(clampedAngle);
	p.x -= rBig;

	pRotY(p, angle + Tau * part_rot_big_offset_rt_float);
	pRotX(p, angle + Tau * part_rot_big_offset_rt_float);

	return myTorusPartial(p.zxy, rSmall, part_mult_rt_float*rSmall, halfAngle);
}

float fScene(vec3 p) {
	float f_tor = myTorusPartialBig(p, 1./part_mult_rt_float*part_r_big_rt_float, 1./part_mult_rt_float*part_r_small_rt_float, Tau * part_angle_rt_float);

	float f = f_tor;

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
