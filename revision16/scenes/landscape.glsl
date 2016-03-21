#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#line 5

float fGuard(vec2 p, float t) {
	return 0;
}

float cheapHash(ivec2 c) {
	int x = 0x3504f333*c.x*c.x + c.y;
	int y = 0xf1bbcdcb*c.y*c.y + c.x;
	return float(x*y)*(2.0/8589934592.0)+0.5;
}

float saw(float p) {
	float y = p;
	float k = .8;
	for (int i = 0; i < 3; i++) {
		y = sin(p + k*y );
	}
	return y;
}

float mountainNoise(float p) {
	int index = int(floor(p));
	float d = smoothstep(0, 1, fract(p));
	float v0 = rand(ivec2(index, 0));
	float v1 = rand(ivec2(index + 1, 0));
	return mix(saw(p * 2), mix(v0, v1, d) * 2 - 1, .5);
}

float mountainFbm(float p) {
	float result = 0.;
	for (int i = 0; i < 3; i++) {
		result += exp2(-i) * mountainNoise(p);
		p *= 2.;
	}
	return result / 1.75;
}

float f2Lighthouse(vec2 p) {
	float f_tower = f2ConeCapped(p, 1, .4, 2.5);
	vec2 p_stripes = p;
	float c = 5. / 3;
	pDomrep(p_stripes.y, c);
	float f_stripes = abs(p_stripes.y) - c * .25;
	vec2 p_stripes2 = p;
	pRot(p_stripes2, radians(30));
	float c2 = .1;
	pDomrep(p_stripes2.y, c2 * 1.5);
	float f_stripes2 = abs(p_stripes2.y) - c2 * .25;
	f_stripes = min(f_stripes, f_stripes2);
	f_stripes = max(f_tower, f_stripes);
	f_tower = min(abs(f_tower) -.05, f_stripes);
	pTrans(p.y, 2.5 + .5);
	pTrans(p.y, .7);
	float f_top = f2ConeCapped(p, .5, 0, .3);
	pTrans(p.y, -.7);
	vec2 p_unmirrored = p;
	pMirrorTrans(p.y, .4);
	f_top = min(f_top, f2Box(p, vec2(.52, .08)));
	p = p_unmirrored;
	pMirrorTrans(p.x, .4);
	f_top = min(f_top, f2Box(p, vec2(.03, .5)));
	p = p_unmirrored;
	pTrans(p.y, -.15);
	float f_lamp = f2BoxRounded(p, vec2(.13, .25), .1);
	f_top = min(f_top, f_lamp);
	f_tower = min(f_top, f_tower);
	return f_tower;
}

float fInner(vec2 p, inout float f_frame, float t) {
	vec2 p_f16 = p;
	pTrans(p_f16, land_f16_offset_rt_vec2);
	pTrans(p_f16.x, (t / 528 * 2 - 1) * land_f16_motion_rt_float);
	float f_f16 = fF16Ground(p_f16, 3);
	float f = f_f16;
	if (t > 8 && t <= 56) {
		vec2 p_mountain = p;
		float mountain_t = saturate((t - 8) / 48);
		pTrans(p_mountain.x, (lay_frame_dim.x + 4.5) * (1 - 2 * mountain_t));
		// mountain
		float n = mountainFbm(p_mountain.x) * .5;
		pRot(p_mountain, radians(5));
		float s = pMirror(p_mountain.x);
		pRot(p_mountain, -radians(50));
		float f_mountain = p_mountain.y - n;
		f = min(f, f_mountain);
	}
	if (t > 64 && t <= 64 + 48) {
		float lighthouse_t = saturate((t - 64) / 48);
		pTrans(p.y, -lay_frame_dim.y + 1.95);
		pTrans(p.x, (lay_frame_dim.x + 1) * (1 - 2 * lighthouse_t));
		f = min(f, f2Lighthouse(p*1.3)/1.3);
	}
	return f;
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
