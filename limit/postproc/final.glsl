#include "post.glsl"
#include "helper.glsl"
#include "noise.glsl"
#line 5
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

uniform bool post_scanlines;
uniform float post_scanlines_offset;
uniform float post_scanlines_width;
uniform float post_scanlines_darken;

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

void main() {
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

	if((floor(mod(gl_FragCoord.y, post_scanlines_offset)) < post_scanlines_width) && post_scanlines){
		out_color *= post_scanlines_darken;
	}

	vec2 tc_vignette = tc;
	if (post_vignette_distortion != 0.) {
		tc_vignette += smoothNoise(vec2(time * 10, gl_FragCoord.y)).x * post_vignette_distortion;
	}
	out_color *= vignette(post_vignette_intensity, tc_vignette);

	out_color = tonemap(out_color * 2)/tonemap(vec3(W));

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

	// color grading
	out_color = pow(max(vec3(0.), post_colorgrading_gain * 2. * (out_color + (2. * post_colorgrading_lift - 1.) * (1. - out_color))), 1./max(post_colorgrading_gamma * 2., 1e-6));
}
