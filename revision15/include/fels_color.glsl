void fels_color(vec3 p, out vec3 color, out float rough) {
	rough = mk_fels_rough_rt_float;
	float fels_noise = pow(cfbm(p * mk_fels_freq_rt_float) * .5 + .5, mk_fels_noise_power_rt_float);
	fels_noise *= smoothstep(0., mk_kristall_h_rt_float, p.y * mk_height_color_rt_float);
	color = mix(mk_fels_color1_rt_color, mk_fels_color2_rt_color, fels_noise);
}
