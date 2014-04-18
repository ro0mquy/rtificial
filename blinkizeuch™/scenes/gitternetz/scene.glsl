vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;
//uniform vec3 light_position;
//uniform vec3 color_gitter;
//uniform vec3 color_kugel;
//uniform vec3 color_text;
//uniform float radius_kugel;
//uniform float udy_kugel;
//uniform float sigma_delle;
//uniform float sigma_trichter;

vec3 light_position = vec3(-1.7236928939819336, 4.1704692840576172, 8.9048290252685547);
vec3 color_gitter = vec3(1);
vec3 color_kugel = vec3(0.9960784912109375, 0.08235294371843338, 0.0);
vec3 color_text = vec3(0.51764708757400513, 1.0, 0.0039215688593685627);
float radius_kugel = 1;
float udy_kugel = -1.5;
float sigma_delle = 1.8;
float sigma_trichter = 30;

uniform sampler2D tex_thanks;
uniform sampler2D tex_greetings;
uniform sampler2D tex_alcatraz;
uniform sampler2D tex_conspiracy;
uniform sampler2D tex_iq;
uniform sampler2D tex_mercury;
uniform sampler2D tex_urs;
uniform sampler2D tex_nerd2nerd;
uniform sampler2D tex_nerdarzt;
uniform sampler2D tex_stroboholics;

vec2 origdim_thanks = vec2(36, 7);
vec2 origdim_greetings = vec2(66, 7);
vec2 origdim_alcatraz = vec2(45, 7);
vec2 origdim_conspiracy = vec2(59, 7);
vec2 origdim_iq = vec2(9, 8);
vec2 origdim_mercury = vec2(43, 7);
vec2 origdim_urs = vec2(18, 7);
vec2 origdim_nerd2nerd = vec2(54, 7);
vec2 origdim_nerdarzt = vec2(46, 7);
vec2 origdim_stroboholics = vec2(67, 7);

