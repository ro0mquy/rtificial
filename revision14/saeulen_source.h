static const char scene_saeulen_source[] = "\
void initValues();\n\
vec2 f(vec3 p);\n\
float octo_box(vec3 p, float d);\n\
float ao(vec3 p, vec3 n, float d, float i);\n\
vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color);\n\
float softshadow(vec3 ro, vec3 rd, float k);\n\
\n\
DEFINE_MARCH(march, f)\n\
DEFINE_NORMAL(calc_normal, f)\n\
DEFINE_SOFTSHADOW(softshadow, f)\n\
DEFINE_AO(ao, f)\n\
\n\
uniform float foo1;\n\
uniform float foo2;\n\
\n\
float light1_x = 10.0;\n\
float light1_y = 3.5;\n\
float light1_z = 1.5;\n\
\n\
float light2_x = -1.0;\n\
float light2_y = 3.5;\n\
float light2_z = 4.0;\n\
\n\
float normal_noise_radius = 0.019999999552965164;\n\
float diffuse_intensity = 1.0;\n\
//uniform float jump_duration;\n\
\n\
#define MAT_BOUNDING 0.\n\
#define MAT_FLOOR 1.\n\
#define MAT_CEILING 1.\n\
#define MAT_SAEULEN 2.\n\
\n\
const int number_lights = 2;\n\
vec3 lights[number_lights];\n\
\n\
vec3 color_background = vec3(0.05, 0.05, 0.05);\n\
vec3 color_fog = vec3(.67, .0, .9); // in hsv\n\
vec3 color_saeulen = vec3(1., 1., 1.);\n\
vec3 color_lights[number_lights];\n\
\n\
float intensity_lights[number_lights];\n\
\n\
void main() {\n\
	initValues();\n\
\n\
	vec3 color = vec3(0.);\n\
	vec3 p = view_position;\n\
	vec3 direction = get_direction();\n\
	vec3 light_color_factor = vec3(1.);\n\
	float reflection_factor = 1.;\n\
\n\
	for (int reflections = 0; reflections < 2; reflections++) {\n\
		int i;\n\
		p = march(p, direction, i);\n\
		if (i >= 100) {\n\
			// hit nothing\n\
			color = hsv2rgb(color_fog);\n\
			break;\n\
		}\n\
		float material = f(p)[1];\n\
\n\
		// apply some noise to the normal\n\
		vec3 normal = calc_normal(p);\n\
		float phi = smooth_noise(p) * TAU;\n\
		vec3 jitter = normal_noise_radius * smooth_noise(p) * vec3(cos(phi), sin(phi), 0.);\n\
		normal = normalize(normal - dot(jitter, normal) * normal + jitter);\n\
\n\
		vec3 light_color;\n\
		vec3 newColor = lighting(p, color_saeulen, direction, normal, light_color);\n\
		newColor += smoothstep(10., 50., float(i)); // iteration glow\n\
\n\
		// fog\n\
		vec3 hsv_newColor = rgb2hsv(newColor);\n\
		float fog_intensity = smoothstep(5., 35., distance(p, view_position));\n\
		hsv_newColor.yz = mix(hsv_newColor.yz, color_fog.yz, fog_intensity);\n\
		//newColor = hsv2rgb(hsv_newColor);\n\
\n\
		color += newColor * reflection_factor * light_color_factor;\n\
\n\
		// only floor and ceiling are reflective\n\
		if (material != MAT_FLOOR && material != MAT_CEILING) {\n\
			break;\n\
		}\n\
\n\
		light_color_factor *= light_color;\n\
		reflection_factor *= .4;\n\
\n\
		direction = reflect(direction, normal);\n\
	}\n\
\n\
	color = contrast(color, 1.23);\n\
	color *= vignette(.5);\n\
	out_color.rgb = color;\n\
	out_color.a = 2 * (senvelopes[12] + senvelopes[13]); // more bloom here, even if it' bright enough :)\n\
}\n\
\n\
void initValues() {\n\
	lights[0] = vec3(light1_x, light1_y, light1_z);\n\
	lights[1] = vec3(light2_x, light2_y, light2_z);\n\
\n\
	color_lights[0] = vec3(1., 1., .9);\n\
	color_lights[1] = vec3(1., .2, .0);\n\
\n\
	intensity_lights[0] = .9;\n\
	intensity_lights[1] = .9;\n\
}\n\
\n\
vec2 f(vec3 p) {\n\
	vec2 sphery = vec2(-sphere(p - view_position, 200.), MAT_BOUNDING); // bounding sphere\n\
\n\
	vec2 floor_plane = vec2(p.y, MAT_FLOOR);\n\
	vec2 ceiling_plane = vec2(-abs(p.y) + 25., MAT_CEILING);\n\
	vec2 room = min_material(floor_plane, ceiling_plane);\n\
\n\
	// jumpi di jump\n\
	float domrep_x = 28.;\n\
	float domrep_z = 24.;\n\
	float height_saeulen = 11.9; // - fuge_hoehe\n\
	float height_jump = 6.;\n\
	vec3 b = p;\n\
	float jump_duration = 60./136.;\n\
	//if (jump_duration != 0.) {\n\
		float old_time = time;\n\
		float offset = - .34 * jump_duration;\n\
		float time = old_time + offset;\n\
		float cut = 132.877;\n\
		float cut2 = 136.41;\n\
		if(old_time > cut) {\n\
			time = cut + offset + (min(old_time, cut2) - cut) * .05;\n\
		}\n\
		if(old_time > cut2) {\n\
			time += (old_time - cut) * 2.;\n\
		}\n\
		if(old_time > 159.246) {\n\
			time = old_time - .5 * jump_duration;\n\
		}\n\
		float progress = mod(time / jump_duration, 1.) - .5;\n\
		// cycloids are fun \\o/\n\
		// x = t - sin(t)\n\
		// y = 1 - cos(t)\n\
		p = trans(p,\n\
				(domrep_x /*- height_saeulen*/) * (progress - sin(TAU * progress) / TAU),\n\
				height_jump * (1. - cos(TAU * progress)) / 2.,\n\
				0.);\n\
\n\
		b = domrep(p, domrep_x, 1., domrep_z);\n\
		b.y = p.y;\n\
\n\
		float shift_y = height_saeulen / 2. * smoothstep(0., .5, abs(progress));\n\
		b.y -= shift_y;\n\
		b = rZ(TAU / 2. * progress) * b;\n\
\n\
		float angle = TAU / 16. * b.y / height_saeulen * 2 * (smoothstep(-.5, .5, progress) - .5);\n\
		float c = cos(angle);\n\
		float s = sin(angle);\n\
		mat2 m = mat2(c, s, -s, c);\n\
		b.xy = m * b.xy;\n\
		b.y += shift_y;\n\
	//} else {\n\
	//	b = domrep(p, domrep_x, 1., domrep_z);\n\
	//	b.y = p.y;\n\
	//}\n\
\n\
	float octo = octo_box(b, 2.);\n\
	octo = max(octo, -b.y);\n\
	octo = smax(octo, b.y - height_saeulen, .1);\n\
\n\
	float fuge_hoehe = .1;\n\
	float fuge_tiefe = .02;\n\
	float mod_y = mod(b.y + fuge_hoehe, 3.);\n\
	float fuge = smoothstep(.5 * fuge_hoehe, .0, mod_y)\n\
		+ smoothstep(.5 * fuge_hoehe, fuge_hoehe, mod_y);\n\
	fuge *= fuge_tiefe;\n\
\n\
	octo -= fuge;\n\
	vec2 saeulen = vec2(octo, MAT_SAEULEN);\n\
\n\
	return min_material(min_material(sphery, room), saeulen);\n\
}\n\
\n\
float octo_box(vec3 p, float d) {\n\
	float octo = max(abs(p.x), abs(p.z)) - d;\n\
	p = rY(TAU/8.) * p;\n\
	octo = smax(octo, max(abs(p.x), abs(p.z)) - d, .05);\n\
	return octo;\n\
}\n\
\n\
vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color) {\n\
	light_color = vec3(0.);\n\
	for (int i = 0; i < number_lights; i++) {\n\
		// normale (diffuse) lighting\n\
		vec3 to_light = normalize(lights[i] - p);\n\
		//float diffuse = lambert(to_light, normal);\n\
		float diffuse = oren_nayar(to_light, normal, -direction, .5);\n\
		diffuse *= diffuse_intensity;\n\
\n\
		// specular lighting\n\
		float specular = 0.;\n\
		specular = phong(to_light, normal, -direction, 30.);\n\
\n\
		// softshadows\n\
		float shadow = 1.;\n\
		//shadow = softshadow(p, lights[i], 32.);\n\
\n\
		light_color += color_lights[i] * intensity_lights[i] * (diffuse + specular) * shadow;\n\
	}\n\
\n\
	light_color += ao(p, direction, -0.15, 3.); // sub surface scattering\n\
	light_color *= ao(p, normal, 0.15, 5.); // ambient occlusion\n\
\n\
	return color * light_color;\n\
}\n\
";
