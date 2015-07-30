#include "march.glsl"
#include "noise.glsl"
#line 4

uniform float glum_anim;

const float id_tunnel = 0.;
const float id_strebe = 1.;
const float id_strebe_stairs = 2.;
const float id_signal = 3.;
const float id_spikes = 4.;
const float id_spikes_top = 5.;
const float id_blades = 6.;
const float id_blades_chamfer = 7.;
const float id_floor = 8.;

float fSpimiDisc(vec3 p, float scale, float i, float discAtan) {
	float innerRadius = glum_spimi_radius_rt_float + sin(i) * glum_spimi_radius_var_rt_float;
	innerRadius *= scale;
	// TODO parameterize
	float rotationAngle = Tau / 10 * i / (5 + 3.5 * sin(time));
	pRotY(p, rotationAngle);
	pDomrepAngleWithAtan(p.xz, glum_spimi_n_spikes_rt_float, innerRadius, rotationAngle + discAtan);
	float disc_height = glum_spimi_disc_height_rt_float * scale;
	float d = fBox(p, vec3(disc_height, disc_height, scale));
	pTrans(p.x, disc_height + glum_spimi_spike_length_rt_float * scale);
	float d2 = fConeCapped(p.zxy, disc_height - .05 * scale, disc_height * glum_spimi_spike_cap_rt_float, glum_spimi_spike_length_rt_float * scale);
	return opUnionChamfer(d, d2, .05 * scale);
}

float fSpimi(vec3 p, float scale) {
	float maxSpimiRadius = glum_spimi_radius_rt_float + glum_spimi_radius_var_rt_float;
	maxSpimiRadius += glum_spimi_disc_height_rt_float + glum_spimi_spike_length_rt_float * 2;
	maxSpimiRadius *= scale;
	float f_bounding = max(f2Sphere(p.xz, maxSpimiRadius), -p.y);
	if (f_bounding > 1) {
		return f_bounding;
	}
	float c = 2 * glum_spimi_disc_height_rt_float * scale;
	float i = pDomrepSingle(p.y, c);
	//float i = 0;
	float discAtan = atan(p.z, p.x);
	// TODO use simpler objects as guards
	pTrans(p.y, c);
	float d1 = fSpimiDisc(p, scale, i + 1, discAtan);
	pTrans(p.y, -c);
	float d2 = fSpimiDisc(p, scale, i, discAtan);
	pTrans(p.y, -c);
	float d3 = fSpimiDisc(p, scale, i - 1, discAtan);
	return min(d1, min(d2, d3));
}

float fTunnel(vec3 p) {
	vec3 p_tunnel = p;
	pMirrorLoco(p_tunnel.xy, vec2(7));
	pFlip(p_tunnel.x);
	pMirror(p_tunnel.y);
	pRotZ(p_tunnel, .1 * Tau);
	pDomrepMirror(p_tunnel.z, 3);
	pRotY(p_tunnel, glum_tunnel_pulse_rt_float * Tau);
	pDomrepMirror(p_tunnel.y, 3);
	pRotZ(p_tunnel, glum_tunnel_pulse_rt_float  * Tau);
	float f = p_tunnel.x;
	pTrans(p_tunnel.x, -.3);
	float f2 = fPlane(p_tunnel, unitVector(-.125 * Tau, .2 * Pi).yxz);
	f = opUnionStairs(f, f2, .6, 4);
	return f;
}

float fFloor(vec3 p, float height) {
	vec3 p_floor = p;
	float f_floor = f2BoxEdge(p_floor.xy, vec2(3, height));
	vec3 p_floor_stuetze = p_floor;
	pTrans(p_floor_stuetze.y, -4);
	float f_floor_stuetze = f2BoxEdge(p_floor_stuetze.xy, vec2(.3, 4));
	f_floor = min(f_floor, f_floor_stuetze);
	return f_floor;
}

float fStreben(vec3 p, float atan_value) {
	vec3 p_blades = p;
	pDomrepAngleWithAtan(p_blades.xz, 10, .0, atan_value);
	float f_blades = f2BoxEdge(p_blades.zy, .1);
	return f_blades;
}

