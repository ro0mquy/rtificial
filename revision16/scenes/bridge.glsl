#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#line 5

float fGuard(vec2 p, float t) {
	return 0;
}

float opUnionContour(float a, float b, float r) {
	a = max(a, -(abs(b) - r));
	return min(a, b);
}

float f2Car(vec2 p) {
	float f_body = f2BoxRounded(p, vec2(5, 1), .3);
	vec2 p_wheels = p;
	pMirrorTrans(p_wheels.x, 3);
	pTrans(p_wheels.y, -1);
	float f_wheels = f2Sphere(p_wheels, 1);
	vec2 p_top = p;
	pTrans(p_top.x, 1.);
	float f_top = f2BoxRounded(p_top, vec2(3.3, 3), 1.5);
	float f_window = f2BoxRounded(p_top, vec2(2.8, 2.7), 1.0);
	f_top = max(f_top, -f_window);
	f_top = min(f_top, f2Box(p_top, vec2(.2, 3)));
	f_top = max(f_top, -p_top.y);
	f_body = opUnionChamfer(f_body, f_top, .2);
	f_body = opUnionContour(f_body, f_wheels, .1);
	return f_body;
}

float f2Bridge(vec2 p, float scale, float t) {
	vec2 p_pillar = p;
	pMirrorTrans(p_pillar.x, bridge_pillar_distance_rt_float);
	vec2 pillar_dim = scale * bridge_pillar_dim_rt_vec2;
	float pillar_bottom_extension = 10;
	pTrans(p_pillar.y, -pillar_bottom_extension);
	float f_pillar = f2Box(p_pillar, pillar_dim + vec2(0, pillar_bottom_extension));
	pTrans(p_pillar.y, pillar_bottom_extension);

	vec2 p_pillar_top = p_pillar;
	vec2 pillar_top_dim = bridge_pillar_top_dim_rt_vec2 * pillar_dim.x;
	pTrans(p_pillar_top.y, pillar_dim.y + pillar_top_dim.y);
	float f_pillar_top = f2Box(p_pillar_top, pillar_top_dim);

	vec2 p_chains = p_pillar;
	pMirrorTrans(p_chains.x, pillar_dim.x);
	float chain_length = bridge_chain_length_rt_float * scale;
	float platform_height = bridge_platform_height_rt_float * pillar_dim.y;
	vec2 a = vec2(0, pillar_dim.y);
	vec2 b = vec2(chain_length, -pillar_dim.y + platform_height);
	float radius = distance(a, b) * bridge_chain_radius_rt_float;
	vec2 center = centerOfCircle(a, b, radius);
	vec2 p_chains_shape = p_chains;
	pTrans(p_chains_shape, center);
	float f_chains_shape = f2Sphere(p_chains_shape, radius);
	pTrans(p_chains.y, .5 * platform_height);
	f_chains_shape = max(f2Box(p_chains, vec2(chain_length, pillar_dim.y - .5 * platform_height)), -f_chains_shape);
	pDomrep(p_chains.x, bridge_chains_horizontal_width_rt_float
		* 2 * bridge_chains_horizontal_space_rt_float * scale);
	float f_chains = f2Box(p_chains, vec2(bridge_chains_horizontal_width_rt_float * scale, pillar_dim.y));
	f_chains = max(f_chains, f_chains_shape);
	float f_chains_outline = abs(f_chains_shape) - bridge_chains_outline_width_rt_float * scale;
	f_chains = min(f_chains, f_chains_outline);

	vec2 p_ends = p_pillar;
	vec2 ends_dim = bridge_ends_dim_rt_vec2 * scale;
	pTrans(p_ends.x, chain_length + pillar_dim.x + ends_dim.x);
	pTrans(p_ends.y, bridge_ends_offset_rt_float * scale);
	float f_ends = f2Box(p_ends, ends_dim);

	float f = min(f_pillar, f_pillar_top);
	f = min(f, f_chains);
	f = min(f, f_ends);
	pTrans(p.y, -pillar_dim.y + platform_height + .4);
	float car_t = saturate((t-132)/24);
	pTrans(p.x, 8 * (1 - 2 * car_t));
	float f_dings = f2Car(p * 5.) / 5;
	f = min(f, f_dings);

	return f;
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	//return f2Car(p);
	vec2 p_f16 = p;
	pF16Bridge(p_f16, t);
	float f_f16 = fF16Air(p_f16, 4);
	float f_ground = p.y + lay_frame_dim.y;
	vec2 p_bridge = p;
	pTrans(p_bridge.y, -lay_frame_dim.y + 1.3);
	pTrans(p_bridge.x, lay_frame_dim.x + 54);
	float f_bridge = f2Bridge(p_bridge, .8, t);
	// gap
	vec2 p_gap = p_bridge;
	pMirrorTrans(p_gap.x, 8);
	f_ground = max(f_ground, -p_gap.x);
	float f = f_ground;
	f = min(f, f_bridge);
	f_frame = max(min(f_frame, f_f16), -max(f_frame, f_f16));
	return MatWrap(f, layerMaterialId(p, t));
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
