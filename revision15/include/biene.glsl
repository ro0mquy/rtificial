float opal(vec3 p) {
	const float size = 1.;

	vec3 p_sphere = p;
	p_sphere.x /= morph_opal_stretch_x_rt_float;
	p_sphere.y /= morph_opal_stretch_y_rt_float;
	float f_sphere = sphere(p_sphere, size);
	f_sphere *= min(1., min(morph_opal_stretch_x_rt_float, morph_opal_stretch_y_rt_float));

	vec3 p_box1 = p;
	p_box1.xz *= rot2D(TAU / 12.);
	float f_box1 = box(p_box1, .5*vec3(morph_opal_stretch_x_rt_float, morph_opal_stretch_y_rt_float, size));

	vec3 p_box2 = p;
	p_box2.xz *= rot2D(-TAU / 12.);
	float f_box2 = box(p_box2, .5*vec3(morph_opal_stretch_x_rt_float, morph_opal_stretch_y_rt_float, size));

	float f = min(f_box1, f_box2);
	f = mix(f_sphere, f, morph_opal_smooth_rt_float);


	float smooth_plane = size * morph_opal_smooth_plane_rt_float;

	float f_plane = abs(p.y);
	f_plane = smax(f, f_plane, smooth_plane);

	float f_plane1 = abs(p_box1.x);
	f_plane1 = smax(f, f_plane1, smooth_plane);
	f_plane = min(f_plane, f_plane1);

	float f_plane2 = abs(p_box2.x);
	f_plane2 = smax(f, f_plane2, smooth_plane);
	f_plane = min(f_plane, f_plane2);

	f = mix(f, f_plane, smooth_plane);

	return f;
}
