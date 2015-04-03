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

vec3 augenlicht(vec3 p, vec3 d, vec3 normal) {
	//vec3 r = refract();
	vec3 reflection_dir = reflect(d, normal);
	vec3 reflection_color = textureLod(environment, reflection_dir, 0.).rgb;

	vec3 n2 = vec3(1.1, 1.104, 1.106) * morph_reflectivness_rt_float;
	/*
	vec3 refraction_dir_r = refract(d, normal, n2.x);
	vec3 refraction_dir_g = refract(d, normal, n2.y);
	vec3 refraction_dir_b = refract(d, normal, n2.z);
	float refraction_red = textureLod(environment, refraction_dir_r, 0.).r;
	float refraction_green = textureLod(environment, refraction_dir_g, 0.).g;
	float refraction_blue = textureLod(environment, refraction_dir_b, 0.).b;
	vec3 refraction_color = vec3(refraction_red, refraction_green, refraction_blue);
	// */

	vec3 r0 = (1. - n2) / (1. + n2);
	r0 *= r0;
	float HdotV = -dot(normal, d);
	vec3 r = r0 + (1. - r0) * pow(1. - HdotV, 5.);

	vec3 out_color;
	//out_color.rgb = vec3(max(dot(normal, normalize(vec3(1., .5, .5))), 0.) + .1);

	//// TODO: maybe use the normal sss (ao) function
	vec3 n = -d;
	float ao_factor = 0.;
	float l = -.3;
	float i = 5.;
	for(; i > 0; i--) {
		vec3 p_i = p + n * i * l;
		float noise = cnoise(p_i * morph_noise_freq_rt_float);
		//ao_factor -= (i * l - noise * f(p_i, false)[0]) / exp2(i);
		ao_factor += noise * abs(f(p_i, false)[0]) / exp2(i);
		//ao_factor += noise / exp2(i);
	}
	//out_color.rgb *= .1;
	//refraction_color *= pow(clamp(ao_factor, 0., 1.), 10.);
	//out_color = mix(reflection_color, refraction_color, r);
	//out_color *= morph_rt_color;

	vec3 col = morph_rt_color;
	float rough = morph_rough_rt_float;
	float metallic = 1.;
	out_color = ambientColor(normal, -d, col, rough, metallic);
	// ganz viel spucke!
	//out_color.rgb *= .05;
	//out_color.rgb += ao_factor * 2.;
	return mix((ao_factor * 15. + 1.) * morph_rt_color, out_color.rgb, r);
}
