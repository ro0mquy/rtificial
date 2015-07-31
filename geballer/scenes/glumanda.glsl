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
	float cell_tunnel = pDomrepMirror(p_tunnel.z, 3);
	float tunnel_pulse = glum_tunnel_pulse_rt_float;
	tunnel_pulse += glum_tunnel_pulse_amp_rt_float * sin(Tau * glum_anim);
	pRotY(p_tunnel, tunnel_pulse * Tau);
	pDomrepMirror(p_tunnel.y, 3);
	pRotZ(p_tunnel, tunnel_pulse * Tau);
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
	vec3 cell_mirror_total = pMirrorLoco(p.xz, vec2(glum_total_loco_trans_rt_float));
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
	mUnion(f_tunnel, MaterialId(id_tunnel, p, vec4(cell_mirror_total, 0.)));

	// signal
	vec3 p_signal = p_domrep_cell;
	float hash_dom = rand(int(i_domrep * i_mirror));
	float anim_n = 1.;
	float t_anim = fract(hash_dom + glum_anim / anim_n * (1. + floor(glum_signal_max_velo_rt_float * anim_n * hash_dom)));
	float t_pos = 1. - smoothstep(0., .9, t_anim);
	float start_pos = glum_signal_start_rt_float + glum_signal_start_outer_rt_float * step(.5, mod(i_domrep + 1., 3.));
	p_signal.x -= glum_signal_end_rt_float + start_pos * t_pos;
	float t_radius = 1. - smoothstep(.9, 1., t_anim);
	float f_signal = fBox(p_signal, glum_signal_dim_rt_float * t_radius);
	MaterialId mat_signal = newMaterialId(id_signal, p_signal);
	mat_signal.misc.xy = vec2(t_anim, hash_dom);
	mUnion(f_signal, mat_signal);

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

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 emission = material.emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;

	vec3 adpating_is_hard = vec3(63,124,126)/255.;
	vec3 murky = vec3(41,40,57)/255.;
	vec3 hex = vec3(157,29,8)/255.;
	vec3 tatiana = vec3(214,47,39)/255.;
	vec3 zwo = vec3(242,110,4)/255.;

	// glow
	if (materialId.id == id_signal) {
		float t_anim = materialId.misc.x;
		float hash_dom = materialId.misc.y;

		float t_final_hit = min(1., t_anim + glum_signal_impulse_offset_rt_float);
		t_final_hit = iqPowerCurve(glum_signal_impulse_k_rt_float, 3., t_final_hit);

		float n_wobbel = 2. + floor(3. * hash_dom);
		float t_mid_wobbel = mod(t_anim + hash_dom, 1./n_wobbel) * n_wobbel;
		t_mid_wobbel = iqPowerCurve(23., 3., t_mid_wobbel);
		//t_mid_wobbel = smoothstep(0., 1., t_mid_wobbel);
		//t_mid_wobbel = square(sin(Pi * t_mid_wobbel));

		float glow_intensity = t_final_hit + glum_signal_glow_mid_rt_float * t_mid_wobbel + glum_signal_glow_base_rt_float;
		glow_intensity /= 1. + glum_signal_glow_mid_rt_float + glum_signal_glow_base_rt_float;
		glow_intensity *= 1000. * glum_signal_glow_total_rt_float;

		mat.emission = glow_intensity * adpating_is_hard;
	} else if (materialId.id == id_tunnel) {
		vec3 p_tunnel = materialId.coord;
		vec3 cell_mirror = materialId.misc.xyz;

		float t_height = linstep(glum_tunnel_glow_start_rt_float - glum_tunnel_glow_height_rt_float, glum_tunnel_glow_start_rt_float, p_tunnel.y);
		t_height = pow(t_height, glum_tunnel_glow_falloff_rt_float);

		float pos_length = mult(cell_mirror) * p_tunnel.z - glum_anim * glum_tunnel_glow_stripes_small_rt_float * glum_tunnel_glow_stripes_big_rt_float;
		float t_length = max(0., iqPowerCurve(23., 3., fract(pos_length / glum_tunnel_glow_stripes_small_rt_float)));
		t_length *= max(0., iqPowerCurve(23., 3., fract(pos_length / glum_tunnel_glow_stripes_small_rt_float / glum_tunnel_glow_stripes_big_rt_float)));

		float t_glow = t_height * t_length;

		mat.emission = 1000. * t_glow * glum_tunnel_glow_intensity_rt_float * adpating_is_hard;
	}

	if (materialId.id == id_tunnel) {
		mat.color = hex;
	} else if (materialId.id == id_strebe || materialId.id == id_strebe_stairs || materialId.id == id_spikes || materialId.id == id_spikes_top) {
		mat.color = murky;
	} else if (materialId.id == id_floor) {
		mat.color = tatiana;
	} else if (materialId.id == id_blades || materialId.id == id_blades_chamfer) {
		mat.color = murky;
	}

	return mat;
}
