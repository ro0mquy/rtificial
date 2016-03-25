#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#line 5

float fGuard(vec2 p, float t) {
	return 0;
}

float cheapHash(ivec2 c) {
	int x = int(0x3504f333)*c.x*c.x + c.y;
	int y = int(0xf1bbcdcb)*c.y*c.y + c.x;
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

float f2LighthouseWrapper(vec2 p, float t) {
	pTrans(p.y, -lay_frame_dim.y + 1.95);
	pTrans(p.x, (lay_frame_dim.x + 1) * (1 - 2 * t));
	return f2Lighthouse(p*1.3)/1.3;
}

float pSplit(inout vec2 p, vec2 c, float angle, inout float scale) {
	float s2 = sign(p.y);
	c.y *= .9;
	pTrans(p.y, c.y);
	float s = pMirrorTrans(p.y, c.y);
	if (s > 0.) {
		float side = pMirror(p.x);
		pFlip(c.y);
		pTrans(p, c);
		pRot(p, -angle * Tau);
		float decay = tree_size_decay_rt_float;
		scale *= decay;
		p *= decay;
		pTrans(p, -c);
	}
	return s2;
}

float f2Tree(vec2 p, float t) {
	float big_scale = mix(2.6, 4., t);
	pTrans(p.x, -(2 * t - 1) * (lay_frame_dim.x + 2.2));
	pTrans(p.y, -lay_frame_dim.y - .1);
	p *= big_scale;
	vec2 pTree = p;
	vec2 c = vec2(.4, 2.);
	float scale = 1.;
	float angle = tree_branch_angle_rt_float + .04 * sin(t * Pi - Pi*.5);
	float s = pSplit(pTree, c, angle, scale);
	for (int i = 0; i < 6; i++) {
		pSplit(pTree, c, angle, scale);
	}
	float fTree = f2Box(pTree, c);
	fTree *= s;
	fTree /= scale;
	fTree = max(fTree, -p.y + .1);

	return fTree / big_scale;
}

float f2Mountain(vec2 p, float t) {
	vec2 p_mountain = p;
	pTrans(p_mountain.y, -.5);
	pTrans(p_mountain.x, (lay_frame_dim.x + 4.5) * (1 - 2 * t));
	// mountain
	float n = mountainFbm(p_mountain.x) * .5;
	pRot(p_mountain, radians(5));
	float s = pMirror(p_mountain.x);
	pRot(p_mountain, -radians(50));
	return p_mountain.y - n;
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	//float f_tree = f2Tree(p, t);
	//f_frame = max(min(f_frame, f_tree), -max(f_frame, f_tree));
	//return f_frame;
	//return max(f2Tree(p, t);
	float start_width = 13 * lay_frame_dim.y / lay_frame_dim.x;;
	float zoomout_duration = 64;

	vec2 p_f16 = p;
	pF16Landscape(p_f16, t);
	float exhaust = saturate(t/64);
	float wheel_rotation = mix(.056, 1., saturate(t / 40));
	float f_f16 = fF16Ground(p_f16, mix(start_width, 4, saturate(t / zoomout_duration)), exhaust, wheel_rotation);
	f_frame = max(min(f_frame, f_f16), -max(f_frame, f_f16));
	float f = Inf;

	float tree_start = 8 + zoomout_duration;
	float tree_duration = 128;
	float lighthouse_start = tree_start + tree_duration;
	float lighthouse_duration = 96;
	float mountain_start = lighthouse_start + lighthouse_duration;
	float mountain_duration = 64;

	if (t >= tree_start && t < tree_start + tree_duration) {
		float f_tree = f2Tree(p, (t - tree_start) / tree_duration);
		f = min(f, f_tree);
	}
	if (t > lighthouse_start && t <= lighthouse_start + lighthouse_duration) {
		float f_lighthouse = f2LighthouseWrapper(p, (t - lighthouse_start) / lighthouse_duration);
		f = min(f, f_lighthouse);
	}
	if (t > mountain_start && t <= mountain_start + mountain_duration) {
		float f_mountain = f2Mountain(p, (t - mountain_start) / mountain_duration);
		f = min(f, f_mountain);
	}
	float f_ground = p.y + lay_frame_dim.y;
	f = min(f, f_ground);

	return MatWrap(f, layerMaterialId(p, t));
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

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .5;
	float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
	mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);
	return mat;
}
