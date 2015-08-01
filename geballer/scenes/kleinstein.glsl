#include "march.glsl"
#include "noise.glsl"
#line 4

const float id_kern = 0.;
const float id_kern_deckel = 1.;
const float id_kern_deckel_ecke = 2.;
const float id_kern_deckel_chamfer = 3.;
const float id_inside = 4.;
const float id_rahmen = 5.;
const float id_tribune = 7.;
const float id_tribin = 8.;
const float id_wand = 9.;
const float id_wand_chamfer = 10.;
const float id_tunnel = 11.;
const float id_boden = 12.;
const float id_stairbox_holes = 13.;
const float id_stairbox_stamper = 14.;

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
	float f_balken = f2BoxEdge(p.xz, vec2(1, 2));
	f_balken = opUnionChamfer(f_balken, f2BoxEdge(p.xz, vec2(2, 1)), 1/(sqrt(2) + 1));
	pTrans(p.x, -2);
	return opSubtractChamfer(f_balken, f2BoxEdge(p.xz, vec2(.25)), .25);
}

float fStairBox(vec3 p, float height) {
	pMirrorTrans(p, vec3(klest_stairbox_width_rt_float, height, klest_stairbox_width_rt_float));
	float r = klest_stairbox_chamfer_rt_float;
	float n = klest_stairbox_stairs_rt_float;
	float f_stairbox = opIntersectChamfer(p.x, p.z, r * 2);
	return opIntersectStairs(p.y, f_stairbox, r, n);
}

MatWrap wStairBoxRow(vec3 p) {
	float f_boden = p.y;
	MatWrap w_boden = MatWrap(f_boden, newMaterialId(id_boden, p));

	float i = pDomrepInterval(p.z, klest_stairbox_spacing_rt_float * klest_stairbox_width_rt_float, -3, 2);
	i += 2;

	vec3 p_holes = p;
	float f_holes = fStairBox(p_holes, klest_stairbox_chamfer_rt_float);
	MatWrap w_holes = MatWrap(f_holes, newMaterialId(id_stairbox_holes, p_holes));

	vec3 p_stamper = p_holes;
	float t_stamp = fract(klest_stamp_rt_float + i / 6 * .5);
	float height_stamper = Golden_Ratio * klest_stairbox_width_rt_float;
	pTrans(p_stamper.y, height_stamper + stamp(t_stamp) - klest_stairbox_chamfer_rt_float);
	float f_stamper = fStairBox(p_stamper, height_stamper);
	MatWrap w_stamper = MatWrap(f_stamper, newMaterialId(id_stairbox_stamper, p_stamper));
	w_stamper.m.misc.x = t_stamp;


	MatWrap w = w_boden;
	w = mSubtract(w, w_holes);
	w = mUnion(w, w_stamper);

	return w;
}

float fTripyramid(vec3 p, float phi) {
	vec3 q = vec3(p.xy, abs(p.z));

	vec3 n1;
	n1.xz = cos(phi) * unitVector(Tau / 6);
	n1.y = sin(phi);
	float f1 = fPlane(q, n1);
	float f2 = f2PlaneAngle(q.xy, Pi - phi);

	return max(f1, f2);
}

MatWrap wWand(vec3 p, float phi) {
	pRotZ(p, phi);
	float f_plane = p.x;

	vec3 p_cubes = p;
	pRotX(p_cubes, Tau * .1);
	vec2 cell_cubes = pDomrep(p_cubes.yz, vec2(klest_wand_spacing_rt_float));
	float hash_cubes = .1 + .9 * rand(ivec2(cell_cubes)) * (.3 + .7 * (1. - stamp()));
	float length_cubes = hash_cubes * klest_wand_ratio_rt_float;
	vec3 dim_cubes = vec3(klest_wand_ratio_rt_float - length_cubes, length_cubes, length_cubes);
	float f_cubes = fBoxEdge(p_cubes, dim_cubes);

	float f_wand = opIntersectChamfer(f_plane, -f_cubes, klest_wand_chamfer_rt_float);
	return MatWrap(f_wand, newMaterialId(id_wand, p));
}

float fTunnelPrimitive(vec3 p, float r) {
	pMirror(p.z);
	pRotX(p, Tau / -12.);
	p.z -= .5 * r;
	float f_plane = p.z;

	vec3 p_balken = p;
	p_balken.x -= klest_tunnel_balken_offset_rt_float;
	pDomrep(p_balken.x, klest_tunnel_balken_spacing_rt_float);
	float f_balken = fKlestBalken(p_balken);
	float f = opUnionChamfer(f_plane, f_balken, .4);
	return f;
}

