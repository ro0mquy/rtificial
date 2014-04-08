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
uniform float radius_kugel;
uniform float dy_kugel;
uniform float sigma_delle;

#define mat_bounding 0
#define mat_gitter 1
#define mat_untergrund 2
#define mat_kugel 3

vec3[] mat_colors = vec3[](
		vec3(.001),
		color_gitter,
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
		//final_color += .95 * lambert(light_direction, normal) + .05;
		final_color += .95 * oren_nayar(light_direction, normal, -dir, .5) + .05;
		//final_color += .95 * cook_torrance(light_direction, normal, -dir, .5, 450) + .05;
		final_color += vec3(phong(light_direction, normal, -dir, 50.));

		final_color *= mat_colors[int(material)];
		final_color *= .95 * softshadow(hit, 10 * light_direction, 32.) + .05;
		// black fog
		final_color *= mix(final_color, vec3(0), smoothstep(35, 45, length(hit)));
	}
	out_color = pow(final_color, vec3(1/2.2));
}

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(p, 50.), mat_bounding);

	vec3 p_delle = trans(p, 5 * sin(TAU / 4 * time), 0, 3 * sin(TAU / 5 * time));

	float l = length(p_delle.xz) / sigma_delle;
	float f_delle = 1 / (l*l + 1);
	//float f_delle = exp(-.5 * l*l);
	f_delle *= dy_kugel;

	vec3 p_untergrund = domrep(p, 5, 5, 5);
	p_untergrund.y -= f_delle / sqrt(-p.y);
	float f_untergrund = roundbox(p_untergrund, vec3(.04), .02);
	//float f_untergrund = sphere(p_untergrund, .05);
	f_untergrund = max(f_untergrund, p.y);
	vec2 vec_untergrund = vec2(f_untergrund, mat_untergrund);

	vec3 p_gitter = domrep(p, 1, 1, 1);
	p_gitter.y = p.y - f_delle;
	p_gitter = abs(p_gitter);
	float f_gitter = smax(p_gitter.y, smin(p_gitter.x, p_gitter.z, .01), .01) - .01;
	vec2 vec_gitter = vec2(f_gitter, mat_gitter);

	vec3 p_kugel = trans(p_delle, 0, dy_kugel + radius_kugel, 0);
	float f_kugel = sphere(p_kugel, radius_kugel);
	vec2 vec_kugel = vec2(f_kugel, mat_kugel);

	return min_material(min_material(min_material(bounding, vec_untergrund), vec_gitter), vec_kugel);
}
