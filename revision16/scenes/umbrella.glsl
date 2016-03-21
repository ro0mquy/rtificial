#include "march.glsl"
#include "layer.glsl"
#line 4

float fGuard(vec2 p, float t) {
	return 0;
}

float fFlame(vec2 p, float t, float r, float freq_t, float freq_y, float phase, float amp_x, float curve) {
	pTrans(p.y, .5*r);
	float h = 1.5*r;
	float y = clamp((p.y + .5*r) / h, 0, 1);
	p.x += sin(t/20 * freq_t * Tau  +  y * freq_y * Tau + phase) * pow(y, curve) * amp_x * r * sqrt(3)/2;
	return f2Triprism(p, r);
}

// todo
// - runder griff
// - balancing der einzelanimationen (stretch_x, stretch_y, sphere_morph)
// - mapping für reales t
float fUmbrella(vec2 p, float t, float r) {
	float stretch_x = t;
	float stretch_y = 1/(stretch_x * 1.2);
	float sphere_morph = .431 * t;

	vec2 p_stretch = p;
	p_stretch.x *= stretch_x;

	vec2 p_tri = p_stretch;
	pTrans(p_tri.y, r);
	p_tri.y *= stretch_y;
	pTrans(p_tri.y, -r);
	float top = mix(f2Triprism(p_tri, r), f2Sphere(p_tri, r), sphere_morph);
	top /= max(stretch_y, 1);

	vec2 p_cutout = p_stretch;
	pTrans(p_cutout.y, r - 1.5*r/stretch_y);

	vec2 ps0 = p_cutout;
	float r0 = 0.5*r;
	pTrans(ps0, 0, -r0*0.7);
	float s0 = f2Sphere(ps0, r0);

	vec2 ps1 = p_cutout;
	pMirror(ps1.x);
	float r1 = 0.4*r;
	pTrans(ps1, r1*1.5, -r1*3/4);
	float s1 = f2Sphere(ps1, r1);

	float bottom = min(s0, s1);

	vec2 pline = p;
	float lineb = -1.2*r;
	pTrans(pline, 0, lineb);
	vec3 v = vec3(0);
	v.y = 3.4*r + lineb;
	float line = fLine(vec3(pline, 0), 0.015*r, v);

	//return min(top, bottom);
	return min(line, max(top, -bottom)/max(stretch_x, 1));
}

float fInner(vec2 p, inout float f_frame, float t) {
	float rain = 1000;
	float umbrella = fUmbrella(p, t, umbrella_r_rt_float);

	return min(umbrella, umbrella);
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
