#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 conic_bobbel_color; // color
uniform vec3 conic_ring_color; // color

uniform float conic_smooth_factor; // float
uniform float manual_time; // float
uniform float conic_ring_intensity; // float

vec3 colors[] = vec3[](
		vec3(.03, .0, .0),
		vec3(.0),
		conic_bobbel_color,
		conic_ring_color
		);

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march_adv(camera_position, direction, i, 100, 1.);

	vec3 color = vec3(0.);
	if(i < 150) {
		int material = int(f(hit).y);
		vec3 normal = calc_normal(hit);

		Material mat;
		if(material == 0.) {
			float size = 2.;
			float stripes = mod(floor(hit.x / size), 2.);
			mat = Material(colors[material] * stripes, 0.5, 0.);
		} else if(material == 1.) {
			mat = Material(colors[material], 1., 0.);
		} else {
			mat = Material(colors[material], 0.2, 1.);
		}
		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
		if (material == 3.) {
			color = conic_ring_color * conic_ring_intensity * 100.;
		}
	}

	output_color(color, 4.01);//distance(hit, camera_position));
}

vec2 f(vec3 p) {
	vec3 p_bobbel = trans(p, -1., 0., -3.);
	float f_bobbel = p.y;

	// uncomment to manipulate time by slider; remove in the near future
	//float time = 10. * manual_time;

	////// "domrep of whole bobbels"-code
	vec3 s_domrep = vec3(30, 1., 25.); // domrep cell size, 1. probably means no domrep
	vec3 p_pre_cone = trans(p_bobbel, 10. * time, 0., 0.); // move with time
	// find the current cell
	float cell_x = floor(p_pre_cone.x / s_domrep.x);
	float cell_y = floor(p_pre_cone.y / s_domrep.y);
	float cell_z = floor(p_pre_cone.z / s_domrep.z);
	// move single cells a bit sidewards, maybe amplitude should be smaller than domrep cell
	p_pre_cone = trans(p_pre_cone,
			sin(cell_z * cell_z) * 32.,
			0.,
			sin(cell_x * cell_x) * 50.);
	vec3 p_cone = domrepv(p_pre_cone, s_domrep);
	p_cone.y = p_pre_cone.y; // dont't domrep y
	p_cone = p_bobbel; // remove for repetition and movement

	////// distfunctions for body (some day in the past it was a cone)
	float l_body = 2.; // length of body
	float radius_mixer = smoothstep(l_body * .15, l_body * 1., p_cone.x)
		+ .3 * smoothstep(l_body * .45, l_body * 0., p_cone.x); // used to mix between min_ and max_radius
	float max_radius = .5;
	float min_radius = .0;
	float line_radius = mix(min_radius, max_radius, radius_mixer);;
	float f_cone = line(p_cone, vec3(l_body, 0., 0.), vec3(0., 0., 0.), line_radius);

	////// akward spikes
	float num_spikes = 8.;
	float height_spikes = .8;
	float sharpness_spikes = 7.;
	// move where the center of the spikes should be
	vec3 p_spike = trans(p_cone, l_body * 1., 0., 0.);
	// make cylindrical domrep; x -> x, y -> radius, z -> angle
	p_spike.yz = vec2(length(p_spike.yz), atan(p_spike.y, p_spike.z) / TAU);
	vec3 p_pre_spike = p_spike;
	p_spike = domrep(p_spike, 1., 1., 1./num_spikes);
	p_spike.xy = p_pre_spike.xy; // don't domrep x and radius
	p_spike.y -= height_spikes;
	p_spike.z *= 3.2;
	// you may uncomment following rotation line, but then there will be lots of ugly artefacts, if you do, then change the center of the spikes to x = l_body * .9
	//p_spike = rZ(TAU * -(p_spike.y + 1.) / 7.)* p_spike;
	float f_spike = cone(p_spike.xzy, normalize(vec2(sharpness_spikes, 1.)));
	f_cone = smin(f_cone, f_spike, .1);

	////// perlin make some noise!
	vec2 surface_coord = vec2(p_cone.x * 3. + time, atan(p_cone.y, p_cone.z));
	f_cone -= smoothstep(0., 1., cnoise(surface_coord * 1.5)) * .03; // smoothstep cuts the lower half of the noise to zero

	////// big, bright rings
	float T = 3.; // duration of one anmation cyclus
	float deltaT = T / 2.5; // delta between first and second ring
	float bgn_rng_anim = l_body + 0.; // start point of ring animation
	float end_rng_anim = -1.; // end point of ring anmation
	float max_ring_amp = 1.5; // maximum ring amplitude
	vec3 p_ring = trans(p_cone, bgn_rng_anim + (end_rng_anim - bgn_rng_anim) * mod(time, T) / T, 0., 0.);
	float ring_radius = max_ring_amp * impulse(8., mod(time, T) / T); // first arg is impulse shape
	float ring_r_thickness = .2 * ring_radius; // thickness of ring in r direction
	float ring_x_thickness = .03; // thickness of ring in x direction
	float f_ring = p.y;
	f_ring = abs(sphere(p_ring, ring_radius)) - ring_r_thickness;
	f_ring = smax(f_ring, abs(p_ring.x) - ring_x_thickness, .02);

	// second ring
	p_ring = trans(p_cone, bgn_rng_anim + (end_rng_anim - bgn_rng_anim) * mod(time + deltaT, T) / T, 0., 0.);
	ring_radius = max_ring_amp * impulse(8., mod(time + deltaT, T) / T); // first arg is impulse shape
	ring_r_thickness = .2 * ring_radius; // thickness of ring in r direction
	float f_ring2 = f_ring;
	f_ring2 = abs(sphere(p_ring, ring_radius)) - ring_r_thickness;
	f_ring2 = smax(f_ring2, abs(p_ring.x) - ring_x_thickness, .02);
	f_ring = min(f_ring, f_ring2);

	////// assembling and boundings
	vec2 m_cone = vec2(f_cone, 2.);
	vec2 m_ring = vec2(f_ring, 3.);
	vec2 m_bobbel = smin_material(m_cone, m_ring, conic_smooth_factor);

	vec2 bottom = vec2(p.y + 2., 0.);
	vec2 bounding = vec2(-sphere(p - camera_position, 300.), 1.);
	return min_material(m_bobbel, min_material(bottom, bounding));
}
