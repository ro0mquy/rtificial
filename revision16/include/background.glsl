mat2 rM2D(float phi) {
	float c = cos(phi);
	float s = sin(phi);
	return mat2(c, -s, s, c);
}

vec3 background(vec3 d) {
	vec3 background_color = background_color_rt_color;

	//* no stars but stripes
	float tilt = 0.2;
	vec3 v = vec3(1,0,0);
	v.xz *= rM2D(0.043);

	vec3 d1 = d;
	float r1 = 0.1;
	d1.xy *= rM2D(tilt);
	if ( abs(dot(d1, v)) < r1 ) {
		background_color += 0.003;
		//background_color += 1;
	}

	vec3 d2 = d;
	float r2 = 0.015;
	d2.xz *= rM2D(0.133);
	d2.xy *= rM2D(tilt);
	if ( abs(dot(d2, v)) < r2 ) {
		background_color += 0.0015;
		//background_color += 0.25;
	}

	vec3 d3 = d;
	vec3 v3 = vec3(1,0,0);
	float r3 = 0.008;
	d3.xz *= rM2D(0.17);
	d3.xy *= rM2D(tilt);
	if ( abs(dot(d3, v)) < r3 ) {
		background_color += 0.0015;
		//background_color += 0.0625;
	}

	return background_color;
}
