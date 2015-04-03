#include "scene_head.glsl"
#include "rtificial.glsl"
#include "background.glsl"
#include "biene.glsl"
#include "noise.glsl"
#include "fels_color.glsl"
#line 7

const float fels_id = 1.;
const float biene_id = 2.;
const float dellen_id = 3.;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 100., screenDist);

	vec3 out_color;
	if (isinf(t)) {
		o.y += 10.;
		out_color.rgb = environmentColor(o, d, 100.);
	} else {
		vec3 p = o + t * d;
		vec3 normal = calc_normal(p, false);
		float material = f(p, true)[1];
		float rough = 1.;
		float metallic = 0.;
		vec3 col = vec3(1.);
		if (material == fels_id) {
			fels_color(p, col, rough);
		} else if (material == biene_id) {
			metallic = 1.;
			col = biene_color_rt_color;
			rough = biene_rough_rt_float;
		} else if (material == dellen_id) {
			metallic = 1.;
			col = biene_dellen_color_rt_color;
			rough = biene_dellen_rough_rt_float;
		}

		out_color.rgb = ambientColor(normal, -d, col, rough, metallic);
	}
	output_color(out_color, t);
}

vec2 bee_body(vec3 p, bool last_step) {
	// anim thorax
	float biene_body_thorax_height_rt_float = mix(biene_body_thorax_height_rt_float, 0.,
			smoothstep(0., 1., biene_body_anim_rt_float));
	float biene_body_thorax_stretch_rt_float = mix(1., biene_body_thorax_stretch_rt_float,
			smoothstep(0., 1., biene_body_anim_rt_float));

	// anim head
	float biene_body_head_radius_rt_float = mix(0., biene_body_head_radius_rt_float,
			smoothstep(1., 2., biene_body_anim_rt_float));
	float biene_body_head_stretch_rt_float = mix(1., biene_body_head_stretch_rt_float,
			smoothstep(1., 2., biene_body_anim_rt_float));
	float biene_body_head_offset_rt_float = mix(biene_body_head_offset_rt_float, 0.,
			smoothstep(1., 2., biene_body_anim_rt_float));

	// anim abdomen
	float biene_body_radius_rt_float = mix(0., biene_body_radius_rt_float,
			smoothstep(2., 3., biene_body_anim_rt_float));
	float biene_body_abdomen_offset_rt_float = mix(-.1, biene_body_abdomen_offset_rt_float,
			smoothstep(2., 3., biene_body_anim_rt_float));
	float biene_body_bend_rt_float = mix(0., biene_body_bend_rt_float,
			smoothstep(2., 3., biene_body_anim_rt_float));
	float biene_body_length_rt_float = mix(1e-6, biene_body_length_rt_float,
			smoothstep(2., 3., biene_body_anim_rt_float));
	float biene_body_abdomen_smooth_rt_float = mix(0., biene_body_abdomen_smooth_rt_float,
			smoothstep(2., 2.5, biene_body_anim_rt_float));

	// anim dellen
	float dellen_anim = mix(0., 1., smoothstep(3., 4., biene_body_anim_rt_float));

	// anim mouth
	float biene_mouth_height_rt_float = mix(.5, biene_mouth_height_rt_float,
			smoothstep(4., 5., biene_body_anim_rt_float));

	// anim fuel
	float biene_body_head_fuel_length_rt_float = mix(0., biene_body_head_fuel_length_rt_float,
		   smoothstep(5., 6., biene_body_anim_rt_float));
	float biene_body_head_fuel_thick_rt_float = mix(0., biene_body_head_fuel_thick_rt_float,
			smoothstep(5., 5.5, biene_body_anim_rt_float));

	// anim eyes
	float biene_body_head_eye_falldown_rt_float = mix(biene_body_head_eye_falldown_rt_float, 0.,
			smoothstep(6., 7., biene_body_anim_rt_float));
	float biene_body_head_eye_rot_rt_float = mix(biene_body_head_eye_rot_rt_float, 0.,
			smoothstep(6., 7., biene_body_anim_rt_float));


	vec3 p_thorax = p;
	p_thorax.y -= biene_body_thorax_height_rt_float;
	p_thorax.z /= biene_body_thorax_stretch_rt_float;
	float thorax_radius = biene_body_max_thick_rt_float * biene_body_thorax_scale_rt_float;
	float thorax_length = thorax_radius * biene_body_thorax_stretch_rt_float;
	float d_thorax = sphere(p_thorax, thorax_radius);
	d_thorax *= min(biene_body_thorax_stretch_rt_float, 1.);

	vec3 p_head = p;
	float head_radius = thorax_radius * biene_body_head_radius_rt_float;
	float head_length = head_radius * biene_body_head_stretch_rt_float;
	p_head.z += thorax_length + head_length + biene_body_head_offset_rt_float;
	vec3 p_fuel = p_head;
	p_head.z /= biene_body_head_stretch_rt_float;
	vec3 p_mouth = p_head;
	p_head.x /= biene_body_head_w_stretch_rt_float;
	float d_head = sphere(p_head, head_radius);
	d_head *= min(min(biene_body_head_stretch_rt_float, biene_body_head_w_stretch_rt_float), 1.);

	// mouth
	p_mouth.x = abs(p_mouth.x);
	p_mouth.x -= biene_mouth_dist_rt_float;
	p_mouth.z += biene_mouth_front_rt_float * head_length;
	float mouth_height = biene_mouth_height_rt_float;
	float t_mouth = clamp(-p.y / mouth_height, 0., 1.);
	p_mouth.xy *= rot2D(radians(1.) * biene_mouth_bend_rt_float * pow(t_mouth, biene_mouth_bend_power_rt_float));
	p_mouth.y += mouth_height + head_radius * biene_mouth_offset_rt_float;
	float d_mouth = cone(vec3(p_mouth.x, p_mouth.z, -p_mouth.y), normalize(biene_mouth_c_rt_vec2));
	d_mouth = max(d_mouth, p_mouth.y - mouth_height + .5); // cap top
	d_mouth = smax(d_mouth,
		mouth_height * biene_mouth_bottom_rt_float - p_mouth.y,
		biene_mouth_smooth_bottom_rt_float); // cap bottom
	d_head = smin(d_head, d_mouth, head_radius * biene_mouth_smooth_rt_float);

	// eyes TODO
	vec3 p_eyes = p_head;
	p_eyes.x = abs(p_eyes.x);
	p_eyes.y -= biene_body_head_eye_falldown_rt_float;
	p_eyes.zx *= rot2D(radians(biene_body_head_eye_dist_rt_float));
	p_eyes.zy *= rot2D(radians(biene_body_head_eye_height_rt_float));
	//p_eyes.x -= biene_body_head_eye_dist_rt_float;
	p_eyes.z += head_length - biene_body_head_eye_offset_rt_float;
	//p_eyes.xz *= rot2D(radians(biene_body_head_eye_angle_rt_float));
	p_eyes.yz *= rot2D(radians(biene_body_head_eye_rot_rt_float));
	//p_eyes.y /= biene_body_head_eye_stretch_rt_float;
	//float d_eyes = sphere(p_eyes, biene_body_head_eye_radius_rt_float * head_radius);
	float d_eyes = scale(opal, p_eyes.xzy, biene_body_head_eye_scale_rt_float);
	d_head = min(d_head, d_eyes);

	// fuel TODO
	p_fuel.x = abs(p_fuel.x);
	p_fuel.zx *= rot2D(radians(biene_body_head_fuel_dist_rt_float));
	p_fuel.zy *= rot2D(radians(biene_Body_head_fuel_height_rt_float));
	p_fuel.z += head_length;
	float t_fuel = clamp(-p_fuel.z, 0., biene_body_head_fuel_length_rt_float);
	p_fuel.yz *= rot2D(radians(1.) * pow(t_fuel, biene_body_head_fuel_bend_power_rt_float)
		* biene_body_head_fuel_bend_rt_float);
	float fuel_thick = biene_body_head_fuel_thick_rt_float;
	float d_fuel = line(p_fuel, vec3(0., 0., -biene_body_head_fuel_length_rt_float), fuel_thick);
	d_head = smin(d_head, d_fuel, fuel_thick * biene_body_head_fuel_smooth_rt_float);
	// immer nach gefühl

	vec3 p_abdomen = p;
	float abdomen_length = biene_body_length_rt_float + .5 * biene_body_radius_rt_float;
	p_abdomen.z -= (abdomen_length + thorax_length) * (1. + biene_body_abdomen_offset_rt_float);
	float t_body = clamp(p_abdomen.z / (biene_body_length_rt_float + .5 * biene_body_radius_rt_float), -1., 1.);
	p_abdomen.zy *= rot2D(radians(biene_body_bend_rt_float) * (t_body * .5 + .5));
	t_body = clamp(p_abdomen.z / (biene_body_length_rt_float + .5 * biene_body_radius_rt_float), -1., 1.);
	p_abdomen.xy /= (biene_body_max_thick_rt_float - biene_body_min_thick_rt_float) * (1. - t_body * t_body) + biene_body_min_thick_rt_float;

	vec3 a = vec3(0., 0., biene_body_length_rt_float);
	vec3 b = -a;
	float d_abdomen = line(p_abdomen - a, b - a, biene_body_radius_rt_float);

	d_abdomen *= min(biene_body_min_thick_rt_float, 1.);

	float dellen_space = 2. * biene_body_length_rt_float / biene_dellen_num_rt_float;
	vec3 p_dellen = p_abdomen;
	float dellen_cell = +floor(p_dellen.z / dellen_space); // -ceil() for other direction
	dellen_anim -= .5;
	float dellen_current_anim = clamp(dellen_anim * biene_dellen_num_rt_float - dellen_cell, 0., 1.);
	float biene_dellen_depth_rt_float = mix(0., biene_dellen_depth_rt_float,
			smoothstep(0., 1., dellen_current_anim));
	p_dellen.z = domrep(p_dellen.z, dellen_space);
	float d_dellen = smax(
		cylinder(p_dellen.xy, biene_body_radius_rt_float * (1. + biene_dellen_depth_rt_float)),
		abs(p_dellen.z) - biene_dellen_thick_rt_float,
		biene_dellen_smooth_aussen_rt_float * biene_dellen_depth_rt_float
	);
	d_dellen = smax(d_dellen, abs(p_abdomen.z) - biene_body_length_rt_float, biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float);
	vec2 m_dellen = vec2(d_dellen, dellen_id);

	vec2 m_abdomen = vec2(d_abdomen, biene_id);
	m_abdomen = smin_material(m_abdomen, m_dellen, biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float);

	d_head = smin(d_thorax, d_head, biene_body_head_smooth_rt_float * head_radius);
	vec2 m_body = smin_material(m_abdomen, vec2(d_head, biene_id),
		biene_body_abdomen_smooth_rt_float * biene_body_max_thick_rt_float);

	return m_body;
}

