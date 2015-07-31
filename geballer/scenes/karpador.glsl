#include "march.glsl"
#line 3

const float id_raum = 0.;
const float id_raum_boden = 1.;
const float id_raum_groundhole = 2.;
const float id_retweto_big_torus = 4.;
const float id_retweto_big_glider = 5.;
const float id_retweto_small_torus = 6.;
const float id_retweto_small_glider = 7.;
const float id_blubberbrunnen = 8.;
const float id_blubberbrunnen_smooth = 9.;

uniform float kpd_anim;

const float kpd_blubber_loco_angle = -kpd_anim;
const float kpd_locopyr_rot = .5 * kpd_anim;
const float kpd_raum_loco_rot = kpd_anim;
const float kpd_tweto_loco_angle = -.5 * kpd_anim;
const float tweto_glider_rotation = kpd_anim;
const float tweto_tor_rotation = -kpd_anim;

float fTotwi(vec3 p, float rBig, float rSmall) {
	vec2 q = vec2(f2Sphere(p.xz, rBig), p.y);
	pRot(q, Tau * kpd_locopyr_rot);
	return f2BoxRounded(q, vec2(1., .5) * rSmall, kpd_locopyr_rounded_rt_float);
}

MatWrap wPyramidLoco(vec3 p, float height, float angle) {
	pMirrorLoco(p.zx, vec2(sin(angle) * height));
	pRotX(p, -angle);
	/*
	pDomrepMirror(p.x, kpd_locopyr_domrep_rt_float);
	pRotY(p, Tau * kpd_locopyr_rot);
	float f = f2BoxRounded(p.xz, kpd_locopyr_dim_rt_vec2, kpd_locopyr_rounded_rt_float);
	// */
	pDomrep(p.xy, vec2(kpd_locopyr_domrep_rt_float));
	float f = -p.z;
	p.z -= kpd_locopyr_dim_rt_vec2.y;
	f = smin(f, fTotwi(p.xzy, kpd_locopyr_dim_rt_vec2.x, kpd_locopyr_dim_rt_vec2.y), .5);
	return MatWrap(f, newMaterialId(id_raum, p));
}

MatWrap wRaum(vec3 p) {
	// boden
	float f_boden = p.y - kpd_raum_ground_rt_float;
	MatWrap w_boden = MatWrap(f_boden, newMaterialId(id_raum_boden, p));

	// ringgraben
	vec3 p_groundhole = p;
	p_groundhole.y -= kpd_raum_ground_rt_float;
	float f_groundhole = fHexprismEdge(p_groundhole, kpd_raum_groundhole_dim_rt_vec2.x, kpd_raum_groundhole_dim_rt_vec2.y);
	vec2 q_groundhole = vec2(f_groundhole, p_groundhole.y);
	pMirrorLoco(q_groundhole, vec2(0.));
	pRot(q_groundhole, Tau * kpd_raum_loco_rot);
	f_groundhole = f2BoxEdge(q_groundhole, 1.);
	MatWrap w_groundhole = MatWrap(f_groundhole, newMaterialId(id_raum_groundhole, p_groundhole));

	// loco raum
	p.y -= 50.;
	pMirrorLoco(p.zyx, kpd_raum_trans_rt_vec3.yzx);
	pMirrorTrans(p, kpd_raum_trans_rt_vec3);
	pRotY(p, Tau * kpd_raum_loco_angle_rt_float);
	MatWrap w_raum = wPyramidLoco(p, kpd_raum_h_rt_float, Tau * kpd_raum_angle_rt_float);

	w_raum = mUnion(w_raum, w_boden);
	w_raum = mSubtract(w_raum, w_groundhole);
	return w_raum;
}

MatWrap wTweto(vec3 p, float scale, float my_id_torus, float my_id_glider) {
	vec3 p_tor = p;
	// bounding torus
	float f_torus = fTorus(p_tor, tweto_tor_r_big_rt_float * scale, tweto_tor_r_small_rt_float * scale);
	MatWrap w_torus = MatWrap(f_torus, newMaterialId(my_id_torus, p_tor));
	if (w_torus.f < 1) {
		float r_tor = length(p_tor.xz) - tweto_tor_r_big_rt_float * scale;
		float angle_tor = atan(p_tor.z, p_tor.x);
		vec2 q_tor = vec2(r_tor, p_tor.y);
		pRot(q_tor, angle_tor / 4. * tweto_tor_num_twists_rt_float + Tau * tweto_tor_rotation);
		pMirrorLoco(q_tor, vec2(0.));
		pRot(q_tor, Tau * kpd_tweto_loco_angle);
		w_torus.f = f2Box(q_tor, vec2(.5, 1.) * tweto_tor_r_small_rt_float * scale);
		w_torus.m.coord = vec3(q_tor, angle_tor);
	}

	// glider
	vec3 p_glider = p;
	pRotY(p_glider, Tau * tweto_glider_rotation);
	p_glider.x -= tweto_tor_r_big_rt_float * scale;
	pRotZ(p_glider, Tau / 8. + Tau * tweto_tor_rotation - Tau * tweto_glider_rotation / 4. * tweto_tor_num_twists_rt_float);
	pMirrorTrans(p_glider.xy, vec2(tweto_glider_r_big_rt_float * tweto_tor_r_small_rt_float * sqrt(.5) * scale));
	float f_glider = fSphere(p_glider, tweto_glider_r_small_rt_float * scale);
	MatWrap w_glider = MatWrap(f_glider, newMaterialId(my_id_glider, p_glider));

	return mUnion(w_torus, w_glider);
}

