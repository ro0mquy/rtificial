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

uniform sampler2D tex_thanks;
uniform sampler2D tex_greetings;
uniform sampler2D tex_alcatraz;
uniform sampler2D tex_asd;
uniform sampler2D tex_conspiracy;
uniform sampler2D tex_fairlight;
uniform sampler2D tex_horology;
uniform sampler2D tex_iq;
uniform sampler2D tex_mercury;
uniform sampler2D tex_urs;
uniform sampler2D tex_nerd2nerd;
uniform sampler2D tex_nerdarzt;
uniform sampler2D tex_stroboholics;

vec2 origdim_thanks = vec2(36, 7);
vec2 origdim_greetings = vec2(66, 7);
vec2 origdim_alcatraz = vec2(45, 7);
vec2 origdim_asd = vec2(18, 7);
vec2 origdim_conspiracy = vec2(59, 7);
vec2 origdim_fairlight = vec2(45, 7);
vec2 origdim_horology = vec2(48, 7);
vec2 origdim_iq = vec2(9, 8);
vec2 origdim_mercury = vec2(43, 7);
vec2 origdim_urs = vec2(18, 7);
vec2 origdim_nerd2nerd = vec2(54, 7);
vec2 origdim_nerdarzt = vec2(46, 7);
vec2 origdim_stroboholics = vec2(67, 7);

vec3 color_thanks_greetings = color_text;
vec3 color_mercury_urs = vec3(1, .35, 0);
vec3 color_iq = color_text;
vec3 color_stroboholics = color_text;

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
	vec3 light_position = light_position;
	if (time < 48.759) {
		// bouncing
		light_position = vec3(-85, 5, -40);
	} else if (time < 54.0) {
		// rolling
		float time = time - 48.759;
		light_position = vec3(-85, -5, -40);
		light_position.y += .1 * time;
	} else if (time < 60) {
		// thanks & greetings
		light_position = vec3(-50, -10, -10);
	} else if (time < 100) {
		float time = time - 81;
		light_position = vec3(0, 14, 5);
		light_position.y -= 1.5 * time;
	}

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
			float f_text = 0;
			vec3 color_text = vec3(0);

			if (time > 54 && time < 60) {
				// thanks and greetings
				float time = time - 54;
				vec2 dim_thanks = vec2(origdim_thanks.x / origdim_thanks.y, 1.) * base_dim;
				vec2 p_thanks = hit.xz - vec2(-51, -55 + time * 20);
				vec2 p_tex_thanks = vec2(-1, 1) * p_thanks.yx / dim_thanks.xy + .5;
				float f_thanks = texture(tex_thanks, p_tex_thanks).r;

				vec2 dim_greetings = vec2(origdim_greetings.x / origdim_greetings.y, 1.) * base_dim;
				vec2 p_greetings = hit.xz - vec2(-40, 10 - time * 4);
				vec2 p_tex_greetings = vec2(-1, 1) * p_greetings.yx / dim_greetings.xy + .5;
				float f_greetings = texture(tex_greetings, p_tex_greetings).r;

				f_text = f_greetings + f_thanks;
				color_text = color_thanks_greetings;
			} else if (time > 57 && time < 64) {
				// mercury -- urs
				float time = time - 57;
				vec2 dim_mercury = vec2(origdim_mercury.x / origdim_mercury.y, 1.) * base_dim;
				vec2 p_mercury = hit.xz - vec2(-40, -15 - time * 3);
				vec2 p_tex_mercury = vec2(-1, 1) * p_mercury.yx / dim_mercury.xy + .5;
				float f_mercury = texture(tex_mercury, p_tex_mercury).r;

				vec2 dim_urs = vec2(origdim_urs.x / origdim_urs.y, 1.) * base_dim;
				vec2 p_urs = hit.xz - vec2(-42 - 3.2 * time, -8);
				vec2 p_tex_urs = vec2(1, 1) * p_urs.xy / dim_urs.xy + .5;
				float f_urs = texture(tex_urs, p_tex_urs).r;

				f_text = f_mercury + f_urs;
				color_text = color_mercury_urs;
			} else if (time > 64 && time < 69) {
				// iq
				float time = time - 64;
				vec2 dim_iq = vec2(origdim_iq.x / origdim_iq.y, 1.) * base_dim;
				vec2 p_iq = hit.xz - vec2(-55, -10);
				vec2 p_tex_iq = vec2(-1, 1) * p_iq.yx / dim_iq.xy + .5;
				float f_iq = texture(tex_iq, p_tex_iq).r;

				f_text = f_iq;
				color_text = color_iq;
			} else if (time > 81.0 && time < 90) {
				// stroboholics
				float time = time - 81;
				vec2 dim_stroboholics = vec2(origdim_stroboholics.x / origdim_stroboholics.y, 1.) * base_dim;
				vec2 p_stroboholics = hit.xz - vec2(-45, 25);
				vec2 p_tex_stroboholics = vec2(1, -1) * p_stroboholics.yx / dim_stroboholics.xy + .5;
				float f_stroboholics = texture(tex_stroboholics, p_tex_stroboholics).r;

				f_text = f_stroboholics;
				color_text = color_stroboholics;
			}

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
	if (time < 46) {
		// dropdown
		float time = time - 44;
		pos_delle += vec2(-90, -45);
		float anim_duration = 2;
		float start_height = 40;
		float t = time / anim_duration;
		dy_kugel += -start_height * t*t + start_height;
	} else if (time < 48) {
		// bouncing
		float time = time - 46;
		pos_delle += vec2(-90, -45);
		float v_0 = 2 * 20; // 2 * start_height
		float omega_d = 5.5;
		float daempfung = 2.3;
		dy_kugel += v_0 / omega_d * exp(-daempfung * time) * cos(omega_d * time + TAU / 4);
	} else if (time < 50) {
		// rolling
		float time = time - 48;
		pos_delle += vec2(-90, -45);
		pos_delle += vec2(6 * time, 20 * sin(TAU / 4 * smoothstep(0, 2, time)));
	} else if (time < 54) {
		// rolling2
		float time = time - 50;
		pos_delle += vec2(-78, -25);
		pos_delle += vec2(-6 * time, 20 * sin(TAU / 4 * smoothstep(0, 2, time)));
	} else if (time < 60) {
		// thanks & greetings
		float time = time - 54;
		pos_delle += vec2(-90, 15);
		pos_delle += vec2(3 * time * time, -11 * time);
	} else if (time < 64) {
		// mercury & urs
		float time = time - 57;
		pos_delle += vec2(-90, -45);
		pos_delle += time * vec2(9, 7.24);
	} else if (time < 66) {
		// iq, hin
		float time = time - 64;
		pos_delle += vec2(-55.5, -11.5);
		pos_delle += smoothstep(2, 0, time) * vec2(40, 0);
	} else if (time < 68) {
		// iq, weg
		float time = time - 66;
		pos_delle += vec2(-55.5, -11.5);
		pos_delle += smoothstep(0, 2, time) * vec2(0, -40);
	} else if (time < 100) {
		// trichter
		float time = time - 82.2;
		float a = 70; // use to tweak shape of spiral
		float t = time + 1.2;
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
