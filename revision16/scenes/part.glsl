#include "march.glsl"
#include "layer.glsl"
#include "materials.glsl"
#include "noise.glsl"
#line 6

const float id_part = 1;

float fGuard(vec2 p, float t) {
	return 0;
}

float myTorusTwisted(vec3 p, float rBig, float rSmall, float t) {
	float r = length(p.xz);
	float angle = atan(p.z, p.x);

	p.x = r - rBig;
	p.z = 0.;

	pRotZ(p, angle + Tau * part_rot_small_offset_rt_float * t / 50.);

	return fBoxRounded(p, rSmall, part_r_roundedness_rt_float*rSmall);
}

float f2Part(vec2 p, float t) {
	float duration = 392.;

	float bez_t = t / duration;
	vec2 bez1 = part_bez1_rt_vec2;
	vec2 bez2 = part_bez2_rt_vec2;
	vec2 bez3 = part_bez3_rt_vec2;
	vec2 bez4 = part_bez4_rt_vec2;
	vec2 bez12 = mix(bez1, bez2, bez_t);
	vec2 bez23 = mix(bez2, bez3, bez_t);
	vec2 bez34 = mix(bez3, bez4, bez_t);
	vec2 bez123 = mix(bez12, bez23, bez_t);
	vec2 bez234 = mix(bez23, bez34, bez_t);
	vec2 bez1234 = mix(bez123, bez234, bez_t);
	pTrans(p, bez1234);

	float rot_t = smoothstep(0, duration, t);
	float other_t = smoothstep(.4, .6, rot_t) * 2. - 1.;
	rot_t += part_rot2D_param2_rt_float * (other_t*other_t*other_t - other_t);
	//pRot(p, t * part_rot2D_param_rt_float);
	pRot(p, Tau * rot_t * part_rot2D_param_rt_float);

	pTrans(p, vec2(cos(t/100. * 2 * Tau), sin(t/50. * 2 * Tau)) * part_sin_amp_rt_float);

	vec3 p_tor = vec3(p, 0.);
	// good values for rot_big_offset: 0.145; -0.116
	//pRotY(p_tor, Tau * part_rot_big_offset_rt_float);
	//pRotX(p_tor, Tau * part_rot_big_offset_rt_float);
	pQuatRotate(p_tor, part_rot_rt_quat);
	float f_tor = myTorusTwisted(p_tor.zxy, part_r_big_rt_float, part_r_small_rt_float, t);
	return f_tor;
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	const float torus_stop = 492;
	const float torus_slow = 428;
	if (t >= torus_stop) {
		return MatWrap(Inf, layerMaterialId(p, t));
	}
	vec2 p_part = p;
	float chamfer_fix = 1;
	float front_plane = f2Box(p, lay_frame_dim + chamfer_fix);
	vec2 p_spheres = p;
	pRot(p_spheres, .005 * Tau * t);
	float sphere_rotation_radius = 2 + (t - 4) * .01;
	float f_spheres = Inf;
	if (t >= 4) {
		pTrans(p_spheres.x, sphere_rotation_radius);
		float sphere1 = f2Sphere(p_spheres, 1);
		f_spheres = min(f_spheres, sphere1);
		front_plane = max(front_plane, -abs(sphere1 - .2) + .2);
		pTrans(p_spheres.x, -sphere_rotation_radius);
	}
	if (t >= 8) {
		pTrans(p_spheres.x, -sphere_rotation_radius);
		float sphere2 = f2Sphere(p_spheres, 1);
		f_spheres = min(f_spheres, sphere2);
		front_plane = max(front_plane, -abs(sphere2 - .2) + .2);
	}
	if (t >= 52) {
		vec2 p_stripes = p;
		float c = .8;
		pTrans(p_stripes.x, c * .25);
		pDomrep(p_stripes.x, c);
		float f_stripes = abs(p_stripes.x) - c * .25;
		front_plane = max(front_plane, -f_stripes);
	}
	if (t >= 84) {
		vec2 p_stripes = p;
		float c = .8;
		pTrans(p_stripes.y, c * .25);
		pDomrep(p_stripes.y, c);
		float f_stripes = abs(p_stripes.y) - c * .25;
		front_plane = max(front_plane, -f_stripes);
	}
	front_plane = min(front_plane, f_spheres);
	if (t >= 116) {
		front_plane = f_spheres;
	}
	MatWrap w_front_plane = MatWrap(front_plane, layerMaterialId(p, t));
	w_front_plane.m.misc.y = f_frame;
	if (t >= 132) {
		float delta_t = min(t, torus_slow) - 132;
		if (t >= torus_slow) {
			float delta_slow = t - torus_slow;
			float duration = (torus_stop - torus_slow) * .75;
			delta_t += duration * (smoothstep(-1., 1., delta_slow / duration) - .5) * 2.;
		}
		float f_part = mix(front_plane, f2Part(p_part, delta_t), saturate(delta_t * .05));
		MaterialId mat_part = MaterialId(id_part, vec3(p_part, 0), vec4(0, -f_part, vec2(0)));
		MatWrap w_part = MatWrap(f_part, mat_part);
		w_front_plane = w_part;
	}
	return w_front_plane;
}