float fScene(vec3 p) {
	pMirrorLoco(p.xz, vec2(glum_total_loco_trans_rt_float));
	pRotY(p, Tau * glum_total_loco_rot_rt_float);
	//pRotZ(p, glum_total_rot_rt_float * p.z);

	// domrapping and shit
	vec3 p_domrep_cell = p;
	float floor_height = .3;
	p_domrep_cell.y -= floor_height;
	p_domrep_cell.z -= glum_tunnel_offset_rt_float;
	float i_domrep = pDomrepMirror(p_domrep_cell.z, 2);
	float i_mirror = pMirrorTrans(p_domrep_cell.x, 2);
	float atan_value = atan(p_domrep_cell.z, p_domrep_cell.x);

	// floor
	vec3 p_floor = p;
	float f_floor = fFloor(p_floor, floor_height);
	mUnion(f_floor, newMaterialId(id_floor, p_floor));

	// strebe
	vec3 p_strebe = p_domrep_cell;
	float f_strebe = fStreben(p_strebe, atan_value);
	mUnionStairs(f_strebe, newMaterialId(id_strebe, p_strebe), .1, 4, id_strebe_stairs);

	// tunnel
	float f_tunnel = fTunnel(p);
	mUnion(f_tunnel, newMaterialId(id_tunnel, p));

	// signal
	vec3 p_signal = p_domrep_cell;
	float hash_dom = rand(int(i_domrep * i_mirror));
	float anim_n = 2.;
	float t_pos = 1. - fract(hash_dom + glum_anim / anim_n * (1. + floor(glum_signal_max_velo_rt_float * anim_n * hash_dom)));
	float start_pos = glum_signal_start_rt_float + glum_signal_start_outer_rt_float * step(.5, mod(i_domrep + 1., 3.));
	p_signal.x -= glum_signal_end_rt_float + start_pos * t_pos;
	float f_signal = fBox(p_signal, glum_signal_dim_rt_float);
	mUnion(f_signal, newMaterialId(id_signal, p_signal));

	// spikes base
	vec3 p_spikes = p_domrep_cell;
	pTrans(p_spikes.y, .5);
	float f_spikes = fConeCapped(p_spikes, .4, .3, .5);
	float f_spikes_cut = f2Sphere(p_spikes.xz, glum_spikes_cut_r_rt_float);
	f_spikes = max(f_spikes, -f_spikes_cut);
	mUnion(f_spikes, newMaterialId(id_spikes, p_spikes));

	// spikes top
	pTrans(p_spikes.y, 1 + 1.5 + sin(Tau * (.1 * (i_domrep + 2. * i_mirror) + glum_anim)) * .5 - 2);
	float f_spikes_top = fConeCapped(p_spikes, .3, .02, 1.5);
	f_spikes = min(f_spikes, f_spikes_top);
	mUnion(f_spikes_top, newMaterialId(id_spikes_top, p_spikes));

	// blades (flower ring)
	vec3 p_blades = p_domrep_cell;
	pTrans(p_blades.y, .5);
	//pTrans(p_blades.y, .2 * sin(glum_anim * Tau * .25));
	float blade_side = pMirrorTrans(p_blades.y, .1);
	float blade_rotation = .1 * Tau * glum_anim * blade_side;
	pDomrepAngleWithAtan(p_blades.xz, 10, .0, atan_value + blade_rotation);
	pTrans(p_blades.x, .3);
	pRotZ(p_blades, .3 * sin(Tau * glum_anim));
	float f_blades = f2Triprism(p_blades.zx, .2);
	f_blades = max(f_blades, abs(p_blades.y) - .03);
	mUnionChamfer(f_blades, newMaterialId(id_blades, p_blades), .01, id_blades_chamfer);

	/*
	float f = f_tunnel;
	f = min(f, f_spikes);
	f_strebe = min(f_strebe, f_signal);
	f_floor = opUnionStairs(f_floor, f_strebe, .1, 4);
	f = min(f, f_floor);
	f = opUnionChamfer(f, f_blades, .01);
	// */

	//mUnion(f, newMaterialId(0., p));
	return current_dist;
}

float fSchwurbelScheisse(vec3 p) {
	float radius = 1;
	float fBoundingCylinder = f2Sphere(p.xz, radius);
	// TODO maybe this can be tuned for performance
	if (fBoundingCylinder > radius * .5) {
		return fBoundingCylinder;
	}
	float a = .5 + .5 * 1;
	pRotY(p, sin(a * p.y) + glum_ss_rot_rt_float);
	float lipTwist = length(vec2(radius, Pi * a));
	pDomrepAngle(p.xz, 3, radius);
	pMirror(p.z);
	pFlip(p.z);
	pRotY(p, Tau / 5);
	return p.x / lipTwist;
}

