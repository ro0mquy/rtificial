#include "march.glsl"
#include "noise.glsl"
#line 4

float fPentaTorus(vec3 p, float rBig, float rSmall) {
	vec2 q = vec2(f2Pentaprism(p.xz, rBig), p.y);
	return f2BoxEdge(q, rSmall);
}

float fKlestStone(vec3 p) {
	// TODO parameters
	pMirrorTrans(p.xz, vec2(.3));
	pMirrorTrans(p.xz, vec2(.9));
	float rot = .2 * 1.2;
	pRotZ(p, rot);
	pRotX(p, -rot);
	float f_bottom_blocks = f2Pentaprism(p.xz, 1);
	vec3 p_block_top = p;
	pTrans(p_block_top.y, 5);
	float f_block_top = f2Plane(p_block_top.yz, vec2(1, 0));
	pDomrep(p.y, .9);
	f_bottom_blocks = opUnionChamfer(f_bottom_blocks, fPentaTorus(p, 1, .1), .1);
	return opIntersectChamfer(f_bottom_blocks, f_block_top, .1);
}

float fKlestBalken(vec3 p) {
	float f_balken = f2Box(p.xz, vec2(1, 2));
	f_balken = opUnionChamfer(f_balken, f2Box(p.xz, vec2(2, 1)), 1/(sqrt(2) + 1));
	pTrans(p.x, -2);
	return opSubtractChamfer(f_balken, f2Box(p.xz, vec2(.25)), .25);
}

float fScene(vec3 p) {
	vec3 p_tunnel = p;
	float f_tunnel = p.y;

	vec3 p_wall = p_tunnel;
	pMirrorTrans(p_wall.x, klest_tunnel_width_rt_float);
	pRotZ(p_wall, -Tau / 12 * .5);
	pTrans(p_wall.y, klest_tunnel_height_rt_float);
	pMirrorAtPlane(p_wall, vec3(-unitVector(Tau / 2 - Tau / 12), 0), 0);
	float f_wall = -p_wall.x;
	vec3 p_balken = p_wall;
	//f_wall = opUnionChamfer(f_wall, fKlestBalken(p_balken), .4);

	f_tunnel = opUnionChamfer(f_tunnel, f_wall, .4);
	float f = f_tunnel;

	mUnion(f, MaterialId(0., p));
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
	mat.roughness = .5;
	return mat;
}
