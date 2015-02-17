#include "rtificial.glsl"
#line 3

out vec4 out_color;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 100., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, .5))), 0.) + .1);
	}
}

vec2 f(vec3 p, bool last_step) {
	float d;
	float f;
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
	d = smin(d, f, morph_smooth_rt_float);

	f = d;
	return vec2(f, 0.);
}