MatWrap wRetweto(vec3 p) {
	// big torus
	p.y -= kpd_retweto_pos_y_rt_float;
	vec3 p_big = p;
	MatWrap w_big = wTweto(p_big, kpd_retweto_scale_outer_rt_float, id_retweto_big_torus, id_retweto_big_glider);

	// small tori
	vec3 p_small = p_big;
	pRotY(p_small, Tau / 6.);
	pDomrepAngle(p_small.xz, 6., 0.);
	p_small.x -= tweto_tor_r_big_rt_float * kpd_retweto_scale_outer_rt_float;
	p_small.z = -p_small.z;
	float small_scale = kpd_retweto_scale_inner_rt_float * kpd_retweto_scale_outer_rt_float;
	MatWrap w_small = wTweto(p_small.xzy, small_scale, id_retweto_small_torus, id_retweto_small_glider);

	return mUnion(w_big, w_small);
}

MatWrap wBlubberbrunnen(vec3 p, float r) {
	pMirrorLoco(p.zyx, vec3(r * kpd_blubber_r_big_rt_float));
	pRotX(p, Tau * kpd_blubber_loco_angle);
	float f_blubber = f2BoxRounded(p.yz, vec2(r * kpd_blubber_r_small_rt_vec2), r * kpd_blubber_r_rounded_rt_float);

	return MatWrap(f_blubber, newMaterialId(id_blubberbrunnen, p));
}

MatWrap wAlleBlubberbrunnen(vec3 p) {
	pMirrorTrans(p.xz, kpd_blubber_pos_rt_vec3.xz);
	p.y -= kpd_blubber_pos_rt_vec3.y;
	pRotY(p, Tau * kpd_blubber_rot_rt_float);
	MatWrap w_alle = wBlubberbrunnen(p, 1.);
	return w_alle;
}

float fScene(vec3 p) {
	// loco raum & ground
	MatWrap w_raum = wRaum(p);
	mUnion(w_raum.f, w_raum.m);

	// brunnen
	MatWrap w_blubberbrunnen = wAlleBlubberbrunnen(p);
	mUnionSmooth(w_blubberbrunnen.f, w_blubberbrunnen.m, kpd_blubber_smooth_rt_float, id_blubberbrunnen_smooth);

	// central recursive tweto thingie
	MatWrap w_retweto = wRetweto(p);
	mUnion(w_retweto.f, w_retweto.m);

	/*
	float f = fRaum(p);
	f = opUnionSmooth(f, fAlleBlubberbrunnen(p), kpd_blubber_smooth_rt_float);
	f = min(f, fRetweto(p));
	// */

	//mUnion(f, newMaterialId(0., p));
	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;

	vec3 ekelhaft = vec3(38,62,54)/255.;
	vec3 lets_celebrate = vec3(49,90,85)/255.;
	vec3 blue = vec3(26,76,107)/255.;
	vec3 egeagb = vec3(59,121,165)/255.;
	vec3 arista = vec3(108,97,125)/255.;

	if (materialId.id == id_retweto_small_torus) {
		vec2 p_torbox = abs(materialId.coord.xy);
		// different colors for different sides of cube
		if (2. * p_torbox.x > p_torbox.y) {
			mat.color = ekelhaft;
		} else {
			mat.color = blue;
		}
	} else if (materialId.id == id_retweto_big_torus) {
		mat.color = blue;
	} else if (materialId.id == id_raum) {
		mat.color = lets_celebrate;
	} else if (materialId.id == id_blubberbrunnen) {
		mat.color = egeagb;
	} else if (materialId.id == id_blubberbrunnen_smooth) {
		mat.color = blue;
	} else if (materialId.id == id_raum_boden) {
		mat.color = arista;
	} else if (materialId.id == id_raum_groundhole) {
		mat.color = ekelhaft;
	}

	return mat;
}
