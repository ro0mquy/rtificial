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

float bubbleLayer(vec2 p, float t, float r, vec2 spacing, float offset, float amount, float speed) {
	vec2 c = spacing * r * 2;
	pTrans(p, vec2(offset * c.x, t * speed + offset * 1982.));
	ivec2 ij = ivec2(pDomrep(p, c));
	float n = cheapHash(ij);
	if (n <= amount) {
		return f2Sphere(p, 0.5 * r * n * 150);
	} else {
		return -f2Box(p, c);
	}
}

float water_offset = rain_water_offset_rt_float;

float waterHeightUp(float t) {
	return (2 * t - 1) * (lay_frame_dim.y + water_offset);
}

float waterHeightDown(float t) {
	return (1 - 2 * t) * lay_frame_dim.y;
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	float rain_amount = smoothstep(0, 1, (t+24)/128);
	vec2 d = rain_drop_dim_rt_vec2;
	vec2 rain_spacing = rain_spacing_rt_vec2;
	float f1 = rainLayer(p, t, d * rain_scale_3_rt_float, rain_spacing, rain_offset_3_rt_float, rain_amount_3_rt_float * rain_amount, rain_speed_3_rt_float);
	vec2 p_water = p;
	float water_rise_begin = 32;
	float water_rise_end = 176;
	float water_half_t = 144;
	float first_half = saturate((t - water_rise_begin) / (water_half_t - water_rise_begin));
	float second_half = saturate((t - water_half_t) / (water_rise_end - water_half_t));;
	float rain_water_height = .5 * first_half + .5 * second_half;
	if (rain_water_height < 1 && rain_water_height > 0) {
		if (abs(p_water.y - waterHeightUp(rain_water_height)) <= .5) {
			rain_water_height += .02 * waterNoise(vec2(10 * p_water.x / lay_frame_dim.x, t * .3));
		}
	}
	pTrans(p_water.y, waterHeightUp(rain_water_height));
	float f_water = p_water.y;

float bubble_r = .2;
vec2 bubble_spacing = vec2(1.);
float bubble_offset = 3.7;
float bubble_amount = 0.01
	* smoothstep(water_rise_begin, water_rise_begin + 56, t)
	* 1 - smoothstep(188, 200, t);
float bubble_speed = .5;
float f_bubble = bubbleLayer(p_water, t, bubble_r, bubble_spacing, bubble_offset, bubble_amount, bubble_speed);
f_water = max(f_water, -f_bubble);

	float f_inner = min(f1, f_water);

	vec2 p_top_cutout = p;
	pTrans(p_top_cutout.y, water_offset * second_half);
	float f_top_cutout = -f2Box(p_top_cutout, lay_frame_dim);
	if (t < 190) {
		f_frame = max(f_frame, f_top_cutout);
	}

	float rain_water_height_frame = (t - 192) / 24.;
	// -.1 to fix seams due to chamfering
	f_inner = max(f_inner, -f_frame - .1);
	if (rain_water_height_frame > 0.) {
		rain_water_height_frame += .03 * waterNoise(vec2(7 * p.x / lay_frame_dim.x, t * .4));
	}
	pTrans(p.y, waterHeightDown(rain_water_height_frame));
	f_frame = max(p.y, f_frame);
	return MatWrap(f_inner, layerMaterialId(p, t));
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
