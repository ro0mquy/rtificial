#ifndef final_H
#define final_H
const char final_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(// lens distort, vignette, noise
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 19) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 66) uniform float time;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 67) uniform float post_lens_distort_k;
)shader_source"
R"shader_source(layout(location = 68) uniform float post_lens_distort_kcube;
)shader_source"
R"shader_source(layout(location = 69) uniform float post_vignette_intensity;
)shader_source"
R"shader_source(layout(location = 70) uniform float post_film_grain_intensity;
)shader_source"
R"shader_source(layout(location = 71) uniform float post_film_grain_frequency;
)shader_source"
R"shader_source(layout(location = 72) uniform float post_film_grain_power;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 lens_distort(float aspect, float k, float kcube, vec2 c) {
)shader_source"
R"shader_source(	c = c * 2. - 1.;
)shader_source"
R"shader_source(	float r2 = (aspect * aspect) * (c.x * c.x) + c.y * c.y;
)shader_source"
R"shader_source(	float r_norm = aspect * aspect + 1.;
)shader_source"
R"shader_source(	float zeuch = k + kcube * sqrt(2.);
)shader_source"
R"shader_source(	float f = (1. + r2 * zeuch) / (1. + r_norm * zeuch);
)shader_source"
R"shader_source(	return (f * c) * .5 + .5;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// add some kind of vignette
)shader_source"
R"shader_source(// 0 <= intensity <= 1
)shader_source"
R"shader_source(// intensity = .7 is good default
)shader_source"
R"shader_source(float vignette(float intensity, vec2 c) {
)shader_source"
R"shader_source(	float one_minus_intesity = 1. - intensity;
)shader_source"
R"shader_source(	c.y *= -1.;
)shader_source"
R"shader_source(	return one_minus_intesity + intensity * 16. * c.x *c.y * (1. - c.x) * (-1. - c.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rand(vec2 co){
)shader_source"
R"shader_source(	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 g(float v) {
)shader_source"
R"shader_source(	v *= 2.0 * 3.1415926;
)shader_source"
R"shader_source(	return vec2(cos(v), sin(v));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 fade(vec2 t) {
)shader_source"
R"shader_source(	return t*t*t*(t*(t*6.0-15.0)+10.0);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float classic_noise(vec2 co) {
)shader_source"
R"shader_source(	vec2 c = fract(co);
)shader_source"
R"shader_source(	vec2 C = floor(co);
)shader_source"
R"shader_source(	vec2 e = vec2(0.0, 1.0);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec4 n = vec4(
)shader_source"
R"shader_source(		dot(g(rand(C + e.xx)), c - e.xx),
)shader_source"
R"shader_source(		dot(g(rand(C + e.xy)), c - e.xy),
)shader_source"
R"shader_source(		dot(g(rand(C + e.yx)), c - e.yx),
)shader_source"
R"shader_source(		dot(g(rand(C + e.yy)), c - e.yy)
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 u = fade(c);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return mix(
)shader_source"
R"shader_source(		mix(n[0], n[2], u.x),
)shader_source"
R"shader_source(		mix(n[1], n[3], u.x),
)shader_source"
R"shader_source(		u.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fbm(vec2 c) {
)shader_source"
R"shader_source(	return (classic_noise(c) + classic_noise(c * 2.) * .5 + classic_noise(c * 4.) * .25)/1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	vec3 col;
)shader_source"
R"shader_source(	float k = post_lens_distort_k;
)shader_source"
R"shader_source(	float kcube = post_lens_distort_kcube;
)shader_source"
R"shader_source(	float aspect = res.x / res.y;
)shader_source"
R"shader_source(	vec3 primaries = vec3(610., 550., 440.)/440.;
)shader_source"
R"shader_source(	for (int i = 0; i < 3; i++) {
)shader_source"
R"shader_source(		col[i] = textureLod(color, lens_distort(aspect, k * primaries[i], kcube, tc), 0.)[i];
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	col *= vignette(post_vignette_intensity, tc);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// TODO richtiger grain
)shader_source"
R"shader_source(	float phi1 = radians(30.);
)shader_source"
R"shader_source(	float phi2 = radians(60.);
)shader_source"
R"shader_source(	vec3 grain = vec3(// so schön weerboß
)shader_source"
R"shader_source(		fbm(vec2(post_film_grain_frequency * (gl_FragCoord.xy + 3289. + time * 2938.) )),
)shader_source"
R"shader_source(		fbm(vec2(post_film_grain_frequency * (mat2(cos(phi1), -sin(phi1), sin(phi1), cos(phi1)) * gl_FragCoord.xy + time * 2738.) )),
)shader_source"
R"shader_source(		fbm(vec2(post_film_grain_frequency * (mat2(cos(phi2), -sin(phi2), sin(phi2), cos(phi2)) * gl_FragCoord.xy + 93829. + time * 2847.) ))
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(	float luma = clamp(dot(col, vec3(.299, .587, .114)), 0., 1.);
)shader_source"
R"shader_source(	float intensity = post_film_grain_intensity * pow(1. - luma, post_film_grain_power);
)shader_source"
R"shader_source(	out_color = col + intensity * grain;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
