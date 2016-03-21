#include "march.glsl"
#include "layer.glsl"
#include "f16.glsl"
#line 4

float fInner(vec2 p, inout float f_frame, float t) {
	float f = fF16Ground(vec2(t * .5, p.x), 15);
	f = -min(f_frame, f);

	f_frame = Inf;
	return f;
}

float fScene(vec3 p) {
	mUnion(wLayerEffect(p));

	float f = p.y - lay_frame_dim.y + .2;
	MaterialId m = MaterialId(0, p, vec4(0));
	//mUnion(MatWrap(f, m));

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
