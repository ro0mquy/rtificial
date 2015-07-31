#include "march.glsl"
#line 3

float fScene(vec3 p) {
	pRotY(p, Tau / 8);
	pMirrorLoco(p.xz, vec2(0));
	pRotY(p, -Tau / 8);
	vec3 p_box = p;
	float f_box = fBox(p_box, vec3(5, 1, 5));

	vec3 p_top = p;
	pTrans(p_top.y, 1);

	vec3 p_fissures = p_top;
	pMirrorLoco(p_fissures.xz, vec2(0));
	float f_fissures = f2BoxEdge(p_fissures.zy, vec2(.1, .5));

	vec3 p_edges = p_top;
	pMirrorTrans(p_edges.xz, vec2(5 - .5));
	float f_edges = fPlane(p_edges, normalize(vec3(1)));

	vec3 p_top_boxes = p_top;
	float top_box_width = 5 / Golden_Ratio * .5;
	pTrans(p_top_boxes.y, top_box_width);
	pMirrorTrans(p_top_boxes.xz, vec2(.15 + .1 + top_box_width + .15 + .15 / 3.));
	float f_top_boxes = fBoxEdge(p_top_boxes, top_box_width);

	vec3 p_top_edges = p_top_boxes;
	pTrans(p_top_edges, vec3(top_box_width - top_box_width * .1 * 2));
	float f_top_edges = fPlane(p_top_edges, normalize(vec3(1)));
	f_top_boxes = max(f_top_boxes, f_top_edges);

	float f = opSubtractStairs(f_box, f_fissures, .15, 3);
	f = max(f_edges, f);
	f = opUnionStairs(f, f_top_boxes, .15, 3);

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;
	return mat;
}
