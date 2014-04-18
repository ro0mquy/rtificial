static const char scene_schwurbeltunnel_source[] = "\
vec2 f(vec3);\n\
float fbm(vec3);\n\
\n\
DEFINE_MARCH(march, f)\n\
DEFINE_NORMAL(calc_normal, f)\n\
DEFINE_AO(ao, f)\n\
\n\
uniform vec3 color_foo1;\n\
uniform vec3 color_foo2;\n\
uniform float foo1;\n\
uniform float foo2;\n\
\n\
void main(void){\n\
	vec3 dir = get_direction();\n\
	vec3 final_color = vec3(0);\n\
	int i;\n\
	float factor = 1.;\n\
	float bloom = 0.;\n\
	vec3 p = view_position;\n\
	for(int j = 0; j < 3; j++) {\n\
		vec3 hit = march(p, dir, i);\n\
		int material = int(f(hit)[1]);\n\
		vec3 normal = calc_normal(hit);\n\
		float time = time - 136.402;\n\
		float duration = 150.526 - 136.402;\n\
		vec3 light;\n\
		float val;\n\
		if(time + 136.402 < 164.2) {\n\
			light = vec3(0, -16. + mix(-45, 45, time/duration), 0);\n\
			val = .5 + senvelopes[4] * 1.5;\n\
		} else {\n\
			light = vec3(0);\n\
			light.y = view_position.y + 8.;\n\
			val = 1.;\n\
		}\n\
		vec3 to_light = light - hit;\n\
\n\
		if(material == 1) {\n\
			vec3 color = val * .85 * cook_torrance(to_light, normal, -dir, 1., 450.) * vec3(1);\n\
			color += .05 * vec3(1);\n\
			final_color += factor * color;\n\
			factor *= .5;\n\
			dir = reflect(normal, dir);\n\
			p = hit + .01 * normal; // noise reduction\n\
		} else if(material == 2) {\n\
			if(j == 0) {\n\
				bloom = 1.;\n\
			}\n\
			final_color += factor * vec3(1, 0, 0) * .9 * oren_nayar(to_light, normal, -dir, 3.);\n\
			final_color += factor * vec3(1, 0, 0) * .05;\n\
			break;\n\
		} else if(material == 0) {\n\
			final_color += factor * mix(vec3(0), vec3(1), fbm(dir * 2.)) * max(1. - val, .3);\n\
			break;\n\
		}\n\
	}\n\
\n\
	out_color.rgb = final_color * vignette(.5);\n\
	out_color.a = bloom;\n\
}\n\
\n\
float schwurbelsaeule(vec3 p) {\n\
	// radial domrep code\n\
	float ra = length(p.xz);\n\
	float phi = atan(p.z, p.x);\n\
	float c = 4.;\n\
	float r = 6;\n\
\n\
	float phi1 = phi + TAU / 32. * p.y;\n\
	phi1 = mod(phi1, TAU /c) - TAU / c * .5;\n\
	vec3 p1 = vec3(ra * cos(phi1), p.y, ra * sin(phi1));\n\
	p1 = trans(p1, r, 0., 0.);\n\
\n\
	float phi2 = phi - TAU / 32. * p.y;\n\
	phi2 = mod(phi2, TAU /c) - TAU / c * .5;\n\
	vec3 p2 = vec3(ra * cos(phi2), p.y, ra * sin(phi2));\n\
	p2 = trans(p2, r, 0., 0.);\n\
\n\
	// actual schwurbel code\n\
	float height = 80.;\n\
	float side_lenght = 1. - .3 * (p.y / height + .5);\n\
\n\
	vec3 q1 = rY(p.y * .7 + time * 5. * .5) * p1;\n\
	vec3 r1 = rY(-p.y * .7 - time * 3. * .5) * p1;\n\
	float f1 = roundbox(q1, vec3(side_lenght, height, side_lenght), .5);\n\
	f1 = smin(f1, roundbox(r1, vec3(side_lenght, height, side_lenght), .5), .1);\n\
\n\
	vec3 q2 = rY(p.y * .7 + time * 5. * .5) * p2;\n\
	vec3 r2 = rY(-p.y * .7 - time * 3. * .5) * p2;\n\
	float f2 = roundbox(q2, vec3(side_lenght, height, side_lenght), .5);\n\
	f2 = smin(f2, roundbox(r2, vec3(side_lenght, height, side_lenght), .5), .1);\n\
\n\
	float f = smin(f1, f2, 1.);\n\
\n\
	return f;\n\
}\n\
\n\
vec2 f(vec3 p){\n\
	float time = time - 136.402;\n\
	float duration = 150.526 - 136.402;\n\
	float foo = schwurbelsaeule(p);\n\
	float bar = sphere(trans(p, 1.5 * sin(time * .5), mix(-50, 60, time / duration), 1.5 * cos(time * .5)), 1);\n\
\n\
	return min_material(vec2(-sphere(p - view_position, 500.), 0), min_material(vec2(foo, 1), vec2(bar, 2)));\n\
}\n\
\n\
float fbm(vec3 p) {\n\
	float sum = 0.;\n\
	float amplitude = .5;\n\
	float freq = 1.;\n\
	for(int i = 0; i < 3; i++) {\n\
		sum += smooth_noise(p * freq) * amplitude;\n\
		freq *= 2.;\n\
		amplitude *= .5;\n\
	}\n\
	return sum;\n\
}";
