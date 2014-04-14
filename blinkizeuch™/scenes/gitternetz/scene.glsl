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
uniform vec3 color_text;
uniform float radius_kugel;
uniform float udy_kugel;
uniform float sigma_delle;
uniform float sigma_trichter;

uniform sampler2D tex_mercury;
uniform sampler2D tex_urs;
uniform sampler2D tex_iq;

vec2 origdim_mercury = vec2(43, 7);
vec2 origdim_urs = vec2(18, 7);
vec2 origdim_iq = vec2(9, 8);

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

float[] mat_blooms = float[](
		0,
		0,
		.1,
		1
);

void main(void) {
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	float final_bloom = 0;
	int i;
	vec3 hit = march(view_position, dir, i);
	if (i < 100) {
		vec3 normal = calc_normal(hit);
		vec3 to_light = light_position - hit;
		float material = f(hit)[1];
		if (material == mat_kugel) {
			//final_color += .05 + .95 * cook_torrance(to_light, normal, -dir, .5, 450);
			final_color += .05 + .95 * lambert(to_light, normal);
		} else {
			final_color += .05 + .95 * oren_nayar(to_light, normal, -dir, .5);
		}
		final_color += vec3(phong(to_light, normal, -dir, 50.));

		final_color *= mat_colors[int(material)];
		if (material != mat_bounding) {
			final_color *= .95 * softshadow(hit, light_position, 32.) + .05;
		}

		if (material == mat_gitter) {
			vec2 base_dim = vec2(8., 10.);

			vec2 dim_mercury = vec2(origdim_mercury.x / origdim_mercury.y, 1.) * base_dim;
			vec2 p_mercury = hit.zx - vec2(-15 + 2 * time, 0);
			vec2 p_tex_mercury = vec2(1, -1) * p_mercury / dim_mercury.xy + .5;
			float f_mercury = texture(tex_mercury, p_tex_mercury).r;

			vec2 dim_urs = vec2(origdim_urs.x / origdim_urs.y, 1.) * base_dim;
			vec2 p_urs = hit.zx - vec2(-20 + 1.2 * time, -15);
			vec2 p_tex_urs = vec2(1, -1) * p_urs / dim_urs.xy + .5;
			float f_urs = texture(tex_urs, p_tex_urs).r;

			vec2 dim_iq = vec2(origdim_iq.x / origdim_iq.y, 1.) * base_dim;
			vec2 p_iq = hit.xz - vec2(-15 + 3 * time, 0);
			vec2 p_tex_iq = vec2(1, 1) * p_iq / dim_iq.xy + .5;
			float f_iq = texture(tex_iq, p_tex_iq).r;

			float f_text = max(max(f_mercury, f_urs), f_iq);
			final_color += f_text * color_text;
			final_bloom += f_text * (1 - mod(time, 1));
		}

		// black fog
		final_color *= mix(final_color, vec3(0), smoothstep(75, 95, distance(hit, view_position)));

		final_bloom += mat_blooms[int(material)];
	}
	out_color.rgb = final_color;
	out_color.a = final_bloom;
}

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(transv(p, view_position), 100.), mat_bounding);

	vec2 pos_delle = vec2(0);
	float dy_kugel = udy_kugel;
	if (time < 2) {
		pos_delle += vec2(5);
		float time = time;
		float anim_duration = 2;
		float start_height = 40;
		float t = time / anim_duration;
		dy_kugel += -start_height * t*t + start_height;
	} else if (time < 10) {
		pos_delle += vec2(5);
		float time = time - 2;
		float v_0 = 2 * 20; // 2 * start_height
		float omega_d = 10 * foo1;
		float daempfung = 10 * foo2;
		dy_kugel += v_0 / omega_d * exp(-daempfung * time) * cos(omega_d * time + TAU / 4);
	} else if (time < 100) {
		float a = 70; // use to tweak shape of spiral
		float t = time + .001;
		t = t * (.5 + pow(t * .09, 8)); // use to tweak the speed of the kugel
		pos_delle = a / t * vec2(-sin(t), cos(t));
		pos_delle += vec2(25, 0);
	}
	vec3 p_delle = trans(p, pos_delle.x, 0, pos_delle.y);

	float l = length(p_delle.xz) / sigma_delle;
	float f_delle = 1 / (l*l + 1);
	f_delle *= clamp(dy_kugel, -100, 0);

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
	p_gitter.xz = abs(p_gitter.xz);
	p_gitter.y = p.y - f_delle;

	float de_trichter = abs(f_trichter - p_gitter.y) / sqrt(1 + f_trichter * f_trichter / d / d);
	float f_gitter = smax(de_trichter, smin(p_gitter.x, p_gitter.z, .01), .01) - .01;
	vec2 vec_gitter = vec2(f_gitter, mat_gitter);

	vec2 pos_kugel_trichter = pos_delle - pos_trichter;
	vec2 npos_kugel_trichter = normalize(pos_kugel_trichter);
	float x_kugel = length(pos_kugel_trichter);
	float f_trichter_pos_kugel = - sigma_trichter / x_kugel;
	vec3 correction_kugel = normalize(vec3(npos_kugel_trichter.x, x_kugel / f_trichter_pos_kugel, npos_kugel_trichter.y));
	vec3 p_kugel = trans(p_delle, 0, f_trichter_pos_kugel + dy_kugel, 0);
	p_kugel += radius_kugel * correction_kugel;
	float f_kugel = sphere(p_kugel, radius_kugel);
	vec2 vec_kugel = vec2(f_kugel, mat_kugel);

	return min_material(min_material(min_material(bounding, vec_untergrund), vec_gitter), vec_kugel);
}
