#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#include "parl.glsl"
#line 6

float fGuard(vec2 p, float t) {
	return 0;
}

float fInner(vec2 p, inout float f_frame, float t) {
	vec2 c = vec2(11);
	pTrans(p.x, 256 * .3);
	pTrans(p.x, 68);
	pTrans(p.x, c.x);
	pTrans(p.y, -lay_frame_dim.y);
	pTrans(p, vec2(-c.x, c.y));
	float scale = 1;
	float shrink_start = 112;
	float shrink_duration = 32;
	float shrink_t = 1 - smoothstep(0, 1, abs(2 * (t - shrink_start) / shrink_duration - 1));
	scale += shrink_t * 2;
	p *= scale;

	vec2 p_curtain = p;
	pMirrorTrans(p_curtain.x, c.x);
	pFlip(p_curtain.x);
	pTrans(p_curtain.x, 4);
	float hide_start = 0;
	float hide_duration = 56;
	float curtain_hiding = 1 - smoothstep(0, 1, abs(2 * (t - hide_start) / hide_duration - 1));
	pTrans(p_curtain.y, -c.y * (1 - curtain_hiding));
	float f_curtain = f2Corner(p_curtain);

	float mirror_start = 56;
	float rotation = clamp(t - mirror_start, 0, 32) * smoothstep(0, 1, (t - mirror_start)/32) * radians(3);
	float second_rotation = shrink_start + 16;
	rotation -= clamp(t - second_rotation, 0, 32) * smoothstep(0, 1, (t - second_rotation)/32) * radians(3);
	pRot(p, rotation);
	bool schwurbel_mirror = t >= 32;// mirror_start;
	//if (schwurbel_mirror_rt_bool) {
	vec3 s = vec3(0);
	if (schwurbel_mirror) {
		s = pMirrorLoco(p.yx, c);
		pFlip(p.y);
	} else {
		pTrans(p, vec2(c.x, -c.y));
	}
	float schwurbel_rot = smoothstep(0, 1, (t - mirror_start) / 24);
	//pRot(p, radians(22.5) * schwurbel_rot_rt_float);
	pRot(p, radians(22.5) * schwurbel_rot);
	float schwurbel_sep = smoothstep(0, 1, (t - mirror_start) / 64);
	// pTrans(p.x, -c.x + schwurbel_parl_separation_rt_float * c.x * .5);
	pTrans(p.x, -c.x + schwurbel_sep * c.x * .5);
	//pMirrorGrid(p.yx, 3);
	pTrans(p.y, lay_frame_dim.y);
	float f = f2Parl(p);
	if (t < mirror_start) {
		f = min(f, f_curtain);
	}
	f_frame = p.y + lay_frame_dim.y;

	return f / scale;
}

float fScene(vec3 p) {
	mUnion(wLayerEffect(p));

	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return ambientColor(normal, -direction, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .5;
	float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
	mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);
	return mat;
}
