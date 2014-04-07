vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;
uniform vec3 light_direction;
uniform vec3 color_gitter;
uniform vec3 color_kugel;

#define mat_bounding 0
#define mat_gitter 1
#define mat_kugel 2

vec3[] mat_colors = vec3[](
		vec3(.001),
		color_gitter,
		color_kugel
);

void main(void) {
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if (i < 100) {
		vec3 normal = calc_normal(hit);
		float material = f(hit)[1];
		//final_color += vec3(.95 * lambert(light_direction, normal) + .05);
		final_color += .95 * oren_nayar(light_direction, normal, -dir, .5) + .05;
		final_color += vec3(phong(light_direction, normal, -dir, 50.));
		if (material == mat_gitter) {
			/*
			vec3 c = vec3(floor(mod(hit.x, 2)));
			c += vec3(floor(mod(hit.z, 2)));
			c *= color_foo2;
			if (c.r >= 1) c *= color_foo2;
			final_color *= .8 * c + .1;
			*/

			// da step()t der baer
			vec3 c = vec3(step(mod(hit.x/2, 1), .05));
			c += vec3(step(mod(hit.z/2, 1), .05));
			c *= mat_colors[mat_gitter];
			final_color *= .9 * c + .01;
		} else {
			final_color *= mat_colors[int(material)];
		}
		final_color *= .95 * softshadow(hit, 10 * light_direction, 64.) + .05;
		// black fog
		//final_color = final_color * mix(final_color, vec3(0), smoothstep(60, 90, distance(hit, view_position)));
	}
	out_color = pow(final_color, vec3(1/2.2));
}

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(p, 50.), mat_bounding);

	vec3 p_kugel = trans(p, 5 * sin(TAU / 4 * time), 0, 3 * sin(TAU / 5 * time));

	float l = length(p_kugel.xz);
	vec3 p_gitter = p;
	//vec3 p_gitter = domrep(p, 1, 1, 1);
	p_gitter.y = p.y + 2 / (l*l + 1); // TODO maybe try some gaussian
	//p_gitter = abs(p_gitter);
	//vec2 p2_gitter = max(p_gitter.xy, p_gitter.yz);
	//float f_gitter = min(p2_gitter.x, p2_gitter.y) - .005;
	float f_gitter = p_gitter.y;
	vec2 vec_gitter = vec2(f_gitter, mat_gitter);

	float f_kugel = sphere(trans(p_kugel, 0, -.5, 0), 1);
	vec2 vec_kugel = vec2(f_kugel, mat_kugel);

	return min_material(min_material(bounding, vec_gitter), vec_kugel);
}
