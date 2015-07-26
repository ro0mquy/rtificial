#include "march.glsl"
#include "noise.glsl"
#line 4

float stamp(float t) {
	t = fract(t);

	float t_up = linstep(.0, klest_stamp_up_rt_float, t);

	float t_down = linstep(klest_stamp_up_rt_float, klest_stamp_up_rt_float + klest_stamp_down_rt_float, t);
	t_down = square(t_down)*t_down;

	return t_up - t_down;
}

float stamp() {
	return stamp(klest_stamp_rt_float);
}

float fKolben(vec3 p) {
	float height = klest_kolben_h_rt_float * stamp();

	vec3 p_stiel = p;
	p_stiel.x -= height;
	float f_stiel = fBox(p_stiel, vec3(height, vec2(klest_kolben_stiel_r_rt_float)));

	vec3 p_kopf = p_stiel;
	p_kopf.x -= height;
	p_kopf.x -= klest_kolben_head_h_rt_float;
	float f_kopf = fBox(p_kopf, vec3(klest_kolben_head_h_rt_float, vec2(klest_kolben_head_r_rt_float)));

	float f = f_kopf;
	f = min(f, f_stiel);
	return f;
}

float fHolerow(vec3 p) {
	float cell = pDomrepInterval(p.z, klest_holerow_domrep_rt_float, 0., 4.);
	vec3 dim = klest_holerow_dim_rt_vec3;
	dim.x *= stamp(klest_stamp_rt_float - cell * klest_holerow_delay_rt_float);
	float f_hole = fBox(p, dim);
	return f_hole;
}

float fSchranke(vec3 p) {
	float cell = pMirrorTrans(p.z, klest_schranke_dim_rt_vec3.z * 2.);
	p.z = -p.z;
	if (cell > 0.) {
		pMirrorTrans(p.z, 2. * klest_schranke_dim_rt_vec3.x);
	}
	pRotY(p, Tau * .25 * stamp());
	float f_schanier = fCylinder(p, 2. * klest_schranke_dim_rt_vec3.x, klest_schranke_dim_rt_vec3.y);
	p.xz -= klest_schranke_dim_rt_vec3.xz;
	float f_schranke = fBox(p, klest_schranke_dim_rt_vec3);
	return min(f_schranke, f_schanier);
}

float fRieglung(vec3 p) {
	p.x -= klest_rieglung_dim_rt_vec3.x;

	vec3 p1 = p;
	p1.y -= klest_rieglung_dim_rt_vec3.y;
	p1.z -= klest_rieglung_amplitude_rt_float * stamp();
	float f1 = fBox(p1, klest_rieglung_dim_rt_vec3);

	vec3 p2 = p;
	p2.y += klest_rieglung_dim_rt_vec3.y;
	p2.z += klest_rieglung_amplitude_rt_float * stamp();
	float f2 = fBox(p2, klest_rieglung_dim_rt_vec3);

	return min(f1, f2);
}

float fBorg(vec3 p) {
	pMirrorGrid(p, klest_borg_r_rt_float + klest_borg_amp_rt_float * stamp());

	vec3 p_side = p;
	float f_side = p_side.x;
	p_side.x = -p_side.x;
	f_side = max(f_side, fPyramid(p_side.yxz, klest_borg_r_rt_float, Tau * .125));

	vec3 p_kolben = p;
	p_kolben.yz -= klest_kolben_pos_rt_vec2;
	float f_kolben = fKolben(p_kolben);

	vec3 p_holerow = p;
	p_holerow.yz -= klest_holerow_pos_rt_vec2;
	float f_holerow = fHolerow(p_holerow);

	vec3 p_schranke = p;
	p_schranke.yz -= klest_schranke_pos_rt_vec2;
	float f_schranke = fSchranke(p_schranke.xzy);

	vec3 p_rieglung = p;
	p_rieglung.yz -= klest_rieglung_pos_rt_vec2;
	float f_rieglung = fRieglung(p_rieglung);

	float f = f_side;
	f = min(f, f_kolben);
	f = max(f, -f_holerow);
	f = min(f, f_schranke);
	f = min(f, f_rieglung);
	return f;
}

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

float fStairBox(vec3 p) {
	float i = pDomrepInterval(p.z, 3, -3, 2);
	i += 3;

	float t_stamp = fract(klest_stamp_rt_float + i / 6 * .5);
	pMirrorTrans(p.y, Golden_Ratio + stamp(t_stamp));

	pMirrorTrans(p, vec3(1, Golden_Ratio, 1));
	float r = .2;
	float n = 4;
	//float f2_stairbox = opIntersectStairs(p.x, p.z, r * 2, n * 2);
	float f2_stairbox = opIntersectChamfer(p.x, p.z, r * 2);
	return opIntersectStairs(p.y, f2_stairbox, r, n);
}

float fScene(vec3 p) {
	/*
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
	// */

	//float f = fBorg(p);

	float f = fStairBox(p);

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