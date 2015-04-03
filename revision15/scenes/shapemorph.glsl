#include "scene_head.glsl"
#include "rtificial.glsl"
#include "noise.glsl"
#include "background.glsl"
#include "biene.glsl"
#include "font.glsl"
#include "greetings.glsl"
#line 9

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 100., screenDist);

	vec3 out_color;
	if (isinf(t)) {
		o.y += 10.;
		out_color.rgb = environmentColor(o, d, 100.);
	} else {
		vec3 normal = calc_normal(o + t * d, false);

		//vec3 r = refract();
		vec3 reflection_dir = reflect(d, normal);
		vec3 reflection_color = textureLod(environment, reflection_dir, 0.).rgb;

		vec3 n2 = vec3(1.1, 1.104, 1.106) * morph_reflectivness_rt_float;
		vec3 refraction_dir_r = refract(d, normal, n2.x);
		vec3 refraction_dir_g = refract(d, normal, n2.y);
		vec3 refraction_dir_b = refract(d, normal, n2.z);
		float refraction_red = textureLod(environment, refraction_dir_r, 0.).r;
		float refraction_green = textureLod(environment, refraction_dir_g, 0.).g;
		float refraction_blue = textureLod(environment, refraction_dir_b, 0.).b;
		vec3 refraction_color = vec3(refraction_red, refraction_green, refraction_blue);

		vec3 r0 = (1. - n2) / (1. + n2);
		r0 *= r0;
		float HdotV = -dot(normal, d);
		vec3 r = r0 + (1. - r0) * pow(1. - HdotV, 5.);

		//out_color.rgb = vec3(max(dot(normal, normalize(vec3(1., .5, .5))), 0.) + .1);

		//// TODO: maybe use the normal sss (ao) function
		vec3 p = o + t * d;
		vec3 n = -d;
		float ao_factor = 0.;
		float l = -.3;
		float i = 5.;
		for(; i > 0; i--) {
			vec3 p_i = p + n * i * l;
			float noise = cfbm(p_i * morph_noise_freq_rt_float);
			//ao_factor -= (i * l - noise * f(p_i, false)[0]) / exp2(i);
			ao_factor += noise * abs(f(p_i, false)[0]) / exp2(i);
			//ao_factor += noise / exp2(i);
		}
		//out_color.rgb *= .1;
		refraction_color *= pow(clamp(ao_factor, 0., 1.), 10.);
		out_color = mix(reflection_color, refraction_color, r);
		out_color *= morph_rt_color;

		vec3 col = morph_rt_color;
		float rough = morph_rough_rt_float;
		float metallic = 1.;
		out_color.rgb = ambientColor(normal, -d, col, rough, metallic);
		// ganz viel spucke!
		//out_color.rgb *= .05;
		//out_color.rgb += ao_factor * 2.;
		out_color.rgb = mix((ao_factor * 15. + 1.) * morph_rt_color, out_color.rgb, r);

		//out_color.rgb = abs(normal);
	}


	vec2 position = gl_FragCoord.xy / res * 2. - 1.;
	greetings(position, out_color, t);

	output_color(out_color, t);
}

float trishape(vec3 p) {
	float height_hex = .3;
	vec3 p_hex = p;
	float r_hex = mix(1., .5, clamp(p_hex.y / height_hex, 0., 10.));
	p_hex.y -= height_hex * .5;
	float f_hex = triprism(p_hex.xzy, vec2(r_hex, height_hex * .5));

	vec3 p_hex2 = p_hex;
	p_hex2.xz *= rot2D(TAU / 3. / 2.);
	float f_hex2 = triprism(p_hex2.xzy, vec2(.7 * r_hex, height_hex * .5));

	float f = f_hex;
	f = smin(f, f_hex2, morph_trishape_smooth_rt_float); // 0.7 is a good value
	f *= .5;
	return f;
}

float kantenklumpen(vec3 p) {
	vec3 p_penta = p;
	float f_penta = pentaprism(p_penta.xzy, vec2(1., 1.));

	p_penta = quat_rotate(p_penta, morph_rot_rt_quat);
	float f_penta2 = pentaprism(p_penta.xzy, vec2(1., 1.));

	p_penta = quat_rotate(p_penta, morph_rot_rt_quat);
	float f_penta3 = pentaprism(p_penta.xzy, vec2(1., 1.));

	p_penta = quat_rotate(p_penta, morph_rot_rt_quat);
	float f_penta4 = pentaprism(p_penta.xzy, vec2(1., 1.));

	float f = f_penta;
	f = smax(f, f_penta2, morph_kantenklumpen_smooth_rt_float); // -.3 or 0.5 is a good value
	f = smax(f, f_penta3, morph_kantenklumpen_smooth_rt_float);
	f = smax(f, f_penta4, morph_kantenklumpen_smooth_rt_float);
	return f;
}

