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

float f2Logo(vec2 p) {
	// standing V
	vec2 p_V = p;
	pMirror(p_V.x);
	float f_V = f2PlaneAngle(p_V, extlogo_V_angle_rt_float * Tau);
	pMirrorTrans(f_V, extlogo_thickness_rt_float);
	f_V = max(f_V, p_V.y - extlogo_V_height_rt_float);

	// lying V
	vec2 p_lt = p;
	pTrans(p_lt, extlogo_lt_origin_rt_vec2);
	pRot(p_lt, extlogo_lt_angle_global_rt_float * Tau);
	float i_lt_updown = pMirror(p_lt.x);

	float lt_thickness = extlogo_thickness_rt_float;
	if (i_lt_updown < 0.) {
		pRot(p_lt, saturate((p_lt.x - extlogo_lt_down_bend_offset_rt_float) / extlogo_lt_down_bend_scale_rt_float) * extlogo_lt_down_bend_angle_rt_float * Tau);
		lt_thickness *= extlogo_lt_down_thickness_factor_rt_float;
	}

	float f_lt = f2PlaneAngle(p_lt, extlogo_lt_angle_rt_float * Tau);
	pMirrorTrans(f_lt, lt_thickness);

	if (i_lt_updown > 0.) {
		float f_lt_cutout = p.x;
		f_lt_cutout = min(-f_lt_cutout - extlogo_lt_cutout_left_rt_float, f_lt_cutout + extlogo_lt_cutout_right_rt_float);
		f_lt = max(f_lt, f_lt_cutout);

		f_lt = max(f_lt, f2PlaneAngle(p - extlogo_lt_up_cut_pos_rt_vec2, extlogo_lt_up_cut_angle_rt_float * Tau));
	} else {
		f_lt = max(f_lt, f2PlaneAngle(p - extlogo_lt_down_cut_pos_rt_vec2, extlogo_lt_down_cut_angle_rt_float * Tau));
	}

	return min(f_V, f_lt);
}

void main() {
	/*
	out_color = iqCosinePalette(tc.x, pal_base_rt_color, pal_amplitude_rt_color, pal_frequency_rt_vec3, pal_phase_rt_vec3);
	out_color = mix(bg_color1_rt_color, bg_color2_rt_color, lin2srgb(tc.x));
	out_color = lin2srgb(out_color);
	return;
	// */

		vec2 p2;
		float f_penta_1, f_penta_2;
	if (post_credits_visible_rt_bool) {
		p2 = gl_FragCoord.xy / res.x;
		vec2 dim = res / res.x;
		pTrans(p2, dim/2);

		vec2 p2_penta = p2;
		pTrans(p2_penta, post_credits_penta_pos_rt_vec2);
		p2_penta.y = -p2_penta.y;

		f_penta_1 = f2Pentaprism(p2_penta, post_credits_penta_r_rt_float);
		f_penta_2 = f2Pentaprism(p2_penta, post_credits_penta_r_rt_float - post_credits_penta_dr_rt_float);
	}

	vec2 tc_lens = tc;
	if (post_image_distortion != 0. && (!post_credits_visible_rt_bool || f_penta_1 < 0)) {
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

	// credits
	if (post_credits_visible_rt_bool) {
		if (f_penta_1 < 0) {
			out_color = mix(out_color, post_credits_penta_color_rt_color, post_credits_penta_alpha_rt_float * post_credits_alpha_rt_float);
		}
		if (f_penta_2 < 0) {
			out_color = mix(out_color, post_credits_penta_color_rt_color, post_credits_penta_alpha_rt_float * post_credits_alpha_rt_float);
		}

		vec2 p2_logo = p2;
		pTrans(p2_logo, post_credits_logo_pos_rt_vec2);
		p2_logo.x = -p2_logo.x;
		if (f2Logo(p2_logo*post_credits_logo_scale_rt_float*10) < 0) {
			out_color = mix(out_color, post_credits_logo_color_rt_color, post_credits_alpha_rt_float);
		}
	}

}
