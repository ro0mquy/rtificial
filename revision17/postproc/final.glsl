#include "post.glsl"
#include "helper.glsl"
#include "noise.glsl"
#include "sdf/domain.glsl"
#include "sdf/distances.glsl"
#line 7
// lens distort, vignette, tonemapping, color grading, noise

uniform sampler2D color; // vec3
out vec3 out_color;

uniform float post_image_distortion;
uniform float post_lens_distort_k;
uniform float post_lens_distort_kcube;
uniform float post_vignette_intensity;
uniform float post_vignette_distortion;
uniform float post_film_grain_intensity;
uniform float post_film_grain_frequency;
uniform float post_film_grain_power;
uniform bool post_disable_grain;

uniform vec3 post_colorgrading_slope; // color
uniform vec3 post_colorgrading_power; // color
uniform vec3 post_colorgrading_offset; // color

uniform vec3 post_colorgrading_lift; // color
uniform vec3 post_colorgrading_gamma; // color
uniform vec3 post_colorgrading_gain; // color

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 11.2;

vec3 tonemap(vec3 color) {
   return ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
}

vec2 lens_distort(float aspect, float k, float kcube, vec2 c) {
	c = c * 2. - 1.;
	float r2 = (aspect * aspect) * (c.x * c.x) + c.y * c.y;
	float r_norm = aspect * aspect + 1.;
	float zeuch = k + kcube * sqrt(2.);
	float f = (1. + r2 * zeuch) / (1. + r_norm * zeuch);
	return (f * c) * .5 + .5;
}

// add some kind of vignette
// 0 <= intensity <= 1
// intensity = .7 is good default
float vignette(float intensity, vec2 c) {
	float one_minus_intesity = 1. - intensity;
	c.y *= -1.;
	return one_minus_intesity + intensity * 16. * c.x *c.y * (1. - c.x) * (-1. - c.y);
}

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float classic_noise(vec2 co) {
	vec2 c = fract(co);
	vec2 C = floor(co);
	vec2 e = vec2(0.0, 1.0);

	vec4 n = vec4(
		dot(unitVector(Tau * rand(C + e.xx)), c - e.xx),
		dot(unitVector(Tau * rand(C + e.xy)), c - e.xy),
		dot(unitVector(Tau * rand(C + e.yx)), c - e.yx),
		dot(unitVector(Tau * rand(C + e.yy)), c - e.yy)
	);

	vec2 u = smootherstep(c, vec2(0.), vec2(1.));

	return mix(
		mix(n[0], n[2], u.x),
		mix(n[1], n[3], u.x),
		u.y);
}

float fbm(vec2 c) {
	return (classic_noise(c) + classic_noise(c * 2.) * .5 + classic_noise(c * 4.) * .25)/1.75;
}

// https://en.wikipedia.org/wiki/Blend_modes#Overlay
vec3 blend_overlay(vec3 base, vec3 top) {
	vec3 dark = 2*base*top;
	vec3 light = 1 - 2 * (1-base) * (1-top);
	return mix(dark, light, step(0.5, base));
}

float fGeo() {
	vec2 p = gl_FragCoord.xy / res.x;
	vec2 dim = res / res.x;
	pTrans(p, dim/2);

	float rot_angle = (-mand_rot_global_rt_float + .5 * (ext_extrude_h_rt_float + ext_rot_rt_float) + geo_rotation_offset_rt_float) * Tau;
	pRot(p, rot_angle);

	float radius = geo_radius_rt_float * mand_domrep_angle_r_rt_float * .01;
	float f;

	int mode = int(geo_mode_rt_float) % 7;

	if (mode != 5) {
		if (mode == 0) {
			f = f2Triprism(p, radius);
		} else if (mode == 1) {
			f = f2Pentaprism(p, radius);
		} else if (mode == 2) {
			f = f2Quadprism(p, radius);
		} else if (mode == 3) {
			f = f2Hexprism(p, radius);
		} else if (mode == 4) {
			vec2 p_domrep = p;
			pDomrepAngle(p_domrep, 8, radius);
			pMirror(p_domrep.y);
			f = f2PlaneAngle(p_domrep, 3./16. * Tau);
		} else if (mode == 6) {
			vec2 p_tri = p;
			float f1 = f2Triprism(p_tri, radius);
			p_tri.y = -p_tri.y;
			float f2 = f2Triprism(p_tri, radius * geo_tri_scale_rt_float);
			f = max(f1, f2);
		}

		pMirrorTrans(f, geo_thickness_rt_float * .01);

	} else {
		float box_offset = geo_box_offset_rt_float * mand_domrep_angle_r_rt_float * .01;
		float f1 = f2Quadprism(p - vec2(box_offset, 0.), radius - box_offset);
		float f2 = f2Quadprism(p + vec2(box_offset, 0.), radius - box_offset);
		float f_line = f2LineX(p, 0., radius - 2. * box_offset);

		pMirrorTrans(f1, geo_thickness_rt_float * .01);
		pMirrorTrans(f2, geo_thickness_rt_float * .01);
		pMirrorTrans(f_line, geo_thickness_rt_float * .01);

		f = min(f1, f2);
		//f = min(f, f_line);
	}

	f = step(0., -f);

	return f;
}

