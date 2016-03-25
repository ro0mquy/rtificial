#include "march.glsl"
#include "layer.glsl"
#include "materials.glsl"
#line 5

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
	pTrans(p, vec2(cos(t/100. * 2 * Tau), sin(t/50. * 2 * Tau)) * 1);
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
	float front_plane = f2Box(p, lay_frame_dim);
	vec2 p_spheres = p;
	pRot(p_spheres, .005 * Tau * t);
	float sphere_rotation_radius = 2 + (t - 4) * .01;
	float f_spheres = Inf;
	if (t >= 4) {
		pTrans(p_spheres.x, sphere_rotation_radius);
		float sphere1 = f2Sphere(p_spheres, 1);
		f_spheres = min(f_spheres, sphere1);
		front_plane = max(front_plane, -abs(sphere1) + .2);
		pTrans(p_spheres.x, -sphere_rotation_radius);
	}
	if (t >= 8) {
		pTrans(p_spheres.x, -sphere_rotation_radius);
		float sphere2 = f2Sphere(p_spheres, 1);
		f_spheres = min(f_spheres, sphere2);
		front_plane = max(front_plane, -abs(sphere2) + .2);
	}
	if (t >= 52) {
		vec2 p_stripes = p;
		float c = .8;
		pTrans(p_stripes.x, c * .25);
		pDomrep(p_stripes.x, c);
		float f_stripes = abs(p_stripes.x) - c * .25;
		f_stripes = max(f_stripes, -f_spheres);
		front_plane = max(front_plane, -f_stripes);
	}
	if (t >= 84) {
		vec2 p_stripes = p;
		float c = .8;
		pTrans(p_stripes.y, c * .25);
		pDomrep(p_stripes.y, c);
		float f_stripes = abs(p_stripes.y) - c * .25;
		f_stripes = max(f_stripes, -f_spheres);
		front_plane = max(front_plane, -f_stripes);
	}
	if (t >= 116) {
		front_plane = f_spheres;
	}
	MatWrap w_front_plane = MatWrap(front_plane, layerMaterialId(p, t));
	if (t >= 132) {
		float delta_t = min(t, torus_slow) - 132;
		if (t >= torus_slow) {
			float delta_slow = t - torus_slow;
			float duration = (torus_stop - torus_slow) * .75;
			delta_t += duration * saturate(delta_slow / duration);
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
	vec3 front_light = layerLight(origin, marched, direction, hit, normal, material);
	return ambientColor(normal, -direction, material) + material.emission + front_light;
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
		return max(0, -f_spheres);
	}
	return f_frame;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	if (materialId.id == id_layer) {
		if (abs(f2Glow(materialId.coord.xy)) < .03) {
			mat.emission = vec3(10 * part_frame_glow_rt_float);
		}
		mat.roughness = .5;
		float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
		mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);
	} else if (materialId.id == id_part) {
		mat.color = vec3(.01);
		mOutline(mat, materialId, vec3(1), 1);
	}
	return mat;
}