vec2 bee_fluegel(vec3 p, bool last_step) {
	float biene_fluegel_rot_rt_float = biene_fluegel_rot_rt_float;
	float biene_fluegel_angle_rt_float = biene_fluegel_angle_rt_float;
	float biene_fluegel_raise_rt_float = biene_fluegel_raise_rt_float;
	vec2 biene_fluegel_c_rt_vec2 = biene_fluegel_c_rt_vec2;
	float biene_fluegel_length_rt_float = biene_fluegel_length_rt_float;
	if (biene_fluegel_anim_rt_float > 0.) {
		biene_fluegel_rot_rt_float = 20. * sin(TAU * biene_fluegel_anim_rt_float);
		biene_fluegel_angle_rt_float = 20. * sin(TAU * (biene_fluegel_anim_rt_float - .25)) - 20.;
		biene_fluegel_raise_rt_float = 20. * sin(TAU * biene_fluegel_anim_rt_float);
	} else {
		biene_fluegel_c_rt_vec2 = mix(vec2(4.487, 0.539), biene_fluegel_c_rt_vec2,
			smoothstep(-2., -1., biene_fluegel_anim_rt_float));
		biene_fluegel_length_rt_float = mix(1., biene_fluegel_length_rt_float,
			smoothstep(-2., -1., biene_fluegel_anim_rt_float));

		biene_fluegel_rot_rt_float = mix(70., biene_fluegel_rot_rt_float,
			smoothstep(-1., 0., biene_fluegel_anim_rt_float));
		biene_fluegel_angle_rt_float = mix(-90., biene_fluegel_angle_rt_float,
			smoothstep(-1., 0., biene_fluegel_anim_rt_float));
		biene_fluegel_raise_rt_float = mix(0., biene_fluegel_raise_rt_float,
			smoothstep(-1., 0., biene_fluegel_anim_rt_float));
	}



	vec3 p_fluegel = p;
	p_fluegel.x = abs(p_fluegel.x);
	p_fluegel.zx *= rot2D(radians(1.) * biene_fluegel_rot_rt_float);
	p_fluegel.yz *= rot2D(radians(1.) * biene_fluegel_angle_rt_float);
	p_fluegel.yx *= rot2D(radians(1.) * biene_fluegel_raise_rt_float);
	p_fluegel.x -= biene_fluegel_start_rt_float;
	vec2 c = normalize(biene_fluegel_c_rt_vec2);
	float d_fluegel = cone(vec3(p_fluegel.z, p_fluegel.y, -p_fluegel.x), c);
	float fluegel_length = biene_fluegel_length_rt_float;
	float fluegel_thick = biene_fluegel_thick_rt_float;
	d_fluegel = smax(d_fluegel,
		cylinder(p_fluegel.xz - vec2(fluegel_length, -fluegel_length * biene_fluegel_asym_rt_float), fluegel_length),
		biene_fluegel_end_smooth_rt_float * fluegel_thick); // cap ends
	d_fluegel = smax(d_fluegel, abs(p_fluegel.y) - fluegel_thick,
		biene_fluegel_thick_smooth_rt_float * fluegel_thick); // cap bottom/top

	// musterung
	vec3 p_filling = p_fluegel;
	p_filling.x -= biene_fluegel_musterung_start_rt_float;
	float phi = atan(p_filling.z, p_filling.x);
	float r = length(p_filling.xz);
	phi += pow(r, biene_fluegel_musterung_phi_pow_rt_float) * biene_fluegel_musterung_phi_factor_rt_float;
	float dr_phi = TAU / 6.;
	phi = domrep(phi, dr_phi);

	p_filling.xz = r * vec2(cos(phi), sin(phi));
	p_filling.x -= biene_fluegel_musterung_space_rt_float * -.25;
	p_filling.x = squarerep(p_filling.x, biene_fluegel_musterung_space_rt_float, biene_fluegel_musterung_anim_rt_float, 1.);

	float f_filling = slowbox2(p_filling.xy, vec2(biene_fluegel_musterung_space_rt_float * .25, fluegel_thick * 2.));
	f_filling = smax(f_filling, d_fluegel - biene_fluegel_musterung_thick_rt_float, fluegel_thick * biene_fluegel_musterung_smooth_rt_float);
	d_fluegel = smin(d_fluegel, f_filling, fluegel_thick * biene_fluegel_musterung_smooth_rt_float);

	return vec2(d_fluegel, biene_id);
}

float rmk_tunnel(vec3 p) {
    float f_tri = max(abs(p.x) * .866025 + p.z * .5, -p.z) - biene_tunnel_size_rt_float;
	return -f_tri;
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_bee = p - biene_position_rt_vec3;
	vec2 body = bee_body(p_bee, last_step);
	vec2 fluegel = bee_fluegel(p_bee, last_step);
	vec2 m_bee = smin_material(body, fluegel, biene_fluegel_smooth_rt_float * biene_fluegel_thick_rt_float);
	vec2 m_bg = vec2(background(p), fels_id);
	vec2 m_tunnel = vec2(rmk_tunnel(p), 0.);
	m_bg = smax_material(m_bg, m_tunnel, biene_tunnel_smooth_rt_float);
	m_bg = min_material(m_bg, m_bee);
	return m_bg;
}
