#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 conic_bobbel_color; // color
uniform vec3 conic_ring_color; // color
uniform vec3 conic_lampe_color; // color

uniform float conic_smooth_factor; // float
uniform float conic_ring_intensity; // float
uniform float conic_bobbel_noifreq;
uniform float conic_bobbel_roughness;
uniform float conic_ring_animation;
uniform float conic_bobbel_xcoord;
uniform float conic_domrep_spacing;
uniform bool  conic_domrep_enabled;

vec3 colors[5] = vec3[5](
		vec3(.0),
		vec3(.03, .0, .0),
		conic_lampe_color,
		conic_bobbel_color,
		conic_ring_color
		);

const float material_bounding = 0.;
const float material_boden    = 1.;
const float material_lampe    = 2.;
const float material_bobbel   = 3.;
const float material_ring     = 4.;

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march_adv(camera_position, direction, i, 100, 1.);

	vec3 color = vec3(0.);
	float materialId = f(hit)[1];
	vec3 normal = calc_normal(hit);

	SphereLight light1 = SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.);
	if(materialId == material_boden) {
		float size = 2.;
		float stripes = mod(floor(hit.x / size), 2.);
		Material mat = Material(colors[int(material_boden)] * stripes, 0.5, 0.);
		color = apply_light(hit, normal, -direction, mat, light1);
	} else if(materialId == material_bounding) {
		Material mat = Material(colors[int(material_bounding)], 1., 0.);
		color = apply_light(hit, normal, -direction, mat, light1);
	} else if (materialId >= material_lampe && materialId <= material_bobbel) {
		Material material1 = Material(colors[int(material_lampe)], 0.2, 1.);
		vec3 color1 = apply_light(hit, normal, -direction, material1, light1);
		Material material2 = Material(colors[int(material_bobbel)], 0.2, 1.);
		vec3 color2 = apply_light(hit, normal, -direction, material2, light1);
		float mixfactor = pow(materialId - material_lampe, 3.); // change the exponent for sharpness of transition
		color = mix(color1, color2, mixfactor);
	} else if (materialId >= material_bobbel && materialId <= material_ring) {
		Material material1 = Material(colors[int(material_bobbel)], 0.2, 1.);
		vec3 color1 = apply_light(hit, normal, -direction, material1, light1);
		vec3 color2 = emit_light(colors[int(material_ring)], conic_ring_intensity);
		float mixfactor = pow(materialId - material_bobbel, 6.); // change the exponent for sharpness of transition
		color = mix(color1, color2, mixfactor);
	} else {
		Material mat = Material(colors[int(materialId)], 0.2, 1.);
		color = apply_light(hit, normal, -direction, mat, light1);
	}

	output_color(color, distance(hit, camera_position));
}

float conicbobbel(vec3 p_cone, float l_body) {
	////// distfunctions for body (some day in the past it was a cone)
	float radius_mixer = smoothstep(l_body * .3, l_body * 1., p_cone.x)
		+ .3 * (1. - smoothstep(l_body * .0, l_body * .3, p_cone.x)); // used to mix between min_ and max_radius
	float max_radius = .5;
	float min_radius = .15;
	float line_radius = mix(min_radius, max_radius, radius_mixer);;
	float f_cone = line(p_cone, vec3(l_body, 0., 0.), vec3(0., 0., 0.), line_radius);

	////// akward spikes
	float num_spikes = 8.;
	float height_spikes = .8;
	float sharpness_spikes = 7.;
	// move where the center of the spikes should be
	vec3 p_spike = trans(p_cone, l_body * .9, 0., 0.);
	// make cylindrical domrep; x -> x, y -> radius, z -> angle
	p_spike.yz = vec2(length(p_spike.yz), atan(p_spike.y, p_spike.z) / TAU);
	vec3 p_pre_spike = p_spike;
	p_spike = domrep(p_spike, 1., 1., 1./num_spikes);
	p_spike.xy = p_pre_spike.xy; // don't domrep x and radius
	p_spike.y -= height_spikes;
	p_spike.z *= 3.2;
	// you may uncomment following rotation line, but then there will be lots of ugly artefacts, if you do, then change the center of the spikes to x = l_body * .9
	p_spike = rZ(TAU * -(min(p_spike.y, .0) + 1.) / 7.)* p_spike;
	float f_spike = cone(p_spike.xzy, normalize(vec2(sharpness_spikes, 1.)));
	f_cone = smin(f_cone, f_spike, .1);

	////// perlin make some noise!
	vec2 surface_coord = vec2(p_cone.x * 3. + time, atan(p_cone.y, p_cone.z));
	float noise_amplitude = smoothstep(-.6, 0., p_cone.x) * (1. - smoothstep(l_body + .3, l_body + .5, p_cone.x)) * .03;
	f_cone -= noise_amplitude * smoothstep(conic_bobbel_roughness, 1., cnoise(surface_coord * conic_bobbel_noifreq)); // smoothstep cuts the lower half of the noise to zero, -1 and 0 are both good values for conic_bobbel_roughness

	return f_cone;
}

