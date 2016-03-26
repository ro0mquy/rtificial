mat2 rM2D(float phi) {
	float c = cos(phi);
	float s = sin(phi);
	return mat2(c, -s, s, c);
}

vec3 live_background(vec3 d) {
	vec3 background_color = background_color_rt_color;

	// no stars but stripes
	float tilt = 0.2;
	vec3 v = vec3(1,0,0);
	v.xz *= rM2D(0.043);

	vec3 d1 = d;
	float r1 = 0.1;
	d1.xy *= rM2D(tilt);
	if ( abs(dot(d1, v)) < r1 ) {
		background_color += background_stripe1_intensity_rt_float / 100;
	}

	vec3 d2 = d;
	float r2 = 0.015;
	d2.xz *= rM2D(0.133);
	d2.xy *= rM2D(tilt);
	if ( abs(dot(d2, v)) < r2 ) {
		background_color += background_stripe2_intensity_rt_float / 100;
	}

	vec3 d3 = d;
	vec3 v3 = vec3(1,0,0);
	float r3 = 0.008;
	d3.xz *= rM2D(0.17);
	d3.xy *= rM2D(tilt);
	if ( abs(dot(d3, v)) < r3 ) {
		background_color += background_stripe3_intensity_rt_float / 100;
	}

	// subtle glow foo
	vec3 c1 = normalize(background_c1_center_rt_vec3 + vec3(0.1 * sin(time * 0.05), 0.25 * cos(time * 0.1), 0));
	float s1 = background_c1_size_rt_float;
	float o1 = background_c1_opacity_rt_float;
	background_color += o1 * pow(abs(dot(d, c1)), s1);

	vec3 c2 = normalize(background_c2_center_rt_vec3 + vec3(0.2 * sin(time * 0.1), 0.3 * cos(time * 0.2), 0));
	float s2 = background_c2_size_rt_float;
	float o2 = background_c2_opacity_rt_float;
	background_color += o2 * pow(abs(dot(d, c2)), s2);

	return background_color;
}
