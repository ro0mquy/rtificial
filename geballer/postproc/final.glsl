#version 430
// lens distort, vignette, noise
#include "noise.glsl"

in vec2 tc;
layout(location = 0) uniform vec2 res;

uniform sampler2D color; // vec3
out vec3 out_color;

uniform float time;

uniform float post_lens_distort_k;
uniform float post_lens_distort_kcube;
uniform float post_vignette_intensity;
uniform float post_film_grain_intensity;
uniform float post_film_grain_frequency;
uniform float post_film_grain_power;

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

vec2 g(float v) {
	v *= 2.0 * 3.1415926;
	return vec2(cos(v), sin(v));
}

vec2 fade(vec2 t) {
	return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float classic_noise(vec2 co) {
	vec2 c = fract(co);
	vec2 C = floor(co);
	vec2 e = vec2(0.0, 1.0);

	vec4 n = vec4(
		dot(g(rand(C + e.xx)), c - e.xx),
		dot(g(rand(C + e.xy)), c - e.xy),
		dot(g(rand(C + e.yx)), c - e.yx),
		dot(g(rand(C + e.yy)), c - e.yy)
	);

	vec2 u = fade(c);

	return mix(
		mix(n[0], n[2], u.x),
		mix(n[1], n[3], u.x),
		u.y);
}

float fbm(vec2 c) {
	return (classic_noise(c) + classic_noise(c * 2.) * .5 + classic_noise(c * 4.) * .25)/1.75;
}

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

void main() {
	vec3 col;
	float k = post_lens_distort_k;
	float kcube = post_lens_distort_kcube;
	float aspect = res.x / res.y;
	vec3 primaries = vec3(610., 550., 440.)/440.;
	for (int i = 0; i < 3; i++) {
		col[i] = textureLod(color, lens_distort(aspect, k * primaries[i], kcube, tc), 0.)[i];
	}


	vec2 tc2 = tc + vec2(1, 0) * smoothNoise(vec2(time * 10, gl_FragCoord.y)) * post_distortion_scheiss_rt_float;
	col *= vignette(post_vignette_intensity, tc2);

	// TODO richtiger grain
	float phi1 = radians(30.);
	float phi2 = radians(60.);
	vec3 grain = vec3(// so schön weerboß
		fbm(vec2(post_film_grain_frequency * (gl_FragCoord.xy + 3289. + time * 2938.) )),
		fbm(vec2(post_film_grain_frequency * (mat2(cos(phi1), -sin(phi1), sin(phi1), cos(phi1)) * gl_FragCoord.xy + time * 2738.) )),
		fbm(vec2(post_film_grain_frequency * (mat2(cos(phi2), -sin(phi2), sin(phi2), cos(phi2)) * gl_FragCoord.xy + 93829. + time * 2847.) ))
	);
	float luma = clamp(dot(col, vec3(.299, .587, .114)), 0., 1.);
	float intensity = post_film_grain_intensity * pow(1. - luma, post_film_grain_power);
	out_color = col + intensity * grain;

	out_color = tonemap(out_color * 2)/tonemap(vec3(W));
}
