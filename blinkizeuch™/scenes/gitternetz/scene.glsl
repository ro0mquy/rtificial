vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;
uniform vec3 light_position;
uniform vec3 color_gitter;
uniform vec3 color_kugel;
uniform float radius_kugel;
uniform float dy_kugel;
uniform float sigma_delle;
uniform float sigma_trichter;

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
		vec3 to_light = light_position - hit;
		float material = f(hit)[1];
		//final_color += .95 * lambert(to_light, normal) + .05;
		final_color += .95 * oren_nayar(to_light, normal, -dir, .5) + .05;
		//final_color += .95 * cook_torrance(to_light, normal, -dir, .5, 450) + .05;
		final_color += vec3(phong(to_light, normal, -dir, 50.));

		final_color *= mat_colors[int(material)];
		final_color *= .95 * softshadow(hit, light_position, 32.) + .05;
		// black fog
		final_color *= mix(final_color, vec3(0), smoothstep(35, 45, length(hit)));
	}
	out_color.rgb = final_color;
}

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(p, 50.), mat_bounding);

	vec2 pos_delle = vec2(5 * sin(TAU / 4 * time), 3 * sin(TAU / 5 * time));
	vec3 p_delle = trans(p, pos_delle.x, 0, pos_delle.y);

	float l = length(p_delle.xz) / sigma_delle;
	float f_delle = 1 / (l*l + 1);
	f_delle *= dy_kugel;

	vec2 pos_trichter = vec2(25, 0);
	vec3 p_trichter = trans(p, pos_trichter.x, 0, pos_trichter.y);
	float d = length(p_trichter.xz);
	float f_trichter = - sigma_trichter / d;

	vec3 p_untergrund = domrep(p, 5, 5, 5);
	p_untergrund.y -= (f_delle + f_trichter) / sqrt(-p.y);
	float f_untergrund = roundbox(p_untergrund, vec3(.04), .02);
	f_untergrund = max(f_untergrund, p.y);
	vec2 vec_untergrund = vec2(f_untergrund, mat_untergrund);

	vec3 p_gitter = domrep(p, 1, 1, 1);
	p_gitter = abs(p_gitter);
	p_gitter.y = p.y - f_delle;

	float de_trichter = abs(f_trichter - p_gitter.y) / sqrt(1 + f_trichter * f_trichter / d / d);
	float f_gitter = smax(de_trichter, smin(p_gitter.x, p_gitter.z, .01), .01) - .01;
	vec2 vec_gitter = vec2(f_gitter, mat_gitter);

	float f_trichter_pos_kugel = - sigma_trichter / distance(pos_delle, pos_trichter);
	vec3 p_kugel = trans(p_delle, 0, f_trichter_pos_kugel + dy_kugel + radius_kugel, 0);
	float f_kugel = sphere(p_kugel, radius_kugel);
	vec2 vec_kugel = vec2(f_kugel, mat_kugel);

	return min_material(min_material(min_material(bounding, vec_untergrund), vec_gitter), vec_kugel);
}
