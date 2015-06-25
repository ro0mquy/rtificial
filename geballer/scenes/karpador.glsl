#include "march.glsl"
#line 3

float fPyramidLoco(vec3 p, float height, float angle) {
	pMirrorLoco(p.zx, vec2(sin(angle) * height));
	pRotX(p, -angle);
	pDomrepMirror(p.x, kpd_locopyr_domrep_rt_float);
	pRotY(p, Tau * kpd_locopyr_rot_rt_float);
	float f = f2BoxRounded(p.xz, kpd_locopyr_dim_rt_vec2, kpd_locopyr_rounded_rt_float);
	return f;
}

float fRaum(vec3 p) {
	float f_boden = p.y - kpd_raum_ground_rt_float;
	vec3 p_groundhole = p;
	p_groundhole.y -= kpd_raum_ground_rt_float;
	float f_groundhole = fHexprismEdge(p_groundhole, kpd_raum_groundhole_dim_rt_vec2.x, kpd_raum_groundhole_dim_rt_vec2.y);
	vec2 q_groundhole = vec2(f_groundhole, p_groundhole.y);
	pMirrorLoco(q_groundhole, vec2(0.));
	pRot(q_groundhole, Tau * kpd_raum_loco_rot_rt_float);
	f_groundhole = f2BoxEdge(q_groundhole, 1.);

	p.y -= 50.;
	pMirrorLoco(p, kpd_raum_trans_rt_vec3.yzx);
	pMirrorTrans(p, kpd_raum_trans_rt_vec3);
	pRotY(p, Tau * kpd_raum_loco_angle_rt_float);
	float f_raum = fPyramidLoco(p, kpd_raum_h_rt_float, Tau * kpd_raum_angle_rt_float);

	f_raum = min(f_raum, f_boden);
	f_raum = max(f_raum, -f_groundhole);
	//f_raum = opSubtractChamfer(f_raum, f_groundhole, kpd_raum_groundhole_smooth_rt_float);
	return f_raum;
}

float fTweto(vec3 p, float scale) {
	vec3 p_tor = p;
	float r_tor = length(p_tor.xz) - tweto_tor_r_big_rt_float * scale;
	float angle_tor = atan(p_tor.z, p_tor.x);
	vec2 q_tor = vec2(r_tor, p_tor.y);
	pRot(q_tor, angle_tor / 4. * tweto_tor_num_twists_rt_float + Tau * tweto_tor_rotation_rt_float);
	pMirrorLoco(q_tor, vec2(0.));
	pRot(q_tor, Tau * kpd_tweto_loco_angle_rt_float);
	float f_torus = f2Box(q_tor, vec2(.5, 1.) * tweto_tor_r_small_rt_float * scale);

	vec3 p_glider = p;
	pRotY(p_glider, Tau * tweto_glider_rotation_rt_float);
	p_glider.x -= tweto_tor_r_big_rt_float * scale;
	pRotZ(p_glider, Tau / 8. + Tau * tweto_tor_rotation_rt_float - Tau * tweto_glider_rotation_rt_float / 4. * tweto_tor_num_twists_rt_float);
	pMirrorTrans(p_glider.xy, vec2(tweto_glider_r_big_rt_float * tweto_tor_r_small_rt_float * sqrt(.5) * scale));
	float f_glider = fSphere(p_glider, tweto_glider_r_small_rt_float * scale);

	float f = f_torus;
	f = min(f, f_glider);
	return f;
}

float fRetweto(vec3 p) {
	p.y -= kpd_retweto_pos_y_rt_float;
	vec3 p_big = p;
	//pRotY(p_big, Tau * reto_big_rot_rt_float);
	/*
	float f_big_outer = f2Pentaprism(p_big.xz, reto_big_outer_r_rt_float);
	vec2 q_big = vec2(f_big_outer, p_big.y);
	float f_big = f2Sphere(q_big, reto_big_inner_r_rt_float);
	// */
	float f_big = fTweto(p_big, kpd_retweto_scale_outer_rt_float);

	vec3 p_small = p_big;
	pRotY(p_small, Tau / 6.);
	pDomrepAngle(p_small.xz, 6., 0.);
	p_small.x -= tweto_tor_r_big_rt_float * kpd_retweto_scale_outer_rt_float;
	//pRotZ(p_small, Tau * reto_small_rot_rt_float);
	p_small.z = -p_small.z;

	/*
	float s_o_r = reto_big_inner_r_rt_float * reto_small_outer_r_rt_float;
	float f_small_outer = f2Sphere(p_small.xy, s_o_r);
	vec2 q_small = vec2(p_small.z, f_small_outer);
	float f_small = f2Pentaprism(q_small, reto_small_inner_r_rt_float);
	// */
	float small_scale = kpd_retweto_scale_inner_rt_float * kpd_retweto_scale_outer_rt_float;
	float f_small = fTweto(p_small.xzy, small_scale);

	/*
	vec3 p_cutout = p_small;
	pDomrepAngle(p_cutout.xy, 5., 0.);
	pMirrorTrans(p_cutout.y, reto_cutout_r_rt_float);
	float f_cutout = p_cutout.y;
	f_small = opSubtractStairs(f_small, f_cutout, reto_cutout_smooth_rt_float, 5.);
	// */

	float f = min(f_big, f_small);

	mUnion(f, MaterialId(0., p));
	return f;
}

float fBlubberbrunnen(vec3 p, float r) {
	pMirrorLoco(p, vec3(r * kpd_blubber_r_big_rt_float));
	pRotX(p, Tau * kpd_blubber_loco_angle_rt_float);
	float f_blubber = f2BoxRounded(p.yz, vec2(r * kpd_blubber_r_small_rt_vec2), r * kpd_blubber_r_rounded_rt_float);

	return f_blubber;
}

float fAlleBlubberbrunnen(vec3 p) {
	pMirrorTrans(p.xz, kpd_blubber_pos_rt_vec3.xz);
	p.y -= kpd_blubber_pos_rt_vec3.y;
	pRotY(p, Tau * kpd_blubber_rot_rt_float);
	float f_alle = fBlubberbrunnen(p, 1.);
	return f_alle;
}

float fScene(vec3 p) {
	float f = fRaum(p);
	f = opUnionSmooth(f, fAlleBlubberbrunnen(p), kpd_blubber_smooth_rt_float);
	f = min(f, fRetweto(p));

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
	return mat;
}
