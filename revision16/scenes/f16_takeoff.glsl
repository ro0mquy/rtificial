#include "march.glsl"
#include "f16.glsl"
#line 4

uniform vec2  lay_frame_dim;
uniform float lay_last_layer_index;
uniform float lay_layer_dist;
uniform float lay_layer_thickness;

float fScene(vec3 p) {
	// TODO does not exactly match layers
	//float frame_z = lay_layer_dist * lay_last_layer_index * .5;
	float frame_z = square(lay_frame_dim.x) / lay_frame_dim.y;
	pTrans(p.z, -frame_z);
	vec2 p_f16 = p.zx;
	float f_f16 = fF16Ground(p_f16, 13);
	float f_frame = fBox(p, vec3(lay_frame_dim, frame_z));
	vec3 p_layer = p;
	pTrans(p_layer.xz, vec2(-lay_frame_dim.x * .5, -frame_z));
	pRotY(p_layer, takeoff_layer_rotation_rt_float * Tau);
	pTrans(p_layer.xz, -vec2(-lay_frame_dim.x * .5, -frame_z));
	float t = pDomrep(p_layer.z, lay_layer_dist);
	pMirrorTrans(p_layer.z, lay_layer_thickness);
	float f_layers = max(p_layer.z, f_frame);;
	float f = max(f_layers, -f_f16);
	MaterialId m = MaterialId(0, p, vec4(t, vec3(0)));
	//f = max(-f2Box(p.zx, vec2(frame_z, lay_frame_dim.x)), abs(p.y - 5) - .05);
	mUnion(MatWrap(f, m));

	float g = min(f, p.y - lay_frame_dim.y * (2 * takeoff_background_height_rt_float - 1) + .3);
	mUnion(MatWrap(g, MaterialId(0, p, vec4(0))));

	return f;
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

