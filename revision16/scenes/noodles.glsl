#include "march.glsl"
#include "layer.glsl"
#line 4

// I can do all the art with noodles now, which is more fun
// wants its p centered on bottom edge of the frame
// top-/bottom_params: vec4(freq, phase, width, offset)
// wobble_params: vec4(freq_t, freq_x, amp_x, phase_t)
float fNoodle(vec2 p, float t, float r, vec4 top_params, vec4 bottom_params, vec4 wobble_params) {
	t = t/20;
	pTrans(p, sin(t * Tau * top_params.x + top_params.y) * lay_frame_dim.x * top_params.z + top_params.w, 0);
	p.x += sin(t * Tau * wobble_params.x + wobble_params.y * Tau * (p.y+lay_frame_dim.y)/(lay_frame_dim.y*2) + wobble_params.w) * wobble_params.z;
	vec3 v = vec3(0);
	v.xy = vec2(sin(t * Tau * bottom_params.x + bottom_params.y) * lay_frame_dim.x * bottom_params.z + bottom_params.w, lay_frame_dim.y*2);
	return fLine(vec3(p, 0), r, v);
}

float fInner(vec2 p, inout float f_frame, float t) {
	pTrans(p, 0, -lay_frame_dim.y);
	return fNoodle(p, t, 0.05, vec4(1,0,0.125,0), vec4(1,0,0.125,0), vec4(1,1,0.25,0));
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