MatWrap wTunnel(vec3 p) {
	float f1 = fTunnelPrimitive(p, klest_gang_r_rt_float);

	p.x -= 30.;
	pMirrorLoco(p, vec3(30., 40., 50.));
	pMirrorAtPlane(p, normalize(klest_tunnel_loco_plane_rt_vec3), length(klest_tunnel_loco_plane_rt_vec3));
	pRotY(p, Tau * klest_tunnel_loco_phi1_rt_float);
	//pRotZ(p, Tau * klest_tunnel_loco_phi2_rt_float);
	//pMirrorLoco(p, vec3(20., 10., 15.));

	float f2 = fTunnelPrimitive(p, klest_gang_r_rt_float);

	float f_tunnel = min(f1, f2);
	return MatWrap(f_tunnel, newMaterialId(id_tunnel, p));
}

void fReaktor(vec3 p) {
	vec3 p_mirror = p;
	vec2 cell_mirror = pMirror(p_mirror.xz);

	// kern
	vec3 p_kern = p_mirror;
	p_kern.y -= klest_kern_dim_rt_vec2.y + klest_kern_amp_rt_float * stamp();
	pRotX(p_kern, Tau * klest_kern_rot_rt_float);
	pRotZ(p_kern, Tau * -klest_kern_rot_rt_float);
	float f_kern = f2CornerEdge(p_kern.xz) - klest_kern_dim_rt_vec2.x;
	f_kern = max(f_kern, abs(p_kern.y) - klest_kern_dim_rt_vec2.y);

	// kern cut stuff
	vec2 p_kern_cut1 = p_mirror.xz;
	p_kern_cut1 -= klest_kern_cut1_pos_rt_float * klest_kern_dim_rt_vec2.x;
	float f_kern_cut1 = f2Box(p_kern_cut1, klest_kern_cut1_dim_rt_float * klest_kern_dim_rt_vec2.x);
	vec2 p_kern_cut2 = p_mirror.xz;
	p_kern_cut2 -= klest_kern_cut2_pos_rt_float * klest_kern_dim_rt_vec2.x;
	float f_kern_cut2 = f2Box(p_kern_cut2, klest_kern_cut2_dim_rt_float * klest_kern_dim_rt_vec2.x);
	f_kern = max(f_kern, -min(f_kern_cut1, f_kern_cut2));
	//f_kern = opIntersectChamfer(f_kern, -opUnionChamfer(f_kern_cut1, f_kern_cut2, klest_kern_cut_chamfer_rt_float), klest_kern_cut_chamfer_rt_float);
	mUnion(f_kern, newMaterialId(id_kern, p_kern));

	// kern deckel
	vec3 p_kern_deckel = p_kern;
	p_kern_deckel.y -= klest_kern_dim_rt_vec2.y + klest_kern_deckel_dim_rt_vec2.y;
	pMirrorAtPlane(p_kern_deckel, normalize(vec3(-1, 0, 1)), 0.);
	pRotY(p_kern_deckel, Tau * klest_kern_deckel_rot_rt_float);
	float f_kern_deckel = fBox(p_kern_deckel, klest_kern_deckel_dim_rt_vec2.xyx);
	MatWrap w_kern_deckel = MatWrap(f_kern_deckel, newMaterialId(id_kern_deckel, p_kern_deckel));

	// kern deckel ecken
	vec3 p_kern_deckel_ecke = p_kern_deckel;
	pMirror(p_kern_deckel_ecke.y);
	p_kern_deckel_ecke -= klest_kern_deckel_offset_rt_float * klest_kern_deckel_dim_rt_vec2.xyx;
	float f_kern_deckel_ecke = fBoxEdge(p_kern_deckel_ecke, klest_kern_deckel_ecke_dim_rt_float);
	MatWrap w_kern_deckel_ecke = MatWrap(f_kern_deckel_ecke, newMaterialId(id_kern_deckel_ecke, p_kern_deckel_ecke));

	w_kern_deckel = mUnion(w_kern_deckel, w_kern_deckel_ecke);
	mUnionChamfer(w_kern_deckel.f, w_kern_deckel.m, klest_kern_deckel_chamfer_rt_float, id_kern_deckel_chamfer);


	// blocks inside kern
	vec3 p_inside = p_mirror;
	p_inside.xz -= klest_kern_cut1_pos_rt_float * klest_kern_dim_rt_vec2.x;
	float f_inside;
	vec3 dim_inside = klest_inside_dim_rt_vec2.xyx;
	dim_inside.y *= dot(cell_mirror, klest_inside_hash_rt_vec2) + 1.;
	f_inside = fBox(p_inside, dim_inside);
	mUnion(f_inside, newMaterialId(id_inside, p_inside));


	// small rahmen blocks
	vec3 p_mirrorgrid = p;
	pMirrorGrid(p_mirrorgrid.xz, 0.);
	vec3 p_rahmen = p_mirrorgrid;
	p_rahmen.x -= klest_kern_dim_rt_vec2.x + klest_rahmen_dim_rt_vec3.x;
	p_rahmen.y -= klest_rahmen_dim_rt_vec3.y;
	float f_rahmen = fBox(p_rahmen, klest_rahmen_dim_rt_vec3);
	mUnion(f_rahmen, newMaterialId(id_rahmen, p_rahmen));


	// stairbox stamper
	vec3 p_stairbox = p_mirror;
	p_stairbox.xz -= klest_stairbox_offset_rt_float;
	pRotY(p_stairbox, Tau / 8);
	MatWrap w_stairbox = wStairBoxRow(p_stairbox);
	mUnion(w_stairbox.f, w_stairbox.m);


	// two arrow tribunes
	vec3 p_tribune = p;
	pMirrorLoco(p_tribune.xz, vec2(0.));
	p_tribune.xz -= klest_trib_trans_rt_vec2;
	pRotY(p_tribune, Tau * klest_trib_angle_outer_rt_float);
	pMirrorLoco(p_tribune.xz, vec2(0.));
	pRotY(p_tribune, Tau * klest_trib_angle_inner_rt_float);
	float f_tribune = f2BoxEdge(p_tribune.zy, klest_trib_r_rt_float);
	mUnion(f_tribune, newMaterialId(id_tribune, p_tribune));

	// block between tribunes and rahmen
	vec3 p_tribin = p_mirrorgrid;
	p_tribin.y -= klest_tribin_dim_rt_vec3.y;
	p_tribin.x -= klest_tribin_pos_rt_float;
	float f_tribin = fBox(p_tribin, klest_tribin_dim_rt_vec3);
	mUnion(f_tribin, newMaterialId(id_tribin, p_tribin));


	// hoehlen wand
	vec3 p_wand = p_mirrorgrid;
	p_wand.x -= klest_wand_pos_rt_float;
	MatWrap w_wand = wWand(p_wand, Tau * klest_wand_angle_rt_float);
	w_wand.f = -w_wand.f;

	// wand & gang
	vec3 p_gang = p_stairbox;
	p_gang.z -= klest_wand2_pos_rt_float;
	MatWrap w_wand2 = wWand(p_gang.zyx, Tau * klest_wand2_angle_rt_float);
	w_wand2.f = -w_wand2.f;
	w_wand = mUnionChamfer(w_wand, w_wand2, klest_wand_chamfer_rt_float, id_wand_chamfer);

	MatWrap w_gang = wTunnel(p_gang.zyx);
	w_wand = mSubtract(w_wand, w_gang);
	mUnionChamfer(w_wand.f, w_wand.m, 1., id_wand_chamfer);


	/*
	float f = f_kern;
	f = opUnionChamfer(f, f_kern_deckel, klest_kern_deckel_chamfer_rt_float);
	f = min(f, f_inside);
	f = min(f, f_rahmen);
	f = min(f, f_stairbox);
	f = min(f, f_tribune);
	f = min(f, f_tribin);
	f = opUnionChamfer(f, f_wand, 1.);

	return f;
	// */
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
	f_wall = opUnionChamfer(f_wall, fKlestBalken(p_balken), .4);

	f_tunnel = opUnionChamfer(f_tunnel, f_wall, .4);
	float f = f_tunnel;
	// */

	//float f = fBorg(p);

	//float f = fStairBox(p);

	fReaktor(p);

	//mUnion(f, newMaterialId(0., p));
	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .5;

	vec3 hills = vec3(58,34,29)/255.;
	vec3 phryne = vec3(89,76,74)/255.;
	vec3 noe = vec3(223,157,85)/255.;
	vec3 warm_wool = vec3(66,52,37)/255.;
	vec3 gizibe = vec3(215,198,185)/255.;

	mat.color = gizibe;

	if (materialId.id == id_kern || materialId.id == id_kern_deckel_ecke) {
		mat.color = hills;
	} else if (materialId.id == id_kern_deckel || materialId.id == id_kern_deckel_chamfer) {
		mat.color = phryne;
	} else if (materialId.id == id_wand || materialId.id == id_tunnel) {
		mat.color = warm_wool;
	} else if (materialId.id == id_stairbox_stamper) {
		mat.color = noe;
	}
	if (materialId.id == id_boden) {
	}
	if (materialId.id == id_wand_chamfer) {
		mat.color = phryne;
	}
	if (materialId.id == id_inside) {
		//mat.emission = noe * 2000;
	}
	if (materialId.id == id_rahmen) {
		//mat.emission = hills * 8000;
	}
	if (materialId.id == id_stairbox_stamper) {
		vec3 p_stamper = materialId.coord;
		float height_stamper = Golden_Ratio * klest_stairbox_width_rt_float;
		p_stamper.y += height_stamper;
		float t_stamp = materialId.misc.x;

		float t_stamp_scaled = max(0., (t_stamp - .9) * 10.);
		float t_glow = step(.9, t_stamp);
		t_glow *= iqPowerCurve(1.3, 4.7, t_stamp_scaled);
		t_glow *= 1. - smoothstep(klest_stairbox_glow_begin_rt_float, klest_stairbox_glow_end_rt_float, length(p_stamper));
		t_glow = pow(t_glow, klest_stairbox_glow_falloff_rt_float);

		mat.emission = vec3(1.) * 1000. * t_glow * klest_stairbox_glow_intensity_rt_float;
	}

	return mat;
}
