#include "march.glsl"
#include "noise.glsl"
#line 3

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

float fScene(vec3 p) {
	vec3 p_tunnel = p;
	float tunnel_side = pMirrorTrans(p_tunnel.x, klest_tunnel_width_rt_float);
	float f_tunnel = f2PlaneAngle(p_tunnel.xy, -Tau / 12 * klest_tunnel_wall_angle_rt_float + .5 * Tau);
	vec3 p_decke = p_tunnel;
	pTrans(p_decke.y, klest_tunnel_height_rt_float);
	f_tunnel = min(f_tunnel, f2PlaneAngle(p_decke.xy, -Tau * klest_tunnel_ceil_angle_rt_float));
	f_tunnel = min(f_tunnel, f2Plane(p_tunnel.xy, vec2(0, 1)));

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