float bobbelring(vec3 p_cone, float l_body, float factorDeltaT) {
	////// big, bright rings
	float T = 1.; // duration of one animation cyclus
	float deltaT = T * factorDeltaT; // delta between first and second ring
	float bgn_rng_anim = l_body + .7; // start point of ring animation
	float end_rng_anim = -1.; // end point of ring anmation
	float max_ring_amp = 1.5; // maximum ring amplitude
	vec3 p_ring = trans(p_cone, bgn_rng_anim + (end_rng_anim - bgn_rng_anim) * mod(conic_ring_animation + deltaT, T) / T, 0., 0.);
	float ring_radius = max_ring_amp * impulse(8., mod(conic_ring_animation + deltaT, T) / T); // first arg is impulse shape
	float ring_r_thickness = .2 * ring_radius; // thickness of ring in r direction
	float ring_x_thickness = .03; // thickness of ring in x direction
	float f_ring = 0.;
	f_ring = abs(sphere(p_ring, ring_radius)) - ring_r_thickness;
	f_ring = smax(f_ring, abs(p_ring.x) - ring_x_thickness, .02);

	return f_ring;
}

vec2 f(vec3 p) {
	vec3 p_bobbel = trans(p, conic_bobbel_xcoord, 0., -3.);

	vec3 s_domrep = conic_domrep_spacing * vec3(10, 13., 9.); // domrep cell size, 1. probably means no domrep
	vec3 p_pre_cone = p_bobbel;
	//p_pre_cone = trans(p_bobbel, 10. * time, 0., 0.); // move with time
	// find the current cell
	vec3 cell = floor(p_pre_cone / s_domrep) + 100.;
	// move single cells a bit sidewards, maybe amplitude should be smaller than domrep cell
	vec3 translation_vector = conic_domrep_spacing * vec3(
			sin(cell.z * cell.z) * 4.,
			sin(cell.z * cell.x) * 3.,
			sin(cell.x * cell.x) * 3.6);
	p_pre_cone = transv(p_pre_cone, translation_vector);
	vec3 p_cone = domrepv(p_pre_cone, s_domrep);
	//p_cone.y = p_pre_cone.y; // dont't domrep y
	if (!conic_domrep_enabled) {
		// remove for repetition and movement
		p_cone = p_bobbel;
	}

	// get bobbel body and rings
	float l_body = 2.; // length of body
	float f_cone = conicbobbel(p_cone, l_body);
	float f_ring = bobbelring(p_cone, l_body, 0.);
	//f_ring = min(f_ring, bobbelring(p_cone, l_body, 1/2.5));

	////// assembling and boundings
	vec2 m_cone = vec2(f_cone, material_bobbel);
	vec2 m_ring = vec2(f_ring, material_ring);
	vec2 m_bobbel = smin_smaterial(m_cone, m_ring, conic_smooth_factor);

	// lampen sphere and tunnel for bobbel
	float radius_lampe = 10.;
	vec3 p_lampe = trans(p, -15, 0, -3);
	float f_lampe = sphere(p_lampe, radius_lampe);

	vec3 p_delle = p_bobbel;
	float f_delle = line(p_delle, vec3(1.7, 0., 0.), vec3(.1), 1.);
	f_lampe = smax(f_lampe, -f_delle, 1.);

	vec2 m_lampe = vec2(f_lampe, material_lampe);

	vec2 m_bobbel_lampe = smin_smaterial(m_bobbel, m_lampe, 1.);

	vec2 m_content = m_bobbel_lampe;
	vec2 bottom = vec2(p.y + 20., material_boden);
	vec2 bounding = vec2(-sphere(p - camera_position, 100.), material_bounding);
	return min_material(m_content, min_material(bottom, bounding));
}
