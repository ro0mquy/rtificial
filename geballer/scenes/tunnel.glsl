#include "march.glsl"
#include "noise.glsl"
#line 4 "rotator"

float fSupport(vec3 p, float radius) {
	float rBig = radius;
	float rSmall = tunnel_support_radius_rt_float * rBig;
	rBig -= rSmall;
	vec2 q = vec2(f2Hexprism(p.xy, rBig), p.z);
	vec2 q_cutout = q;
	pRot(q, Tau / 6 * .5);
	float d_torus = f2Hexprism(q, rSmall);
	float width = tunnel_support_cutout_width_rt_float * rSmall;
	float depth = tunnel_support_cutout_depth_rt_float * rSmall;
	pDomrepAngle(q_cutout, 6, rSmall - depth);
	float d_cutout = f2Box(q_cutout, vec2(depth, width - depth));
	d_torus = opSubtractStairs(d_torus, d_cutout, depth, 2);
	return d_torus;
}

float fScene(vec3 p) {
	float d = fSphere(p, 1);

	float tunnel_radius = tunnel_radius_rt_float;
	float distance_between_support = tunnel_support_spacing_rt_float * tunnel_radius;
	vec3 p_tunnel = p;
	pRotZ(p_tunnel, Tau * tunnel_crazy_z_rt_float);
	pDomrepGrid(p_tunnel.zxy, vec3(distance_between_support * 3));
	pRotZ(p_tunnel, -Tau * tunnel_crazy_z_rt_float);
	pDomrepGrid(p_tunnel.xz, vec2(distance_between_support * 5));
	pRotY(p_tunnel, tunnel_crazy_y_rt_float * Tau);
	pDomrepGrid(p_tunnel.zx, vec2(distance_between_support * 3));
	pTrans(p_tunnel.z, distance_between_support * .5);
	float tunnel_height = tunnel_radius * (Golden_Ratio - 1.);
	pCutAndExtrude(p_tunnel, vec3(0, -tunnel_height * .5, 0), vec3(0, tunnel_height, 0));
	pTrans(p_tunnel.x, -tunnel_height);
	pCutAndExtrude(p_tunnel, vec3(0), vec3(tunnel_height * 2, 0, 0));
	vec3 v = vec3(0, tunnel_height, 0);
	vec3 p_support = p_tunnel;
	float d_tunnel = -f2Hexprism(p_tunnel.xy, tunnel_radius);
	d = d_tunnel;

	pDomrep(p_support.z, distance_between_support);
	float d_support = fSupport(p_support, tunnel_radius);
	d = min(d, d_support);

	mUnion(d, MaterialId(0, p));
	return d;
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
