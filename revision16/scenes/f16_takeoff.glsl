#include "march.glsl"
#include "f16.glsl"
#include "lights.glsl"
#line 5

uniform vec2  lay_frame_dim;
uniform float lay_last_layer_index;
uniform float lay_layer_dist;
uniform float lay_layer_thickness;

const float id_layer = 1.;
const float id_ground = 2.;

float fScene(vec3 p) {
	// align front layer
	pTrans(p.z, funk_takeoff_offset_rt_float);
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
	float thickness = lay_layer_thickness + takeoff_thickness_rt_float;
	pMirrorTrans(p_layer.z, thickness);
	float f_layers = opIntersectChamfer(p_layer.z, f_frame, .05 * .25);
	float f = max(f_layers, -f_f16);
	MaterialId m = MaterialId(id_layer, p, vec4(t, vec3(0)));
	//f = max(-f2Box(p.zx, vec2(frame_z, lay_frame_dim.x)), abs(p.y - 5) - .05);
	mUnion(MatWrap(f, m));

	float g = p.y - lay_frame_dim.y * (2 * takeoff_background_height_rt_float - 1) + .3;
	mUnion(MatWrap(g, newMaterialId(id_ground, p)));

	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	// TODO
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
	mat.color = mix(mat.color, lay_color1_rt_color, smoothstep(0.05, 0.3, lay_layer_thickness));

	float glow_intensity = part_glow_intensity_rt_float;
	vec3 glow_color = part_glow_color_rt_color;
	if (materialId.id == id_layer && int(materialId.misc[0] + 1 + takeoff_glow_offset_rt_float) % int(takeoff_num_glow_rt_float) == 0) {
		mat.emission = glow_color * glow_intensity;
	}
	return mat;
}

