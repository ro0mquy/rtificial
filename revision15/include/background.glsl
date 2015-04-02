float fels_noise(vec3 p_fels, vec2 domrep_size, vec3 box_size) {
	vec2 cell_fels = floor(p_fels.xz / domrep_size);
	//cell_fels = vec2(0.);
	p_fels.xz = domrep(p_fels.xz, domrep_size);

	//  8 1 5
	//  4 0 2  >x
	//  7 3 6 vz
	vec3 p_fels_0 = p_fels;
	vec3 p_fels_1 = trans(p_fels, 0., 0., -domrep_size.y);
	vec3 p_fels_2 = trans(p_fels,  domrep_size.x, 0., 0.);
	vec3 p_fels_3 = trans(p_fels, 0., 0.,  domrep_size.y);
	vec3 p_fels_4 = trans(p_fels, -domrep_size.x, 0., 0.);
	vec3 p_fels_5 = trans(p_fels,  domrep_size.x, 0., -domrep_size.y);
	vec3 p_fels_6 = trans(p_fels,  domrep_size.x, 0.,  domrep_size.y);
	vec3 p_fels_7 = trans(p_fels, -domrep_size.x, 0.,  domrep_size.y);
	vec3 p_fels_8 = trans(p_fels, -domrep_size.x, 0., -domrep_size.y);

	vec2 cell_fels_0 = cell_fels;
	vec2 cell_fels_1 = cell_fels + vec2( 0., -1.);
	vec2 cell_fels_2 = cell_fels + vec2( 1.,  0.);
	vec2 cell_fels_3 = cell_fels + vec2( 0.,  1.);
	vec2 cell_fels_4 = cell_fels + vec2(-1.,  0.);
	vec2 cell_fels_5 = cell_fels + vec2( 1., -1.);
	vec2 cell_fels_6 = cell_fels + vec2( 1.,  1.);
	vec2 cell_fels_7 = cell_fels + vec2(-1.,  1.);
	vec2 cell_fels_8 = cell_fels + vec2(-1., -1.);

	p_fels_0.xy *= rot2D(cell_fels_0.x * domrep_size.x * 32.73101);
	p_fels_1.xy *= rot2D(cell_fels_1.x * domrep_size.x * 32.73101);
	p_fels_2.xy *= rot2D(cell_fels_2.x * domrep_size.x * 32.73101);
	p_fels_3.xy *= rot2D(cell_fels_3.x * domrep_size.x * 32.73101);
	p_fels_4.xy *= rot2D(cell_fels_4.x * domrep_size.x * 32.73101);
	p_fels_5.xy *= rot2D(cell_fels_5.x * domrep_size.x * 32.73101);
	p_fels_6.xy *= rot2D(cell_fels_6.x * domrep_size.x * 32.73101);
	p_fels_7.xy *= rot2D(cell_fels_7.x * domrep_size.x * 32.73101);
	p_fels_8.xy *= rot2D(cell_fels_8.x * domrep_size.x * 32.73101);

	p_fels_0.xz *= rot2D(cell_fels_0.y * domrep_size.y * 49.29012);
	p_fels_1.xz *= rot2D(cell_fels_1.y * domrep_size.y * 49.29012);
	p_fels_2.xz *= rot2D(cell_fels_2.y * domrep_size.y * 49.29012);
	p_fels_3.xz *= rot2D(cell_fels_3.y * domrep_size.y * 49.29012);
	p_fels_4.xz *= rot2D(cell_fels_4.y * domrep_size.y * 49.29012);
	p_fels_5.xz *= rot2D(cell_fels_5.y * domrep_size.y * 49.29012);
	p_fels_6.xz *= rot2D(cell_fels_6.y * domrep_size.y * 49.29012);
	p_fels_7.xz *= rot2D(cell_fels_7.y * domrep_size.y * 49.29012);
	p_fels_8.xz *= rot2D(cell_fels_8.y * domrep_size.y * 49.29012);

	/*
	p_fels_0.yz *= rot2D(cell_fels_0.y * domrep_size.x * 52.40165);
	p_fels_1.yz *= rot2D(cell_fels_1.y * domrep_size.x * 52.40165);
	p_fels_2.yz *= rot2D(cell_fels_2.y * domrep_size.x * 52.40165);
	p_fels_3.yz *= rot2D(cell_fels_3.y * domrep_size.x * 52.40165);
	p_fels_4.yz *= rot2D(cell_fels_4.y * domrep_size.x * 52.40165);
	p_fels_5.yz *= rot2D(cell_fels_5.y * domrep_size.x * 52.40165);
	p_fels_6.yz *= rot2D(cell_fels_6.y * domrep_size.x * 52.40165);
	p_fels_7.yz *= rot2D(cell_fels_7.y * domrep_size.x * 52.40165);
	p_fels_8.yz *= rot2D(cell_fels_8.y * domrep_size.x * 52.40165);
	// */

	float fels_0 = box(p_fels_0, box_size);
	float fels_1 = box(p_fels_1, box_size);
	float fels_2 = box(p_fels_2, box_size);
	float fels_3 = box(p_fels_3, box_size);
	float fels_4 = box(p_fels_4, box_size);
	float fels_5 = box(p_fels_5, box_size);
	float fels_6 = box(p_fels_6, box_size);
	float fels_7 = box(p_fels_7, box_size);
	float fels_8 = box(p_fels_8, box_size);

	float fels_12 = min(fels_1, fels_2);
	float fels_34 = min(fels_3, fels_4);
	float fels_56 = min(fels_5, fels_6);
	float fels_78 = min(fels_7, fels_8);
	float fels_1234 = min(fels_12, fels_34);
	float fels_5678 = min(fels_56, fels_78);
	float fels_12345678 = min(fels_1234, fels_5678);
	float fels_012345678 = smin(fels_0, fels_12345678, bg_smin_felsen_rt_float * box_size.x);
	fels_012345678 = smin(fels_0, fels_1234, bg_smin_felsen_rt_float * box_size.x); // TODO: maybe remove this line for less/more? artefacts

	return fels_012345678;
}

float kristall(vec3 p_kristall, float height_kristall, float radius_kristall, float size_cap) {
	float r_kristall = radius_kristall * min((height_kristall - radius_kristall) - p_kristall.y, size_cap) / size_cap;
	p_kristall.y -= height_kristall * .5;
	return hexprism(p_kristall.xzy, vec2(r_kristall, height_kristall));
}

float background(vec3 p) {
	p.y -= -10.;

	vec3 p_kristall = p;
	p_kristall.y -= bg_kristall_offset_rt_vec3.y;
	p_kristall.x = abs(p_kristall.x);
	p_kristall.x -= bg_kristall_offset_rt_vec3.x;
	p_kristall.z = abs(p_kristall.z);
	p_kristall.z -= bg_kristall_offset_rt_vec3.z;
	p_kristall.yz *= rot2D(TAU * .48);
	p_kristall.xy *= rot2D(TAU * .45);
	float f_kristall = kristall(p_kristall, bg_kristall_h_rt_float, bg_kristall_r_rt_float, bg_kristall_cap_rt_float);

	vec3 p_fels = p;
	p_fels.xz -= vec2(237., 349.); // origin looks shitty
	p_fels.xz *= rot2D(TAU * .1);
	float f_fels = fels_noise(p_fels, vec2(10.), vec3(2.));

	vec3 p_boden = p;
	p_boden.y -= bg_boden_height_rt_float;
	float f_boden = p_boden.y;
	f_fels = smin(f_fels, f_boden, bg_smin_boden_rt_float);

	float f = smin(f_kristall, f_fels, bg_smin_kristall_rt_float);
	return f;
}
