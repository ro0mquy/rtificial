#include "scene_head.glsl"
#include "rtificial.glsl"
#include "noise.glsl"
#include "background.glsl"
#include "biene.glsl"
#include "font.glsl"
#include "greetings.glsl"
#include "fels_color.glsl"
#line 10

const float kristall_id = 1.;

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
		vec3 p = o + t * d;
		vec3 normal = calc_normal(p, false);
		float material = f(p, true)[1];
		if (material == kristall_id) {
			out_color.rgb = augenlicht(p, d, normal);
		} else if (material == background_fels_id || material == background_kristall_id) {
			vec3 col;
			float rough;
			float metallic = 0.;
			fels_color(p, col, rough);
			out_color.rgb = ambientColor(normal, -d, col, rough, metallic);
		}

		if (material == background_kristall_id) {
			vec3 kristAllColor = augenlicht(p, d, normal);
			out_color = kristAll_color(p, out_color, kristAllColor);
		}

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
		f = mix(hexshape(p), octahedronthingie(p), smoothstep(1., 2., morph_mix_rt_float));
	} else if (morph_mix_rt_float <= 3.) {
		f = mix(octahedronthingie(p), trishape(p), smoothstep(2., 3., morph_mix_rt_float));
	} else if (morph_mix_rt_float <= 4.) {
		f = mix(trishape(p), trillant(p), smoothstep(3., 4., morph_mix_rt_float));
	} else if (morph_mix_rt_float <= 5.) {
		f = mix(trillant(p), opal(p), smoothstep(4., 5., morph_mix_rt_float));
	}
	// */

	//f = octahedronthingie(p);

	vec2 m_kristall = vec2(f, kristall_id);
	vec2 m_bg = background(p);
	m_kristall = min_material(m_bg, m_kristall);
	return m_kristall;
}
