#ifndef glumanda_H
#define glumanda_H
const char glumanda_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(layout(location = 94) uniform float glum_floor_glow_intensity_rt_float;
)shader_source"
R"shader_source(layout(location = 95) uniform float glum_floor_glow_space_rt_float;
)shader_source"
R"shader_source(layout(location = 96) uniform float glum_floor_glow_falloff_rt_float;
)shader_source"
R"shader_source(layout(location = 97) uniform float glum_tunnel_glow_posy_offset_rt_float;
)shader_source"
R"shader_source(layout(location = 98) uniform float glum_tunnel_glow_intensity_rt_float;
)shader_source"
R"shader_source(layout(location = 99) uniform float glum_floor_glow_velo_rt_float;
)shader_source"
R"shader_source(layout(location = 100) uniform float glum_tunnel_glow_stripes_big_rt_float;
)shader_source"
R"shader_source(layout(location = 101) uniform float glum_tunnel_glow_falloff_rt_float;
)shader_source"
R"shader_source(layout(location = 102) uniform float glum_tunnel_glow_height_rt_float;
)shader_source"
R"shader_source(layout(location = 103) uniform float glum_tunnel_glow_start_rt_float;
)shader_source"
R"shader_source(layout(location = 104) uniform float glum_signal_glow_total_rt_float;
)shader_source"
R"shader_source(layout(location = 105) uniform float glum_signal_impulse_k_rt_float;
)shader_source"
R"shader_source(layout(location = 106) uniform float glum_spimi_spike_cap_rt_float;
)shader_source"
R"shader_source(layout(location = 107) uniform float glum_tunnel_glow_stripes_small_rt_float;
)shader_source"
R"shader_source(layout(location = 108) uniform float glum_tunnel_pulse_rt_float;
)shader_source"
R"shader_source(layout(location = 109) uniform float glum_spimi_radius_var_rt_float;
)shader_source"
R"shader_source(layout(location = 110) uniform float glum_total_rot_rt_float;
)shader_source"
R"shader_source(layout(location = 111) uniform float glum_floor_glow_length_rt_float;
)shader_source"
R"shader_source(layout(location = 112) uniform float glum_spimi_disc_height_rt_float;
)shader_source"
R"shader_source(layout(location = 64) uniform float camera_shake_intensity_rt_float;
)shader_source"
R"shader_source(layout(location = 65) uniform float camera_shake_freq_rt_float;
)shader_source"
R"shader_source(layout(location = 113) uniform float glum_signal_glow_mid_rt_float;
)shader_source"
R"shader_source(layout(location = 114) uniform float glum_signal_max_velo_rt_float;
)shader_source"
R"shader_source(layout(location = 115) uniform float glum_signal_end_rt_float;
)shader_source"
R"shader_source(layout(location = 116) uniform float camera_exposure_rt_float;
)shader_source"
R"shader_source(layout(location = 117) uniform float glum_spimi_radius_rt_float;
)shader_source"
R"shader_source(layout(location = 118) uniform float glum_spimi_spike_length_rt_float;
)shader_source"
R"shader_source(layout(location = 119) uniform float glum_signal_dim_rt_float;
)shader_source"
R"shader_source(layout(location = 120) uniform float glum_spikes_cut_r_rt_float;
)shader_source"
R"shader_source(layout(location = 121) uniform float glum_tunnel_pulse_amp_rt_float;
)shader_source"
R"shader_source(layout(location = 122) uniform float glum_floor_glow_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 123) uniform float glum_total_loco_trans_rt_float;
)shader_source"
R"shader_source(layout(location = 124) uniform float glum_signal_glow_base_rt_float;
)shader_source"
R"shader_source(layout(location = 125) uniform float glum_total_loco_rot_rt_float;
)shader_source"
R"shader_source(layout(location = 126) uniform float glum_signal_start_outer_rt_float;
)shader_source"
R"shader_source(layout(location = 127) uniform float glum_tunnel_offset_rt_float;
)shader_source"
R"shader_source(layout(location = 128) uniform float glum_signal_start_rt_float;
)shader_source"
R"shader_source(layout(location = 129) uniform float glum_spimi_n_spikes_rt_float;
)shader_source"
R"shader_source(layout(location = 130) uniform float glum_signal_impulse_offset_rt_float;
)shader_source"
R"shader_source(#line 3 "fragment"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// main output color
)shader_source"
R"shader_source(out vec3 out_color;
)shader_source"
R"shader_source(out float out_depth;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// common uniforms
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(layout(location = 66) uniform float time;
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
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 4 "sdf/domain"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pTrans(inout float p, float d) {
)shader_source"
R"shader_source(	p -= d;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pTrans(inout vec2 p, vec2 d) {
)shader_source"
R"shader_source(	p -= d;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pTrans(inout vec2 p, float x, float y) {
)shader_source"
R"shader_source(	pTrans(p, vec2(x, y));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pTrans(inout vec3 p, vec3 d) {
)shader_source"
R"shader_source(	p -= d;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pTrans(inout vec3 p, float x, float y, float z) {
)shader_source"
R"shader_source(	pTrans(p, vec3(x, y, z));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// rotates clockwise when looking in the direction given by the right-hand rule
)shader_source"
R"shader_source(// don't use this directly (unless for 2d coordinates)!
)shader_source"
R"shader_source(void pRot(inout vec2 p, float phi) {
)shader_source"
R"shader_source(	p *= mat2(cos(phi), sin(phi), -sin(phi), cos(phi));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// rotate clockwise around X axis
)shader_source"
R"shader_source(void pRotX(inout vec3 p, float phi) {
)shader_source"
R"shader_source(	pRot(p.yz, phi);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// rotate clockwise around Y axis
)shader_source"
R"shader_source(void pRotY(inout vec3 p, float phi) {
)shader_source"
R"shader_source(	pRot(p.zx, phi);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// rotate clockwise around Z axis
)shader_source"
R"shader_source(void pRotZ(inout vec3 p, float phi) {
)shader_source"
R"shader_source(	pRot(p.xy, phi);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pDomrep(inout float p, float c) {
)shader_source"
R"shader_source(	p += .5 * c;
)shader_source"
R"shader_source(	float i = floor(p/c);
)shader_source"
R"shader_source(	p = mod(p, c) - .5 * c;
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 pDomrep(inout vec2 p, vec2 c) {
)shader_source"
R"shader_source(	p += .5 * c;
)shader_source"
R"shader_source(	vec2 i = floor(p/c);
)shader_source"
R"shader_source(	p = mod(p, c) - .5 * c;
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 pDomrep(inout vec2 p, float cx, float cy) {
)shader_source"
R"shader_source(	return pDomrep(p, vec2(cx, cy));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 pDomrep(inout vec3 p, vec3 c) {
)shader_source"
R"shader_source(	p += .5 * c;
)shader_source"
R"shader_source(	vec3 i = floor(p/c);
)shader_source"
R"shader_source(	p = mod(p, c) - .5 * c;
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 pDomrep(inout vec3 p, float cx, float cy, float cz) {
)shader_source"
R"shader_source(	return pDomrep(p, vec3(cx, cy, cz));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pMirrorDomrepped(inout float p, float i) {
)shader_source"
R"shader_source(	p *= mod(i, 2.) * 2. - 1.;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pMirrorDomrepped(inout vec2 p, vec2 i) {
)shader_source"
R"shader_source(	p *= mod(i, 2.) * 2. - 1.;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pMirrorDomrepped(inout vec3 p, vec3 i) {
)shader_source"
R"shader_source(	p *= mod(i, 2.) * 2. - 1.;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pDomrepMirror(inout float p, float c) {
)shader_source"
R"shader_source(	float i = pDomrep(p, c);
)shader_source"
R"shader_source(	p *= mod(i, 2.) * 2. - 1.;
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 pDomrepMirror(inout vec2 p, vec2 c) {
)shader_source"
R"shader_source(	vec2 i = pDomrep(p, c);
)shader_source"
R"shader_source(	p *= mod(i, 2.) * 2. - 1.;
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 pDomrepMirror(inout vec3 p, vec3 c) {
)shader_source"
R"shader_source(	vec3 i = pDomrep(p, c);
)shader_source"
R"shader_source(	p *= mod(i, 2.) * 2. - 1.;
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 pDomrepGrid(inout vec2 p, vec2 c) {
)shader_source"
R"shader_source(	vec2 i = pDomrepMirror(p, c);
)shader_source"
R"shader_source(	p -= .5 * c;
)shader_source"
R"shader_source(	if (p.x > p.y) {
)shader_source"
R"shader_source(		p.xy = p.yx;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return floor(.5 * i);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 pDomrepGrid(inout vec3 p, vec3 c) {
)shader_source"
R"shader_source(	vec3 i = pDomrepMirror(p, c);
)shader_source"
R"shader_source(	p -= .5 * c;
)shader_source"
R"shader_source(	if (p.x < p.y) {
)shader_source"
R"shader_source(		if (p.y > p.z) {
)shader_source"
R"shader_source(			if (p.x < p.z) {
)shader_source"
R"shader_source(				p = p.xzy;
)shader_source"
R"shader_source(			} else {
)shader_source"
R"shader_source(				p = p.zxy;
)shader_source"
R"shader_source(			}
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		if (p.z < p.y) {
)shader_source"
R"shader_source(			p = p.zyx;
)shader_source"
R"shader_source(		} else {
)shader_source"
R"shader_source(			if (p.z < p.x) {
)shader_source"
R"shader_source(				p = p.yzx;
)shader_source"
R"shader_source(			} else {
)shader_source"
R"shader_source(				p = p.yxz;
)shader_source"
R"shader_source(			}
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return floor(.5 * i);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// domrep only in positive half
)shader_source"
R"shader_source(float pDomrepSingle(inout float p, float c) {
)shader_source"
R"shader_source(	float halfC = .5 * c;
)shader_source"
R"shader_source(	float i = 0.;
)shader_source"
R"shader_source(	if (p > halfC) {
)shader_source"
R"shader_source(		p += halfC;
)shader_source"
R"shader_source(		i = floor(p/c);
)shader_source"
R"shader_source(		p = mod(p, c) - halfC;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// domrep only in positive half
)shader_source"
R"shader_source(vec2 pDomrepSingle(inout vec2 p, vec2 c) {
)shader_source"
R"shader_source(	vec2 halfC = .5 * c;
)shader_source"
R"shader_source(	vec2 i = vec2(0.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (p.x > halfC.x) {
)shader_source"
R"shader_source(		p.x += halfC.x;
)shader_source"
R"shader_source(		i.x = floor(p.x/c.x);
)shader_source"
R"shader_source(		p.x = mod(p.x, c.x) - halfC.x;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (p.y > halfC.y) {
)shader_source"
R"shader_source(		p.y += halfC.y;
)shader_source"
R"shader_source(		i.y = floor(p.y/c.y);
)shader_source"
R"shader_source(		p.y = mod(p.y, c.y) - halfC.y;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// domrep only in positive half
)shader_source"
R"shader_source(vec3 pDomrepSingle(inout vec3 p, vec3 c) {
)shader_source"
R"shader_source(	vec3 halfC = .5 * c;
)shader_source"
R"shader_source(	vec3 i = vec3(0.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (p.x > halfC.x) {
)shader_source"
R"shader_source(		p.x += halfC.x;
)shader_source"
R"shader_source(		i.x = floor(p.x/c.x);
)shader_source"
R"shader_source(		p.x = mod(p.x, c.x) - halfC.x;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (p.y > halfC.y) {
)shader_source"
R"shader_source(		p.y += halfC.y;
)shader_source"
R"shader_source(		i.y = floor(p.y/c.y);
)shader_source"
R"shader_source(		p.y = mod(p.y, c.y) - halfC.y;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (p.z > halfC.z) {
)shader_source"
R"shader_source(		p.z += halfC.z;
)shader_source"
R"shader_source(		i.z = floor(p.z/c.z);
)shader_source"
R"shader_source(		p.z = mod(p.z, c.z) - halfC.z;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// domrep from cell start to (inclusively) end
)shader_source"
R"shader_source(float pDomrepInterval(inout float p, float c, float start, float end) {
)shader_source"
R"shader_source(	p += .5 * c;
)shader_source"
R"shader_source(	float i = floor(p/c);
)shader_source"
R"shader_source(	p = mod(p, c) - .5 * c;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (i > end) {
)shader_source"
R"shader_source(		p += c * (i - end);
)shader_source"
R"shader_source(		i = end;
)shader_source"
R"shader_source(	} else if (i < start) {
)shader_source"
R"shader_source(		p += c * (i - start);
)shader_source"
R"shader_source(		i = start;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// domrep from cell start to (inclusively) end
)shader_source"
R"shader_source(vec2 pDomrepInterval(inout vec2 p, vec2 c, vec2 start, vec2 end) {
)shader_source"
R"shader_source(	p += .5 * c;
)shader_source"
R"shader_source(	vec2 i = floor(p/c);
)shader_source"
R"shader_source(	p = mod(p, c) - .5 * c;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (i.x > end.x) {
)shader_source"
R"shader_source(		p.x += c.x * (i.x - end.x);
)shader_source"
R"shader_source(		i.x = end.x;
)shader_source"
R"shader_source(	} else if (i.x < start.x) {
)shader_source"
R"shader_source(		p.x += c.x * (i.x - start.x);
)shader_source"
R"shader_source(		i.x = start.x;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (i.y > end.y) {
)shader_source"
R"shader_source(		p.y += c.y * (i.y - end.y);
)shader_source"
R"shader_source(		i.y = end.y;
)shader_source"
R"shader_source(	} else if (i.y < start.y) {
)shader_source"
R"shader_source(		p.y += c.y * (i.y - start.y);
)shader_source"
R"shader_source(		i.y = start.y;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// domrep from cell start to (inclusively) end
)shader_source"
R"shader_source(vec3 pDomrepInterval(inout vec3 p, vec3 c, vec3 start, vec3 end) {
)shader_source"
R"shader_source(	p += .5 * c;
)shader_source"
R"shader_source(	vec3 i = floor(p/c);
)shader_source"
R"shader_source(	p = mod(p, c) - .5 * c;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (i.x > end.x) {
)shader_source"
R"shader_source(		p.x += c.x * (i.x - end.x);
)shader_source"
R"shader_source(		i.x = end.x;
)shader_source"
R"shader_source(	} else if (i.x < start.x) {
)shader_source"
R"shader_source(		p.x += c.x * (i.x - start.x);
)shader_source"
R"shader_source(		i.x = start.x;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (i.y > end.y) {
)shader_source"
R"shader_source(		p.y += c.y * (i.y - end.y);
)shader_source"
R"shader_source(		i.y = end.y;
)shader_source"
R"shader_source(	} else if (i.y < start.y) {
)shader_source"
R"shader_source(		p.y += c.y * (i.y - start.y);
)shader_source"
R"shader_source(		i.y = start.y;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (i.z > end.z) {
)shader_source"
R"shader_source(		p.z += c.z * (i.z - end.z);
)shader_source"
R"shader_source(		i.z = end.z;
)shader_source"
R"shader_source(	} else if (i.z < start.z) {
)shader_source"
R"shader_source(		p.z += c.z * (i.z - start.z);
)shader_source"
R"shader_source(		i.z = start.z;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pDomrepAngleWithAtan(inout vec2 p, float repetitions, float radius, float preCalcAtan) {
)shader_source"
R"shader_source(	float at = preCalcAtan;
)shader_source"
R"shader_source(	float cAngle = Tau / repetitions;
)shader_source"
R"shader_source(	float i = pDomrep(at, cAngle);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float r = length(p);
)shader_source"
R"shader_source(	p = r * unitVector(at);
)shader_source"
R"shader_source(	pTrans(p.x, radius);
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pDomrepAngle(inout vec2 p, float repetitions, float radius) {
)shader_source"
R"shader_source(	return pDomrepAngleWithAtan(p, repetitions, radius, atan(p.y, p.x));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pDomrepRadiusWithAtan(inout vec2 p, float c, float preCalcAtan) {
)shader_source"
R"shader_source(	float r = length(p);
)shader_source"
R"shader_source(	float i = floor(r / c);
)shader_source"
R"shader_source(	r = mod(r, c);
)shader_source"
R"shader_source(	p = r * unitVector(preCalcAtan);
)shader_source"
R"shader_source(	return i;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pDomrepRadius(inout vec2 p, float c) {
)shader_source"
R"shader_source(	return pDomrepRadiusWithAtan(p, c, atan(p.y, p.x));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pMirror(inout float p) {
)shader_source"
R"shader_source(	float s = sgn(p);
)shader_source"
R"shader_source(	p = abs(p);
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 pMirror(inout vec2 p) {
)shader_source"
R"shader_source(	vec2 s = sgn(p);
)shader_source"
R"shader_source(	p = abs(p);
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 pMirror(inout vec3 p) {
)shader_source"
R"shader_source(	vec3 s = sgn(p);
)shader_source"
R"shader_source(	p = abs(p);
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pMirrorTrans(inout float p, float c) {
)shader_source"
R"shader_source(	float s = pMirror(p);
)shader_source"
R"shader_source(	pTrans(p, c);
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 pMirrorTrans(inout vec2 p, vec2 c) {
)shader_source"
R"shader_source(	vec2 s = pMirror(p);
)shader_source"
R"shader_source(	pTrans(p, c);
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 pMirrorTrans(inout vec3 p, vec3 c) {
)shader_source"
R"shader_source(	vec3 s = pMirror(p);
)shader_source"
R"shader_source(	pTrans(p, c);
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 pMirrorLoco(inout vec2 p, vec2 c) {
)shader_source"
R"shader_source(	vec3 s;
)shader_source"
R"shader_source(	s.xy = pMirrorTrans(p, c);
)shader_source"
R"shader_source(	if (p.y > p.x) {
)shader_source"
R"shader_source(		s.z = -1.;
)shader_source"
R"shader_source(		p.xy = p.yx;
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		s.z = 1.;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 pMirrorLoco(inout vec3 p, vec3 c) {
)shader_source"
R"shader_source(	p = p.zyx;
)shader_source"
R"shader_source(	vec3 s = pMirrorTrans(p, c);
)shader_source"
R"shader_source(	if (p.x < p.y) {
)shader_source"
R"shader_source(		if (p.y > p.z) {
)shader_source"
R"shader_source(			if (p.x < p.z) {
)shader_source"
R"shader_source(				p = p.xzy;
)shader_source"
R"shader_source(			} else {
)shader_source"
R"shader_source(				p = p.zxy;
)shader_source"
R"shader_source(			}
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		if (p.z < p.y) {
)shader_source"
R"shader_source(			p = p.zyx;
)shader_source"
R"shader_source(		} else {
)shader_source"
R"shader_source(			if (p.z < p.x) {
)shader_source"
R"shader_source(				p = p.yzx;
)shader_source"
R"shader_source(			} else {
)shader_source"
R"shader_source(				p = p.yxz;
)shader_source"
R"shader_source(			}
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	p = p.zyx;
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see pMirrorGrid(vec3, float)
)shader_source"
R"shader_source(vec2 pMirrorGrid(inout vec2 p, float c) {
)shader_source"
R"shader_source(	vec2 q = p;
)shader_source"
R"shader_source(	pMirror(q);
)shader_source"
R"shader_source(	vec2 s = vec2(0);
)shader_source"
R"shader_source(	if (q.y > q.x) {
)shader_source"
R"shader_source(		p = p.yx;
)shader_source"
R"shader_source(		s.x = 1;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	s.y = pMirrorTrans(p.x, c);
)shader_source"
R"shader_source(	p.y *= s.y;
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// s.x: axis (0: x, 1: y, 2: z)
)shader_source"
R"shader_source(// s.y: mirroring (-1 or 1)
)shader_source"
R"shader_source(vec2 pMirrorGrid(inout vec3 p, float c) {
)shader_source"
R"shader_source(	vec3 q = p;
)shader_source"
R"shader_source(	pMirror(q);
)shader_source"
R"shader_source(	vec2 s = vec2(0);
)shader_source"
R"shader_source(	if (q.z > q.x || q.y > q.x) {
)shader_source"
R"shader_source(		if (q.y > q.z) {
)shader_source"
R"shader_source(			p = p.yxz;
)shader_source"
R"shader_source(			s.x = 1;
)shader_source"
R"shader_source(		} else {
)shader_source"
R"shader_source(			p = p.zyx;
)shader_source"
R"shader_source(			s.x = 2;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	s.y = pMirrorTrans(p.x, c);
)shader_source"
R"shader_source(	p.y *= s.y;
)shader_source"
R"shader_source(	return s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pMirrorAtPlane(inout vec3 p, vec3 planeNormal, float offset) {
)shader_source"
R"shader_source(	float dist = dot(p, planeNormal) + offset;
)shader_source"
R"shader_source(	if (dist < 0.) {
)shader_source"
R"shader_source(		p -= 2. * dist * planeNormal;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return sgn(dist);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// cut through a plane at point a with normal normalize(v) and translate positive
)shader_source"
R"shader_source(// halfspace by v, filling the gap with the extruded cut plane
)shader_source"
R"shader_source(float pCutAndExtrude(inout vec3 p, vec3 a, vec3 v) {
)shader_source"
R"shader_source(	pTrans(p, a);
)shader_source"
R"shader_source(	float h = saturate(dot(p, v)/ dot(v, v));
)shader_source"
R"shader_source(	p -= v * h;
)shader_source"
R"shader_source(	pTrans(p, -a);
)shader_source"
R"shader_source(	return h;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pFlip(inout float p) {
)shader_source"
R"shader_source(	p = -p;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pFlip(inout vec2 p) {
)shader_source"
R"shader_source(	p = -p;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void pFlip(inout vec3 p) {
)shader_source"
R"shader_source(	p = -p;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 3 "sdf/operators"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// unions two object and produces a very smooth transition
)shader_source"
R"shader_source(// affects an area of r between the meet point, underestimates the distance
)shader_source"
R"shader_source(// can be used with any kind of objects
)shader_source"
R"shader_source(float opUnionSmooth(float f1, float f2, float r) {
)shader_source"
R"shader_source(	// maybe remove this min() and if() stuff
)shader_source"
R"shader_source(	//float f_min = min(f1, f2);
)shader_source"
R"shader_source(	if (f1 < r && f2 < r) {
)shader_source"
R"shader_source(		float h = clamp(.5 + .5 * (f2 - f1) / r, 0., 1. );
)shader_source"
R"shader_source(		float f_smooth = mix(f2, f1, h) - r * h * (1. - h);
)shader_source"
R"shader_source(		return f_smooth;
)shader_source"
R"shader_source(		//return min(f_smooth, f_min);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	float f_min = min(f1, f2);
)shader_source"
R"shader_source(	return f_min;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// intersects two object and produces a very smooth transition
)shader_source"
R"shader_source(// affects an area of r between the meet point, underestimates the distance
)shader_source"
R"shader_source(// can be used with any kind of objects
)shader_source"
R"shader_source(float opIntersectSmooth(float f1, float f2, float r) {
)shader_source"
R"shader_source(	// maybe remove this max() and if() stuff
)shader_source"
R"shader_source(	//float f_max = max(f1, f2);
)shader_source"
R"shader_source(	//if (f1 > -r && f2 > -r) {
)shader_source"
R"shader_source(	if (abs(f1) < r && abs(f2) < r) {
)shader_source"
R"shader_source(		float h = clamp(.5 - .5 * (f2 - f1) / r, 0., 1. );
)shader_source"
R"shader_source(		float f_smooth = mix(f2, f1, h) + r * h * (1. - h);
)shader_source"
R"shader_source(		return f_smooth;
)shader_source"
R"shader_source(		//return max(f_smooth, f_max);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	float f_max = max(f1, f2);
)shader_source"
R"shader_source(	return f_max;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// subtracts f2 from f1 and produces a very smooth transition
)shader_source"
R"shader_source(// affects an area of r between the meet point, underestimates the distance
)shader_source"
R"shader_source(// can be used with any kind of objects
)shader_source"
R"shader_source(float opSubtractSmooth(float f1, float f2, float r) {
)shader_source"
R"shader_source(	return opIntersectSmooth(f1, -f2, r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// all hail the smin
)shader_source"
R"shader_source(float smin(float f1, float f2, float r) {
)shader_source"
R"shader_source(	return opUnionSmooth(f1, f2, r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float smax(float f1, float f2, float r) {
)shader_source"
R"shader_source(	return opIntersectSmooth(f1, f2, r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// unions two objects and adds rounded edges with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opUnionRounded(float f1, float f2, float r) {
)shader_source"
R"shader_source(	float f_min = min(f1, f2);
)shader_source"
R"shader_source(	if (f1 < r && f2 < r) {
)shader_source"
R"shader_source(		vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(		vec2 center = vec2(r);
)shader_source"
R"shader_source(		float f_round = r - distance(center, q); // inside of fSphere2
)shader_source"
R"shader_source(		return min(f_round, f_min);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return f_min;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// intersects two objects and adds rounded edges with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opIntersectRounded(float f1, float f2, float r) {
)shader_source"
R"shader_source(	float f_max = max(f1, f2);
)shader_source"
R"shader_source(	if (f1 > -r && f2 > -r) {
)shader_source"
R"shader_source(		vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(		vec2 center = vec2(-r);
)shader_source"
R"shader_source(		float f_round = distance(center, q) - r;
)shader_source"
R"shader_source(		return max(f_round, f_max);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return f_max;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// subtracts f2 from f1 and adds rounded edges with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opSubtractRounded(float f1, float f2, float r) {
)shader_source"
R"shader_source(	return opIntersectRounded(f1, -f2, r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// unions two objects and adds some kind of chamfer with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opUnionChamfer(float f1, float f2, float r) {
)shader_source"
R"shader_source(	float f_min = min(f1, f2);
)shader_source"
R"shader_source(	//if (f1 < r && f2 < r) {
)shader_source"
R"shader_source(		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
)shader_source"
R"shader_source(		return min(f_chamfer, f_min);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(	//return f_min;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// intersects two objects and adds some kind of chamfer with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opIntersectChamfer(float f1, float f2, float r) {
)shader_source"
R"shader_source(	float f_max = max(f1, f2);
)shader_source"
R"shader_source(	//if (f1 > -r && f2 > -r) {
)shader_source"
R"shader_source(		float f_chamfer = sqrt(.5) * (f1 + f2 + r);
)shader_source"
R"shader_source(		return max(f_chamfer, f_max);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(	//return f_max;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// subtracts f2 from f1 and adds some kind of chamfer with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opSubtractChamfer(float f1, float f2, float r) {
)shader_source"
R"shader_source(	return opIntersectChamfer(f1, -f2, r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// unions two objects and adds some kind of smooth chamfer with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opUnionChamferSmooth(float f1, float f2, float r) {
)shader_source"
R"shader_source(	float f_min = min(f1, f2);
)shader_source"
R"shader_source(	//if (f1 < r && f2 < r) {
)shader_source"
R"shader_source(		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
)shader_source"
R"shader_source(		return opUnionSmooth(f_chamfer, f_min, .1 * r);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(	//return f_min;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// intersects two objects and adds some kind of chamfer with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opIntersectChamferSmooth(float f1, float f2, float r) {
)shader_source"
R"shader_source(	float f_max = max(f1, f2);
)shader_source"
R"shader_source(	//if (f1 > -r && f2 > -r) {
)shader_source"
R"shader_source(		float f_chamfer = sqrt(.5) * (f1 + f2 + r);
)shader_source"
R"shader_source(		return opIntersectSmooth(f_chamfer, f_max, .1 * r);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(	//return f_max;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// subtracts f2 from f1 and adds some kind of chamfer with radius r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opSubtractChamferSmooth(float f1, float f2, float r) {
)shader_source"
R"shader_source(	return opIntersectChamferSmooth(f1, -f2, r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// unions two objects and adds n columns within a radius of r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opUnionColumns(float f1, float f2, float r, float n) {
)shader_source"
R"shader_source(	// Speckröllchen
)shader_source"
R"shader_source(	float f_min = min(f1, f2);
)shader_source"
R"shader_source(	if (f1 < 2.*r && f2 < 2.*r) {
)shader_source"
R"shader_source(		vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(		//float radius = sqrt(2.) * .5 * r / (n + sqrt(2.) - 1.);
)shader_source"
R"shader_source(		//q.x -= radius;
)shader_source"
R"shader_source(		float radius = sqrt(.5) * r / (n + sqrt(.5) - 1.);
)shader_source"
R"shader_source(		q.y -= r - radius;
)shader_source"
R"shader_source(		pRot(q, -Tau / 8.);
)shader_source"
R"shader_source(		pDomrepInterval(q.x, 2. * radius, 0., n-1.);
)shader_source"
R"shader_source(		float f_columns = length(q) - radius;
)shader_source"
R"shader_source(		f_columns = min(f_columns, q.y);
)shader_source"
R"shader_source(		return min(f_columns, f_min);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return f_min;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// intersects two objects and adds n columns within a radius of r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opIntersectColumns(float f1, float f2, float r, float n) {
)shader_source"
R"shader_source(	// Speckröllchen
)shader_source"
R"shader_source(	float f_max = max(f1, f2);
)shader_source"
R"shader_source(	if (f1 > -r && f2 > -r) {
)shader_source"
R"shader_source(		vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(		//float radius = sqrt(2.) * .5 * r / (n + sqrt(2.) - 1.);
)shader_source"
R"shader_source(		//q.x -= radius;
)shader_source"
R"shader_source(		float radius = sqrt(.5) * r / (n + sqrt(.5) - 1.);
)shader_source"
R"shader_source(		q.x -= -r;
)shader_source"
R"shader_source(		q.y -= -radius;
)shader_source"
R"shader_source(		pRot(q, -Tau / 8.);
)shader_source"
R"shader_source(		pDomrepInterval(q.x, 2. * radius, 0., n-1.);
)shader_source"
R"shader_source(		float f_columns = length(q) - radius;
)shader_source"
R"shader_source(		f_columns = min(f_columns, q.y);
)shader_source"
R"shader_source(		return max(f_columns, f_max);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return f_max;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// subtracts f2 from f1 and adds n columns within a radius of r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opSubtractColumns(float f1, float f2, float r, float n) {
)shader_source"
R"shader_source(	return opIntersectColumns(f1, -f2, r, n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// unions two objects and adds n stairs within a radius of r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opUnionStairs(float f1, float f2, float r, float n) {
)shader_source"
R"shader_source(	// Speckröllchen
)shader_source"
R"shader_source(	float f_min = min(f1, f2);
)shader_source"
R"shader_source(	//if (f1 < 2.*r && f2 < 2.*r) {
)shader_source"
R"shader_source(		vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(		float radius = r / n * sqrt(.5);
)shader_source"
R"shader_source(		q.y -= r - sqrt(.5) * radius;
)shader_source"
R"shader_source(		q.x -= sqrt(.5) * radius;
)shader_source"
R"shader_source(		pRot(q, -Tau / 8.);
)shader_source"
R"shader_source(		pDomrep(q.x, 2. * radius);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		q.x = abs(q.x);
)shader_source"
R"shader_source(		float offset = radius * sqrt(.5);
)shader_source"
R"shader_source(		float f_columns = dot(q, vec2(sqrt(.5))) - offset;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		f_columns = min(f_columns, q.y);
)shader_source"
R"shader_source(		return min(f_columns, f_min);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(	//return f_min;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// intersects two objects and adds n stairs within a radius of r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opIntersectStairs(float f1, float f2, float r, float n) {
)shader_source"
R"shader_source(	// stairs are symmetric, so we can use opUnionStairs
)shader_source"
R"shader_source(	return -opUnionStairs(-f1, -f2, r, n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// subtracts f2 from f1 and adds n stairs within a radius of r
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opSubtractStairs(float f1, float f2, float r, float n) {
)shader_source"
R"shader_source(	return opIntersectStairs(f1, -f2, r, n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// like normal min()-union but with correct distance at corners
)shader_source"
R"shader_source(// gives correct interior
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opUnionEuclid(float f1, float f2) {
)shader_source"
R"shader_source(	vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(	return max(minV(q), 0.) - length(min(q, 0.));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// like normal max()-intersection but with correct distance at corners
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opIntersectEuclid(float f1, float f2) {
)shader_source"
R"shader_source(	vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(	return min(maxV(q), 0.) + length(max(q, 0.));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// like normal max()-subtraction but with correct distance at corners
)shader_source"
R"shader_source(// use only with orthogonal objects
)shader_source"
R"shader_source(float opSubtractEuclid(float f1, float f2) {
)shader_source"
R"shader_source(	return opIntersectEuclid(f1, -f2);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 4 "material"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(struct MaterialId {
)shader_source"
R"shader_source(	float id;
)shader_source"
R"shader_source(	vec3 coord;
)shader_source"
R"shader_source(	vec4 misc;
)shader_source"
R"shader_source(};
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(struct MatWrap {
)shader_source"
R"shader_source(	float f;
)shader_source"
R"shader_source(	MaterialId m;
)shader_source"
R"shader_source(};
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(struct Material {
)shader_source"
R"shader_source(	vec3 color;
)shader_source"
R"shader_source(	float roughness;
)shader_source"
R"shader_source(	float metallic;
)shader_source"
R"shader_source(	float height;
)shader_source"
R"shader_source(	vec3 emission;
)shader_source"
R"shader_source(};
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(bool calculate_material = false;
)shader_source"
R"shader_source(float current_dist = Inf;
)shader_source"
R"shader_source(MaterialId current_material = MaterialId(0., vec3(0.), vec4(0.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// der witz: jetzt in einmal komplett neu!
)shader_source"
R"shader_source(void mUnion(float f, MaterialId m) {
)shader_source"
R"shader_source(	if (calculate_material) {
)shader_source"
R"shader_source(		if (f < current_dist) {
)shader_source"
R"shader_source(			current_dist = f;
)shader_source"
R"shader_source(			current_material = m;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		current_dist = min(current_dist, f);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void mUnion(MatWrap w) {
)shader_source"
R"shader_source(	mUnion(w.f, w.m);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(MatWrap mUnion(MatWrap w1, MatWrap w2) {
)shader_source"
R"shader_source(	return w1.f < w2.f ? w1 : w2;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(MatWrap mIntersect(MatWrap w1, MatWrap w2) {
)shader_source"
R"shader_source(	return w1.f > w2.f ? w1 : w2;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(MatWrap mSubtract(MatWrap w1, MatWrap w2) {
)shader_source"
R"shader_source(	w2.f = -w2.f;
)shader_source"
R"shader_source(	return mIntersect(w1, w2);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// und hier der andere neue witz
)shader_source"
R"shader_source(void mIntersect(float f, MaterialId m) {
)shader_source"
R"shader_source(	if (calculate_material) {
)shader_source"
R"shader_source(		if (f > current_dist) {
)shader_source"
R"shader_source(			current_dist = f;
)shader_source"
R"shader_source(			current_material = m;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		current_dist = max(current_dist, f);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(MaterialId newMaterialId(float id, vec3 coord) {
)shader_source"
R"shader_source(	return MaterialId(id, coord, vec4(0.));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(Material defaultMaterial(vec3 color) {
)shader_source"
R"shader_source(	return Material(
)shader_source"
R"shader_source(		color,
)shader_source"
R"shader_source(		.5,
)shader_source"
R"shader_source(		0,
)shader_source"
R"shader_source(		0,
)shader_source"
R"shader_source(		vec3(0)
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void mUnionChamfer(float f2, MaterialId m, float r, float id_chamfer) {
)shader_source"
R"shader_source(	//if (calculate_material) {
)shader_source"
R"shader_source(		float f1 = current_dist;
)shader_source"
R"shader_source(		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
)shader_source"
R"shader_source(		mUnion(f2, m);
)shader_source"
R"shader_source(		mUnion(f_chamfer, newMaterialId(id_chamfer, vec3(f1, f2, 0.)));
)shader_source"
R"shader_source(	//} else {
)shader_source"
R"shader_source(		//current_dist = opUnionChamfer(current_dist, f2, r);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void mUnionStairs(float f2, MaterialId m, float r, float n, float id_stairs) {
)shader_source"
R"shader_source(	//if (calculate_material) {
)shader_source"
R"shader_source(		float f1 = current_dist;
)shader_source"
R"shader_source(		//float f_min = min(f1, f2);
)shader_source"
R"shader_source(		//if (f1 < 2.*r && f2 < 2.*r) {
)shader_source"
R"shader_source(			vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(			float radius = r / n * sqrt(.5);
)shader_source"
R"shader_source(			q.y -= r - sqrt(.5) * radius;
)shader_source"
R"shader_source(			q.x -= sqrt(.5) * radius;
)shader_source"
R"shader_source(			pRot(q, -Tau / 8.);
)shader_source"
R"shader_source(			pDomrep(q.x, 2. * radius);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(			q.x = abs(q.x);
)shader_source"
R"shader_source(			float offset = radius * sqrt(.5);
)shader_source"
R"shader_source(			float f_columns = dot(q, vec2(sqrt(.5))) - offset;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(			f_columns = min(f_columns, q.y);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(			mUnion(f2, m);
)shader_source"
R"shader_source(			mUnion(f_columns, newMaterialId(id_stairs, vec3(q, 0.)));
)shader_source"
R"shader_source(			//return min(f_columns, f_min);
)shader_source"
R"shader_source(		//}
)shader_source"
R"shader_source(		//return f_min;
)shader_source"
R"shader_source(	//} else {
)shader_source"
R"shader_source(		//current_dist = opUnionStairs(current_dist, f2, r, n);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void mUnionStairs(MatWrap w, float r, float n, float id_stairs) {
)shader_source"
R"shader_source(	mUnionStairs(w.f, w.m, r, n, id_stairs);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void mUnionSmooth(float f2, MaterialId m, float r, float id_smooth) {
)shader_source"
R"shader_source(	float f1 = current_dist;
)shader_source"
R"shader_source(	if (f1 < r && f2 < r) {
)shader_source"
R"shader_source(		float h = clamp(.5 + .5 * (f2 - f1) / r, 0., 1. );
)shader_source"
R"shader_source(		float f_smooth = mix(f2, f1, h) - r * h * (1. - h);
)shader_source"
R"shader_source(		current_dist = f_smooth;
)shader_source"
R"shader_source(		current_material = newMaterialId(id_smooth, vec3(f1, f2, h));
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	mUnion(f2, m);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(MatWrap mUnionChamfer(MatWrap w1, MatWrap w2, float r, float id_chamfer) {
)shader_source"
R"shader_source(	//if (calculate_material) {
)shader_source"
R"shader_source(		MatWrap w_min = mUnion(w1, w2);
)shader_source"
R"shader_source(		float f_chamfer = sqrt(.5) * (w1.f + w2.f - r);
)shader_source"
R"shader_source(		return mUnion(w_min, MatWrap(f_chamfer, newMaterialId(id_chamfer, vec3(w1.f, w2.f, 0.))));
)shader_source"
R"shader_source(	//} else {
)shader_source"
R"shader_source(		//current_dist = opUnionChamfer(current_dist, f2, r);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(MatWrap mUnionSmooth(MatWrap w1, MatWrap w2, float r, float id_smooth) {
)shader_source"
R"shader_source(	float f1 = w1.f;
)shader_source"
R"shader_source(	float f2 = w2.f;
)shader_source"
R"shader_source(	if (f1 < r && f2 < r) {
)shader_source"
R"shader_source(		float h = clamp(.5 + .5 * (f2 - f1) / r, 0., 1. );
)shader_source"
R"shader_source(		float f_smooth = mix(f2, f1, h) - r * h * (1. - h);
)shader_source"
R"shader_source(		return MatWrap(f_smooth, newMaterialId(id_smooth, vec3(f1, f2, 0.)));
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	MatWrap w_min = mUnion(w1, w2);
)shader_source"
R"shader_source(	return w_min;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(MatWrap mUnionStairs(MatWrap w1, MatWrap w2, float r, float n, float id_stairs) {
)shader_source"
R"shader_source(	//if (calculate_material) {
)shader_source"
R"shader_source(		float f1 = w1.f;
)shader_source"
R"shader_source(		float f2 = w2.f;
)shader_source"
R"shader_source(		//float f_min = min(f1, f2);
)shader_source"
R"shader_source(		//if (f1 < 2.*r && f2 < 2.*r) {
)shader_source"
R"shader_source(			vec2 q = vec2(f1, f2);
)shader_source"
R"shader_source(			float radius = r / n * sqrt(.5);
)shader_source"
R"shader_source(			q.y -= r - sqrt(.5) * radius;
)shader_source"
R"shader_source(			q.x -= sqrt(.5) * radius;
)shader_source"
R"shader_source(			pRot(q, -Tau / 8.);
)shader_source"
R"shader_source(			pDomrep(q.x, 2. * radius);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(			q.x = abs(q.x);
)shader_source"
R"shader_source(			float offset = radius * sqrt(.5);
)shader_source"
R"shader_source(			float f_columns = dot(q, vec2(sqrt(.5))) - offset;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(			f_columns = min(f_columns, q.y);
)shader_source"
R"shader_source(			MatWrap w_columns = MatWrap(f_columns, newMaterialId(id_stairs, vec3(q, 0.)));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(			MatWrap w_min = mUnion(w1, w2);
)shader_source"
R"shader_source(			w_min = mUnion(w_min, w_columns);
)shader_source"
R"shader_source(			return w_min;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(			//mUnion(f2, m);
)shader_source"
R"shader_source(			//mUnion(f_columns, newMaterialId(id_stairs, vec3(q, 0.)));
)shader_source"
R"shader_source(			//return min(f_columns, f_min);
)shader_source"
R"shader_source(		//}
)shader_source"
R"shader_source(		//return f_min;
)shader_source"
R"shader_source(	//} else {
)shader_source"
R"shader_source(		//current_dist = opUnionStairs(current_dist, f2, r, n);
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 5 "sdf/distances"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fSphere(vec3 p, float r) {
)shader_source"
R"shader_source(	return length(p) - r;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2Sphere(vec2 p, float r) {
)shader_source"
R"shader_source(	return length(p) - r;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// capped cylinder, h is half height
)shader_source"
R"shader_source(float fCylinder(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	float sp2 = f2Sphere(p.xz, r);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return opIntersectEuclid(sp2, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// capped cylinder, h is half height
)shader_source"
R"shader_source(float fCylinderEdge(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	float sp2 = f2Sphere(p.xz, r);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return max(sp2, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fCorner(vec3 p) {
)shader_source"
R"shader_source(	return min(maxV(p), 0.) + length(max(p, 0.));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fCornerRounded(vec3 p, float r) {
)shader_source"
R"shader_source(	return fCorner(p + r) - r;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fCornerEdge(vec3 p) {
)shader_source"
R"shader_source(	return maxV(p);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2Corner(vec2 p) {
)shader_source"
R"shader_source(	return min(maxV(p), 0.) + length(max(p, 0.));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fCornerRounded2(vec2 p, float r) {
)shader_source"
R"shader_source(	return f2Corner(p + r) - r;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2CornerEdge(vec2 p) {
)shader_source"
R"shader_source(	return maxV(p);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fBox(vec3 p, vec3 r) {
)shader_source"
R"shader_source(	vec3 q = abs(p) - r;
)shader_source"
R"shader_source(	return min(maxV(q), 0.) + length(max(q, 0.));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fBox(vec3 p, float rx, float ry, float rz) {
)shader_source"
R"shader_source(	return fBox(p, vec3(rx, ry, rz));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fBox(vec3 p, float r) {
)shader_source"
R"shader_source(	return fBox(p, vec3(r));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fBoxRounded(vec3 p, vec3 r, float rCorner) {
)shader_source"
R"shader_source(	return fBox(p, r - rCorner) - rCorner;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fBoxRounded(vec3 p, float r, float rCorner) {
)shader_source"
R"shader_source(	return fBoxRounded(p, vec3(r), rCorner);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fBoxEdge(vec3 p, vec3 r) {
)shader_source"
R"shader_source(	return maxV(abs(p) - r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fBoxEdge(vec3 p, float rx, float ry, float rz) {
)shader_source"
R"shader_source(	return fBoxEdge(p, vec3(rx, ry, rz));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fBoxEdge(vec3 p, float r) {
)shader_source"
R"shader_source(	return fBoxEdge(p, vec3(r));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2Box(vec2 p, vec2 r) {
)shader_source"
R"shader_source(	vec2 q = abs(p) - r;
)shader_source"
R"shader_source(	return min(maxV(q), 0.) + length(max(q, 0.));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2Box(vec2 p, float rx, float ry) {
)shader_source"
R"shader_source(	return f2Box(p, vec2(rx, ry));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2Box(vec2 p, float r) {
)shader_source"
R"shader_source(	return f2Box(p, vec2(r));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2BoxRounded(vec2 p, vec2 r, float rCorner) {
)shader_source"
R"shader_source(	return f2Box(p, r - rCorner) - rCorner;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2BoxRounded(vec2 p, float r, float rCorner) {
)shader_source"
R"shader_source(	return f2BoxRounded(p, vec2(r), rCorner);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2BoxEdge(vec2 p, vec2 r) {
)shader_source"
R"shader_source(	return maxV(abs(p) - r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2BoxEdge(vec2 p, float rx, float ry) {
)shader_source"
R"shader_source(	return f2BoxEdge(p, vec2(rx, ry));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2BoxEdge(vec2 p, float r) {
)shader_source"
R"shader_source(	return f2BoxEdge(p, vec2(r));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fPlane(vec3 p, vec3 n) {
)shader_source"
R"shader_source(	// n must be normalized
)shader_source"
R"shader_source(	return dot(p, n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fPlaneAngle(vec3 p, float phi, float theta) {
)shader_source"
R"shader_source(	return fPlane(p, unitVector(phi, theta));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2Plane(vec2 p, vec2 n) {
)shader_source"
R"shader_source(	// n must be normalized
)shader_source"
R"shader_source(	return dot(p, n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float f2PlaneAngle(vec2 p, float phi) {
)shader_source"
R"shader_source(	return f2Plane(p, unitVector(phi));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// r is the radius from the origin to the vertices
)shader_source"
R"shader_source(float f2Triprism(vec2 p, float r) {
)shader_source"
R"shader_source(	return max(f2PlaneAngle(vec2(abs(p.x), p.y), radians(30)), -p.y) - .5 * r;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// capped triprism, h is half height
)shader_source"
R"shader_source(float fTriprism(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	float tri2 = f2Triprism(p.xz, r);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return opIntersectEuclid(tri2, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// capped triprism, h is half height
)shader_source"
R"shader_source(float fTriprismEdge(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	float tri2 = f2Triprism(p.xz, r);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return max(tri2, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// r is the radius from the origin to the vertices
)shader_source"
R"shader_source(// just like a rotated f2BoxEdge
)shader_source"
R"shader_source(float f2Quadprism(vec2 p, float r) {
)shader_source"
R"shader_source(	float offset = r * sqrt(.5);
)shader_source"
R"shader_source(	vec2 q = abs(p);
)shader_source"
R"shader_source(	float quad = f2Plane(q, vec2(sqrt(.5))) - offset;
)shader_source"
R"shader_source(	return quad;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// r is the radius from the origin to the vertices
)shader_source"
R"shader_source(float f2Pentaprism(vec2 p, float r) {
)shader_source"
R"shader_source(	float phi1 = radians(108. / 2.);
)shader_source"
R"shader_source(	float phi2 = radians(-18.);
)shader_source"
R"shader_source(	float offset = r * cos(Tau / 5. / 2.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 q = vec2(abs(p.x), p.y);
)shader_source"
R"shader_source(	float side1 = f2PlaneAngle(q, phi1);
)shader_source"
R"shader_source(	float side2 = -p.y;
)shader_source"
R"shader_source(	float side3 = f2PlaneAngle(q, phi2);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float pentagon = max3(side1, side2, side3) - offset;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return pentagon;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// capped pentaprism, h is half height
)shader_source"
R"shader_source(float fPentaprism(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	float penta2 = f2Pentaprism(p.xz, r);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return opIntersectEuclid(penta2, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// capped pentaprism, h is half height
)shader_source"
R"shader_source(float fPentaprismEdge(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	float penta2 = f2Pentaprism(p.xz, r);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return max(penta2, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// r is the radius from the origin to the vertices
)shader_source"
R"shader_source(float f2Hexprism(vec2 p, float r) {
)shader_source"
R"shader_source(	float offset = r * cos(Tau / 6. / 2.);
)shader_source"
R"shader_source(    vec2 q = abs(p);
)shader_source"
R"shader_source(	float side1 = f2PlaneAngle(q, radians(30.));
)shader_source"
R"shader_source(	float side2 = q.y;
)shader_source"
R"shader_source(	float hexagon = max(side1, side2) - offset;
)shader_source"
R"shader_source(    return hexagon;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// capped hexprism, h is half height
)shader_source"
R"shader_source(float fHexprism(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	float hex2 = f2Hexprism(p.xz, r);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return opIntersectEuclid(hex2, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// capped hexprism, h is half height
)shader_source"
R"shader_source(float fHexprismEdge(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	float hex2 = f2Hexprism(p.xz, r);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return max(hex2, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// awesome supershapes directly at your hands!
)shader_source"
R"shader_source(// a and b control the total size
)shader_source"
R"shader_source(// m is the number of spikes
)shader_source"
R"shader_source(// n1, n2, n3 control the exact shape
)shader_source"
R"shader_source(// http://paulbourke.net/geometry/supershape/
)shader_source"
R"shader_source(// http://de.wikipedia.org/wiki/Superformel
)shader_source"
R"shader_source(// have fun playing around!
)shader_source"
R"shader_source(float f2Supershape(vec2 p, float a, float b, float m, float n1, float n2, float n3) {
)shader_source"
R"shader_source(	float phi = atan(p.y, p.x);
)shader_source"
R"shader_source(	float d = length(p);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float m4 = m / 4.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float c = cos(m4 * phi);
)shader_source"
R"shader_source(	float s = sin(m4 * phi);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float ca = c / a;
)shader_source"
R"shader_source(	float sb = s / b;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float gc = ca < 0. ? -1. : 1.;
)shader_source"
R"shader_source(	float gs = sb < 0. ? -1. : 1.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float absc = ca * gc;
)shader_source"
R"shader_source(	float abss = sb * gs;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float ab2 = pow(absc, n2);
)shader_source"
R"shader_source(	float ab3 = pow(abss, n3);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//float ab21 = pow(absc, n2 - 1.);
)shader_source"
R"shader_source(	//float ab31 = pow(abss, n3 - 1.);
)shader_source"
R"shader_source(	float ab21 = ab2 / absc;
)shader_source"
R"shader_source(	float ab31 = ab3 / abss;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float rw = ab2 + ab3;
)shader_source"
R"shader_source(	float r = pow(rw, -1./n1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float k = -n2 * ab21 * gc / a * s;
)shader_source"
R"shader_source(	float l =  n3 * ab31 * gs / b * c;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//float drpre = m4 / n1 * pow(rw, -1./n1 - 1.);
)shader_source"
R"shader_source(	float drpre = m4 / n1 * r / rw;
)shader_source"
R"shader_source(	float dr2 = drpre * drpre * (k * k + 2. * k * l + l * l);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f = (d - r) / sqrt(1 + dr2);
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fTorus(vec3 p, float rBig, float rSmall) {
)shader_source"
R"shader_source(	// also try replacing f2Sphere by something like f2Box/f2BoxEdge/f2BoxRounded
)shader_source"
R"shader_source(	vec2 q = vec2(f2Sphere(p.xz, rBig), p.y);
)shader_source"
R"shader_source(	return f2Sphere(q, rSmall);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fTorus(vec3 p, vec2 r) {
)shader_source"
R"shader_source(	return fTorus(p, r.x, r.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fTorusBox(vec3 p, float rBig, float rSmall) {
)shader_source"
R"shader_source(	vec2 q = vec2(f2BoxEdge(p.xz, rBig), p.y);
)shader_source"
R"shader_source(	return f2Box(q, rSmall);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fTorusSphereBox(vec3 p, float rBig, float rSmall) {
)shader_source"
R"shader_source(	vec2 q = vec2(f2Sphere(p.xz, rBig), p.y);
)shader_source"
R"shader_source(	return f2Box(q, rSmall);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fTorusPartial(vec3 p, float rBig, float rSmall, float halfAngle) {
)shader_source"
R"shader_source(	float r = length(p.xz);
)shader_source"
R"shader_source(	float angle = atan(p.z, p.x);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	angle -= clamp(angle, -halfAngle, halfAngle);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p.xz = r * unitVector(angle);
)shader_source"
R"shader_source(	p.x -= rBig;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return fSphere(p, rSmall);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// n is plane normal and must be normalized
)shader_source"
R"shader_source(float fCone(vec3 p, vec2 n) {
)shader_source"
R"shader_source(	vec2 q = vec2(length(p.xz), p.y);
)shader_source"
R"shader_source(	return f2Plane(q, n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fConeAngle(vec3 p, float angle) {
)shader_source"
R"shader_source(	vec2 q = vec2(length(p.xz), p.y);
)shader_source"
R"shader_source(	return f2PlaneAngle(q, angle);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fConeCapped(vec3 p, float rBig, float rSmall, float h) {
)shader_source"
R"shader_source(	float a = rBig - rSmall;
)shader_source"
R"shader_source(	float b = 2 * h;
)shader_source"
R"shader_source(	float side = sqrt(a * a + b * b);
)shader_source"
R"shader_source(	vec2 n = vec2(b/side, a/side);
)shader_source"
R"shader_source(	vec3 q = p;
)shader_source"
R"shader_source(	pTrans(q.y, rSmall * n.x / n.y + h);
)shader_source"
R"shader_source(	float cone = fCone(q, n);
)shader_source"
R"shader_source(	float y = abs(p.y) - h;
)shader_source"
R"shader_source(	return max(cone, y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// line from origin to v, inflated by r
)shader_source"
R"shader_source(float fLine(vec3 p, float r, vec3 v) {
)shader_source"
R"shader_source(	float h = saturate(dot(p, v)/ dot(v, v));
)shader_source"
R"shader_source(	return fSphere(p - v*h, r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// line along x-axis, form -h to h
)shader_source"
R"shader_source(float fLineX(vec3 p, float r, float h) {
)shader_source"
R"shader_source(	p.x -= clamp(p.x, -h, h);
)shader_source"
R"shader_source(	return fSphere(p, r);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// line along x-axis with cones as caps, n is cone normal
)shader_source"
R"shader_source(float fLineCone(vec3 p, float r, float h, vec2 n) {
)shader_source"
R"shader_source(	float a = r * n.x / n.y; // r / tan(phi)
)shader_source"
R"shader_source(	p.x = abs(p.x);
)shader_source"
R"shader_source(	p.x -= min(h, p.x);
)shader_source"
R"shader_source(	p.x -= a;
)shader_source"
R"shader_source(	return fCone(p.yxz, n);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// line along x-axis with cones as caps, angle is cone angle
)shader_source"
R"shader_source(float fLineConeAngle(vec3 p, float r, float h, float angle) {
)shader_source"
R"shader_source(	return fLineCone(p, r, h, unitVector(angle));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// r is the radius from the origin to the vertices
)shader_source"
R"shader_source(float fOctahedron(vec3 p, float r) {
)shader_source"
R"shader_source(	vec3 q = abs(p);
)shader_source"
R"shader_source(	q.y -= r;
)shader_source"
R"shader_source(	//q.y -= .707106781 * s; // sqrt(2) / 2 * s
)shader_source"
R"shader_source(	//float phi = TAU/4. - acos(-1./3.) * .5;
)shader_source"
R"shader_source(	float plane1 = f2Plane(q.xy, vec2(.816496581, .577350269)); // cos(phi), sin(phi)
)shader_source"
R"shader_source(	float plane2 = f2Plane(q.zy, vec2(.816496581, .577350269));
)shader_source"
R"shader_source(	float f_oktaeder = max(plane1, plane2);
)shader_source"
R"shader_source(	return f_oktaeder;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// spiral starting at the orgin
)shader_source"
R"shader_source(// dist is distance between tubes, r is radius of tubes
)shader_source"
R"shader_source(float fSpiral(vec3 p, float dist, float r) {
)shader_source"
R"shader_source(	float radius = length(p.xz);
)shader_source"
R"shader_source(	float angle = atan(p.z, p.x);
)shader_source"
R"shader_source(	radius -= angle * dist / Tau;
)shader_source"
R"shader_source(	vec2 q = vec2(radius, p.y);
)shader_source"
R"shader_source(	pDomrep(q.x, dist);
)shader_source"
R"shader_source(	float d = f2Sphere(q, r);
)shader_source"
R"shader_source(	return d;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// l: half distance of focus points, r: radius
)shader_source"
R"shader_source(float f2Spheroid(vec2 p, float l, float r) {
)shader_source"
R"shader_source(	vec2 q1 = p, q2 = p;
)shader_source"
R"shader_source(	q1.x -= l;
)shader_source"
R"shader_source(	q2.x -= -l;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f = length(q1) + length(q2);
)shader_source"
R"shader_source(	f = f * .5 - r;
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// l: half distance of focus points, r: radius
)shader_source"
R"shader_source(float fSpheroid(vec3 p, float l, float r) {
)shader_source"
R"shader_source(	vec3 q1 = p, q2 = p;
)shader_source"
R"shader_source(	q1.x -= l;
)shader_source"
R"shader_source(	q2.x -= -l;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f = length(q1) + length(q2);
)shader_source"
R"shader_source(	f = f * .5 - r;
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// four sided pyramid, h is height, phi is angle of planes
)shader_source"
R"shader_source(float fPyramid(vec3 p, float h, float phi) {
)shader_source"
R"shader_source(	vec3 q = p;
)shader_source"
R"shader_source(	q.xz = abs(q.xz);
)shader_source"
R"shader_source(	q.y -= h;
)shader_source"
R"shader_source(	float plane1 = f2PlaneAngle(q.xy, phi);
)shader_source"
R"shader_source(	float plane2 = f2PlaneAngle(q.zy, phi);
)shader_source"
R"shader_source(	float f_oktaeder = max(plane1, plane2);
)shader_source"
R"shader_source(	return f_oktaeder;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 4 "lighting"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 0) uniform sampler2D brdf;
)shader_source"
R"shader_source(layout(binding = 1) uniform samplerCube environment;
)shader_source"
R"shader_source(layout(binding = 2) uniform samplerCube filteredDiffuse;
)shader_source"
R"shader_source(layout(binding = 3) uniform samplerCube filteredSpecular;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(struct SphereLight {
)shader_source"
R"shader_source(	vec3 position;
)shader_source"
R"shader_source(	float radius;
)shader_source"
R"shader_source(	vec3 color;
)shader_source"
R"shader_source(	float power;
)shader_source"
R"shader_source(};
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 getSpecularDominantDir(vec3 N, vec3 R, float NdotV, float roughness) {
)shader_source"
R"shader_source(	float smoothness = saturate(1 - roughness);
)shader_source"
R"shader_source(	return normalize(mix(N, R, smoothness * (sqrt(smoothness) + roughness)));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// n: normal
)shader_source"
R"shader_source(// v: vector from hit to camera (for example -dir)
)shader_source"
R"shader_source(// color: base color of object
)shader_source"
R"shader_source(// roughness: between 0 and 1
)shader_source"
R"shader_source(vec3 approximateSpecular(vec3 n, vec3 v, vec3 color, float roughness) {
)shader_source"
R"shader_source(	float NoV = saturate(dot(n, v));
)shader_source"
R"shader_source(	vec3 r = 2. * dot(n, v) * n - v;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dir = getSpecularDominantDir(n, r, NoV, sqrt(roughness));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 prefiltered = textureLod(filteredSpecular, dir, sqrt(roughness) * 5.).rgb;
)shader_source"
R"shader_source(	vec2 envBRDF = textureLod(brdf, vec2(roughness, NoV), 0.).rg;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return prefiltered * (color * envBRDF.x + envBRDF.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// n: normal
)shader_source"
R"shader_source(// v: vector from hit to camera (for example -dir)
)shader_source"
R"shader_source(// color: base color of object
)shader_source"
R"shader_source(// roughness: between 0 and 1
)shader_source"
R"shader_source(// metallic: only 0 or 1
)shader_source"
R"shader_source(vec3 ambientColor(vec3 n, vec3 v, Material mat) {
)shader_source"
R"shader_source(	vec3 diffuse = textureLod(filteredDiffuse, n, 0.).rgb;
)shader_source"
R"shader_source(	vec3 dielectric = mat.color * diffuse
)shader_source"
R"shader_source(		+ approximateSpecular(n, v, vec3(.04), mat.roughness);
)shader_source"
R"shader_source(	vec3 metal = approximateSpecular(n, v, mat.color, mat.roughness);
)shader_source"
R"shader_source(	return mix(dielectric, metal, mat.metallic);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// o: camera origin
)shader_source"
R"shader_source(// d: camera view direction
)shader_source"
R"shader_source(// r: radius of "bounding sphere"
)shader_source"
R"shader_source(vec3 environmentColor(vec3 o, vec3 d, float r) {
)shader_source"
R"shader_source(	// hmmmmm…
)shader_source"
R"shader_source(	o.xz -= camera_position.xz;
)shader_source"
R"shader_source(	float radicand = square(dot(d, o)) - dot(o, o) + r * r;
)shader_source"
R"shader_source(	float t = -dot(d, o) + sqrt(radicand);
)shader_source"
R"shader_source(	return textureLod(environment, normalize(o + t * d), 0.).rgb;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// handy standard applyLights() function at your hands, just copy this into yout applyLights() function
)shader_source"
R"shader_source(/*
)shader_source"
R"shader_source(	return applyNormalLights(origin, marched, direction, hit, normal, material);
)shader_source"
R"shader_source(// */
)shader_source"
R"shader_source(vec3 applyNormalLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
)shader_source"
R"shader_source(	material.color = .5 * normal + .5;
)shader_source"
R"shader_source(	return ambientColor(normal, -direction, material);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float diffuseSphereLight(vec3 hit, vec3 N, SphereLight light) {
)shader_source"
R"shader_source(	vec3 L_unormalized = light.position - hit;
)shader_source"
R"shader_source(	float light_distance2 = dot(L_unormalized, L_unormalized);
)shader_source"
R"shader_source(	vec3 L = L_unormalized * inversesqrt(light_distance2);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float Beta = acos(dot(N, L));
)shader_source"
R"shader_source(	float H = sqrt(light_distance2);
)shader_source"
R"shader_source(	float h = H / light.radius;
)shader_source"
R"shader_source(	float x = sqrt(square(h) - 1);
)shader_source"
R"shader_source(	float y = -x / tan(Beta);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float illuminance = 0;
)shader_source"
R"shader_source(	if (h * cos(Beta) > 1) {
)shader_source"
R"shader_source(		illuminance = cos(Beta) / square(h);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		illuminance = (1. / (Pi * square(h))) *
)shader_source"
R"shader_source(			(cos(Beta) * acos(y) - x * sin(Beta) * sqrt(1 - y * y)) +
)shader_source"
R"shader_source(			(1 / Pi) * atan(sin(Beta) * sqrt(1 - square(y)) / x);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	illuminance *= Pi;
)shader_source"
R"shader_source(	return illuminance;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 specularSphereLightClosestPoint(vec3 hit, vec3 N, vec3 V, float roughness, SphereLight light) {
)shader_source"
R"shader_source(	vec3 R  = reflect(-V, N);
)shader_source"
R"shader_source(	vec3 dominantDir = getSpecularDominantDir(N, R, pdot(N, V), roughness);
)shader_source"
R"shader_source(	vec3 L_unnorm = light.position - hit;
)shader_source"
R"shader_source(	vec3 centerToRay = dot(L_unnorm, dominantDir) * dominantDir - L_unnorm;
)shader_source"
R"shader_source(	vec3 closestPoint = L_unnorm + centerToRay * saturate(light.radius / length(centerToRay));
)shader_source"
R"shader_source(	return closestPoint;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// not actually the delta function, adapted for use in brdfSpecularEpicSmith
)shader_source"
R"shader_source(float deltaGGX(float alpha2, float NdotV) {
)shader_source"
R"shader_source(	float NdotV2 = square(NdotV);
)shader_source"
R"shader_source(	float tan_theta_alpha2 = (1 - NdotV2) / NdotV2 * alpha2;
)shader_source"
R"shader_source(	return sqrt(1 + tan_theta_alpha2);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float brdfSpecularEpicSmithWithoutFresnel(vec3 V, vec3 L, vec3 N, float roughness) {
)shader_source"
R"shader_source(	vec3 H = normalize(V + L);
)shader_source"
R"shader_source(	float NdotL = pdot(N, L);
)shader_source"
R"shader_source(	float NdotV = pdot(N, V);
)shader_source"
R"shader_source(	float NdotH = pdot(N, H);
)shader_source"
R"shader_source(	float alpha = square(roughness);
)shader_source"
R"shader_source(	float alpha2 = square(alpha);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float divisor = max(Pi * square(square(NdotH) * (alpha2 - 1) + 1), 1e-8); // specular D part
)shader_source"
R"shader_source(	divisor *= deltaGGX(alpha2, NdotV) + deltaGGX(alpha2, NdotL); // height-correlated smith masking-shadowing * 2
)shader_source"
R"shader_source(	divisor *= 2 * NdotL * NdotV; // 2 moved to G term
)shader_source"
R"shader_source(	return max(alpha2 / divisor, 0);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 fresnel(vec3 V, vec3 L, vec3 F0) {
)shader_source"
R"shader_source(	vec3 H = normalize(V + L);
)shader_source"
R"shader_source(	float VdotH = pdot(V, H);
)shader_source"
R"shader_source(	return F0 + (1 - F0) * exp2((-5.55473 * VdotH - 6.98316) * VdotH);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 applySphereLight(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, SphereLight light) {
)shader_source"
R"shader_source(	// diffuse part
)shader_source"
R"shader_source(	vec3 L = light.position - hit;
)shader_source"
R"shader_source(	float lightDistance2 = dot(L, L);
)shader_source"
R"shader_source(	L /= sqrt(lightDistance2);
)shader_source"
R"shader_source(	float NoL = pdot(L, normal);
)shader_source"
R"shader_source(	float incomingLight = light.power / (4 * square(Pi) * square(light.radius));
)shader_source"
R"shader_source(	incomingLight *= diffuseSphereLight(hit, normal, light);
)shader_source"
R"shader_source(	vec3 diffuse = material.color * (max(0, NoL * incomingLight) / Pi);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// specular part
)shader_source"
R"shader_source(	vec3 closestPoint = specularSphereLightClosestPoint(hit, normal, -direction, material.roughness, light);
)shader_source"
R"shader_source(	float roughnessAlpha = saturate(material.roughness + light.radius / (2 * sqrt(lightDistance2)));
)shader_source"
R"shader_source(	float closestPointDistance2 = dot(closestPoint, closestPoint);
)shader_source"
R"shader_source(	closestPoint /= sqrt(closestPointDistance2);
)shader_source"
R"shader_source(	float normalization = square(material.roughness / roughnessAlpha);
)shader_source"
R"shader_source(	float incomingLightSpecular = light.power * normalization / (4 * Pi * closestPointDistance2);
)shader_source"
R"shader_source(	vec3 F_metal = fresnel(-direction, closestPoint, material.color);
)shader_source"
R"shader_source(	float specular = incomingLightSpecular * NoL * brdfSpecularEpicSmithWithoutFresnel(-direction, closestPoint, normal, material.roughness);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 F_dielectric = fresnel(-direction, closestPoint, vec3(.04));
)shader_source"
R"shader_source(	return mix(diffuse + F_dielectric * specular, specular * F_metal, material.metallic);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 11 "march"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/// marchingloopkram.glsl
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 131) uniform float main_normal_epsilon_bias;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 132) uniform float debug_mode;
)shader_source"
R"shader_source(layout(location = 133) uniform vec3 debug_plane_normal;
)shader_source"
R"shader_source(layout(location = 134) uniform float debug_plane_height;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(const float debug_plane_material_id = 423.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(bool debug_default_pass_scene_visible = true;
)shader_source"
R"shader_source(bool debug_default_pass_plane_visible = false;
)shader_source"
R"shader_source(bool debug_isoline_pass_scene_visible = false;
)shader_source"
R"shader_source(bool debug_isoline_pass_plane_visible = false;
)shader_source"
R"shader_source(bool debug_gradient_visualization = false;
)shader_source"
R"shader_source(bool debug_gradient_pass_scene_visible = false;
)shader_source"
R"shader_source(bool debug_gradient_pass_plane_visible = false;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(bool scene_visible = debug_default_pass_scene_visible;
)shader_source"
R"shader_source(bool debug_plane_visible = debug_default_pass_plane_visible;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fScene(vec3 p);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fDebugPlane(vec3 p) {
)shader_source"
R"shader_source(	return abs(fPlane(p, normalize(debug_plane_normal)) - debug_plane_height);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fMain(vec3 p, bool calc_m) {
)shader_source"
R"shader_source(	current_dist = Inf;
)shader_source"
R"shader_source(	calculate_material = calc_m;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (debug_plane_visible) {
)shader_source"
R"shader_source(		mUnion(fDebugPlane(p), newMaterialId(debug_plane_material_id, p));
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (scene_visible) {
)shader_source"
R"shader_source(		fScene(p);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return current_dist;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 sdfNormalRaw(vec3 p, float e) {
)shader_source"
R"shader_source(	// writing the gradient this way, causes the compiler not to inline f six times
)shader_source"
R"shader_source(	// thanks to mercury, stupid compilers
)shader_source"
R"shader_source(	vec3 s[6] = vec3[6](vec3(e,0,0), vec3(0,e,0), vec3(0,0,e), vec3(-e,0,0), vec3(0,-e,0), vec3(0,0,-e));
)shader_source"
R"shader_source(	float d[6] = float[6](0,0,0,0,0,0);
)shader_source"
R"shader_source(	for(int i = 0; i < 6; i++) {
)shader_source"
R"shader_source(		d[i] = fMain(p+s[i], false);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return vec3(d[0]-d[3], d[1]-d[4], d[2]-d[5]);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 sdfNormal(vec3 p, float epsilon) {
)shader_source"
R"shader_source(	return normalize(sdfNormalRaw(p, epsilon));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 sdfNormal(vec3 p) {
)shader_source"
R"shader_source(	float epsilon = max(.00005, camGetPixelSize(distance(p, camera_position)));
)shader_source"
R"shader_source(	epsilon *= main_normal_epsilon_bias;
)shader_source"
R"shader_source(	return sdfNormal(p, epsilon);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 sdfNormalForeward(vec3 p, vec3 direction) {
)shader_source"
R"shader_source(	vec3 normal = sdfNormal(p);
)shader_source"
R"shader_source(	// try eliminating backfacing normals
)shader_source"
R"shader_source(	if (dot(direction, normal) > 0.) {
)shader_source"
R"shader_source(		normal = normalize(dFdx(normal) + normal);
)shader_source"
R"shader_source(		if (dot(direction, normal) > 0.) {
)shader_source"
R"shader_source(			normal = normalize(dFdy(normal) + normal);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return normal;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 sdfGradient(vec3 p, float epsilon) {
)shader_source"
R"shader_source(	return sdfNormalRaw(p, epsilon) / (2. * epsilon);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 sdfGradient(vec3 p) {
)shader_source"
R"shader_source(	float epsilon = max(.00005, camGetPixelSize(distance(p, camera_position)));
)shader_source"
R"shader_source(	epsilon *= main_normal_epsilon_bias;
)shader_source"
R"shader_source(	return sdfGradient(p, epsilon);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// ein fachmenschich kopierter marchingloop
)shader_source"
R"shader_source(float sdfMarchAdvanced(vec3 o, vec3 d, float t_min, float t_max, float pixelRadius, int max_iterations, float omega, bool forceHit) {
)shader_source"
R"shader_source(	// o, d : ray origin, direction (normalized)
)shader_source"
R"shader_source(	// t_min, t_max: minimum, maximum t values
)shader_source"
R"shader_source(	// pixelRadius: radius of a pixel at t = 1
)shader_source"
R"shader_source(	// forceHit: boolean enforcing to use the
)shader_source"
R"shader_source(	//           candidate_t value as result
)shader_source"
R"shader_source(	float t = t_min;
)shader_source"
R"shader_source(	float candidate_error = Real_Inf;
)shader_source"
R"shader_source(	float candidate_t = t_min;
)shader_source"
R"shader_source(	float previousRadius = 0;
)shader_source"
R"shader_source(	float stepLength = 0;
)shader_source"
R"shader_source(	float functionSign = sgn(fMain(o, false));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	for (int i = 0; i < max_iterations; ++i) {
)shader_source"
R"shader_source(		float signedRadius = functionSign * fMain(d*t + o, false);
)shader_source"
R"shader_source(		float radius = abs(signedRadius);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		bool sorFail = omega > 1 && (radius + previousRadius) < stepLength;
)shader_source"
R"shader_source(		if (sorFail) {
)shader_source"
R"shader_source(			stepLength -= omega * stepLength;
)shader_source"
R"shader_source(			omega = 1;
)shader_source"
R"shader_source(		} else {
)shader_source"
R"shader_source(			stepLength = signedRadius * omega;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		previousRadius = radius;
)shader_source"
R"shader_source(		float error = radius / t;
)shader_source"
R"shader_source(		if (!sorFail && error < candidate_error) {
)shader_source"
R"shader_source(			candidate_t = t;
)shader_source"
R"shader_source(			candidate_error = error;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (!sorFail && error < pixelRadius || t > t_max) {
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		t += stepLength;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if ((t > t_max || candidate_error > pixelRadius) && !forceHit) {
)shader_source"
R"shader_source(		return Real_Inf;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return candidate_t;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float sdfMarch(vec3 o, vec3 d, float t_max) {
)shader_source"
R"shader_source(	float marched = sdfMarchAdvanced(o, d, .001, t_max, camGetPixelSize(1), 256, 1.2, false);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (isinf(marched)) {
)shader_source"
R"shader_source(		return marched;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// discontinuity reduction
)shader_source"
R"shader_source(	for (int i = 0; i < 3; i++) {
)shader_source"
R"shader_source(		marched += fMain(o + marched * d, false) - camGetPixelSize(marched);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return marched;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void setDebugParameters() {
)shader_source"
R"shader_source(	int mode = int(debug_mode);
)shader_source"
R"shader_source(	switch (mode) {
)shader_source"
R"shader_source(		case 0: // default
)shader_source"
R"shader_source(			debug_default_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_default_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_isoline_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_isoline_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_gradient_visualization = false;
)shader_source"
R"shader_source(			debug_gradient_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_gradient_pass_plane_visible = false;
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(		case 1: // debug plane
)shader_source"
R"shader_source(			debug_default_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_default_pass_plane_visible = true;
)shader_source"
R"shader_source(			debug_isoline_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_isoline_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_gradient_visualization = false;
)shader_source"
R"shader_source(			debug_gradient_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_gradient_pass_plane_visible = false;
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(		case 2: // debug plane without scene geometry
)shader_source"
R"shader_source(			debug_default_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_default_pass_plane_visible = true;
)shader_source"
R"shader_source(			debug_isoline_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_isoline_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_gradient_visualization = false;
)shader_source"
R"shader_source(			debug_gradient_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_gradient_pass_plane_visible = false;
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(		case 3: // visualize gradient length
)shader_source"
R"shader_source(			debug_default_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_default_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_isoline_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_isoline_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_gradient_visualization = true;
)shader_source"
R"shader_source(			debug_gradient_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_gradient_pass_plane_visible = false;
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(		case 4: // visualize gradient length with debug plane
)shader_source"
R"shader_source(			debug_default_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_default_pass_plane_visible = true;
)shader_source"
R"shader_source(			debug_isoline_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_isoline_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_gradient_visualization = true;
)shader_source"
R"shader_source(			debug_gradient_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_gradient_pass_plane_visible = false;
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(		case 5: // visualize gradient length with debug plane and without geometry
)shader_source"
R"shader_source(			debug_default_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_default_pass_plane_visible = true;
)shader_source"
R"shader_source(			debug_isoline_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_isoline_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_gradient_visualization = true;
)shader_source"
R"shader_source(			debug_gradient_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_gradient_pass_plane_visible = false;
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(		default: // same as default
)shader_source"
R"shader_source(			debug_default_pass_scene_visible = true;
)shader_source"
R"shader_source(			debug_default_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_isoline_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_isoline_pass_plane_visible = false;
)shader_source"
R"shader_source(			debug_gradient_visualization = false;
)shader_source"
R"shader_source(			debug_gradient_pass_scene_visible = false;
)shader_source"
R"shader_source(			debug_gradient_pass_plane_visible = false;
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	scene_visible = debug_default_pass_scene_visible;
)shader_source"
R"shader_source(	debug_plane_visible = debug_default_pass_plane_visible;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(////// beleuchtungskram.glsl
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 debugIsolineTexture(float sdf_dist, vec3 camera_pos, float camera_dist) {
)shader_source"
R"shader_source(	float small_lines = abs(sin(Pi * 10. * sdf_dist));
)shader_source"
R"shader_source(	small_lines = 1. - (1. - smoothstep(8., 15., camera_dist)) * (1. - small_lines);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float medium_lines = abs(sin(Pi * 1. * sdf_dist));
)shader_source"
R"shader_source(	medium_lines = 1. - (.8 + .2 * smoothstep(6., 10., camera_dist)) * (1. - smoothstep(60., 80., camera_dist)) * (1. - medium_lines);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float big_lines = abs(sin(Pi * 1./10. * sdf_dist));
)shader_source"
R"shader_source(	big_lines = 1. - (.8 + .2 * smoothstep(30., 50., camera_dist)) * (1. - smoothstep(80., 150., camera_dist)) * (1. - big_lines);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float height = fDebugPlane(camera_pos);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 lines_color = vec3(0.);
)shader_source"
R"shader_source(	vec3 near_color = vec3(0.29804, 0.18824, 0.43922);
)shader_source"
R"shader_source(	vec3 far_color = vec3(0.12549, 0.52941, 0.36078);
)shader_source"
R"shader_source(	vec3 inner_color = vec3(0.02095, 0.19046, 0.60548);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 base_color = mix(near_color, far_color, smoothstep(.1 * height, height, sdf_dist));
)shader_source"
R"shader_source(	if (sdf_dist < 0.) {
)shader_source"
R"shader_source(		base_color = inner_color;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	base_color = rgb2hsv(base_color);
)shader_source"
R"shader_source(	base_color.y *= 1. - smoothstep(height, 10. * height, abs(sdf_dist)); // desaturate
)shader_source"
R"shader_source(	base_color = hsv2rgb(base_color);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	base_color = mix(lines_color, base_color, small_lines);
)shader_source"
R"shader_source(	base_color = mix(lines_color, base_color, medium_lines);
)shader_source"
R"shader_source(	base_color = mix(lines_color, base_color, big_lines);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return base_color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 debugIsolineTextureFiltered(vec3 p, vec3 camera_pos, float camera_dist) {
)shader_source"
R"shader_source(	scene_visible = debug_isoline_pass_scene_visible;
)shader_source"
R"shader_source(	debug_plane_visible = debug_isoline_pass_plane_visible;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float sdf_dist = fMain(p, false);
)shader_source"
R"shader_source(	vec3 sdf_normal = sdfNormal(p);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	scene_visible = debug_default_pass_scene_visible;
)shader_source"
R"shader_source(	debug_plane_visible = debug_default_pass_plane_visible;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 pX = dFdx(p);
)shader_source"
R"shader_source(	vec3 pY = dFdy(p);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	/*
)shader_source"
R"shader_source(	float detail = 100.;
)shader_source"
R"shader_source(	int MaxSamples = 10;
)shader_source"
R"shader_source(	int sx = 1 + clamp( int( detail*length(pX) ), 0, MaxSamples-1 );
)shader_source"
R"shader_source(	int sy = 1 + clamp( int( detail*length(pY) ), 0, MaxSamples-1 );
)shader_source"
R"shader_source(	// */
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//*
)shader_source"
R"shader_source(	// fuck it - just supersample everything!
)shader_source"
R"shader_source(	int sx = 5;
)shader_source"
R"shader_source(	int sy = 5;
)shader_source"
R"shader_source(	// */
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 no = vec3(0);
)shader_source"
R"shader_source(	for(int j = 0; j < sy; j++ ) {
)shader_source"
R"shader_source(		for(int i = 0; i < sx; i++ ) {
)shader_source"
R"shader_source(			vec2 st = (vec2(i, j) + .5)/vec2(sx, sy) - .5;
)shader_source"
R"shader_source(			vec3 delta = st.x * pX + st.y * pY;
)shader_source"
R"shader_source(			float f_dist = sdf_dist + dot(sdf_normal, delta);
)shader_source"
R"shader_source(			no += debugIsolineTexture(f_dist, camera_pos, camera_dist);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return no / float(sx*sy);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 debugColorIsolines(vec3 origin, float marched, vec3 hit) {
)shader_source"
R"shader_source(	return debugIsolineTextureFiltered(hit, origin, marched);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 debugColorGradient(vec3 p) {
)shader_source"
R"shader_source(	scene_visible = debug_gradient_pass_scene_visible;
)shader_source"
R"shader_source(	debug_plane_visible = debug_gradient_pass_plane_visible;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 gradient = sdfGradient(p);
)shader_source"
R"shader_source(	float len_grad = length(gradient);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	scene_visible = debug_default_pass_scene_visible;
)shader_source"
R"shader_source(	debug_plane_visible = debug_default_pass_plane_visible;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 under_color = vec3(0.18014, 0.74453, 0.03288);
)shader_source"
R"shader_source(	vec3 over_color = vec3(0.71547, 0.03995, 0.02537);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 base_color = vec3(1.);
)shader_source"
R"shader_source(	base_color = mix(base_color, under_color, 1. - smoothstep(.8, 1., len_grad));
)shader_source"
R"shader_source(	base_color = mix(base_color, over_color, smoothstep(1., 1.2, len_grad));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return base_color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(Material getMaterial(MaterialId materialId);
)shader_source"
R"shader_source(vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material);
)shader_source"
R"shader_source(vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 135) uniform float main_marching_distance;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// Bump Mapping Unparametrized Surfaces on the GPU [Mikkelsen2010]
)shader_source"
R"shader_source(void perturbNormal(vec3 p, inout vec3 n, float height) {
)shader_source"
R"shader_source(	vec3 sigma_s = dFdx(p);
)shader_source"
R"shader_source(	vec3 sigma_t = dFdy(p);
)shader_source"
R"shader_source(	vec3 a = cross(sigma_t, n);
)shader_source"
R"shader_source(	vec3 b = cross(n, sigma_s);
)shader_source"
R"shader_source(	float determinant = dot(sigma_s, a);
)shader_source"
R"shader_source(	float d_beta_s = dFdx(height);
)shader_source"
R"shader_source(	float d_beta_t = dFdy(height);
)shader_source"
R"shader_source(	vec3 surface_gradient = (d_beta_s * a + d_beta_t * b) * sign(determinant);
)shader_source"
R"shader_source(	vec3 n_perturbed = normalize(abs(determinant) * n - surface_gradient);
)shader_source"
R"shader_source(	n = n_perturbed;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	setDebugParameters();
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 origin = camera_position;
)shader_source"
R"shader_source(	vec3 direction = camGetDirection();
)shader_source"
R"shader_source(	float marched = sdfMarch(origin, direction, main_marching_distance);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (isinf(marched)) {
)shader_source"
R"shader_source(		out_color = environmentColor(origin, direction, main_marching_distance);
)shader_source"
R"shader_source(		out_depth = main_marching_distance;
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		vec3 hit = origin + marched * direction;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float marching_error = fMain(hit, true);
)shader_source"
R"shader_source(		MaterialId materialId = current_material;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		vec3 normal = sdfNormalForeward(hit, direction);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (materialId.id == debug_plane_material_id) {
)shader_source"
R"shader_source(			vec3 c_isoline = debugColorIsolines(origin, marched, hit);
)shader_source"
R"shader_source(			if (debug_gradient_visualization) {
)shader_source"
R"shader_source(				vec3 c_gradient = debugColorGradient(hit);
)shader_source"
R"shader_source(				c_isoline = mix(c_isoline, c_gradient, .5);
)shader_source"
R"shader_source(			}
)shader_source"
R"shader_source(			Material m_isoline = defaultMaterial(c_isoline);
)shader_source"
R"shader_source(			m_isoline.roughness = 1.;
)shader_source"
R"shader_source(			out_color = ambientColor(normal, -direction, m_isoline);
)shader_source"
R"shader_source(		} else {
)shader_source"
R"shader_source(			if (debug_gradient_visualization) {
)shader_source"
R"shader_source(				vec3 c_gradient = debugColorGradient(hit);
)shader_source"
R"shader_source(				Material m_gradient = defaultMaterial(c_gradient);
)shader_source"
R"shader_source(				m_gradient.roughness = 0.;
)shader_source"
R"shader_source(				out_color = ambientColor(normal, -direction, m_gradient);
)shader_source"
R"shader_source(			} else {
)shader_source"
R"shader_source(				Material material = getMaterial(materialId);
)shader_source"
R"shader_source(				// TODO move this somewhere else
)shader_source"
R"shader_source(				// should be done for debug materials too to avoid false differentials
)shader_source"
R"shader_source(				//perturbNormal(hit, normal, material.height);
)shader_source"
R"shader_source(				material.roughness = square(material.roughness);
)shader_source"
R"shader_source(				out_color = applyLights(origin, marched, direction, hit, normal, materialId, material);
)shader_source"
R"shader_source(			}
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(		out_depth = marched;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	out_color = applyAfterEffects(origin, marched, direction, out_color);
)shader_source"
R"shader_source(	out_color *= exp2(camera_exposure_rt_float);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 4
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 136) uniform float glum_anim;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(const float id_tunnel = 0.;
)shader_source"
R"shader_source(const float id_strebe = 1.;
)shader_source"
R"shader_source(const float id_strebe_stairs = 2.;
)shader_source"
R"shader_source(const float id_signal = 3.;
)shader_source"
R"shader_source(const float id_spikes = 4.;
)shader_source"
R"shader_source(const float id_spikes_top = 5.;
)shader_source"
R"shader_source(const float id_blades = 6.;
)shader_source"
R"shader_source(const float id_blades_chamfer = 7.;
)shader_source"
R"shader_source(const float id_floor = 8.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fSpimiDisc(vec3 p, float scale, float i, float discAtan) {
)shader_source"
R"shader_source(	float innerRadius = glum_spimi_radius_rt_float + sin(i) * glum_spimi_radius_var_rt_float;
)shader_source"
R"shader_source(	innerRadius *= scale;
)shader_source"
R"shader_source(	// TODO parameterize
)shader_source"
R"shader_source(	float rotationAngle = Tau / 10 * i / (5 + 3.5 * sin(time));
)shader_source"
R"shader_source(	pRotY(p, rotationAngle);
)shader_source"
R"shader_source(	pDomrepAngleWithAtan(p.xz, glum_spimi_n_spikes_rt_float, innerRadius, rotationAngle + discAtan);
)shader_source"
R"shader_source(	float disc_height = glum_spimi_disc_height_rt_float * scale;
)shader_source"
R"shader_source(	float d = fBox(p, vec3(disc_height, disc_height, scale));
)shader_source"
R"shader_source(	pTrans(p.x, disc_height + glum_spimi_spike_length_rt_float * scale);
)shader_source"
R"shader_source(	float d2 = fConeCapped(p.zxy, disc_height - .05 * scale, disc_height * glum_spimi_spike_cap_rt_float, glum_spimi_spike_length_rt_float * scale);
)shader_source"
R"shader_source(	return opUnionChamfer(d, d2, .05 * scale);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fSpimi(vec3 p, float scale) {
)shader_source"
R"shader_source(	float maxSpimiRadius = glum_spimi_radius_rt_float + glum_spimi_radius_var_rt_float;
)shader_source"
R"shader_source(	maxSpimiRadius += glum_spimi_disc_height_rt_float + glum_spimi_spike_length_rt_float * 2;
)shader_source"
R"shader_source(	maxSpimiRadius *= scale;
)shader_source"
R"shader_source(	float f_bounding = max(f2Sphere(p.xz, maxSpimiRadius), -p.y);
)shader_source"
R"shader_source(	if (f_bounding > 1) {
)shader_source"
R"shader_source(		return f_bounding;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	float c = 2 * glum_spimi_disc_height_rt_float * scale;
)shader_source"
R"shader_source(	float i = pDomrepSingle(p.y, c);
)shader_source"
R"shader_source(	//float i = 0;
)shader_source"
R"shader_source(	float discAtan = atan(p.z, p.x);
)shader_source"
R"shader_source(	// TODO use simpler objects as guards
)shader_source"
R"shader_source(	pTrans(p.y, c);
)shader_source"
R"shader_source(	float d1 = fSpimiDisc(p, scale, i + 1, discAtan);
)shader_source"
R"shader_source(	pTrans(p.y, -c);
)shader_source"
R"shader_source(	float d2 = fSpimiDisc(p, scale, i, discAtan);
)shader_source"
R"shader_source(	pTrans(p.y, -c);
)shader_source"
R"shader_source(	float d3 = fSpimiDisc(p, scale, i - 1, discAtan);
)shader_source"
R"shader_source(	return min(d1, min(d2, d3));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fTunnel(vec3 p) {
)shader_source"
R"shader_source(	vec3 p_tunnel = p;
)shader_source"
R"shader_source(	pMirrorLoco(p_tunnel.xy, vec2(7));
)shader_source"
R"shader_source(	pFlip(p_tunnel.x);
)shader_source"
R"shader_source(	pMirror(p_tunnel.y);
)shader_source"
R"shader_source(	pRotZ(p_tunnel, .1 * Tau);
)shader_source"
R"shader_source(	float cell_tunnel = pDomrepMirror(p_tunnel.z, 3);
)shader_source"
R"shader_source(	float tunnel_pulse = glum_tunnel_pulse_rt_float;
)shader_source"
R"shader_source(	tunnel_pulse += glum_tunnel_pulse_amp_rt_float * sin(Tau * glum_anim);
)shader_source"
R"shader_source(	pRotY(p_tunnel, tunnel_pulse * Tau);
)shader_source"
R"shader_source(	pDomrepMirror(p_tunnel.y, 3);
)shader_source"
R"shader_source(	pRotZ(p_tunnel, tunnel_pulse * Tau);
)shader_source"
R"shader_source(	pMirrorTrans(p_tunnel.x, .01);
)shader_source"
R"shader_source(	float f = p_tunnel.x;
)shader_source"
R"shader_source(	pTrans(p_tunnel.x, -.3);
)shader_source"
R"shader_source(	float f2 = fPlane(p_tunnel, unitVector(-.125 * Tau, .2 * Pi).yxz);
)shader_source"
R"shader_source(	f = opUnionStairs(f, f2, .6, 4);
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fFloor(vec3 p, float height) {
)shader_source"
R"shader_source(	vec3 p_floor = p;
)shader_source"
R"shader_source(	float f_floor = f2BoxEdge(p_floor.xy, vec2(3, height));
)shader_source"
R"shader_source(	vec3 p_floor_stuetze = p_floor;
)shader_source"
R"shader_source(	pTrans(p_floor_stuetze.y, -4);
)shader_source"
R"shader_source(	float f_floor_stuetze = f2BoxEdge(p_floor_stuetze.xy, vec2(.3, 4));
)shader_source"
R"shader_source(	f_floor = min(f_floor, f_floor_stuetze);
)shader_source"
R"shader_source(	return f_floor;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fStreben(vec3 p, float atan_value) {
)shader_source"
R"shader_source(	vec3 p_blades = p;
)shader_source"
R"shader_source(	pDomrepAngleWithAtan(p_blades.xz, 10, .0, atan_value);
)shader_source"
R"shader_source(	float f_blades = f2BoxEdge(p_blades.zy, .1);
)shader_source"
R"shader_source(	return f_blades;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fScene(vec3 p) {
)shader_source"
R"shader_source(	vec3 cell_mirror_total = pMirrorLoco(p.xz, vec2(glum_total_loco_trans_rt_float));
)shader_source"
R"shader_source(	pRotY(p, Tau * glum_total_loco_rot_rt_float);
)shader_source"
R"shader_source(	//pRotZ(p, glum_total_rot_rt_float * p.z);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// domrapping and shit
)shader_source"
R"shader_source(	vec3 p_domrep_cell = p;
)shader_source"
R"shader_source(	float floor_height = .3;
)shader_source"
R"shader_source(	p_domrep_cell.y -= floor_height;
)shader_source"
R"shader_source(	p_domrep_cell.z -= glum_tunnel_offset_rt_float;
)shader_source"
R"shader_source(	float i_domrep = pDomrepMirror(p_domrep_cell.z, 2);
)shader_source"
R"shader_source(	float i_mirror = pMirrorTrans(p_domrep_cell.x, 2);
)shader_source"
R"shader_source(	float atan_value = atan(p_domrep_cell.z, p_domrep_cell.x);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// floor
)shader_source"
R"shader_source(	vec3 p_floor = p;
)shader_source"
R"shader_source(	float f_floor = fFloor(p_floor, floor_height);
)shader_source"
R"shader_source(	mUnion(f_floor, MaterialId(id_floor, p_floor, vec4(cell_mirror_total, 0.)));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// strebe
)shader_source"
R"shader_source(	vec3 p_strebe = p_domrep_cell;
)shader_source"
R"shader_source(	float f_strebe = fStreben(p_strebe, atan_value);
)shader_source"
R"shader_source(	mUnionStairs(f_strebe, newMaterialId(id_strebe, p_strebe), .1, 4, id_strebe_stairs);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// tunnel
)shader_source"
R"shader_source(	float f_tunnel = fTunnel(p);
)shader_source"
R"shader_source(	mUnion(f_tunnel, MaterialId(id_tunnel, p, vec4(cell_mirror_total, 0.)));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// signal
)shader_source"
R"shader_source(	vec3 p_signal = p_domrep_cell;
)shader_source"
R"shader_source(	float hash_dom = rand(int(i_domrep * i_mirror));
)shader_source"
R"shader_source(	float anim_n = 1.;
)shader_source"
R"shader_source(	float t_anim = fract(hash_dom + glum_anim / anim_n * (1. + floor(glum_signal_max_velo_rt_float * anim_n * hash_dom)));
)shader_source"
R"shader_source(	float t_pos = 1. - smoothstep(0., .9, t_anim);
)shader_source"
R"shader_source(	float start_pos = glum_signal_start_rt_float + glum_signal_start_outer_rt_float * step(.5, mod(i_domrep + 1., 3.));
)shader_source"
R"shader_source(	p_signal.x -= glum_signal_end_rt_float + start_pos * t_pos;
)shader_source"
R"shader_source(	float t_radius = 1. - smoothstep(.9, 1., t_anim);
)shader_source"
R"shader_source(	float f_signal = fBox(p_signal, glum_signal_dim_rt_float * t_radius);
)shader_source"
R"shader_source(	MaterialId mat_signal = newMaterialId(id_signal, p_signal);
)shader_source"
R"shader_source(	mat_signal.misc.xy = vec2(t_anim, hash_dom);
)shader_source"
R"shader_source(	mUnion(f_signal, mat_signal);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// spikes base
)shader_source"
R"shader_source(	vec3 p_spikes = p_domrep_cell;
)shader_source"
R"shader_source(	pTrans(p_spikes.y, .5);
)shader_source"
R"shader_source(	float f_spikes = fConeCapped(p_spikes, .4, .3, .5);
)shader_source"
R"shader_source(	float f_spikes_cut = f2Sphere(p_spikes.xz, glum_spikes_cut_r_rt_float);
)shader_source"
R"shader_source(	f_spikes = max(f_spikes, -f_spikes_cut);
)shader_source"
R"shader_source(	mUnion(f_spikes, newMaterialId(id_spikes, p_spikes));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// spikes top
)shader_source"
R"shader_source(	pTrans(p_spikes.y, 1 + 1.5 + sin(Tau * (.1 * (i_domrep + 2. * i_mirror) + glum_anim)) * .5 - 2);
)shader_source"
R"shader_source(	float f_spikes_top = fConeCapped(p_spikes, .3, .02, 1.5);
)shader_source"
R"shader_source(	f_spikes = min(f_spikes, f_spikes_top);
)shader_source"
R"shader_source(	mUnion(f_spikes_top, newMaterialId(id_spikes_top, p_spikes));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// blades (flower ring)
)shader_source"
R"shader_source(	vec3 p_blades = p_domrep_cell;
)shader_source"
R"shader_source(	pTrans(p_blades.y, .5);
)shader_source"
R"shader_source(	//pTrans(p_blades.y, .2 * sin(glum_anim * Tau * .25));
)shader_source"
R"shader_source(	float blade_side = pMirrorTrans(p_blades.y, .1);
)shader_source"
R"shader_source(	float blade_rotation = .1 * Tau * glum_anim * blade_side;
)shader_source"
R"shader_source(	pDomrepAngleWithAtan(p_blades.xz, 10, .0, atan_value + blade_rotation);
)shader_source"
R"shader_source(	pTrans(p_blades.x, .3);
)shader_source"
R"shader_source(	pRotZ(p_blades, .3 * sin(Tau * glum_anim));
)shader_source"
R"shader_source(	float f_blades = f2Triprism(p_blades.zx, .2);
)shader_source"
R"shader_source(	f_blades = max(f_blades, abs(p_blades.y) - .03);
)shader_source"
R"shader_source(	mUnionChamfer(f_blades, newMaterialId(id_blades, p_blades), .01, id_blades_chamfer);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	/*
)shader_source"
R"shader_source(	float f = f_tunnel;
)shader_source"
R"shader_source(	f = min(f, f_spikes);
)shader_source"
R"shader_source(	f_strebe = min(f_strebe, f_signal);
)shader_source"
R"shader_source(	f_floor = opUnionStairs(f_floor, f_strebe, .1, 4);
)shader_source"
R"shader_source(	f = min(f, f_floor);
)shader_source"
R"shader_source(	f = opUnionChamfer(f, f_blades, .01);
)shader_source"
R"shader_source(	// */
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//mUnion(f, newMaterialId(0., p));
)shader_source"
R"shader_source(	return current_dist;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
)shader_source"
R"shader_source(	vec3 emission = material.emission;
)shader_source"
R"shader_source(	return ambientColor(normal, -direction, material) + emission;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
)shader_source"
R"shader_source(	return color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(Material getMaterial(MaterialId materialId) {
)shader_source"
R"shader_source(	Material mat = defaultMaterial(vec3(1));
)shader_source"
R"shader_source(	mat.roughness = .1;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 adpating_is_hard = vec3(63,124,126)/255.;
)shader_source"
R"shader_source(	vec3 murky = vec3(41,40,57)/255.;
)shader_source"
R"shader_source(	vec3 hex = vec3(157,29,8)/255.;
)shader_source"
R"shader_source(	vec3 tatiana = vec3(214,47,39)/255.;
)shader_source"
R"shader_source(	vec3 zwo = vec3(242,110,4)/255.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// glow
)shader_source"
R"shader_source(	if (materialId.id == id_signal) {
)shader_source"
R"shader_source(		float t_anim = materialId.misc.x;
)shader_source"
R"shader_source(		float hash_dom = materialId.misc.y;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float t_final_hit = min(1., t_anim + glum_signal_impulse_offset_rt_float);
)shader_source"
R"shader_source(		t_final_hit = iqPowerCurve(glum_signal_impulse_k_rt_float, 3., t_final_hit);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float n_wobbel = 2. + floor(3. * hash_dom);
)shader_source"
R"shader_source(		float t_mid_wobbel = mod(t_anim + hash_dom, 1./n_wobbel) * n_wobbel;
)shader_source"
R"shader_source(		t_mid_wobbel = iqPowerCurve(23., 3., t_mid_wobbel);
)shader_source"
R"shader_source(		//t_mid_wobbel = smoothstep(0., 1., t_mid_wobbel);
)shader_source"
R"shader_source(		//t_mid_wobbel = square(sin(Pi * t_mid_wobbel));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float glow_intensity = t_final_hit + glum_signal_glow_mid_rt_float * t_mid_wobbel + glum_signal_glow_base_rt_float;
)shader_source"
R"shader_source(		glow_intensity /= 1. + glum_signal_glow_mid_rt_float + glum_signal_glow_base_rt_float;
)shader_source"
R"shader_source(		glow_intensity *= 1000. * glum_signal_glow_total_rt_float;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		mat.emission = glow_intensity * adpating_is_hard;
)shader_source"
R"shader_source(	} else if (materialId.id == id_tunnel) {
)shader_source"
R"shader_source(		vec3 p_tunnel = materialId.coord;
)shader_source"
R"shader_source(		vec3 cell_mirror = materialId.misc.xyz;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float t_height = linstep(glum_tunnel_glow_start_rt_float - glum_tunnel_glow_height_rt_float, glum_tunnel_glow_start_rt_float, p_tunnel.y);
)shader_source"
R"shader_source(		t_height = pow(t_height, glum_tunnel_glow_falloff_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float pos_length = mult(cell_mirror) * p_tunnel.z - glum_anim * glum_tunnel_glow_stripes_small_rt_float * glum_tunnel_glow_stripes_big_rt_float;
)shader_source"
R"shader_source(		float t_length = max(0., iqPowerCurve(23., 3., fract(pos_length / glum_tunnel_glow_stripes_small_rt_float)));
)shader_source"
R"shader_source(		t_length *= max(0., iqPowerCurve(23., 3., fract(pos_length / glum_tunnel_glow_stripes_small_rt_float / glum_tunnel_glow_stripes_big_rt_float)));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float t_glow = t_height * t_length;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		mat.emission = 1000. * t_glow * glum_tunnel_glow_intensity_rt_float * adpating_is_hard;
)shader_source"
R"shader_source(	} else if (materialId.id == id_floor) {
)shader_source"
R"shader_source(		vec3 p_floor = materialId.coord;
)shader_source"
R"shader_source(		vec3 cell_mirror = materialId.misc.xyz;
)shader_source"
R"shader_source(		p_floor.y -= glum_tunnel_glow_posy_offset_rt_float;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float pos_z = mult(cell_mirror) * p_floor.z - glum_anim * glum_floor_glow_velo_rt_float;
)shader_source"
R"shader_source(		pos_z = mod(pos_z, glum_floor_glow_length_rt_float) / glum_floor_glow_length_rt_float;
)shader_source"
R"shader_source(		pos_z = max(0., 2. * pos_z - 1.);
)shader_source"
R"shader_source(		pos_z = pow(pos_z, glum_floor_glow_falloff_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float t_glow = step(glum_floor_glow_space_rt_float, fract(p_floor.y / glum_floor_glow_thick_rt_float));
)shader_source"
R"shader_source(		t_glow *= pos_z;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		mat.emission = 1000. * t_glow * glum_floor_glow_intensity_rt_float * adpating_is_hard;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// colors
)shader_source"
R"shader_source(	if (materialId.id == id_tunnel) {
)shader_source"
R"shader_source(		mat.color = hex;
)shader_source"
R"shader_source(	} else if (materialId.id == id_strebe || materialId.id == id_strebe_stairs || materialId.id == id_spikes || materialId.id == id_spikes_top) {
)shader_source"
R"shader_source(		mat.color = murky;
)shader_source"
R"shader_source(	} else if (materialId.id == id_floor) {
)shader_source"
R"shader_source(		mat.color = tatiana;
)shader_source"
R"shader_source(	} else if (materialId.id == id_blades || materialId.id == id_blades_chamfer) {
)shader_source"
R"shader_source(		mat.color = murky;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return mat;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