float fScene(vec3 p) {
	mUnion(wLayerEffect(p));

	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	if (materialId.id == id_layer && -materialId.misc[1] > lay_frame_border_thickness_rt_float) {
		return background_color_rt_color;
	} else {
		vec3 front_light = layerLight(origin, marched, direction, hit, normal, material);
		return ambientColor(normal, -direction, material) + material.emission + front_light;
	}
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

float f2Glow(vec2 p) {
	float t = lay_animation;
	float f_frame = f2BoxEdge(p, lay_frame_dim);
	float f_spheres = Inf;
	vec2 p_spheres = p;
	pRot(p_spheres, .005 * Tau * t);
	float sphere_rotation_radius = 2 + (t - 4) * .01;
	if (t >= 4) {
		pTrans(p_spheres.x, sphere_rotation_radius);
		float sphere1 = f2Sphere(p_spheres, 1);
		f_spheres = min(f_spheres, sphere1);
		pTrans(p_spheres.x, -sphere_rotation_radius);
	}
	if (t >= 8) {
		pTrans(p_spheres.x, -sphere_rotation_radius);
		float sphere2 = f2Sphere(p_spheres, 1);
		f_spheres = min(f_spheres, sphere2);
	}
	if (part_sphere_cross_glow_rt_bool) {
		pDomrep(p.x, .4);
		float f_stripes = abs(p.x);
		f_stripes = max(f_stripes, -f_spheres);
		f_frame = max(f_frame, -f_stripes);
	}
	if (part_stripes2_glow_rt_bool) {
		pDomrep(p.y, .4);
		float f_stripes = abs(p.y);
		f_stripes = max(f_stripes, -f_spheres);
		f_frame = max(f_frame, -f_stripes);
	}
	if (part_glow_everything_rt_bool) {
		f_frame = max(f_frame, -f_spheres + .05);
		return min(0, -f_frame);
	}
	return f_frame;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	float glow_intensity = part_glow_intensity_rt_float;
	vec3 glow_color = part_glow_color_rt_color;
	if (materialId.id == id_layer) {
		if (abs(f2Glow(materialId.coord.xy)) < .03) {
			mat.emission = glow_color * (glow_intensity * part_frame_glow_rt_float);
		}
		mat.roughness = .0;
		float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
		mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);

		vec2 c = materialId.coord.xy;
		// TODO
		float f_dirt = -materialId.misc[1] * 10;
		f_dirt -= smoothFbm(c) * 2;
		float dirtyness = smoothstep(-1., 5., f_dirt);
		float border_noise = smoothFbm(c * 3) * .5 + .5;
		border_noise *= smoothstep(-5., .5, dirtyness);
		border_noise *= dirtyness;
		border_noise *= .2;
		mat.color = mix(mat.color, background_color_rt_color, border_noise);

		if (int(materialId.misc.x) % 8 == 0) {
			//mOutline(mat, materialId, lay_frame_border_color_rt_color, .2);
		}
	} else if (materialId.id == id_part) {
		mat.color = vec3(.01);
		mOutline(mat, materialId, glow_color, glow_intensity);
	}
	return mat;
}