void main() {
	/*
	out_color = iqCosinePalette(tc.x, pal_base_rt_color, pal_amplitude_rt_color, pal_frequency_rt_vec3, pal_phase_rt_vec3);
	out_color = mix(bg_color1_rt_color, bg_color2_rt_color, lin2srgb(tc.x));
	out_color = lin2srgb(out_color);
	return;
	// */

	vec2 tc_lens = tc;
	if (post_image_distortion != 0.) {
		tc_lens += smoothNoise(vec2(time * 10, gl_FragCoord.y)).x * post_image_distortion;
	}
	float k = post_lens_distort_k;
	float kcube = post_lens_distort_kcube;
	float aspect = res.x / res.y;
	vec3 primaries = vec3(610., 550., 440.)/440.;
	for (int i = 0; i < 3; i++) {
		out_color[i] = textureLod(color, lens_distort(aspect, k * primaries[i], kcube, tc_lens), 0.)[i];
	}


	vec2 tc_vignette = tc;
	if (post_vignette_distortion != 0.) {
		tc_vignette += smoothNoise(vec2(time * 10, gl_FragCoord.y)).x * post_vignette_distortion;
	}
	out_color *= vignette(post_vignette_intensity, tc_vignette);

	// slope, power, offset grading
	vec3 slope = 2. * post_colorgrading_slope;
	vec3 power = 2. * post_colorgrading_power;
	vec3 offset = (2. * post_colorgrading_offset - 1.);
	out_color = pow(max(vec3(0.), slope * out_color + offset), power);

	out_color = tonemap(out_color * 2)/tonemap(vec3(W));

	out_color = lin2srgb(out_color);

	out_color = linstep(post_remap_rgb_bottom_rt_vec3, post_remap_rgb_top_rt_vec3, out_color);
	out_color = linstep(post_remap_value_bottom_rt_float, post_remap_value_top_rt_float, out_color);
	out_color = post_remap_value_skew_rt_float * square(out_color) + (1. - post_remap_value_skew_rt_float) * out_color;

	if (!post_disable_grain) {
		// TODO richtiger grain
		vec3 grain = vec3(// so schön weerboß
				fbm(vec2(post_film_grain_frequency * (rotMat2D(Tau*.05) * gl_FragCoord.xy + 3289. + time * 2938.) )),
				fbm(vec2(post_film_grain_frequency * (rotMat2D(Tau*.11) * gl_FragCoord.xy + time * 2738.) )),
				fbm(vec2(post_film_grain_frequency * (rotMat2D(Tau*.23) * gl_FragCoord.xy + 93829. + time * 2847.) ))
				);
		float luma = clamp(rgb2luma(out_color), 0., 1.);
		float intensity = post_film_grain_intensity * pow(1. - luma, post_film_grain_power);
		out_color += intensity * grain;
	}

	// lift, gamma, gain grading
	vec3 lift = 2. * post_colorgrading_lift - 1.;
	vec3 gamma = 2. * post_colorgrading_gamma;
	vec3 gain = 2. * post_colorgrading_gain;
	out_color = pow(max(vec3(0.), gain * (out_color + lift * (1. - out_color))), 1./max(gamma, 1e-6));

	// border
	if ( (abs(0.5 - tc.y) > 0.5 - post_border_y_rt_float/res.y )
		|| (abs(0.5 - tc.x) > 0.5 - post_border_x_rt_float/res.x) ){
		if (post_border_blend_overlay_rt_bool) {
			out_color = blend_overlay(out_color, post_border_color_rt_color);
		} else {
			out_color = post_border_color_rt_color;
		}
	}

	if (geo_enabled_rt_bool) {
		vec3 overlay_color = blend_overlay(out_color, geo_color_rt_color);
		out_color = mix(out_color, overlay_color, fGeo());
	}
}
