#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 50., screenDist);

	vec3 out_color;
	if (isinf(t)) {
		out_color.rgb = textureLod(environment, d, 0.).rgb;
	} else {
		vec3 p = o + t * d;
		vec3 normal = calc_normal(p, false);
		float rough = .2;
		float metallic = 1.;
		vec3 col = vec3(1.);
		out_color.rgb = ambientColor(normal, -d, col, rough, metallic);
	}
	output_color(out_color, t);
}

vec2 bee_body(vec3 p, bool last_step) {
	float biene_body_thorax_scale_rt_float = mix(0., biene_body_thorax_scale_rt_float,
			smoothstep(0., .5, biene_body_anim_rt_float));
	float biene_body_thorax_stretch_rt_float = mix(1., biene_body_thorax_stretch_rt_float,
			smoothstep(.5, 1., biene_body_anim_rt_float));

	float biene_body_head_radius_rt_float = mix(0., biene_body_head_radius_rt_float,
			smoothstep(1., 2., biene_body_anim_rt_float));
	float biene_body_head_stretch_rt_float = mix(1., biene_body_head_stretch_rt_float,
			smoothstep(1., 2., biene_body_anim_rt_float));
	float biene_body_head_offset_rt_float = mix(biene_body_head_offset_rt_float, 0.,
			smoothstep(1., 2., biene_body_anim_rt_float));

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

	float dellen_anim = mix(0., 1., smoothstep(3., 4., biene_body_anim_rt_float));

	vec3 p_thorax = p;
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
	float d_head = sphere(p_head, head_radius);
	d_head *= min(biene_body_head_stretch_rt_float, 1.);

	vec3 p_eyes = p_head;
	p_eyes.x = abs(p_eyes.x);
	p_eyes.zx *= rot2D(radians(biene_body_head_eye_dist_rt_float));
	p_eyes.zy *= rot2D(radians(biene_body_head_eye_height_rt_float));
	p_eyes.z += head_length - biene_body_head_eye_offset_rt_float;
	p_eyes.y /= biene_body_head_eye_stretch_rt_float;
	float d_eyes = sphere(p_eyes, biene_body_head_eye_radius_rt_float * head_radius);
	d_head = min(d_head, d_eyes);

	p_fuel.x = abs(p_fuel.x);
	p_fuel.zx *= rot2D(radians(biene_body_head_fuel_dist_rt_float));
	p_fuel.zy *= rot2D(radians(biene_Body_head_fuel_height_rt_float));
	p_fuel.z += head_length;
	float t_fuel = clamp(-p_fuel.z, 0., 2.);
	p_fuel.yz *= rot2D(radians(10.) * t_fuel);
	float d_fuel = line(p_fuel, vec3(0., 0., -2.), .1);
	d_head = smin(d_head, d_fuel, .05);

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
	float dellen_cell = -ceil(p_dellen.z / dellen_space); // +floor() for other direction
	dellen_anim -= .5;
	float dellen_current_anim = clamp(dellen_anim * biene_dellen_num_rt_float - dellen_cell, 0., 1.);
	float biene_dellen_depth_rt_float = mix(0., biene_dellen_depth_rt_float,
			smoothstep(0., 1., dellen_current_anim));
	p_dellen.z = domrep(p_dellen.z, dellen_space);
	float d_dellen = smax(
		cylinder(p_dellen.xy, biene_body_radius_rt_float * (1. + biene_dellen_depth_rt_float)),
		abs(p_dellen.z) - biene_dellen_thick_rt_float,
		biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float
	);
	d_dellen = smax(d_dellen, abs(p_abdomen.z) - biene_body_length_rt_float, biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float);

	d_abdomen = smin(d_abdomen, d_dellen, biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float);

	float d_body = smin(d_abdomen, smin(d_thorax, d_head, biene_body_head_smooth_rt_float * head_radius),
		biene_body_abdomen_smooth_rt_float * biene_body_max_thick_rt_float);

	return vec2(d_body, 0.);
}

vec2 bee_fluegel(vec3 p, bool last_step) {
	float biene_fluegel_rot_rt_float = 20. * sin(TAU * biene_fluegel_anim_rt_float);
	float biene_fluegel_angle_rt_float = 20. * sin(TAU * (biene_fluegel_anim_rt_float - .25)) - 20.;
	float biene_fluegel_raise_rt_float = 20. * sin(TAU * biene_fluegel_anim_rt_float);

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
	d_fluegel = smax(d_fluegel, cylinder(p_fluegel.xz - vec2(fluegel_length, 0.), fluegel_length),
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
	p_filling.x = squarerep(p_filling.x, biene_fluegel_musterung_space_rt_float, biene_fluegel_musterung_anim_rt_float);

	float f_filling = slowbox2(p_filling.xy, vec2(biene_fluegel_musterung_space_rt_float * .25, fluegel_thick * 2.));
	f_filling = smax(f_filling, d_fluegel - biene_fluegel_musterung_thick_rt_float, fluegel_thick * biene_fluegel_musterung_smooth_rt_float);
	d_fluegel = smin(d_fluegel, f_filling, fluegel_thick * biene_fluegel_musterung_smooth_rt_float);

	return vec2(d_fluegel, 0.);
}

vec2 f(vec3 p, bool last_step) {
	vec2 body = bee_body(p, last_step);
	vec2 fluegel = bee_fluegel(p, last_step);
	return smin_material(body, fluegel, biene_fluegel_smooth_rt_float * biene_fluegel_thick_rt_float);
}
