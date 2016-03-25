#include "material.glsl"

// 
void mOutline(inout Material material, MaterialId materialId, vec3 color, float intensity) {
	float thickness = materials_outline_thickness_rt_float;
	float transition = materials_outline_transition_rt_float * thickness;
	material.emission = color * intensity * (1 - smoothstep(thickness, thickness+transition, materialId.misc[1]));
}
