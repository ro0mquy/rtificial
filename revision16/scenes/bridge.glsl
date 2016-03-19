#include "march.glsl"
#include "layer.glsl"
#line 4

float f2Bridge(vec2 p, float scale) {
	vec2 p_pillar = p;
	pMirrorTrans(p_pillar.x, bridge_pillar_distance_rt_float);
	vec2 pillar_dim = scale * bridge_pillar_dim_rt_vec2;
	float f_pillar = f2Box(p_pillar, pillar_dim);

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

	float f = min(f_pillar, f_pillar_top);
	f = min(f, f_chains);

	return f;
}

float fInner(vec2 p, inout float f_frame, float t) {
	return f2Bridge(p, .5);
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
