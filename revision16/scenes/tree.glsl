#include "march.glsl"
#include "layer.glsl"
#line 3

float pSplit(inout vec2 p, vec2 c, float angle, inout float scale) {
	float s2 = sign(p.y);
	c.y *= .9;
	pTrans(p.y, c.y);
	float s = pMirrorTrans(p.y, c.y);
	if (s > 0.) {
		float side = pMirror(p.x);
		pFlip(c.y);
		pTrans(p, c);
		pRot(p, -angle * Tau);
		float decay = tree_size_decay_rt_float;
		scale *= decay;
		p *= decay;
		pTrans(p, -c);
	}
	return s2;
}

float fInner(vec2 p, inout float f_frame, float t) {
	pTrans(p.x, t * .5 - 3);
	pTrans(p.y, -lay_frame_dim.y - .1);
	p *= 1.6;
	vec2 pTree = p;
	vec2 c = vec2(.4, 2.);
	float scale = 1.;
	float angle = tree_branch_angle_rt_float + .02 * sin(t * .1  * Tau);
	float s = pSplit(pTree, c, angle, scale);
	for (int i = 0; i < 6; i++) {
		pSplit(pTree, c, angle, scale);
	}
	float fTree = f2Box(pTree, c);
	fTree *= s;
	fTree /= scale;
	fTree = max(fTree, -p.y + .1);

	return fTree / 1.6;
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
