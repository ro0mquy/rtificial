static const char scene_ufo_source[] = "\
vec2 f(vec3);\n\
float light_f(vec3 p);\n\
vec3 random_flight(float time);\n\
\n\
DEFINE_MARCH(march, f)\n\
DEFINE_NORMAL(calc_normal, f)\n\
DEFINE_AO(ao, f)\n\
DEFINE_SOFTSHADOW(softshadow, f)\n\
\n\
//uniform vec3 color_foo1;\n\
//uniform vec3 color_foo2;\n\
//uniform float foo1;\n\
//uniform float foo2;\n\
\n\
vec3 color_light = vec3(1.);\n\
vec3 color_sky = vec3 (0.0, 0.0039215688593685627, 0.011764707043766975);\n\
vec3 color_ufo_body = vec3(0.039215687662363052, 0.058823533356189728, 0.078431375324726105);\n\
vec3 color_ufo_cockpit = vec3(0.5215686559677124, 0.94509810209274292, 1.0);\n\
vec3 color_saturn = vec3( 0.80000001192092896, 0.69999998807907104, 0.30000001192092896);\n\
float star_amount = 0.97000002861022949;\n\
\n\
#define MAT_BOUNDING 0\n\
#define MAT_UFO_BODY 1\n\
#define MAT_UFO_COCKPIT 2\n\
#define MAT_UFO_LIGHTS 3\n\
#define MAT_BALL 4\n\
#define mat_saturn 5\n\
#define mat_saturn_rings 6\n\
#define mat_mars 7\n\
#define mat_laser 8\n\
\n\
vec3 colors[] = vec3[](\n\
	color_sky,\n\
	color_ufo_body,\n\
	color_ufo_cockpit,\n\
	vec3(1), // color ufo lights\n\
	vec3(144./255.,0.,0.),\n\
	color_saturn,\n\
	vec3(0.1),\n\
	vec3(0.75,0.7,0.5),\n\
	vec3(241./255., 24./255., .0)\n\
);\n\
\n\
float scene_start_time = 95.6;\n\
float scene_end_time = 117.143;\n\
float flight_start = 102.7;\n\
float flight_end = 111.746;\n\
vec3 p_flight = vec3(0);\n\
\n\
void main(void){\n\
	if (time < scene_end_time + 10) {\n\
		vec3 p1 = vec3(100.,-100.,33.);\n\
		vec3 p2 = vec3(500.,0.,50.);\n\
		vec3 p3 = vec3(100.,20.,100.);\n\
		vec3 p4 = vec3(-141.,-40.,100.);\n\
		float dtime = min(time, flight_end) - flight_start;\n\
		float t = dtime / (flight_end - flight_start);\n\
		vec3 p12 = mix(p1, p2, t);\n\
		vec3 p23 = mix(p2, p3, t);\n\
		vec3 p34 = mix(p3, p4, t);\n\
		p_flight = mix(mix(p12, p23, t), mix(p23, p34, t), t);\n\
	} else if (time < 179) {\n\
		float time = time - 169;\n\
		p_flight = random_flight(time);\n\
		p_flight = trans(p_flight, -217, 44, -100);\n\
	}\n\
\n\
	vec3 direction = get_direction();\n\
	vec3 final_color = vec3(0);\n\
	float final_bloom = 0;\n\
	vec3 light = view_position + vec3(10, 40, 10);\n\
	vec3 hit = view_position;\n\
	vec3 light_color_factor = vec3(1.);\n\
	float reflection_factor = 1.;\n\
\n\
	for (int reflections = 0; reflections < 2; reflections++) {\n\
		int i;\n\
		hit = march(hit, direction, i);\n\
		if (i >= 100) {\n\
			// hit nothing\n\
			break;\n\
		}\n\
\n\
		int material = int(f(hit)[1]);\n\
		vec3 normal = calc_normal(hit);\n\
		vec3 to_light = light - hit;\n\
		vec3 to_view = view_position - hit;\n\
\n\
		vec3 light_color = vec3(lambert(to_light, normal));\n\
		if(material != mat_saturn){\n\
			light_color += vec3(phong(to_light, normal, to_view, 50.));\n\
		}\n\
		light_color *= color_light;\n\
		light_color *= ao(hit, normal, .15, 5);\n\
		if (material != MAT_BOUNDING) {\n\
			light_color *= softshadow(hit, light, 64.);\n\
		}\n\
\n\
		vec3 new_color = colors[material] * light_color;\n\
\n\
		if(material == MAT_BOUNDING) {\n\
			float brightness = smoothstep(star_amount, 1, smooth_noise(150. * direction));\n\
			new_color = mix(new_color, vec3(1), brightness);\n\
			final_bloom = brightness;\n\
		} else if (material == MAT_UFO_LIGHTS) {\n\
			new_color = colors[MAT_UFO_LIGHTS];\n\
			final_bloom = 1;\n\
		} else if (material == mat_saturn) {\n\
			float random = smooth_noise(0.7 * hit.yyy);\n\
			new_color *= random;\n\
			final_bloom = .5;\n\
		} else if (material == mat_mars) {\n\
			float random = smoothstep(0., 1., clamp(smooth_noise(.83 * hit), 0.3, 0.7));\n\
			new_color *= random;\n\
		}\n\
\n\
		final_color += new_color * reflection_factor * light_color_factor;\n\
\n\
		float fuck_you_distance = distance(hit, view_position);\n\
\n\
		// volumetric laz0rs\n\
		// kind of raymarching\n\
		float minimal_dist = light_f(hit);\n\
		for (float t = 0; t < fuck_you_distance; ) {\n\
			vec3 p = view_position + t * direction;\n\
			float dist = light_f(p);\n\
			minimal_dist = min(minimal_dist, dist);\n\
			t += dist;\n\
			if (dist < .001) break;\n\
		}\n\
		float laser = 1. / (pow(minimal_dist * 4, 4 - 3 * smoothstep(171, 178, time)) + 1);\n\
		final_bloom += laser;\n\
		//float laser = .7 * exp(-minimal_dist*minimal_dist);\n\
		if (time < scene_end_time + 10) {\n\
			final_color += colors[mat_laser] * laser;\n\
		} else if (time < 179) {\n\
			final_color += laser * hsv2rgb(vec3(.1 * aenvelopes[2], 1, 1));\n\
		}\n\
\n\
		// only cockpit is reflective\n\
		if (material != MAT_UFO_COCKPIT) {\n\
			break;\n\
		}\n\
\n\
		light_color_factor *= light_color;\n\
		reflection_factor *= .8;\n\
\n\
		direction = reflect(direction, normal);\n\
	}\n\
\n\
	final_color *= vignette(1.);\n\
	final_color = mix(final_color, vec3(0), step(scene_end_time, time) * (1 - step(scene_end_time + 10, time)));\n\
	final_color = mix(final_color, vec3(0), smoothstep(177., 178.709, time));\n\
\n\
	out_color.rgb = final_color;\n\
	out_color.a = final_bloom;\n\
}\n\
\n\
float light_f(vec3 p){\n\
	float f_beam = 10000;\n\
	if (time < scene_end_time + 10) {\n\
		if(time > flight_start){\n\
			p = transv(p, p_flight);\n\
		}\n\
		float my_time = time -113.508;\n\
	//	if(time > scene_end_time -3.5){\n\
	//		my_time *= smoothstep(1., 0., time);\n\
	//	}\n\
		vec3 p_laser = trans(p, 100 * my_time, 20*0.67,0);\n\
		vec3 q = domrep(p_laser, 43, 1,1);\n\
		q.yz = p_laser.yz;\n\
		q.z = abs(q.z);\n\
		float beam = line(q, vec3(-1,0,3), vec3(1,0,3), 0);\n\
		f_beam = max(max(beam, -p.x+2.5), p_laser.x);\n\
	} else if (time < 179) {\n\
		float time = time - 169;\n\
		p = trans(p, 217, -30, 100);\n\
		const float bpm = 60. / 136.;\n\
		float t_offset = 0;\n\
		const int len_offsets = 11;\n\
		float[len_offsets] offsets = float[](1, 6, 10, 13, 15, 17, 18, 19, 19.5, 20, 20.5);\n\
\n\
		for (int i = 0; i < len_offsets; i++) {\n\
			t_offset = offsets[i] * bpm;\n\
			if (time > t_offset && time < t_offset + 4) {\n\
				float t = time - t_offset;\n\
\n\
				vec3 p_start = random_flight(t_offset);\n\
				vec3 p_laser = transv(p, p_start);\n\
				p_laser = rZ(t_offset * 100) * p_laser;\n\
				float r = length(p_laser.xz);\n\
				float alpha = atan(p_laser.z, p_laser.x);\n\
				float c = 20;\n\
				float beta = floor(alpha / TAU * c);\n\
				float phi = mod(alpha, TAU /c) - TAU / c * .5;\n\
				vec3 p_sdom = vec3(r * cos(phi), p_laser.y, r * sin(phi));\n\
\n\
				float rand_beta = rand(vec2(beta, t_offset));\n\
				vec3 dir = vec3(40 + 10 * rand_beta, 20 * rand_beta, 0);\n\
				vec3 l = 6 * normalize(dir);\n\
				f_beam = min(line(p_sdom, t * dir, t * dir + l, 0), f_beam);\n\
			}\n\
		}\n\
	}\n\
	return f_beam;\n\
}\n\
\n\
vec2 f(vec3 p){\n\
	vec2 bounding = vec2(-sphere(transv(p, view_position), 500.), MAT_BOUNDING);\n\
\n\
	vec3 p_saturn = trans(p, 217, -30, 100);\n\
	float saturn = sphere(p_saturn, 13);\n\
	vec2 the_saturn = vec2(saturn, mat_saturn);\n\
\n\
	float ring1 = abs(sphere(p_saturn, 23)) - 1;\n\
	ring1 = max(ring1, abs(p_saturn.y) - .05);\n\
	float ring2 = abs(sphere(p_saturn, 20.5)) - .8;\n\
	if (time < scene_end_time + 10) {\n\
		ring2 = max(ring2, abs(p_saturn.y) - .05);\n\
	} else {\n\
		ring2 = max(ring2, abs(dot(p_saturn, normalize(vec3(1,.8,0)))) - .05);\n\
	}\n\
	float rings = min(ring1, ring2);\n\
	vec2 saturn_rings = vec2(rings, mat_saturn_rings);\n\
\n\
	vec3 p_mars = trans(p, -100, -50, 100);\n\
	float mars = sphere(p_mars, 7);\n\
	vec2 the_mars = vec2(mars, mat_mars);\n\
\n\
/*\n\
	vec2 bezier = vec2(sphere(transv(p, p1), 1), MAT_BALL);\n\
	bezier = min_material(bezier, vec2(sphere(transv(p, p2), 1), MAT_BALL));\n\
	bezier = min_material(bezier, vec2(sphere(transv(p, p3), 1), MAT_BALL));\n\
	bezier = min_material(bezier, vec2(sphere(transv(p, p4), 1), MAT_BALL));\n\
*/\n\
	if(time > flight_start){\n\
		p = transv(p, p_flight);\n\
	}\n\
\n\
	vec3 cockpit_p = trans(p, 0., 50*.23, 0.);\n\
\n\
	float ufo_bottom = sphere(p, 15);\n\
	float ufo_top = sphere(trans(p, 0., 0.55*50., 0.), 15);\n\
	float ufo_ball_hole = max(smax(ufo_top, ufo_bottom, 0.05), -sphere(trans(cockpit_p, 0., 6.5, 0.), 5.35));\n\
	vec2 ufo_body = vec2(ufo_ball_hole, MAT_UFO_BODY);\n\
\n\
	float cock_anim_start = 98.651;\n\
	float cock_anim_duration = 4.05;\n\
	float cock_anim_time = (clamp(time, cock_anim_start, cock_anim_start + cock_anim_duration) -cock_anim_start)/(cock_anim_duration);\n\
	float cockpit_close = 5-5*cock_anim_time;\n\
	vec3 cut_p = trans(cockpit_p, 0., 5*0.59, 0.);\n\
	float cut_planes = min(plane(cut_p, normalize(vec3(0., cockpit_close, -1.))), plane(cut_p, normalize(vec3(0., cockpit_close, 1.))));\n\
	float empty_cockpit = max(max(sphere(cockpit_p, 5), -sphere(p, 15)), -sphere(cockpit_p, 4.9));\n\
	vec2 ufo_cockpit = vec2(max(empty_cockpit, cut_planes), MAT_UFO_COCKPIT);\n\
\n\
	float ball_anim_start = scene_start_time -2.;\n\
	float ball_anim_duration = 5.;\n\
	float ball_anim_time = (clamp(time, ball_anim_start, ball_anim_start + ball_anim_duration) -ball_anim_start)/(ball_anim_duration);\n\
	float ball_gravity = 500;\n\
	float ball_height = 0.5*ball_gravity /* start height */ +  -0.5 * ball_gravity * ball_anim_time*ball_anim_time /* 1/2 a t^2 */ + 14.4 /* end height */;\n\
	vec2 ball = vec2(max(sphere(trans(p, 0., ball_height,  0.), 1.75), -ufo_ball_hole), MAT_BALL);\n\
\n\
\n\
	vec3 p_light = rY(time * TAU /2)*p;\n\
	float r = length(p_light.xz);\n\
	float phi = atan(p_light.z, p_light.x);\n\
	float c = 6.;\n\
	phi = mod(phi, TAU /c) - TAU / c * .5;\n\
	vec3 q = vec3(r * cos(phi), p_light.y, r * sin(phi));\n\
	q = trans(q, 5., 14.1, 0.);\n\
	vec2 ufo_lights = vec2(sphere(q, .1), MAT_UFO_LIGHTS);\n\
\n\
//	ball = min_material(ball, bezier);\n\
	the_saturn.x += 10000 * step(scene_end_time + 10, time);\n\
	return min_material(min_material(min_material(ufo_body, ufo_cockpit), bounding), min_material(ufo_lights, min_material(min_material(ball, the_mars), min_material(the_saturn, saturn_rings))));\n\
}\n\
\n\
vec3 random_flight(float time) {\n\
	vec3 p;\n\
	p.x = 35 * sin(TAU * .5 * time) * sin(TAU * .3 * time);\n\
	p.y = 7 * sin(TAU * time);\n\
	p.z = 35 * sin(TAU * .5 * time) * cos(TAU * .2 * time);\n\
	return p;\n\
}\n\
";
