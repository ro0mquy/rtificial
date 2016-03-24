#include "march.glsl"
#include "layer.glsl"
#line 4

float fGuard(vec2 p, float t) {
	return 0;
}

float cheapHash(ivec2 c) {
  int x = 0x3504f333*c.x*c.x + c.y;
  int y = 0xf1bbcdcb*c.y*c.y + c.x;
  return float(x*y)*(2.0/8589934592.0)+0.5;
}

float waterNoise(vec2 p) {
	ivec2 index = ivec2(floor(p));
	vec2 d = smoothstep(0, 1, fract(p));
	float result = 0;
	float v00 = rand(index);
	float v01 = rand(index + ivec2(0, 1));
	float v10 = rand(index + ivec2(1, 0));
	float v11 = rand(index + ivec2(1, 1));
	return mix(mix(v00, v10, d.x), mix(v01, v11, d.x), d.y) * 2. - 1.;
}

float f2Drop(vec2 p, vec2 d) {
	pTrans(p.y, d.x - d.y);
	float f = f2Sphere(p, d.x);
	pTrans(p.y, d.y - d.x * .5);
	return min(f, f2ConeCapped(p, d.x, 0,  d.y - d.x * .5));
}

float rainLayer(vec2 p, float t, vec2 d, vec2 spacing, float offset, float amount, float speed) {
	pRot(p, .02 * Tau);
	vec2 c = spacing * d * 2;
	pTrans(p, vec2(offset * c.x, -t * speed + offset * 1982.));
	ivec2 ij = ivec2(pDomrep(p, c));
	float n = cheapHash(ij);
	if (n <= amount) {
		return f2Drop(p, d);
	} else {
		return -f2Box(p, c);
	}
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	vec2 d = rain_drop_dim_rt_vec2;
	float f1 = rainLayer(p, t, d, vec2(10, 1.5), .1, .05, 1.3);
	//float f2 = rainLayer(p, t, d * 1.1, vec2(13, 4), 10.5, .04, 1.5);
	//float f3 = rainLayer(p, t, d * .8, vec2(14, 9), 10.8, .06, 1.2);
	//return min(min(f1, f2), f3);

	vec2 p_water = p;
	float rain_water_height = saturate(t / 192.);
	if (rain_water_height < 1) {
		rain_water_height += .02 * waterNoise(vec2(10 * p_water.x / lay_frame_dim.x, t * .3));
	}
	pTrans(p_water.y, (2 * rain_water_height - 1) * lay_frame_dim.y );
	float f_water = p_water.y;
	float f_inner = min(f1, f_water);

	float rain_water_height_frame = (t - 224) / 32.;
	// -.1 to fix seams due to chamfering
	f_inner = max(f_inner, -f_frame - .1);
	rain_water_height_frame += .03 * waterNoise(vec2(7 * p.x / lay_frame_dim.x, t * .4));
	pTrans(p.y, (1 - 2 * rain_water_height_frame) * lay_frame_dim.y );
	f_frame = max(p.y, f_frame);
	return MatWrap(f_inner, layerMaterialId(p, t));
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
