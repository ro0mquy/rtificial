#include "march.glsl"
#line 3

float fInner(vec2 p, float t) {
	pTrans(p, vec2(cos(t * 2 * Tau), sin(t * 2 * Tau)) * 3);
	return f2Box(p, 1);
}

float fScene(vec3 p) {
	float i = pDomrepInterval(p.z, 3., -20, 0);
	float frame = f2Box(p.xy, vec2(5));
	float t = (i/20) +1 + layers_animation_t_rt_float;
	float inner = fInner(p.xy, t);
	pMirrorTrans(p.z, .05);
	float f = p.z;
	f = max(f, min(-frame, inner));

	mUnion(f, MaterialId(0., vec3(p.x, p.y, t)));
	return f;
}

vec3 applyNormalLights2(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	material.color = .5 * normal + .5;
	material.roughness = 1.;
	vec3 color = .1 * ambientColor(normal, -direction, material);
	return color;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	//return applyNormalLights2(origin, marched, direction, hit, normal, material);
	return .1 * ambientColor(normal, -direction, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = 1;
	mat.height = sin(materialId.coord.x * 2) * cos(materialId.coord.y * 2) * .1;
	mat.color = mix(layers_color1_rt_color, layers_color2_rt_color, fract(materialId.coord.z));
	return mat;
}
