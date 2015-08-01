#ifndef ao_H
#define ao_H
const char ao_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(layout(location = 64) uniform float camera_shake_intensity_rt_float;
)shader_source"
R"shader_source(layout(location = 65) uniform float camera_shake_freq_rt_float;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 66) uniform float time;
)shader_source"
R"shader_source(
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
R"shader_source(#line 2 "quat_rotate"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// TODO figure out how to get rid of this special include file
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
)shader_source"
R"shader_source(void pQuatRotate(inout vec3 v, vec4 q) {
)shader_source"
R"shader_source(	vec3 t = 2 * cross(q.xyz, v);
)shader_source"
R"shader_source(	v += q.w * t + cross(q.xyz, t);
)shader_source"
R"shader_source(	// *hex hex*
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 4 "camera"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 67) uniform vec3 camera_position;
)shader_source"
R"shader_source(layout(location = 68) uniform vec4 camera_rotation; // quat
)shader_source"
R"shader_source(layout(location = 69) uniform float camera_focal_length;
)shader_source"
R"shader_source(layout(location = 70) uniform float camera_sensor_width;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// gets set with camGetDirection
)shader_source"
R"shader_source(float screen_distance = camera_focal_length / camera_sensor_width;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 camGetDirectionSS(vec2 coord, out float screen_d) {
)shader_source"
R"shader_source(	vec3 dir = vec3((coord - .5 * res) / res.x,
)shader_source"
R"shader_source(		camera_focal_length / camera_sensor_width);
)shader_source"
R"shader_source(	screen_d = length(dir.xz);
)shader_source"
R"shader_source(	dir = normalize(dir);
)shader_source"
R"shader_source(	return dir;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 camGetDirection() {
)shader_source"
R"shader_source(	vec2 shake = vec2(
)shader_source"
R"shader_source(		smoothNoise(vec2(time * camera_shake_freq_rt_float, 23)),
)shader_source"
R"shader_source(		smoothNoise(vec2(time * camera_shake_freq_rt_float, 283))
)shader_source"
R"shader_source(	) * camera_shake_intensity_rt_float;
)shader_source"
R"shader_source(	vec3 dir = camGetDirectionSS(gl_FragCoord.xy + shake * res, screen_distance);
)shader_source"
R"shader_source(	dir.z = -dir.z;
)shader_source"
R"shader_source(	pQuatRotate(dir, camera_rotation);
)shader_source"
R"shader_source(	return dir;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float camGetPixelSize(float dist) {
)shader_source"
R"shader_source(	return .5 * dist / (screen_distance * res.x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 2 "helper"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(const float Pi = 3.14159265359;
)shader_source"
R"shader_source(const float Tau = 6.28318530718;
)shader_source"
R"shader_source(const float Euler = 2.71828182846;
)shader_source"
R"shader_source(const float Golden_Ratio = 1.61803398875;
)shader_source"
R"shader_source(const float Inf = 1e3; // in erster Näherung oder so...
)shader_source"
R"shader_source(const float Real_Inf = 1./0.; // this could fuck up things
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float minV(vec2 v) {
)shader_source"
R"shader_source(	return min(v.x, v.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float minV(vec3 v) {
)shader_source"
R"shader_source(	return min(min(v.x, v.y), v.z);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float maxV(vec2 v) {
)shader_source"
R"shader_source(	return max(v.x, v.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float maxV(vec3 v) {
)shader_source"
R"shader_source(	return max(max(v.x, v.y), v.z);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float min3(float a, float b, float c) {
)shader_source"
R"shader_source(	return min(min(a, b), c);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float min4(float a, float b, float c, float d) {
)shader_source"
R"shader_source(	return min(min(min(a, b), c), d);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float max3(float a, float b, float c) {
)shader_source"
R"shader_source(	return max(max(a, b), c);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float max4(float a, float b, float c, float d) {
)shader_source"
R"shader_source(	return max(max(max(a, b), c), d);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float sum(vec2 v) {
)shader_source"
R"shader_source(	return v.x + v.y;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float sum(vec3 v) {
)shader_source"
R"shader_source(	return v.x + v.y + v.z;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float mult(vec2 v) {
)shader_source"
R"shader_source(	return v.x * v.y;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float mult(vec3 v) {
)shader_source"
R"shader_source(	return v.x * v.y * v.z;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float square(float x) {
)shader_source"
R"shader_source(	return x * x;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 square(vec2 x) {
)shader_source"
R"shader_source(	return x * x;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 square(vec3 x) {
)shader_source"
R"shader_source(	return x * x;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rcp(float x) {
)shader_source"
R"shader_source(	return 1./x;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 rcp(vec2 x) {
)shader_source"
R"shader_source(	return 1./x;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 rcp(vec3 x) {
)shader_source"
R"shader_source(	return 1./x;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float lengthN(vec2 p, float n) {
)shader_source"
R"shader_source(	return pow(sum(pow(abs(p), vec2(n))), 1./n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float lengthN(vec3 p, float n) {
)shader_source"
R"shader_source(	return pow(sum(pow(abs(p), vec3(n))), 1./n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float saturate(float x) {
)shader_source"
R"shader_source(	return clamp(x, 0., 1.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 saturate(vec2 x) {
)shader_source"
R"shader_source(	return clamp(x, 0., 1.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 saturate(vec3 v) {
)shader_source"
R"shader_source(	return clamp(v, 0., 1.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float linstep(float edge0, float edge1, float x) {
)shader_source"
R"shader_source(	return clamp((x - edge0) / (edge1 - edge0), 0., 1.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float smootherstep(float edge0, float edge1, float x) {
)shader_source"
R"shader_source(	// directly from master Ken
)shader_source"
R"shader_source(	float t = clamp((x - edge0) / (edge1 - edge0), 0., 1.);
)shader_source"
R"shader_source(	return t*t*t*(t*(t*6. - 15.) + 10.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pdot(vec2 a, vec2 b) {
)shader_source"
R"shader_source(	return max(0., dot(a, b));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pdot(vec3 a, vec3 b) {
)shader_source"
R"shader_source(	return max(0., dot(a, b));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float sgn(float x) {
)shader_source"
R"shader_source(	return x < 0. ? -1. : 1.;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 sgn(vec2 x) {
)shader_source"
R"shader_source(	return vec2(
)shader_source"
R"shader_source(			x.x < 0. ? -1. : 1.,
)shader_source"
R"shader_source(			x.y < 0. ? -1. : 1.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 sgn(vec3 x) {
)shader_source"
R"shader_source(	return vec3(
)shader_source"
R"shader_source(			x.x < 0. ? -1. : 1.,
)shader_source"
R"shader_source(			x.y < 0. ? -1. : 1.,
)shader_source"
R"shader_source(			x.z < 0. ? -1. : 1.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 unitVector(float phi) {
)shader_source"
R"shader_source(	return vec2(cos(phi), sin(phi));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// phi geht von 0 bis 2 pi (und fängt bei der x-achse an)
)shader_source"
R"shader_source(// theta von 0 bis pi (und fängt von oben an)
)shader_source"
R"shader_source(vec3 unitVector(float phi, float theta) {
)shader_source"
R"shader_source(	float ct = cos(theta);
)shader_source"
R"shader_source(	float sp = sin(phi);
)shader_source"
R"shader_source(	float st = sin(theta);
)shader_source"
R"shader_source(	float cp = cos(phi);
)shader_source"
R"shader_source(	return vec3(st * cp, ct, st * sp);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// m: anything above stays unchanged
)shader_source"
R"shader_source(// n: value at x = 0
)shader_source"
R"shader_source(// x: x
)shader_source"
R"shader_source(// see http://www.iquilezles.org/www/articles/functions/functions.htm
)shader_source"
R"shader_source(float iqAlmostIdentity(float m, float n, float x) {
)shader_source"
R"shader_source(	if(x > m) return x;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float a = 2. * n - m;
)shader_source"
R"shader_source(	float b = 2. * m - 3. * n;
)shader_source"
R"shader_source(	float t = x / m;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return (a * t + b) * t * t + n;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// k: width of impulse, maximum happens at 1/k
)shader_source"
R"shader_source(// x: x
)shader_source"
R"shader_source(// see http://www.iquilezles.org/www/articles/functions/functions.htm
)shader_source"
R"shader_source(float iqImpulse(float k, float x) {
)shader_source"
R"shader_source(	float h = k * x;
)shader_source"
R"shader_source(	return h * exp(1. - h);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// c: x value of maximum
)shader_source"
R"shader_source(// w: half pulse width
)shader_source"
R"shader_source(// x: x
)shader_source"
R"shader_source(// see http://www.iquilezles.org/www/articles/functions/functions.htm
)shader_source"
R"shader_source(float iqCubicPulse(float c, float w, float x) {
)shader_source"
R"shader_source(	x = abs(x - c);
)shader_source"
R"shader_source(	x /= w;
)shader_source"
R"shader_source(	return 1. - smoothstep(0., 1., x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// k: kind of inverse when the step occurs
)shader_source"
R"shader_source(// n: how stepish the function is
)shader_source"
R"shader_source(// x: x
)shader_source"
R"shader_source(// see http://www.iquilezles.org/www/articles/functions/functions.htm
)shader_source"
R"shader_source(float iqExpStep(float k, float n, float x) {
)shader_source"
R"shader_source(	return exp(-k * pow(x, n));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// k: how thin the function is
)shader_source"
R"shader_source(// x: x
)shader_source"
R"shader_source(// see http://www.iquilezles.org/www/articles/functions/functions.htm
)shader_source"
R"shader_source(float iqParabola(float k, float x) {
)shader_source"
R"shader_source(	return pow(4. * x * (1. - x), k);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// a, b: how much to bend in left and right direction
)shader_source"
R"shader_source(// x: x
)shader_source"
R"shader_source(// reaches 1 at maximum
)shader_source"
R"shader_source(// see http://www.iquilezles.org/www/articles/functions/functions.htm
)shader_source"
R"shader_source(float iqPowerCurve(float a, float b, float x) {
)shader_source"
R"shader_source(    float k = pow(a + b, a + b) / (pow(a, a) * pow(b, b));
)shader_source"
R"shader_source(    return k * pow(x, a) * pow(1. - x, b);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// a, b: how much to bend in left and right direction
)shader_source"
R"shader_source(// x: x
)shader_source"
R"shader_source(// not normalized version
)shader_source"
R"shader_source(// see http://www.iquilezles.org/www/articles/functions/functions.htm
)shader_source"
R"shader_source(float iqPowerCurveUnnormalized(float a, float b, float x) {
)shader_source"
R"shader_source(    return pow(x, a) * pow(1. - x, b);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rgb2luma(vec3 rgb) {
)shader_source"
R"shader_source(	return dot(rgb, vec3(.2126, .7152, .0722)); // magic luminance formular
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 rgb2hsv(vec3 c) {
)shader_source"
R"shader_source(    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
)shader_source"
R"shader_source(    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
)shader_source"
R"shader_source(    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(    float d = q.x - min(q.w, q.y);
)shader_source"
R"shader_source(    float e = 1.0e-10;
)shader_source"
R"shader_source(    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 hsv2rgb(vec3 c) {
)shader_source"
R"shader_source(    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
)shader_source"
R"shader_source(    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
)shader_source"
R"shader_source(    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 6
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 8) uniform sampler2D depth; // float level(1)
)shader_source"
R"shader_source(layout(location = 0) out float out_ao;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// level(1)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(int maxStepsPerRay = 10;
)shader_source"
R"shader_source(float bias = .03;
)shader_source"
R"shader_source(float falloff = 2;
)shader_source"
R"shader_source(layout(location = 71) uniform float post_ao_radius;
)shader_source"
R"shader_source(layout(location = 72) uniform float post_ao_n_rays;
)shader_source"
R"shader_source(layout(location = 73) uniform float post_ao_intensity;
)shader_source"
R"shader_source(layout(location = 74) uniform float post_ao_avg_occlusion;
)shader_source"
R"shader_source(layout(location = 75) uniform bool post_disable_ao;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 sampleDirection(int i, float rotation) {
)shader_source"
R"shader_source(	float phi = Tau / post_ao_n_rays * (i + rotation);
)shader_source"
R"shader_source(	return vec2(cos(phi), sin(phi));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 getViewPosition(vec2 tc) {
)shader_source"
R"shader_source(	float sampleDepth = textureLod(depth, tc, 0).r;
)shader_source"
R"shader_source(	float screen_d;
)shader_source"
R"shader_source(	return sampleDepth * camGetDirectionSS(tc * (res - 1) + .5, screen_d);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 snapToTexel(vec2 coord) {
)shader_source"
R"shader_source(	return coord;
)shader_source"
R"shader_source(	vec2 maxScreenCoords = res - 1;
)shader_source"
R"shader_source(	return round(coord * maxScreenCoords) / maxScreenCoords;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float getSampleOcclusion(vec2 tc, vec3 centerViewPos, vec3 centerNormal, vec3 tangent, inout float topOcclusion) {
)shader_source"
R"shader_source(	vec3 sampleViewPos = getViewPosition(tc);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 horizonVector = sampleViewPos - centerViewPos;
)shader_source"
R"shader_source(	float horizonVectorLength = length(horizonVector);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (dot(tangent, horizonVector) < 0) {
)shader_source"
R"shader_source(		// "estimate"
)shader_source"
R"shader_source(		return post_ao_avg_occlusion;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float occlusion = dot(centerNormal, horizonVector) / horizonVectorLength;
)shader_source"
R"shader_source(	float diff = max(occlusion - topOcclusion, 0);
)shader_source"
R"shader_source(	topOcclusion = max(occlusion, topOcclusion);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float distanceFactor = 1 -  square(saturate(horizonVectorLength / falloff));
)shader_source"
R"shader_source(	return diff * distanceFactor;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float getRayOcclusion(vec2 origin, vec2 direction, float jitter, vec2 projectedRadii, vec3 centerViewPos, vec3 centerNormal, int numStepsPerRay) {
)shader_source"
R"shader_source(	vec2 texelSizedStep = direction / res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 tangent = getViewPosition(origin + texelSizedStep) - centerViewPos;
)shader_source"
R"shader_source(	tangent -= dot(centerNormal, tangent) * centerNormal;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 stepUV = snapToTexel(direction * projectedRadii / (numStepsPerRay - 1));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 jitteredOffset = mix(texelSizedStep, stepUV, jitter);
)shader_source"
R"shader_source(	vec2 uv = snapToTexel(origin + jitteredOffset);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float topOcclusion = bias;
)shader_source"
R"shader_source(	float occlusion = 0;
)shader_source"
R"shader_source(	for (int i = 0; i < numStepsPerRay; i++) {
)shader_source"
R"shader_source(		occlusion += getSampleOcclusion(uv, centerViewPos, centerNormal, tangent, topOcclusion);
)shader_source"
R"shader_source(		uv += stepUV;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return occlusion;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	if (post_disable_ao) {
)shader_source"
R"shader_source(		out_ao = 1;
)shader_source"
R"shader_source(		return;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float centerDepth = textureLod(depth, tc, 0).r;
)shader_source"
R"shader_source(	float screen_d;
)shader_source"
R"shader_source(	vec3 centerViewPos = centerDepth * camGetDirectionSS(gl_FragCoord.xy, screen_d);
)shader_source"
R"shader_source(	vec3 centerNormal = normalize(cross(dFdy(centerViewPos), dFdx(centerViewPos)));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float rotationJitter = valueNoise(gl_FragCoord.xy + 3827.5) * .5;
)shader_source"
R"shader_source(	float positionJitter = valueNoise(gl_FragCoord.xy + 827.5) * .5 + .5;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 projectedRadii = post_ao_radius * screen_d / centerDepth * vec2(1, res.y / res.x);
)shader_source"
R"shader_source(	float screenRadius = projectedRadii.x * res.x;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float totalOcclusion = 0;
)shader_source"
R"shader_source(	if (screenRadius >= 1) {
)shader_source"
R"shader_source(		int numStepsPerRay = min(maxStepsPerRay, int(screenRadius));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		for (int i = 0; i < post_ao_n_rays; i++) {
)shader_source"
R"shader_source(			vec2 sampleDir = sampleDirection(i, rotationJitter);
)shader_source"
R"shader_source(			totalOcclusion += getRayOcclusion(tc, sampleDir, positionJitter, projectedRadii, centerViewPos, centerNormal, numStepsPerRay);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		totalOcclusion *= post_ao_intensity / post_ao_n_rays;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float ao = 1 - saturate(totalOcclusion);
)shader_source"
R"shader_source(	out_ao = ao;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
