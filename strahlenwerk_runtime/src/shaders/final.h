#ifndef final_H
#define final_H
const char final_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(layout(location = 235) uniform float post_distortion_scheiss_rt_float;
)shader_source"
R"shader_source(// lens distort, vignette, noise
)shader_source"
R"shader_source(// jenkins hash
)shader_source"
R"shader_source(uint hash(uint a) {
)shader_source"
R"shader_source(	a = (a + 0x7ed55d16) + (a << 12);
)shader_source"
R"shader_source(	a = (a ^ 0xc761c23c) ^ (a >> 19);
)shader_source"
R"shader_source(	a = (a + 0x165667b1) + (a << 5);
)shader_source"
R"shader_source(	a = (a + 0xd3a2646c) ^ (a << 9);
)shader_source"
R"shader_source(	a = (a + 0xfd7046c5) + (a << 3);
)shader_source"
R"shader_source(	a = (a ^ 0xb55a4f09) ^ (a >> 16);
)shader_source"
R"shader_source(	return a;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 simplexGradients[] = vec3[](
)shader_source"
R"shader_source(	vec3(1,1,0), vec3(-1,1,0), vec3(1,-1,0), vec3(-1,-1,0),
)shader_source"
R"shader_source(	vec3(1,0,1), vec3(-1,0,1), vec3(1,0,-1), vec3(-1,0,-1),
)shader_source"
R"shader_source(	vec3(0,1,1), vec3(0,-1,1), vec3(0,1,-1), vec3(0,-1,-1)
)shader_source"
R"shader_source();
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float uintToFloat(uint x) {
)shader_source"
R"shader_source(	return float(x % (1 << 20)) / float(1 << 20);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rand(int x) {
)shader_source"
R"shader_source(	return uintToFloat(hash(x));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rand(ivec2 x) {
)shader_source"
R"shader_source(	return uintToFloat(hash(x.x + hash(x.y)));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rand(ivec3 x) {
)shader_source"
R"shader_source(	return uintToFloat(hash(x.x + hash(x.y + hash(x.z))));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float valueNoise(float p) {
)shader_source"
R"shader_source(	int index = int(floor(p));
)shader_source"
R"shader_source(	float d = fract(p);
)shader_source"
R"shader_source(	float v0 = rand(index);
)shader_source"
R"shader_source(	float v1 = rand(index + 1);
)shader_source"
R"shader_source(	return mix(v0, v1, d);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float valueNoise(vec2 p) {
)shader_source"
R"shader_source(	ivec2 index = ivec2(floor(p));
)shader_source"
R"shader_source(	vec2 d = fract(p);
)shader_source"
R"shader_source(	float result = 0;
)shader_source"
R"shader_source(	float v00 = rand(index);
)shader_source"
R"shader_source(	float v01 = rand(index + ivec2(0, 1));
)shader_source"
R"shader_source(	float v10 = rand(index + ivec2(1, 0));
)shader_source"
R"shader_source(	float v11 = rand(index + ivec2(1, 1));
)shader_source"
R"shader_source(	return mix(mix(v00, v10, d.x), mix(v01, v11, d.x), d.y) * 2. - 1.;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float valueNoise(vec3 p) {
)shader_source"
R"shader_source(	ivec3 index = ivec3(floor(p));
)shader_source"
R"shader_source(	vec3 d = fract(p);
)shader_source"
R"shader_source(	float result = 0;
)shader_source"
R"shader_source(	float v000 = rand(index);
)shader_source"
R"shader_source(	float v010 = rand(index + ivec3(0, 1, 0));
)shader_source"
R"shader_source(	float v100 = rand(index + ivec3(1, 0, 0));
)shader_source"
R"shader_source(	float v110 = rand(index + ivec3(1, 1, 0));
)shader_source"
R"shader_source(	float v001 = rand(index + ivec3(0, 0, 1));
)shader_source"
R"shader_source(	float v011 = rand(index + ivec3(0, 1, 1));
)shader_source"
R"shader_source(	float v101 = rand(index + ivec3(1, 0, 1));
)shader_source"
R"shader_source(	float v111 = rand(index + ivec3(1, 1, 1));
)shader_source"
R"shader_source(	return mix(
)shader_source"
R"shader_source(		mix(mix(v000, v100, d.x), mix(v010, v110, d.x), d.y),
)shader_source"
R"shader_source(		mix(mix(v001, v101, d.x), mix(v011, v111, d.x), d.y),
)shader_source"
R"shader_source(		d.z
)shader_source"
R"shader_source(	) * 2. - 1.;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float smoothNoise(vec2 p) {
)shader_source"
R"shader_source(	float F2 = .5 * (sqrt(3.) - 1.);
)shader_source"
R"shader_source(	ivec2 index = ivec2(floor(p + (p.x + p.y) * F2));
)shader_source"
R"shader_source(	float G2 = (3. - sqrt(3.)) / 6.;
)shader_source"
R"shader_source(	p -= index - (index.x + index.y) * G2;
)shader_source"
R"shader_source(	ivec2 index1 = p.x > p.y ? ivec2(1, 0) : ivec2(0, 1);
)shader_source"
R"shader_source(	vec2 pV[] = vec2[](p, p - index1 + G2, p - 1. + 2. * G2);
)shader_source"
R"shader_source(	ivec3 vertexX = ivec3(index.x) + ivec3(0, index1.x, 1);
)shader_source"
R"shader_source(	ivec3 vertexY = ivec3(index.y) + ivec3(0, index1.y, 1);
)shader_source"
R"shader_source(	float result = 0;
)shader_source"
R"shader_source(	for (uint i = 0; i < 3; i++) {
)shader_source"
R"shader_source(		float t = max(0, .5 - pV[i].x * pV[i].x - pV[i].y * pV[i].y);
)shader_source"
R"shader_source(		t *= t;
)shader_source"
R"shader_source(		uint gradientIndex = hash(vertexX[i] + hash(vertexY[i])) % 12;
)shader_source"
R"shader_source(		result += t * t * dot(simplexGradients[gradientIndex].xy, pV[i]);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return 70. * result;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float smoothNoise(vec3 p) {
)shader_source"
R"shader_source(	float F3 = 1./3.;
)shader_source"
R"shader_source(	float G3 = 1./6.;
)shader_source"
R"shader_source(	ivec3 index = ivec3(floor(p + (p.x + p.y + p.z) * F3));
)shader_source"
R"shader_source(	p -= index - (index.x + index.y + index.z) * G3;
)shader_source"
R"shader_source(	vec3 g = step(p.yzx, p);
)shader_source"
R"shader_source(	ivec3 index1 = ivec3(min(g, (1. - g).zxy));
)shader_source"
R"shader_source(	ivec3 index2 = ivec3(max(g, (1. - g).zxy));
)shader_source"
R"shader_source(	vec3 pV[] = vec3[](p, p - index1 + G3, p - index2 + 2. * G3, p - 1. + 3. * G3);
)shader_source"
R"shader_source(	int vertexX[] = int[]( index.x, index.x + index1.x, index.x + index2.x, index.x + 1);
)shader_source"
R"shader_source(	int vertexY[] = int[](index.y, index.y + index1.y, index.y + index2.y, index.y + 1);
)shader_source"
R"shader_source(	int vertexZ[] = int[](index.z, index.z + index1.z, index.z + index2.z, index.z + 1);
)shader_source"
R"shader_source(	float result = 0;
)shader_source"
R"shader_source(	for (uint i = 0; i < 4; i++) {
)shader_source"
R"shader_source(		float t = max(0., .6 - pV[i].x * pV[i].x - pV[i].y * pV[i].y - pV[i].z * pV[i].z);
)shader_source"
R"shader_source(		t *= t;
)shader_source"
R"shader_source(		uint gradientIndex = hash(vertexX[i] + hash(vertexY[i] + hash(vertexZ[i]))) % 12;
)shader_source"
R"shader_source(		result += t * t * dot(simplexGradients[gradientIndex], pV[i]);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return 32. * result;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float valueFbm(vec2 p) {
)shader_source"
R"shader_source(	float result = 0.;
)shader_source"
R"shader_source(	for (int i = 0; i < 3; i++) {
)shader_source"
R"shader_source(		result += exp2(-i) * valueNoise(p);
)shader_source"
R"shader_source(		p *= 2.;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return result / 1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float valueFbm(vec3 p) {
)shader_source"
R"shader_source(	float result = 0.;
)shader_source"
R"shader_source(	for (int i = 0; i < 3; i++) {
)shader_source"
R"shader_source(		result += exp2(-i) * valueNoise(p);
)shader_source"
R"shader_source(		p *= 2.;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return result / 1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float smoothFbm(vec2 p) {
)shader_source"
R"shader_source(	float result = 0.;
)shader_source"
R"shader_source(	for (int i = 0; i < 3; i++) {
)shader_source"
R"shader_source(		result += exp2(-i) * smoothNoise(p);
)shader_source"
R"shader_source(		p *= 2.;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return result / 1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float smoothFbm(vec3 p) {
)shader_source"
R"shader_source(	float result = 0.;
)shader_source"
R"shader_source(	for (int i = 0; i < 3; i++) {
)shader_source"
R"shader_source(		result += exp2(-i) * smoothNoise(p);
)shader_source"
R"shader_source(		p *= 2.;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return result / 1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 28) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 66) uniform float time;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 86) uniform float post_lens_distort_k;
)shader_source"
R"shader_source(layout(location = 87) uniform float post_lens_distort_kcube;
)shader_source"
R"shader_source(layout(location = 88) uniform float post_vignette_intensity;
)shader_source"
R"shader_source(layout(location = 89) uniform float post_film_grain_intensity;
)shader_source"
R"shader_source(layout(location = 90) uniform float post_film_grain_frequency;
)shader_source"
R"shader_source(layout(location = 91) uniform float post_film_grain_power;
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
R"shader_source(float A = 0.15;
)shader_source"
R"shader_source(float B = 0.50;
)shader_source"
R"shader_source(float C = 0.10;
)shader_source"
R"shader_source(float D = 0.20;
)shader_source"
R"shader_source(float E = 0.02;
)shader_source"
R"shader_source(float F = 0.30;
)shader_source"
R"shader_source(float W = 11.2;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 tonemap(vec3 color) {
)shader_source"
R"shader_source(   return ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
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
R"shader_source(
)shader_source"
R"shader_source(	vec2 tc2 = tc + vec2(1, 0) * smoothNoise(vec2(time * 10, gl_FragCoord.y)) * post_distortion_scheiss_rt_float;
)shader_source"
R"shader_source(	col *= vignette(post_vignette_intensity, tc2);
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
R"shader_source(
)shader_source"
R"shader_source(	out_color = tonemap(out_color * 2)/tonemap(vec3(W));
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