vec3 color_greetings = vec3(0.51764708757400513, 1.0, 0.0039215688593685627);
vec3 color_alcatraz_conspiracy = vec3(1, 238, 255) / vec3(255.);
vec3 color_iq = vec3(0.51764708757400513, 1.0, 0.0039215688593685627);
vec3 color_mercury_urs = vec3(1, .35, 0);
vec3 color_nerd2nerd_nerdarzt = vec3(1, .35, 0);
vec3 color_stroboholics = vec3(0.51764708757400513, 1.0, 0.0039215688593685627);

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
	} else if (time < 69) {
		// greetings -- alcatraz & conspiracy -- iq
		float t = (time - 54) / (69 - 54);
		light_position = mix(vec3(-50, -10, -10), vec3(-20, -5, 8), t);
	} else if (time < 76) {
		// mercury & urs
		float time = time - 69;
		light_position = vec3(-40, -5, -12);
		light_position.xz += 2 * time * vec2(1, 1);
	} else if (time < 100) {
		// trichter
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

			if (time > 54 && time < 63) {
				// greetings
				float time = time - 54;
				vec2 dim_greetings = vec2(origdim_greetings.x / origdim_greetings.y, 1.) * base_dim;
				vec2 p_greetings = hit.xz - vec2(-50, 10 - time * 4);
				vec2 p_tex_greetings = vec2(-1, 1) * p_greetings.yx / dim_greetings.xy + .5;
				float f_greetings = texture(tex_greetings, p_tex_greetings).r;

				// alcatraz -- conspiracy
				vec2 dim_alcatraz = vec2(origdim_alcatraz.x / origdim_alcatraz.y, 1.) * base_dim;
				vec2 p_alcatraz = hit.xz - vec2(-13 + 2 * time, -20);
				vec2 p_tex_alcatraz = vec2(1, 1) * p_alcatraz.xy / dim_alcatraz.xy + .5;
				float f_alcatraz = texture(tex_alcatraz, p_tex_alcatraz).r;

				vec2 dim_conspiracy = vec2(origdim_conspiracy.x / origdim_conspiracy.y, 1.) * base_dim;
				vec2 p_conspiracy = hit.xz - vec2(-7, 10 + 1.5 * time);
				vec2 p_tex_conspiracy = vec2(1, -1) * p_conspiracy.yx / dim_conspiracy.xy + .5;
				float f_conspiracy = texture(tex_conspiracy, p_tex_conspiracy).r;

				float f_alcatraz_conspiracy = f_alcatraz + f_conspiracy;
				f_text = f_greetings + f_alcatraz_conspiracy;
				color_text = color_greetings * f_greetings + color_alcatraz_conspiracy * f_alcatraz_conspiracy;;
			} else if (time > 63 && time < 69) {
				// alcatraz -- conspiracy
				float time = time - 54;
				vec2 dim_alcatraz = vec2(origdim_alcatraz.x / origdim_alcatraz.y, 1.) * base_dim;
				vec2 p_alcatraz = hit.xz - vec2(-13 + 2 * time, -20);
				vec2 p_tex_alcatraz = vec2(1, 1) * p_alcatraz.xy / dim_alcatraz.xy + .5;
				float f_alcatraz = texture(tex_alcatraz, p_tex_alcatraz).r;

				vec2 dim_conspiracy = vec2(origdim_conspiracy.x / origdim_conspiracy.y, 1.) * base_dim;
				vec2 p_conspiracy = hit.xz - vec2(-7, 10 + 1.5 * time);
				vec2 p_tex_conspiracy = vec2(1, -1) * p_conspiracy.yx / dim_conspiracy.xy + .5;
				float f_conspiracy = texture(tex_conspiracy, p_tex_conspiracy).r;

				// iq
				time -= 10;
				vec2 dim_iq = vec2(origdim_iq.x / origdim_iq.y, 1.) * base_dim;
				vec2 p_iq = hit.xz - vec2(-55, -10);
				vec2 p_tex_iq = vec2(-1, 1) * p_iq.yx / dim_iq.xy + .5;
				float f_iq = texture(tex_iq, p_tex_iq).r;

				float f_alcatraz_conspiracy = f_alcatraz + f_conspiracy;
				f_text = f_iq + f_alcatraz_conspiracy;
				color_text = color_iq * f_iq + color_alcatraz_conspiracy * f_alcatraz_conspiracy;
			} else if (time > 69 && time < 76) {
				// mercury -- urs
				float time = time - 69;
				vec2 dim_mercury = vec2(origdim_mercury.x / origdim_mercury.y, 1.) * base_dim;
				vec2 p_mercury = hit.xz - vec2(-40, -20 - time * 3);
				vec2 p_tex_mercury = vec2(-1, 1) * p_mercury.yx / dim_mercury.xy + .5;
				float f_mercury = texture(tex_mercury, p_tex_mercury).r;

				vec2 dim_urs = vec2(origdim_urs.x / origdim_urs.y, 1.) * base_dim;
				vec2 p_urs = hit.xz - vec2(-52 - 3.2 * time, -8);
				vec2 p_tex_urs = vec2(1, 1) * p_urs.xy / dim_urs.xy + .5;
				float f_urs = texture(tex_urs, p_tex_urs).r;

				f_text = f_mercury + f_urs;
				color_text = color_mercury_urs;
			} else if (time > 76 && time < 81) {
				// nerd2nerd -- nerdarzt
				float time = time - 79;
				time = min(time, 0);
				vec2 dim_nerd2nerd = vec2(origdim_nerd2nerd.x / origdim_nerd2nerd.y, 1.) * base_dim;
				vec2 p_nerd2nerd = hit.xz - vec2(-42 + 4 * time, 15 - time);
				vec2 p_tex_nerd2nerd = vec2(-1, 1) * p_nerd2nerd.yx / dim_nerd2nerd.xy + .5;
				float f_nerd2nerd = texture(tex_nerd2nerd, p_tex_nerd2nerd).r;

				vec2 dim_nerdarzt = vec2(origdim_nerdarzt.x / origdim_nerdarzt.y, 1.) * base_dim;
				vec2 p_nerdarzt = hit.xz - vec2(-42 - 3 * time, -14.8 + time);
				vec2 p_tex_nerdarzt = vec2(-1, 1) * p_nerdarzt.yx / dim_nerdarzt.xy + .5;
				float f_nerdarzt = texture(tex_nerdarzt, p_tex_nerdarzt).r;

				f_text = f_nerd2nerd + f_nerdarzt;
				color_text = color_nerd2nerd_nerdarzt;
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
			final_bloom += f_text * 2 * (senvelopes[12] + senvelopes[13] + senvelopes[14]);
		}

		// black fog
		final_color *= mix(final_color, vec3(0), smoothstep(75, 95, distance(hit, view_position)));
		if (time > 90) {
			final_color = mix(final_color, vec3(0), smoothstep(-30, -37, hit.y));
		}

		final_bloom += mat_blooms[int(material)];
	}
	final_color = contrast(final_color, 1.04);
	final_color *= vignette(1);
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
		float t = smoothstep(0, 6, time);
		vec2 p1 = vec2(-90, 15);
		vec2 p2 = vec2(-50, -50);
		vec2 p3 = vec2(-15.5, -11.5);
		pos_delle += mix(mix(p1, p2, t), mix(p2, p3, t), t);
	} else if (time < 65) {
		// alcatraz -- conspiracy
		pos_delle += vec2(-15.5, -11.5);
	} else if (time < 67) {
		// iq, hin
		float time = time - 65;
		pos_delle += vec2(-55.5, -11.5);
		pos_delle += smoothstep(2, 0, time) * vec2(40, 0);
	} else if (time < 69) {
		// iq, weg
		float time = time - 67.5;
		pos_delle += vec2(-55.5, -11.5);
		pos_delle += smoothstep(0, 2, time) * vec2(0, -40);
	} else if (time < 76) {
		// mercury & urs
		float time = time - 69;
		time = smoothstep(0., 5.5, time);
		pos_delle += vec2(-90, -45);
		pos_delle += 6 * time * vec2(9, 7.24);
	} else if (time < 81) {
		// nerd2nerdarzt
		float time = time - 75;
		pos_delle += vec2(-42, 0);
		pos_delle.x += 7.5 * sin(TAU * .6 * time);
		pos_delle.y += 15 * cos(TAU * .5 * time);
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
