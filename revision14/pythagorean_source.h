static const char scene_pythagorean_source[] = "\
vec2 f(vec3);\n\
vec2 g(vec3);\n\
\n\
float fbm(vec2 p);\n\
\n\
DEFINE_MARCH(march, f)\n\
DEFINE_NORMAL(calc_normal, f)\n\
DEFINE_NORMAL(calc_normal_floor, g)\n\
DEFINE_AO(ao, f)\n\
\n\
uniform vec3 color_foo1;\n\
uniform vec3 color_foo2;\n\
uniform float foo1;\n\
uniform float foo2;\n\
\n\
#define mat_bounding 0\n\
#define mat_floor 1\n\
#define mat_kugel 2\n\
#define mat_tree1 3\n\
#define mat_tree2 4\n\
#define mat_tree3 5\n\
#define mat_tree4 6\n\
#define mat_tree5 7\n\
#define mat_tree6 8\n\
#define mat_tree7 9\n\
#define mat_tree8 10\n\
#define mat_tree9 11\n\
\n\
vec3[] tree_colors = vec3[](\n\
	vec3(.9, .9, .95),\n\
	vec3(.7, .7, .75),\n\
	vec3(.1, .1, .1)\n\
\n\
);\n\
\n\
vec3 colors[] = vec3[](\n\
	vec3(0),\n\
	vec3(.4),\n\
	vec3(144./255., 0, 0),\n\
	tree_colors[0],\n\
	tree_colors[1],\n\
	tree_colors[2],\n\
	tree_colors[0],\n\
	tree_colors[1],\n\
	tree_colors[2],\n\
	tree_colors[1],\n\
	tree_colors[2],\n\
	tree_colors[0]\n\
);\n\
\n\
const int num_rand_colors = 6;\n\
vec3[num_rand_colors] rand_colors = vec3[num_rand_colors](\n\
	vec3(0.4196078431372549, 0.8941176470588236, 0.0),\n\
	vec3(0.0, 0.45098039215686275, 0.24313725490196078),\n\
	vec3(0.8117647058823529, 0.9686274509803922, 0.0),\n\
	vec3(0.8862745098039215, 0.0, 0.2823529411764706),\n\
	vec3(0.0, 0.6901960784313725, 0.3764705882352941),\n\
	vec3(1.)\n\
);\n\
\n\
float wakeup_end = 14.757;\n\
float jump_end = 15.616;\n\
float path1_end = 21.8;//jump_end + 1/.15;\n\
float wakeup = smoothstep(12.594, wakeup_end, time);\n\
float wakeup_limited = wakeup * step(-15, -time);\n\
\n\
void main(void){\n\
	vec3 dir = get_direction();\n\
	vec3 final_color = vec3(0);\n\
	int i;\n\
	vec3 hit = march(view_position, dir, i);\n\
	int material = int(f(hit)[1]);\n\
	vec3 light = vec3(0, 10, 0);\n\
	vec3 normal;\n\
	vec3 color = colors[material];\n\
	vec3 to_light = light - hit;\n\
	float m = 0;\n\
	float bloom = 0.;\n\
	if(material >= mat_tree1) {\n\
		normal = calc_normal(hit);\n\
		m = 0;\n\
\n\
		vec2 coord = floor(hit.xz / 20);\n\
		material += int(time * length(coord));\n\
		vec3 bunt = rand_colors[material % num_rand_colors];\n\
\n\
		int[] playing = int[](10, 11, 2, 3, 31, 31, 31, 31, 31);\n\
		int instrument = playing[material % 9];\n\
		float intensity = clamp(2 * senvelopes[instrument], 0, 1);\n\
		color = mix(color, bunt, intensity);\n\
		bloom += intensity;\n\
	} else if(material == mat_floor) {\n\
		normal = calc_normal_floor(hit);\n\
		m = .42;\n\
		final_color = vec3(.2,.7,.0);\n\
	} else if(material == mat_kugel) {\n\
		normal = calc_normal(hit);\n\
		m = .75;\n\
		float senvs = senvelopes[2] + senvelopes[3];\n\
		final_color += (.1 + senvs + .7 * wakeup_limited) * color;\n\
		bloom = senvs * 2;\n\
		bloom += wakeup;\n\
	}\n\
\n\
	if(material == mat_kugel) {\n\
		final_color += color * cook_torrance(to_light, normal, -dir, m, 450);\n\
		final_color *= ao(hit, normal, .3, 5.);\n\
	} else if(material != mat_bounding) {\n\
		final_color += color * (.05 + .95 * oren_nayar(to_light, normal, -dir, m));\n\
		final_color *= ao(hit, normal, .3, 5.);\n\
	} else {\n\
		//vec2 spherical = vec2(acos(dir.y) / 3.141 * 2., abs(atan(dir.z, dir.x) / 3.141));\n\
		//final_color = hsv2rgb(vec3(.63, .6 + .4 * fbm(spherical * 20.), .15));\n\
		//final_color = vec3(hit.y/100., 0.,0.); // R\n\
		//final_color = vec3(0., hit.y/100.,0.); // G\n\
		final_color = vec3(0.,.5,hit.y/100); // B\n\
	}\n\
\n\
\n\
	//vec3 strobo_bunt = rand_colors[int(time) % num_rand_colors];\n\
	vec3 strobo_bunt = rand_colors[material % num_rand_colors];\n\
	float strobo_intensity = clamp(2 * (senvelopes[2]+ senvelopes[31]+ senvelopes[10] + senvelopes[11]), 0, 1);\n\
	vec3 strobo_color = mix(final_color, strobo_bunt, strobo_intensity);\n\
\n\
	final_color *= smoothstep(250., 50., distance(view_position, hit));\n\
\n\
	if(time > 118.){ // turn on some fuckin strobo\n\
		final_color = 0.5 * (final_color +  strobo_color) ;\n\
		if(strobo_intensity > .2){\n\
			final_color += .1;\n\
		}\n\
	}\n\
	final_color *= vignette(.9);\n\
	final_color = mix(final_color, vec3(0), step(28.7, time) * (1 - step(28.7 + 10, time)));\n\
	out_color.rgb = final_color;\n\
	out_color.a = bloom;\n\
}\n\
\n\
float cube(vec3 p, float r) {\n\
	// seems to be faster than\n\
	// return box(p, vec3(1.));\n\
	float s = sphere(p, r);\n\
	p = abs(p);\n\
	float f = max(p.x, max(p.y, p.z)) - r;\n\
	return mix(s, f, 1. + .4 * smoothstep(5., 15., time));\n\
}\n\
\n\
vec2 pythagoraen(vec3 p) {\n\
	float alpha = (1. - 0.6 * smoothstep(0., 10., time)) * 90.;\n\
	float beta = radians(90. - alpha);\n\
	alpha = radians(alpha);\n\
	float a = cos(alpha);\n\
	float b = sqrt(1. - a * a);\n\
	float p_a = 1.;\n\
	vec2 tree = vec2(cube(p, p_a), mat_tree3);\n\
\n\
	for(int i = 0; i < 10; i++) {\n\
		float q_a = a * p_a;\n\
		vec3 q = trans(p, 0, p_a , -p_a);\n\
		q = rX(alpha) * q;\n\
		q = trans(q, 0, q_a, q_a);\n\
\n\
		float r_a = b * p_a;\n\
		vec3 r = trans(p, 0, p_a, p_a);\n\
		r = rX(-beta) * r;\n\
		r = trans(r, 0, r_a, -r_a);\n\
\n\
		vec2 cube_1 = vec2(cube(r, r_a), mat_tree1 + (i * i) % 9);\n\
		vec2 cube_2 = vec2(cube(q, q_a), mat_tree1 + (i * (i+1)) % 9);\n\
		vec2 next_cubes = min_material(cube_1, cube_2);\n\
		tree = min_material(tree, next_cubes);\n\
\n\
		p = q;\n\
		p_a = q_a;\n\
		p = rY(radians(smoothstep(0., 10., time) * 90.)) * p;\n\
	}\n\
	return tree;\n\
}\n\
\n\
float fbm(vec2 p) {\n\
	float sum = 0.;\n\
	float amplitude = .5;\n\
	float freq = 1.;\n\
	for(int i = 0; i < 3; i++) {\n\
		sum += classic_noise(p * freq) * amplitude;\n\
		freq *= 2.;\n\
		amplitude *= .5;\n\
	}\n\
	return sum;\n\
}\n\
\n\
float f_floor(vec3 p) {\n\
	return p.y + 1.;\n\
}\n\
\n\
vec2 f(vec3 p) {\n\
	float c = 20;\n\
	vec3 q = domrep(p, c, 1., c);\n\
	q.y = p.y;\n\
	float foo = 4. * rand(400. * (floor(p.xz / c) - 123.));\n\
	float foo3 = 4. * rand(400. * (floor(p.xz / c) - 456.));\n\
	q = trans(q, 2. * (foo - 2.), .0, 2. * (foo3 - 2.));\n\
	vec2 tree = pythagoraen(rY(foo * radians(90.)) * q);\n\
	tree.y = mod(floor(tree.y + foo), 9) + mat_tree1;\n\
	vec2 bounding = vec2(-sphere(transv(p, view_position), 200.), mat_bounding);\n\
\n\
	vec3 trans_kugel = vec3(0);\n\
	vec2 p1 = vec2(0);\n\
	vec2 p2 = vec2(30, -5);\n\
	vec2 p3 = vec2(-12, -36);\n\
	vec2 p4 = vec2(5 + .94 * 5., -72 + .44 * 5.);\n\
	vec2 p5 = vec2(-30, -60);\n\
	vec2 p6 = vec2(-22, -29);\n\
	vec2 p7 = vec2(-50, -24);\n\
	if (time < wakeup_end) {\n\
		vec2 k = vec2(time * 5., time * 3.);\n\
		trans_kugel = .1 * vec3(rand(k), rand(k + 2.), rand(k + 3.)) * wakeup_limited;\n\
	} else if (time < jump_end) {\n\
		float time = time - wakeup_end;\n\
		float anim_duration = jump_end - wakeup_end;\n\
		time -= anim_duration / 2;\n\
		float gravity = 20;\n\
		float h = .5 * gravity * time * time;\n\
		h = .5 * gravity * anim_duration * anim_duration / 4 - h;\n\
		trans_kugel.y = h;\n\
	} else if(time < path1_end) {\n\
		float dtime = time - jump_end;\n\
		float t = dtime / (path1_end - jump_end);\n\
		vec2 p12 = mix(p1, p2, t);\n\
		vec2 p23 = mix(p2, p3, t);\n\
		vec2 p34 = mix(p3, p4, t);\n\
		trans_kugel.xz = mix(mix(p12, p23, t), mix(p23, p34, t), t);\n\
	} else {\n\
		float dtime = min(time, 28.2)  - path1_end;\n\
		float t = dtime * .15;\n\
		vec2 p12 = mix(p4, p5, t);\n\
		vec2 p23 = mix(p5, p6, t);\n\
		vec2 p34 = mix(p6, p7, t);\n\
		trans_kugel.xz = mix(mix(p12, p23, t), mix(p23, p34, t), t);\n\
	}\n\
\n\
	vec2 kugel = vec2(sphere(transv(p, trans_kugel), 1.), mat_kugel);\n\
	//kugel = min_material(vec2(sphere(trans(p, p1.x, 0, p4.y), .5), mat_kugel), kugel);\n\
	//kugel = min_material(vec2(sphere(trans(p, p2.x, 0, p4.y), .5), mat_kugel), kugel);\n\
	//kugel = min_material(vec2(sphere(trans(p, p3.x, 0, p4.y), .5), mat_kugel), kugel);\n\
	//kugel = min_material(vec2(sphere(trans(p, p4.x, 0, p4.y), .5), mat_kugel), kugel);\n\
	//kugel = min_material(vec2(sphere(trans(p, p5.x, 0, p5.y), .5), mat_kugel), kugel);\n\
	//kugel = min_material(vec2(sphere(trans(p, p6.x, 0, p6.y), .5), mat_kugel), kugel);\n\
	//kugel = min_material(vec2(sphere(trans(p, p7.x, 0, p7.y), .5), mat_kugel), kugel);\n\
	return min_material(tree, min_material(vec2(f_floor(p), mat_floor), min_material(bounding, kugel)));\n\
}\n\
\n\
vec2 g(vec3 p) {\n\
	return vec2(f_floor(p) + fbm(p.xz * .7) * .03, 2.);\n\
}\n\
";