float trillant(vec3 p) {
	float height = .4;
	float width = mix(3., 1.5, clamp(abs(p.y / (height * .5)), 0., 1.));

	vec2 p_c1 = p.xz;
	p_c1 -= width * vec2(0., 1./sqrt(3.));
	float f_c1 = length(p_c1) - width;

	vec2 p_c2 = p.xz;
	p_c2 -= width * vec2(-.5, -.5/sqrt(3.));
	float f_c2 = length(p_c2) - width;

	vec2 p_c3 = p.xz;
	p_c3 -= width * vec2(.5, -.5/sqrt(3.));
	float f_c3 = length(p_c3) - width;

	float f = f_c1;
	f = max(f, f_c2);
	f = max(f, f_c3);
	f = max(f, abs(p.y) - height * .5);
	f *= .3;

	vec3 p_tri = p;
	p_tri.xz *= rot2D(TAU / 3. / 3.);
	float f_tri = triprism(p_tri.xzy, vec2(.7/3. * width, height * .5));

	p_tri.xz *= rot2D(TAU / 3. / 3.);
	float f_tri2 = triprism(p_tri.xzy, vec2(.7/3. * width, height * .5));
	f_tri = min(f_tri, f_tri2);

	f = mix(f_tri, f, morph_trillant_smooth_rt_float); // 0.9 or 1.2 are nice shapes
	return f;
}

float kristall(vec3 p) {
	vec3 p_kristall = p;
	float height_kristall = mk_kristall_h_rt_float;
	float radius_kristall = mk_kristall_r_rt_float;
	float size_cap = mk_kristall_cap_rt_float;
	p_kristall.y = abs(p_kristall.y);
	float r_kristall = radius_kristall * min((height_kristall - radius_kristall) - p_kristall.y, size_cap) / size_cap;
	float f_kristall = hexprism(p_kristall.xzy, vec2(r_kristall, height_kristall));
	f_kristall *= .5;
	return f_kristall;
}

float hexshape(vec3 p) {
	float f_hex = hexprism(p.xzy, vec2(1., 1.));

	vec3 p_plane = p;
	p_plane.xz *= rot2D(TAU / 6. / 2.);
	float f_plane = hexprism(p_plane.xzy, vec2(1., .0));

	float f_shape = smin(f_hex, f_plane, morph_hexshape_smooth_rt_float); // something around .8
	return f_shape;
}

float octahedronthingie(vec3 p) {
	float f_oktaeder = oktaeder(p, 2.);
	float f_box = box(p, vec3(.5));
	float f = smin(f_oktaeder, f_box, .5);

	p.xy *= rot2D(TAU * .1);
	float f_kanten1 = box(p, vec3(.5));
	p.xz *= rot2D(TAU * .15);
	float f_kanten2 = box(p, vec3(.5));
	p.zy *= rot2D(TAU * .2);
	float f_kanten3 = box(p, vec3(.5));

	float f_kanten = min(f_kanten1, f_kanten2);
	f_kanten = min(f_kanten, f_kanten3);

	f = mix(f, f_kanten, morph_octahedron_smooth_rt_float); // -.2 or .2
	return f;
}

