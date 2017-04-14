#include "helper.glsl"
#line 3

mat2 rM2D(float phi) {
	float c = cos(phi);
	float s = sin(phi);
	return mat2(c, -s, s, c);
}

vec3 envmap(vec3 d) {
	float angle = atan(d.z, d.x);

	vec3 color_pal_1 = iqCosinePalette((d.y * d.x * d.z), pal_base_rt_color, pal_amplitude_rt_color, pal_frequency_rt_vec3, pal_phase_rt_vec3);
	color_pal_1 *= pow(abs(1. - smoothstep(0., .03 + 0.01 * sin(angle * 16. + .25*Tau), d.y)), bg_fade_pow_rt_float);

	vec3 color_pal_2 = mix(bg_color1_rt_color, bg_color2_rt_color, lin2srgb(square(sin(4. * angle))));
	color_pal_2 *= pow(smoothstep(-.01, .0, d.y), bg_fade_pow_rt_float);
	color_pal_2 *= 0.5;

	vec3 color = color_pal_1 + color_pal_2;


	float y = d.y / sqrt(1. - square(d.y)); // tan(arcsin(d.y))
	float pattern_angle = angle + bg_pattern_tilt_rt_float * y + bg_pattern_offset_rt_float;
	float pattern_domrepped = mod(pattern_angle, bg_pattern_domrep_rt_float) / bg_pattern_domrep_rt_float;
	color += step(1. - bg_pattern_width_rt_float, pattern_domrepped) * 0.005;

	color *= .01;
	return color;
}
