uniform float conic_bobbel_noifreq;
uniform float conic_bobbel_roughness;
uniform float conic_ring_animation;

// l_body is size of bobbel, function is optimized for 2.
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

// l_body is size of bobbel, function is optimized for 2.
// this one totally noiseless
float conicbobbel_fast(vec3 p_cone, float l_body) {
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
	//vec2 surface_coord = vec2(p_cone.x * 3. + time, atan(p_cone.y, p_cone.z));
	//float noise_amplitude = smoothstep(-.6, 0., p_cone.x) * (1. - smoothstep(l_body + .3, l_body + .5, p_cone.x)) * .03;
	//f_cone -= noise_amplitude * smoothstep(conic_bobbel_roughness, 1., cnoise(surface_coord * conic_bobbel_noifreq)); // smoothstep cuts the lower half of the noise to zero, -1 and 0 are both good values for conic_bobbel_roughness

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
	float ring_radius = max_ring_amp * (impulse(8., mod(conic_ring_animation + deltaT, T) / T) - .025); // first arg is impulse shape
	float ring_r_thickness = .2 * ring_radius; // thickness of ring in r direction
	float ring_x_thickness = .03; // thickness of ring in x direction
	float f_ring = 0.;
	f_ring = abs(sphere(p_ring, ring_radius)) - ring_r_thickness;
	f_ring = smax(f_ring, abs(p_ring.x) - ring_x_thickness, .02);

	return f_ring;
}
