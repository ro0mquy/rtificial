static const char scene_gitter_source[] = "\
vec2 f(vec3);\n\
\n\
DEFINE_MARCH(march, f)\n\
DEFINE_NORMAL(calc_normal, f)\n\
DEFINE_AO(ao, f)\n\
DEFINE_SOFTSHADOW(softshadow, f)\n\
\n\
vec3 color_gitter = vec3(1);\n\
vec3 color_kugel = vec3(.5647, .0, .0);\n\
vec3 color_text = vec3(.5176, 1., .0039);\n\
float radius_kugel = 1.;\n\
float udy_kugel = -1.5;\n\
float sigma_delle = 1.8;\n\
float sigma_trichter = 30.;\n\
\n\
uniform sampler2D tex_thanks;\n\
uniform sampler2D tex_greetings;\n\
uniform sampler2D tex_alcatraz;\n\
uniform sampler2D tex_conspiracy;\n\
uniform sampler2D tex_iq;\n\
uniform sampler2D tex_mercury;\n\
uniform sampler2D tex_urs;\n\
uniform sampler2D tex_nerd2nerd;\n\
uniform sampler2D tex_nerdarzt;\n\
uniform sampler2D tex_stroboholics;\n\
\n\
vec2 origdim_thanks = vec2(36, 7);\n\
vec2 origdim_greetings = vec2(66, 7);\n\
vec2 origdim_alcatraz = vec2(45, 7);\n\
vec2 origdim_conspiracy = vec2(59, 7);\n\
vec2 origdim_iq = vec2(9, 8);\n\
vec2 origdim_mercury = vec2(43, 7);\n\
vec2 origdim_urs = vec2(18, 7);\n\
vec2 origdim_nerd2nerd = vec2(54, 7);\n\
vec2 origdim_nerdarzt = vec2(46, 7);\n\
vec2 origdim_stroboholics = vec2(67, 7);\n\
\n\
vec3 color_thanks_greetings = color_text;\n\
vec3 color_alcatraz_conspiracy = color_text;\n\
vec3 color_mercury_urs = vec3(1, .35, 0);\n\
vec3 color_iq = color_text;\n\
vec3 color_nerd2nerd_nerdarzt = color_text;\n\
vec3 color_stroboholics = color_text;\n\
\n\
#define mat_bounding 0\n\
#define mat_gitter 1\n\
#define mat_untergrund 2\n\
#define mat_kugel 3\n\
\n\
vec3[] mat_colors = vec3[](\n\
		vec3(.001),\n\
		color_gitter,\n\
		color_gitter,\n\
		color_kugel\n\
);\n\
\n\
float[] mat_blooms = float[](\n\
		0,\n\
		0,\n\
		.1,\n\
		1\n\
);\n\
\n\
void main(void) {\n\
	vec3 light_position;\n\
	if (time < 48.759) {\n\
		// bouncing\n\
		light_position = vec3(-85, 5, -40);\n\
	} else if (time < 54.0) {\n\
		// rolling\n\
		float time = time - 48.759;\n\
		light_position = vec3(-85, -5, -40);\n\
		light_position.y += .1 * time;\n\
	} else if (time < 69) {\n\
		// greetings -- alcatraz & conspiracy -- iq\n\
		float t = (time - 54) / (69 - 54);\n\
		light_position = mix(vec3(-50, -10, -10), vec3(-20, -5, 8), t);\n\
	} else if (time < 76) {\n\
		// mercury & urs\n\
		float time = time - 69;\n\
		light_position = vec3(-40, -5, -12);\n\
		light_position.xz += 2 * time * vec2(1, 1);\n\
	} else if (time < 100) {\n\
		// trichter\n\
		float time = time - 81;\n\
		light_position = vec3(0, 14, 5);\n\
		light_position.y -= 1.5 * time;\n\
	}\n\
\n\
	vec3 dir = get_direction();\n\
	vec3 final_color = vec3(0);\n\
	float final_bloom = 0;\n\
	int i;\n\
	vec3 hit = march(view_position, dir, i);\n\
	if (i < 100) {\n\
		vec3 normal = calc_normal(hit);\n\
		vec3 to_light = light_position - hit;\n\
		float material = f(hit)[1];\n\
		if (material == mat_kugel) {\n\
			//final_color += .05 + .95 * cook_torrance(to_light, normal, -dir, .5, 450);\n\
			final_color += .05 + .95 * lambert(to_light, normal);\n\
		} else {\n\
			final_color += .05 + .95 * oren_nayar(to_light, normal, -dir, .5);\n\
		}\n\
		final_color += vec3(phong(to_light, normal, -dir, 50.));\n\
\n\
		final_color *= mat_colors[int(material)];\n\
		if (material != mat_bounding) {\n\
			final_color *= .95 * softshadow(hit, light_position, 32.) + .05;\n\
		}\n\
\n\
		if (material == mat_gitter) {\n\
			vec2 base_dim = vec2(8., 10.);\n\
			float f_text = 0;\n\
			vec3 color_text = vec3(0);\n\
\n\
			if (time > 54 && time < 63) {\n\
				// thanks and greetings\n\
				float time = time - 54;\n\
				vec2 dim_thanks = vec2(origdim_thanks.x / origdim_thanks.y, 1.) * base_dim;\n\
				vec2 p_thanks = hit.xz - vec2(-51, -55 + time * 20);\n\
				vec2 p_tex_thanks = vec2(-1, 1) * p_thanks.yx / dim_thanks.xy + .5;\n\
				float f_thanks = texture(tex_thanks, p_tex_thanks).r;\n\
\n\
				vec2 dim_greetings = vec2(origdim_greetings.x / origdim_greetings.y, 1.) * base_dim;\n\
				vec2 p_greetings = hit.xz - vec2(-40, 10 - time * 4);\n\
				vec2 p_tex_greetings = vec2(-1, 1) * p_greetings.yx / dim_greetings.xy + .5;\n\
				float f_greetings = texture(tex_greetings, p_tex_greetings).r;\n\
\n\
				// alcatraz -- conspiracy\n\
				vec2 dim_alcatraz = vec2(origdim_alcatraz.x / origdim_alcatraz.y, 1.) * base_dim;\n\
				vec2 p_alcatraz = hit.xz - vec2(-13 + 2 * time, -20);\n\
				vec2 p_tex_alcatraz = vec2(1, 1) * p_alcatraz.xy / dim_alcatraz.xy + .5;\n\
				float f_alcatraz = texture(tex_alcatraz, p_tex_alcatraz).r;\n\
\n\
				vec2 dim_conspiracy = vec2(origdim_conspiracy.x / origdim_conspiracy.y, 1.) * base_dim;\n\
				vec2 p_conspiracy = hit.xz - vec2(-7, 10 + 1.5 * time);\n\
				vec2 p_tex_conspiracy = vec2(1, -1) * p_conspiracy.yx / dim_conspiracy.xy + .5;\n\
				float f_conspiracy = texture(tex_conspiracy, p_tex_conspiracy).r;\n\
\n\
				float f_thanks_greetings = f_greetings + f_thanks;\n\
				float f_alcatraz_conspiracy = f_alcatraz + f_conspiracy;\n\
				f_text = f_thanks_greetings + f_alcatraz_conspiracy;\n\
				color_text = color_thanks_greetings * f_thanks_greetings + color_alcatraz_conspiracy * f_alcatraz_conspiracy;;\n\
			} else if (time > 63 && time < 69) {\n\
				// alcatraz -- conspiracy\n\
				float time = time - 54;\n\
				vec2 dim_alcatraz = vec2(origdim_alcatraz.x / origdim_alcatraz.y, 1.) * base_dim;\n\
				vec2 p_alcatraz = hit.xz - vec2(-13 + 2 * time, -20);\n\
				vec2 p_tex_alcatraz = vec2(1, 1) * p_alcatraz.xy / dim_alcatraz.xy + .5;\n\
				float f_alcatraz = texture(tex_alcatraz, p_tex_alcatraz).r;\n\
\n\
				vec2 dim_conspiracy = vec2(origdim_conspiracy.x / origdim_conspiracy.y, 1.) * base_dim;\n\
				vec2 p_conspiracy = hit.xz - vec2(-7, 10 + 1.5 * time);\n\
				vec2 p_tex_conspiracy = vec2(1, -1) * p_conspiracy.yx / dim_conspiracy.xy + .5;\n\
				float f_conspiracy = texture(tex_conspiracy, p_tex_conspiracy).r;\n\
\n\
				// iq\n\
				time -= 10;\n\
				vec2 dim_iq = vec2(origdim_iq.x / origdim_iq.y, 1.) * base_dim;\n\
				vec2 p_iq = hit.xz - vec2(-55, -10);\n\
				vec2 p_tex_iq = vec2(-1, 1) * p_iq.yx / dim_iq.xy + .5;\n\
				float f_iq = texture(tex_iq, p_tex_iq).r;\n\
\n\
				float f_alcatraz_conspiracy = f_alcatraz + f_conspiracy;\n\
				f_text = f_iq + f_alcatraz_conspiracy;\n\
				color_text = color_iq * f_iq + color_alcatraz_conspiracy * f_alcatraz_conspiracy;\n\
			} else if (time > 69 && time < 76) {\n\
				// mercury -- urs\n\
				float time = time - 69;\n\
				vec2 dim_mercury = vec2(origdim_mercury.x / origdim_mercury.y, 1.) * base_dim;\n\
				vec2 p_mercury = hit.xz - vec2(-40, -20 - time * 3);\n\
				vec2 p_tex_mercury = vec2(-1, 1) * p_mercury.yx / dim_mercury.xy + .5;\n\
				float f_mercury = texture(tex_mercury, p_tex_mercury).r;\n\
\n\
				vec2 dim_urs = vec2(origdim_urs.x / origdim_urs.y, 1.) * base_dim;\n\
				vec2 p_urs = hit.xz - vec2(-52 - 3.2 * time, -8);\n\
				vec2 p_tex_urs = vec2(1, 1) * p_urs.xy / dim_urs.xy + .5;\n\
				float f_urs = texture(tex_urs, p_tex_urs).r;\n\
\n\
				f_text = f_mercury + f_urs;\n\
				color_text = color_mercury_urs;\n\
			} else if (time > 76 && time < 81) {\n\
				// nerd2nerd -- nerdarzt\n\
				float time = time - 79;\n\
				time = min(time, 0);\n\
				vec2 dim_nerd2nerd = vec2(origdim_nerd2nerd.x / origdim_nerd2nerd.y, 1.) * base_dim;\n\
				vec2 p_nerd2nerd = hit.xz - vec2(-42 + 4 * time, 15 - time);\n\
				vec2 p_tex_nerd2nerd = vec2(-1, 1) * p_nerd2nerd.yx / dim_nerd2nerd.xy + .5;\n\
				float f_nerd2nerd = texture(tex_nerd2nerd, p_tex_nerd2nerd).r;\n\
\n\
				vec2 dim_nerdarzt = vec2(origdim_nerdarzt.x / origdim_nerdarzt.y, 1.) * base_dim;\n\
				vec2 p_nerdarzt = hit.xz - vec2(-42 - 3 * time, -14.8 + time);\n\
				vec2 p_tex_nerdarzt = vec2(-1, 1) * p_nerdarzt.yx / dim_nerdarzt.xy + .5;\n\
				float f_nerdarzt = texture(tex_nerdarzt, p_tex_nerdarzt).r;\n\
\n\
				f_text = f_nerd2nerd + f_nerdarzt;\n\
				color_text = color_nerd2nerd_nerdarzt;\n\
			} else if (time > 81.0 && time < 90) {\n\
				// stroboholics\n\
				float time = time - 81;\n\
				vec2 dim_stroboholics = vec2(origdim_stroboholics.x / origdim_stroboholics.y, 1.) * base_dim;\n\
				vec2 p_stroboholics = hit.xz - vec2(-45, 25);\n\
				vec2 p_tex_stroboholics = vec2(1, -1) * p_stroboholics.yx / dim_stroboholics.xy + .5;\n\
				float f_stroboholics = texture(tex_stroboholics, p_tex_stroboholics).r;\n\
\n\
				f_text = f_stroboholics;\n\
				color_text = color_stroboholics;\n\
			}\n\
\n\
			final_color += f_text * color_text;\n\
			final_bloom += f_text * 2 * (senvelopes[12] + senvelopes[13] + senvelopes[14]);\n\
		}\n\
\n\
		// black fog\n\
		final_color *= mix(final_color, vec3(0), smoothstep(75, 95, distance(hit, view_position)));\n\
\n\
		final_bloom += mat_blooms[int(material)];\n\
	}\n\
	out_color.rgb = final_color;\n\
	out_color.a = final_bloom;\n\
}\n\
\n\
vec2 f(vec3 p) {\n\
	vec2 bounding = vec2(-sphere(transv(p, view_position), 100.), mat_bounding);\n\
\n\
	vec2 pos_delle = vec2(0);\n\
	float dy_kugel = udy_kugel;\n\
	if (time < 46) {\n\
		// dropdown\n\
		float time = time - 44;\n\
		pos_delle += vec2(-90, -45);\n\
		float anim_duration = 2;\n\
		float start_height = 40;\n\
		float t = time / anim_duration;\n\
		dy_kugel += -start_height * t*t + start_height;\n\
	} else if (time < 48) {\n\
		// bouncing\n\
		float time = time - 46;\n\
		pos_delle += vec2(-90, -45);\n\
		float v_0 = 2 * 20; // 2 * start_height\n\
		float omega_d = 5.5;\n\
		float daempfung = 2.3;\n\
		dy_kugel += v_0 / omega_d * exp(-daempfung * time) * cos(omega_d * time + TAU / 4);\n\
	} else if (time < 50) {\n\
		// rolling\n\
		float time = time - 48;\n\
		pos_delle += vec2(-90, -45);\n\
		pos_delle += vec2(6 * time, 20 * sin(TAU / 4 * smoothstep(0, 2, time)));\n\
	} else if (time < 54) {\n\
		// rolling2\n\
		float time = time - 50;\n\
		pos_delle += vec2(-78, -25);\n\
		pos_delle += vec2(-6 * time, 20 * sin(TAU / 4 * smoothstep(0, 2, time)));\n\
	} else if (time < 60) {\n\
		// thanks & greetings\n\
		float time = time - 54;\n\
		float t = smoothstep(0, 6, time);\n\
		vec2 p1 = vec2(-90, 15);\n\
		vec2 p2 = vec2(-50, -50);\n\
		vec2 p3 = vec2(-15.5, -11.5);\n\
		pos_delle += mix(mix(p1, p2, t), mix(p2, p3, t), t);\n\
	} else if (time < 65) {\n\
		// alcatraz -- conspiracy\n\
		pos_delle += vec2(-15.5, -11.5);\n\
	} else if (time < 67) {\n\
		// iq, hin\n\
		float time = time - 65;\n\
		pos_delle += vec2(-55.5, -11.5);\n\
		pos_delle += smoothstep(2, 0, time) * vec2(40, 0);\n\
	} else if (time < 69) {\n\
		// iq, weg\n\
		float time = time - 67.5;\n\
		pos_delle += vec2(-55.5, -11.5);\n\
		pos_delle += smoothstep(0, 2, time) * vec2(0, -40);\n\
	} else if (time < 76) {\n\
		// mercury & urs\n\
		float time = time - 69;\n\
		time = smoothstep(0., 5.5, time);\n\
		pos_delle += vec2(-90, -45);\n\
		pos_delle += 6 * time * vec2(9, 7.24);\n\
	} else if (time < 81) {\n\
		// nerd2nerdarzt\n\
		float time = time - 75;\n\
		pos_delle += vec2(-42, 0);\n\
		pos_delle.x += 7.5 * sin(TAU * .6 * time);\n\
		pos_delle.y += 15 * cos(TAU * .5 * time);\n\
	} else if (time < 100) {\n\
		// trichter\n\
		float time = time - 82.2;\n\
		float a = 70; // use to tweak shape of spiral\n\
		float t = time + 1.2;\n\
		t = t * (.5 + pow(t * .09, 8)); // use to tweak the speed of the kugel\n\
		pos_delle = a / t * vec2(-sin(t), cos(t));\n\
		pos_delle += vec2(25, 0);\n\
	}\n\
	vec3 p_delle = trans(p, pos_delle.x, 0, pos_delle.y);\n\
\n\
	float l = length(p_delle.xz) / sigma_delle;\n\
	float f_delle = 1 / (l*l + 1);\n\
	f_delle *= clamp(dy_kugel, -100, 0);\n\
\n\
	vec2 pos_trichter = vec2(25, 0);\n\
	vec3 p_trichter = trans(p, pos_trichter.x, 0, pos_trichter.y);\n\
	float d = length(p_trichter.xz);\n\
	float f_trichter = - sigma_trichter / d;\n\
\n\
	vec3 p_untergrund = domrep(p, 5, 5, 5);\n\
	p_untergrund.y -= (f_delle + f_trichter) / sqrt(-p.y);\n\
	float f_untergrund = roundbox(p_untergrund, vec3(.04), .02);\n\
	f_untergrund = max(f_untergrund, p.y);\n\
	vec2 vec_untergrund = vec2(f_untergrund, mat_untergrund);\n\
\n\
	vec3 p_gitter = domrep(p, 1, 1, 1);\n\
	p_gitter.xz = abs(p_gitter.xz);\n\
	p_gitter.y = p.y - f_delle;\n\
\n\
	float de_trichter = abs(f_trichter - p_gitter.y) / sqrt(1 + f_trichter * f_trichter / d / d);\n\
	float f_gitter = smax(de_trichter, smin(p_gitter.x, p_gitter.z, .01), .01) - .01;\n\
	vec2 vec_gitter = vec2(f_gitter, mat_gitter);\n\
\n\
	vec2 pos_kugel_trichter = pos_delle - pos_trichter;\n\
	vec2 npos_kugel_trichter = normalize(pos_kugel_trichter);\n\
	float x_kugel = length(pos_kugel_trichter);\n\
	float f_trichter_pos_kugel = - sigma_trichter / x_kugel;\n\
	vec3 correction_kugel = normalize(vec3(npos_kugel_trichter.x, x_kugel / f_trichter_pos_kugel, npos_kugel_trichter.y));\n\
	vec3 p_kugel = trans(p_delle, 0, f_trichter_pos_kugel + dy_kugel, 0);\n\
	p_kugel += radius_kugel * correction_kugel;\n\
	float f_kugel = sphere(p_kugel, radius_kugel);\n\
	vec2 vec_kugel = vec2(f_kugel, mat_kugel);\n\
\n\
	return min_material(min_material(min_material(bounding, vec_untergrund), vec_gitter), vec_kugel);\n\
}\n\
";