vec2 f(vec3 p, bool last_step) {
	float d;
	float f;
	/*
	{
		float f_kein = max(length(p.xy) - .5, abs(p.z) - 1.);
		float f_ein = eineck(p, vec2(1.));
		float f_zwei = zweieck(p, vec2(1.));
		float f_tri = triprism(trans(p, 0., -.2, 0.), vec2(.8, 1.));
		float f_cube = box(p, vec3(1.));
		float f_penta = pentaprism(p, vec2(1.));
		float f_hex = hexprism(p, vec2(1.));


		if (morph_mix_rt_float <= 1.) {
			f = mix(f_hex, f_penta, morph_mix_rt_float);
		} else if (morph_mix_rt_float <= 2.) {
			f = mix(f_penta, f_cube, morph_mix_rt_float - 1.);
		} else if (morph_mix_rt_float <= 3.) {
			f = mix(f_cube, f_tri, morph_mix_rt_float - 2.);
		} else if (morph_mix_rt_float <= 4.) {
			f = mix(f_tri, f_zwei, morph_mix_rt_float - 3.);
		} else if (morph_mix_rt_float <= 5.) {
			f = mix(f_zwei, f_ein, morph_mix_rt_float - 4.);
		} else if (morph_mix_rt_float <= 6.) {
			f = mix(f_ein, f_kein, morph_mix_rt_float - 5.);
		}
	}
	d = f;

	{
		//vec3 p = p.zyx;
		vec3 p = p;
		p.xz *= rot2D(morph_rotation_rt_float);
		float f_kein = max(length(p.xy) - .5, abs(p.z) - 1.);
		float f_ein = eineck(p, vec2(1.));
		float f_zwei = zweieck(p, vec2(1.));
		float f_tri = triprism(trans(p, 0., -.2, 0.), vec2(.8, 1.));
		float f_cube = box(p, vec3(1.));
		float f_penta = pentaprism(p, vec2(1.));
		float f_hex = hexprism(p, vec2(1.));


		if (morph_mix_rt_float <= 1.) {
			f = mix(f_hex, f_penta, morph_mix_rt_float);
		} else if (morph_mix_rt_float <= 2.) {
			f = mix(f_penta, f_cube, morph_mix_rt_float - 1.);
		} else if (morph_mix_rt_float <= 3.) {
			f = mix(f_cube, f_tri, morph_mix_rt_float - 2.);
		} else if (morph_mix_rt_float <= 4.) {
			f = mix(f_tri, f_zwei, morph_mix_rt_float - 3.);
		} else if (morph_mix_rt_float <= 5.) {
			f = mix(f_zwei, f_ein, morph_mix_rt_float - 4.);
		} else if (morph_mix_rt_float <= 6.) {
			f = mix(f_ein, f_kein, morph_mix_rt_float - 5.);
		}
	}
	d = smin(d, f, morph_smooth_rt_float);

	{
		//vec3 p = p.xzy;
		vec3 p = p;
		p.yz *= rot2D(morph_rotation_rt_float);
		float f_kein = max(length(p.xy) - .5, abs(p.z) - 1.);
		float f_ein = eineck(p, vec2(1.));
		float f_zwei = zweieck(p, vec2(1.));
		float f_tri = triprism(trans(p, 0., -.2, 0.), vec2(.8, 1.));
		float f_cube = box(p, vec3(1.));
		float f_penta = pentaprism(p, vec2(1.));
		float f_hex = hexprism(p, vec2(1.));


		if (morph_mix_rt_float <= 1.) {
			f = mix(f_hex, f_penta, morph_mix_rt_float);
		} else if (morph_mix_rt_float <= 2.) {
			f = mix(f_penta, f_cube, morph_mix_rt_float - 1.);
		} else if (morph_mix_rt_float <= 3.) {
			f = mix(f_cube, f_tri, morph_mix_rt_float - 2.);
		} else if (morph_mix_rt_float <= 4.) {
			f = mix(f_tri, f_zwei, morph_mix_rt_float - 3.);
		} else if (morph_mix_rt_float <= 5.) {
			f = mix(f_zwei, f_ein, morph_mix_rt_float - 4.);
		} else if (morph_mix_rt_float <= 6.) {
			f = mix(f_ein, f_kein, morph_mix_rt_float - 5.);
		}
	}
	d = smin(d, f, morph_smooth_rt_float); // 0.3 or -0.4
	f = d;
	// */

	/*
	float height_hex = .3;
	vec3 p_hex = p;
	float r_hex = mix(1., .5, clamp(p_hex.y / height_hex, 0., 10.));
	p_hex.y -= height_hex * .5;
	float f_hex = hexprism(p_hex.xzy, vec2(r_hex, height_hex * .5));

	vec3 p_hex2 = p_hex;
	p_hex2.xz *= rot2D(radians(30.));
	float f_hex2 = hexprism(p_hex2.xzy, vec2(r_hex, height_hex * .5));

	f = f_hex;
	f = smax(f, f_hex2, morph_smooth_rt_float); // .3
	f *= .5;
	// */

	//*
	if (morph_mix_rt_float <= 1.) {
		f = mix(kristall(p), hexshape(p), smoothstep(0., 1., morph_mix_rt_float));
	} else if (morph_mix_rt_float <= 2.) {
		f = mix(hexshape(p), kantenklumpen(p), smoothstep(1., 2., morph_mix_rt_float));
	} else if (morph_mix_rt_float <= 3.) {
		f = mix(kantenklumpen(p), octahedronthingie(p), smoothstep(2., 3., morph_mix_rt_float));
	} else if (morph_mix_rt_float <= 4.) {
		f = mix(octahedronthingie(p), trishape(p), smoothstep(3., 4., morph_mix_rt_float));
	} else if (morph_mix_rt_float <= 5.) {
		f = mix(trishape(p), trillant(p), smoothstep(4., 5., morph_mix_rt_float));
	} else if (morph_mix_rt_float <= 6.) {
		f = mix(trillant(p), opal(p), smoothstep(5., 6., morph_mix_rt_float));
	}
	// */

	//f = opal(p);

	vec2 m_kristall = vec2(f, 0.);
	vec2 m_bg = vec2(background(p), 0.);
	m_kristall = min_material(m_bg, m_kristall);
	return m_kristall;
}
