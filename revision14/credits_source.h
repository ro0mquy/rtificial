static const char scene_credits_source[] = "\
vec2 f(vec3);\n\
vec2 f_noise(vec3);\n\
\n\
DEFINE_MARCH(march, f)\n\
DEFINE_NORMAL(calc_normal, f_noise)\n\
DEFINE_AO(ao, f)\n\
DEFINE_SOFTSHADOW(softshadow, f)\n\
\n\
uniform vec3 color_foo1;\n\
uniform vec3 color_foo2;\n\
uniform float foo1;\n\
uniform float foo2;\n\
\n\
uniform sampler2D tex_vincent;\n\
uniform sampler2D tex_drb;\n\
uniform sampler2D tex_ps0ke;\n\
uniform sampler2D tex_ro0mquy;\n\
\n\
vec2 origdim_vincent = vec2(4834, 879);\n\
vec2 origdim_drb = vec2(2735, 684);\n\
vec2 origdim_ps0ke = vec2(3762, 684);\n\
vec2 origdim_ro0mquy = vec2(5081, 684);\n\
\n\
#define mat_bounding 0\n\
#define mat_plane 1\n\
#define mat_kugel 2\n\
#define mat_vincent 3\n\
#define mat_drb 4\n\
#define mat_ps0ke 5\n\
#define mat_ro0mquy 6\n\
\n\
vec3[] mat_colors = vec3[](\n\
		vec3(1), // bounding\n\
		vec3(9,11,15)/vec3(255), // plane\n\
		vec3(.56, 0, 0), // kugel\n\
		//pow(vec3(240/255., 52/255., 173/255.), vec3(2.2)),\n\
		vec3(229, 46, 5)/vec3(255), // _vincent\n\
		pow(vec3(1.0, 0.0, 0.5176470588235295), vec3(2.2)), // drb\n\
		pow(vec3(0.01568627450980392, 0.8431372549019608, 0.9529411764705882), vec3(2.2)), // ps0ke\n\
		pow(vec3(0.,1.,0.), vec3(2.2)) // ro0mquy\n\
);\n\
\n\
const vec2 spread = vec2(-2., 30.);\n\
float dtime = max(0, time - 178.709);\n\
vec3 kugel_trans = vec3(-7 * sin(dtime / 5 * TAU), 1., spread.y /2. - (spread.y*2.) * dtime /5);\n\
\n\
void main(void){\n\
	vec3 dir = get_direction();\n\
	vec3 final_color = vec3(0);\n\
	int i;\n\
	vec3 hit = march(view_position, dir, i);\n\
	float material = f(hit)[1];\n\
	float bloom = 0.;\n\
	if(material > 0) {\n\
		vec3 normal = calc_normal(hit);\n\
		vec3 light = kugel_trans;//vec3(1, 8, 5);\n\
		light.y += 4;\n\
		light.z -= dtime * .2 - 5.;\n\
		final_color = mat_colors[int(material)];\n\
		vec3 to_light = light - hit;\n\
		if(material != mat_kugel) {\n\
			final_color *= lambert(to_light, normal);\n\
			final_color *= .1 + .9 * softshadow(hit, light, 24.);\n\
		} else {\n\
			final_color *= .05 + .95 * cook_torrance(to_light, normal, -dir, 1., 450.);\n\
			bloom = 1.;\n\
		}\n\
		final_color *= ao(hit, normal, .15, 5.);\n\
		float z = 10000000.;\n\
		if(material == mat_vincent) {\n\
			z = 0.;\n\
		} else if(material == mat_drb) {\n\
			z = -spread.y;\n\
		} else if(material == mat_ps0ke) {\n\
			z = -spread.y * 2. + 10;\n\
		} else if(material == mat_ro0mquy) {\n\
			z = -spread.y * 3. + 15;\n\
		} else if(material == mat_plane) {\n\
			final_color *= 1. - .3 * (sin(hit.x) * sin(hit.y) * .5 + .5);\n\
		}\n\
		if(material >= mat_vincent) {\n\
			bloom = 10. * smoothstep(spread.y / 2., 0., abs(kugel_trans.z - z - 1.));\n\
		}\n\
	}\n\
	final_color *= smoothstep(100, 20, distance(hit, view_position));\n\
\n\
	// end fade out\n\
	final_color *= smoothstep(192.8,191.,time);\n\
\n\
	out_color.rgb = final_color;\n\
	out_color.a = bloom;\n\
}\n\
\n\
float textbox(vec3 p, sampler2D tex, vec3 dim, float orig_width) {\n\
	float spread = 200.;\n\
	float d = texture(tex, p.xy / dim.xy + .5).r - .5;\n\
	d *= spread / orig_width * dim.x;\n\
	d += length(max(abs(p.xy) - dim.xy * .5, 0.));\n\
	return max(d, abs(p.z) - dim.z * .5) - .02; // -.02 for extra smoothness\n\
}\n\
\n\
float fbm(vec2 p) {\n\
	float sum = 0.;\n\
	float amplitude = .5;\n\
	float freq = 1.;\n\
	for(int i = 0; i < 2; i++) {\n\
		sum += classic_noise(p * freq) * amplitude;\n\
		freq *= 2.;\n\
		amplitude *= .5;\n\
	}\n\
	return sum;\n\
}\n\
\n\
vec2 f(vec3 p){\n\
	p.y += 1.;\n\
	p = rZ((smoothstep(spread.y, -spread.y * 6., p.z) - .5 ) * .3 + sin(dtime * 2. + .1 * p.z) * .03) * p;\n\
	vec2 bounding = vec2(-sphere(transv(p, view_position), 200.), mat_bounding);\n\
	vec3 base_dim = vec3(2., 2., 1.);\n\
\n\
	vec3 dim_vincent = vec3(origdim_vincent.x /origdim_vincent.y, 1., 1.) * base_dim;\n\
	vec3 dim_drb = vec3(origdim_drb.x /origdim_drb.y, 1., 1.) * base_dim;\n\
	vec3 dim_ps0ke = vec3(origdim_ps0ke.x /origdim_ps0ke.y, 1., 1.) * base_dim;\n\
	vec3 dim_ro0mquy = vec3(origdim_ro0mquy.x /origdim_ro0mquy.y, 1., 1.) * base_dim;\n\
	vec3 p_vincent = trans(p, dim_vincent.x * .5 + spread.x, base_dim.y * .5, 0);\n\
	vec3 p_drb = trans(p, -dim_drb.x * .5 - spread.x, base_dim.y * .5, -spread.y);\n\
	vec3 p_ps0ke = trans(p, dim_ps0ke.x * .5 + spread.x, base_dim.y * .5, -spread.y * 2);\n\
	vec3 p_ro0mquy = trans(p, -dim_ro0mquy.x * .5 - spread.x, base_dim.y * .5, -spread.y*3);\n\
\n\
	vec2 vincent = vec2(textbox(p_vincent, tex_vincent, dim_vincent, origdim_vincent.x), mat_vincent);\n\
	vec2 drb = vec2(textbox(p_drb, tex_drb, dim_drb, origdim_drb.x), mat_drb);\n\
	vec2 ps0ke = vec2(textbox(p_ps0ke, tex_ps0ke, dim_ps0ke, origdim_ps0ke.x), mat_ps0ke);\n\
	vec2 ro0mquy = vec2(textbox(p_ro0mquy, tex_ro0mquy, dim_ro0mquy, origdim_ro0mquy.x), mat_ro0mquy);\n\
	vec2 text = min_material(min_material(vincent, drb), min_material(ro0mquy, ps0ke));\n\
\n\
	vec3 norm1 = normalize(vec3(1, -1, 0));\n\
	vec3 norm2 = normalize(vec3(-1, -1, 0));\n\
	float tunnel = min(plane(trans(p, 0, 20, 0), norm1), plane(trans(p, 0, 20, 0), norm2));\n\
\n\
	vec2 plane = vec2(min(p.y, tunnel) , mat_plane);\n\
	vec2 kugel = vec2(sphere(transv(p, kugel_trans), 1.), mat_kugel);\n\
	return min_material(min_material(min_material(text, plane), bounding), kugel);\n\
}\n\
\n\
vec2 f_noise(vec3 p) {\n\
	vec2 f_result = f(p);\n\
	if(f_result[1] == mat_plane) {\n\
		f_result[0] += .1 * fbm(p.xz + .2 * fbm(5. * p.xz));\n\
	}\n\
	return f_result;\n\
}\n\
";