float fScene_old(vec3 p) {
	/* rotationswalzenspimischlonz
	pRotY(p, Tau * time * .2);
	pRotZ(p, Tau * time * .2);
	pMirrorGrid(p, 10);
	p = p.zxy;
	vec3 p_dings = p;
	pRotY(p, Tau * time * .1);
	pRotZ(p, atan(1/sqrt(2)));
	pRotX(p, Tau / 8);
	vec3 p_spimi = p;
	pMirrorGrid(p_spimi, 5);
	float spimi_scale = .2;
	p_spimi.x *= -1;
	pTrans(p_spimi.y, spimi_scale * glum_spimi_disc_height_rt_float);
	float f = fSpimi(p_spimi.yxz, spimi_scale);
	f = min(f, fBox(p, 1));

	pTrans(p_dings.y, -10);
	float f_dings = fHexprism(p_dings, 1, 10);
	f = opUnionChamfer(f, f_dings, .1);
	// */

	/* tunneldings vielleicht
	//pMirrorLoco(p.yx, vec2(12));
	float f;// = f2Triprism(p.xy, 1);
	vec3 p1 = p;
	float i = pDomrep(p1.z, 3);
	vec3 p2 = p1;
	pTrans(p1.x, rand(int(i)) * 10);
	pDomrepMirror(p1.x, 1);
	float sin_v = sin(.5 *  i + 3 * time);
	sin_v = pow(abs(sin_v), .2) * sgn(sin_v);
	pRotZ(p1, Tau / (20 * (1. + .4 * sin_v)));
	f = f2Box(p1.xz, vec2(.1, 1));
	pMirrorTrans(p2.z, 1.25);
	f = opUnionChamfer(f, f2BoxEdge(p2.yz, vec2(1.3, .25)), .1);
	// */

	/* auf-und-zu-mit-klingen
	// more like hakenkreuzdingsi mit schrecklichen stacheln
	float mirroring = pMirrorTrans(p.y, 2 * (.5 * .66 + .125) + .05);
	vec3 p_base = p;
	float f_base = fHexprism(p_base, .5, .1);
	vec3 p_pole1 = p_base;
	pTrans(p_pole1.y, -.5 * .66 - .05);
	float f_pole1 = fCylinder(p_pole1, .1, .5 * .66);
	vec3 p_pole2 = p_pole1;
	pTrans(p_pole2.y, -.5 * .66 - .125);
	float f_pole2 = fCylinder(p_pole2, .05, .125);
	f_pole1 = min(f_pole1, f_pole2);

	vec3 p_blade = p_pole1;
	float f_blade = fCylinder(p_blade, 2, .01);
	if (f_blade < .1) {
		pRotY(p_blade, Tau / 3 * .25 * mirroring);
		pDomrepAngle(p_blade.xz, 3, 1);
		float t = 0.5;
		vec2 mirror_normal = unitVector(Tau * .5 - radians(45) * (1. - t));
		pMirrorAtPlane(p_blade, vec3(mirror_normal.x, 0., mirror_normal.y), 0);
		f_blade = fBox(p_blade, vec3(1, .01, .1));
		pMirror(p_blade.z);
		pTrans(p_blade.z, .1 + .025);
		pTrans(p_blade.x, .01);
		p_blade.x = -p_blade.x;
		pDomrepInterval(p_blade.x, .2, 1, 5);
		p_blade = p_blade.yzx;
		float f_spikes = fConeCapped(p_blade, .01, .0, .05);
		f_blade = min(f_blade, f_spikes);
		f_blade = min(f_blade, p_pole2.y);
	}

	float f = min(f_base, f_pole1);
	f = min(f, f_blade);
	// */

	pMirror(p.xy);
	pRotZ(p, aa_rt_float * Tau);
	pMirrorTrans(p.x, klest_tunnel_width_rt_float);
	pRotZ(p, -Tau / 12 * .5);
	pTrans(p.y, klest_tunnel_height_rt_float);
	pMirrorAtPlane(p, vec3(-unitVector(Tau / 2 - Tau / 12), 0), 0);
	float f_wall = -p.x;
	//float j = pDomrep(p.z, 30);
	//float kipp = Tau * .01;
	//pMirrorDomrepped(kipp, floor(j / 2));
	//pMirrorDomrepped(kipp, floor(j / 3));
	//pRotX(p, kipp);
	//pMirrorTrans(p.z, .8);
	pTrans(p.y, -6);
	pTrans(p.x, .1);
	p = p.xzy;
	float f = fSchwurbelScheisse(p);
	f = smin(f_wall, f, .03);

	mUnion(f, newMaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 color = .1 * applyNormalLights(origin, marched, direction, hit, normal, material);
	color *= 1. + 100. * material.emission.x;
	return color;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;

	if (materialId.id == id_signal) {
		mat.emission.x = 1.;
	}

	return mat;
}
