#version 430
layout(location = 149) uniform vec3 extbg_ground_color_rt_color; // color
layout(location = 242) uniform vec3 geo_color_rt_color; // color
layout(location = 243) uniform float geo_brightness_rt_float;
layout(location = 244) uniform float geo_plane_distance_rt_float;
layout(location = 245) uniform float geo_plane_num_rt_float;
layout(location = 246) uniform float geo_box_offset_rt_float;
layout(location = 247) uniform float geo_mode_rt_float;
layout(location = 248) uniform float geo_rotation_offset_rt_float;
layout(location = 157) uniform float extlight_1_power_rt_float;
layout(location = 158) uniform vec3 extlight_1_color_rt_color; // color
layout(location = 159) uniform vec3 extlight_1_pos_rt_vec3;
layout(location = 123) uniform vec3 pal_phase_rt_vec3;
layout(location = 160) uniform float bg_pattern_width_rt_float;
layout(location = 162) uniform float bg_pattern_domrep_rt_float;
layout(location = 124) uniform vec3 pal_base_rt_color; // color
layout(location = 165) uniform float ext_rot_rt_float;
layout(location = 166) uniform float bg_fade_pow_rt_float;
layout(location = 167) uniform float extlight_3_radius_rt_float;
layout(location = 148) uniform float extbg_ground_roughness_rt_float;
layout(location = 249) uniform float geo_radius_rt_float;
layout(location = 240) uniform float mand_f_domrep_rt_float;
layout(location = 226) uniform float mandlight_obj_roughness_rt_float;
layout(location = 168) uniform float extlight_1_radius_rt_float;
layout(location = 250) uniform float geo_plane_xpos_rt_float;
layout(location = 171) uniform bool extbg_enabled_rt_bool;
layout(location = 68) uniform float camera_shake_freq_rt_float;
layout(location = 173) uniform float extbg_ground_offset_rt_float;
layout(location = 228) uniform vec3 mand_color_2_rt_color; // color
layout(location = 126) uniform vec3 pal_frequency_rt_vec3;
layout(location = 251) uniform bool geo_enabled_rt_bool;
layout(location = 174) uniform vec2 extbg_domrep_rt_vec2;
layout(location = 67) uniform float camera_shake_intensity_rt_float;
layout(location = 101) uniform vec3 bg_color2_rt_color; // color
layout(location = 161) uniform float extlight_2_power_rt_float;
layout(location = 229) uniform float mand_rot_global_rt_float;
layout(location = 252) uniform float mandext_plane_offset_rt_float;
layout(location = 175) uniform float camera_exposure_rt_float;
layout(location = 176) uniform float extbg_ground_round_r_rt_float;
layout(location = 177) uniform vec3 extbg_offset_rt_vec3;
layout(location = 178) uniform float bg_pattern_tilt_rt_float;
layout(location = 231) uniform float mand_f_thickness_rt_float;
layout(location = 179) uniform vec3 extlight_2_pos_rt_vec3;
layout(location = 180) uniform float extlight_2_radius_rt_float;
layout(location = 181) uniform float extlight_shadow_softness_rt_float;
layout(location = 182) uniform vec3 extlight_2_color_rt_color; // color
layout(location = 113) uniform vec3 pal_amplitude_rt_color; // color
layout(location = 183) uniform vec3 extlight_3_pos_rt_vec3;
layout(location = 128) uniform vec3 bg_color1_rt_color; // color
layout(location = 184) uniform vec3 extlight_3_color_rt_color; // color
layout(location = 253) uniform float geo_tri_scale_rt_float;
layout(location = 185) uniform float extlight_3_power_rt_float;
layout(location = 233) uniform float mand_domrep_angle_r_rt_float;
layout(location = 234) uniform vec3 mand_color_circle_rt_color; // color
layout(location = 254) uniform float geo_thickness_rt_float;
layout(location = 186) uniform float extbg_rot_anim_rt_float;
layout(location = 187) uniform float ext_trans_rt_float;
layout(location = 189) uniform float extbg_tetra_r_rt_float;
layout(location = 236) uniform float mand_rot_cell_rt_float;
layout(location = 237) uniform vec2 mand_trans_inner_rt_vec2;
layout(location = 239) uniform float mand_rot_3D_rt_float;
layout(location = 235) uniform float mand_domrep_angle_num_rt_float;
layout(location = 191) uniform float bg_pattern_offset_rt_float;
layout(location = 192) uniform float ext_extrude_h_rt_float;
layout(location = 193) uniform float ext_extrude_freq_rt_float;
#line 3

// main output color
out vec3 out_color;
out float out_depth;

// common uniforms
layout(location = 0) uniform vec2 res;
layout(location = 64) uniform float time;

#line 2

// TODO figure out how to get rid of this special include file

// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
void pQuatRotate(inout vec3 v, vec4 q) {
	vec3 t = 2 * cross(q.xyz, v);
	v += q.w * t + cross(q.xyz, t);
	// *hex hex*
}

// jenkins hash
uint hash(uint a) {
	a = (a + 0x7ed55d16u) + (a << 12u);
	a = (a ^ 0xc761c23cu) ^ (a >> 19u);
	a = (a + 0x165667b1u) + (a << 5u);
	a = (a + 0xd3a2646cu) ^ (a << 9u);
	a = (a + 0xfd7046c5u) + (a << 3u);
	a = (a ^ 0xb55a4f09u) ^ (a >> 16u);
	return a;
}

// http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java

vec3 simplexGradients[12] = vec3[12](
	vec3(1,1,0), vec3(-1,1,0), vec3(1,-1,0), vec3(-1,-1,0),
	vec3(1,0,1), vec3(-1,0,1), vec3(1,0,-1), vec3(-1,0,-1),
	vec3(0,1,1), vec3(0,-1,1), vec3(0,1,-1), vec3(0,-1,-1)
);

float uintToFloat(uint x) {
	return float(x % (1u << 20u)) / float(1u << 20u);
}

float rand(int x) {
	return uintToFloat(hash(x));
}

float rand(ivec2 x) {
	return uintToFloat(hash(x.x + hash(x.y)));
}

float rand(ivec3 x) {
	return uintToFloat(hash(x.x + hash(x.y + hash(x.z))));
}

float valueNoise(float p) {
	int index = int(floor(p));
	float d = fract(p);
	float v0 = rand(index);
	float v1 = rand(index + 1);
	return mix(v0, v1, d);
}

float valueNoise(vec2 p) {
	ivec2 index = ivec2(floor(p));
	vec2 d = fract(p);
	float result = 0;
	float v00 = rand(index);
	float v01 = rand(index + ivec2(0, 1));
	float v10 = rand(index + ivec2(1, 0));
	float v11 = rand(index + ivec2(1, 1));
	return mix(mix(v00, v10, d.x), mix(v01, v11, d.x), d.y) * 2. - 1.;
}

float valueNoise(vec3 p) {
	ivec3 index = ivec3(floor(p));
	vec3 d = fract(p);
	float result = 0;
	float v000 = rand(index);
	float v010 = rand(index + ivec3(0, 1, 0));
	float v100 = rand(index + ivec3(1, 0, 0));
	float v110 = rand(index + ivec3(1, 1, 0));
	float v001 = rand(index + ivec3(0, 0, 1));
	float v011 = rand(index + ivec3(0, 1, 1));
	float v101 = rand(index + ivec3(1, 0, 1));
	float v111 = rand(index + ivec3(1, 1, 1));
	return mix(
		mix(mix(v000, v100, d.x), mix(v010, v110, d.x), d.y),
		mix(mix(v001, v101, d.x), mix(v011, v111, d.x), d.y),
		d.z
	) * 2. - 1.;
}

float smoothNoise(vec2 p) {
	float F2 = .5 * (sqrt(3.) - 1.);
	ivec2 index = ivec2(floor(p + (p.x + p.y) * F2));
	float G2 = (3. - sqrt(3.)) / 6.;
	p -= index - (index.x + index.y) * G2;
	ivec2 index1 = p.x > p.y ? ivec2(1, 0) : ivec2(0, 1);
	vec2 pV[3] = vec2[3](p, p - index1 + G2, p - 1. + 2. * G2);
	ivec3 vertexX = ivec3(index.x) + ivec3(0, index1.x, 1);
	ivec3 vertexY = ivec3(index.y) + ivec3(0, index1.y, 1);
	float result = 0;
	for (uint i = 0; i < 3; i++) {
		float t = max(0, .5 - pV[i].x * pV[i].x - pV[i].y * pV[i].y);
		t *= t;
		uint gradientIndex = hash(vertexX[i] + hash(vertexY[i])) % 12u;
		result += t * t * dot(simplexGradients[gradientIndex].xy, pV[i]);
	}
	return 70. * result;
}

float smoothNoise(vec3 p) {
	float F3 = 1./3.;
	float G3 = 1./6.;
	ivec3 index = ivec3(floor(p + (p.x + p.y + p.z) * F3));
	p -= index - (index.x + index.y + index.z) * G3;
	vec3 g = step(p.yzx, p);
	ivec3 index1 = ivec3(min(g, (1. - g).zxy));
	ivec3 index2 = ivec3(max(g, (1. - g).zxy));
	vec3 pV[4] = vec3[4](p, p - index1 + G3, p - index2 + 2. * G3, p - 1. + 3. * G3);
	int vertexX[4] = int[4]( index.x, index.x + index1.x, index.x + index2.x, index.x + 1);
	int vertexY[4] = int[4](index.y, index.y + index1.y, index.y + index2.y, index.y + 1);
	int vertexZ[4] = int[4](index.z, index.z + index1.z, index.z + index2.z, index.z + 1);
	float result = 0;
	for (uint i = 0; i < 4; i++) {
		float t = max(0., .6 - pV[i].x * pV[i].x - pV[i].y * pV[i].y - pV[i].z * pV[i].z);
		t *= t;
		uint gradientIndex = hash(vertexX[i] + hash(vertexY[i] + hash(vertexZ[i]))) % 12u;
		result += t * t * dot(simplexGradients[gradientIndex], pV[i]);
	}
	return 32. * result;
}

float valueFbm(vec2 p) {
	float result = 0.;
	for (int i = 0; i < 3; i++) {
		result += exp2(-i) * valueNoise(p);
		p *= 2.;
	}
	return result / 1.75;
}

float valueFbm(vec3 p) {
	float result = 0.;
	for (int i = 0; i < 3; i++) {
		result += exp2(-i) * valueNoise(p);
		p *= 2.;
	}
	return result / 1.75;
}

float smoothFbm(vec2 p) {
	float result = 0.;
	for (int i = 0; i < 3; i++) {
		result += exp2(-i) * smoothNoise(p);
		p *= 2.;
	}
	return result / 1.75;
}

float smoothFbm(vec3 p) {
	float result = 0.;
	for (int i = 0; i < 3; i++) {
		result += exp2(-i) * smoothNoise(p);
		p *= 2.;
	}
	return result / 1.75;
}

#line 2

const float Pi = 3.14159265359;
const float Tau = 6.28318530718;
const float Euler = 2.71828182846;
const float Golden_Ratio = 1.61803398875;
const float Inf = 1e3; // in erster Näherung oder so...
const float Real_Inf = 1./0.; // this could fuck up things

float minV(vec2 v) {
	return min(v.x, v.y);
}

float minV(vec3 v) {
	return min(min(v.x, v.y), v.z);
}

float minV(vec4 v) {
	return min(min(min(v.x, v.y), v.z), v.w);
}

float maxV(vec2 v) {
	return max(v.x, v.y);
}

float maxV(vec3 v) {
	return max(max(v.x, v.y), v.z);
}

float maxV(vec4 v) {
	return max(max(max(v.x, v.y), v.z), v.w);
}

float min3(float a, float b, float c) {
	return min(min(a, b), c);
}

float min4(float a, float b, float c, float d) {
	return min(min(min(a, b), c), d);
}

float max3(float a, float b, float c) {
	return max(max(a, b), c);
}

float max4(float a, float b, float c, float d) {
	return max(max(max(a, b), c), d);
}

float sum(vec2 v) {
	return v.x + v.y;
}

float sum(vec3 v) {
	return v.x + v.y + v.z;
}

float mult(vec2 v) {
	return v.x * v.y;
}

float mult(vec3 v) {
	return v.x * v.y * v.z;
}

float square(float x) {
	return x * x;
}

vec2 square(vec2 x) {
	return x * x;
}

vec3 square(vec3 x) {
	return x * x;
}

float rcp(float x) {
	return 1./x;
}

vec2 rcp(vec2 x) {
	return 1./x;
}

vec3 rcp(vec3 x) {
	return 1./x;
}

float lengthN(vec2 p, float n) {
	return pow(sum(pow(abs(p), vec2(n))), 1./n);
}

float lengthN(vec3 p, float n) {
	return pow(sum(pow(abs(p), vec3(n))), 1./n);
}

float saturate(float x) {
	return clamp(x, 0., 1.);
}

vec2 saturate(vec2 x) {
	return clamp(x, 0., 1.);
}

vec3 saturate(vec3 v) {
	return clamp(v, 0., 1.);
}

float linstep(float edge0, float edge1, float x) {
	return clamp((x - edge0) / (edge1 - edge0), 0., 1.);
}

vec3 linstep(vec3 edge0, vec3 edge1, vec3 x) {
	return clamp((x - edge0) / (edge1 - edge0), 0., 1.);
}

vec3 linstep(float edge0, float edge1, vec3 x) {
	return clamp((x - edge0) / (edge1 - edge0), 0., 1.);
}

float smootherstep(float edge0, float edge1, float x) {
	// directly from master Ken
	float t = clamp((x - edge0) / (edge1 - edge0), 0., 1.);
	return t*t*t*(t*(t*6. - 15.) + 10.);
}

vec2 smootherstep(vec2 edge0, vec2 edge1, vec2 x) {
	// directly from master Ken
	vec2 t = clamp((x - edge0) / (edge1 - edge0), 0., 1.);
	return t*t*t*(t*(t*6. - 15.) + 10.);
}

float pdot(vec2 a, vec2 b) {
	return max(0., dot(a, b));
}

float pdot(vec3 a, vec3 b) {
	return max(0., dot(a, b));
}

float sgn(float x) {
	return x < 0. ? -1. : 1.;
}

vec2 sgn(vec2 x) {
	return vec2(
			x.x < 0. ? -1. : 1.,
			x.y < 0. ? -1. : 1.);
}

vec3 sgn(vec3 x) {
	return vec3(
			x.x < 0. ? -1. : 1.,
			x.y < 0. ? -1. : 1.,
			x.z < 0. ? -1. : 1.);
}

mat2 rotMat2D(float phi) {
	float c = cos(phi);
	float s = sin(phi);
	return mat2(c, -s, s, c);
}

vec2 unitVector(float phi) {
	return vec2(cos(phi), sin(phi));
}

// phi geht von 0 bis 2 pi (und fängt bei der x-achse an)
// theta von 0 bis pi (und fängt von oben an)
vec3 unitVector(float phi, float theta) {
	float ct = cos(theta);
	float sp = sin(phi);
	float st = sin(theta);
	float cp = cos(phi);
	return vec3(st * cp, ct, st * sp);
}
vec3 unitVector(vec2 phitheta) {
	float ct = cos(phitheta.y);
	float sp = sin(phitheta.x);
	float st = sin(phitheta.y);
	float cp = cos(phitheta.x);
	return vec3(st * cp, ct, st * sp);
}

// m: anything above stays unchanged
// n: value at x = 0
// x: x
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqAlmostIdentity(float m, float n, float x) {
	if(x > m) return x;

	float a = 2. * n - m;
	float b = 2. * m - 3. * n;
	float t = x / m;

	return (a * t + b) * t * t + n;
}

// k: width of impulse, maximum happens at 1/k
// x: x
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqImpulse(float k, float x) {
	float h = k * x;
	return h * exp(1. - h);
}

// c: x value of maximum
// w: half pulse width
// x: x
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqCubicPulse(float c, float w, float x) {
	x = abs(x - c);
	x /= w;
	return 1. - smoothstep(0., 1., x);
}

// k: kind of inverse when the step occurs
// n: how stepish the function is
// x: x
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqExpStep(float k, float n, float x) {
	return exp(-k * pow(x, n));
}

// k: how thin the function is
// x: x
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqParabola(float k, float x) {
	return pow(4. * x * (1. - x), k);
}

// a, b: how much to bend in left and right direction
// x: x
// reaches 1 at maximum
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqPowerCurve(float a, float b, float x) {
    float k = pow(a + b, a + b) / (pow(a, a) * pow(b, b));
    return k * pow(x, a) * pow(1. - x, b);
}

// a, b: how much to bend in left and right direction
// x: x
// not normalized version
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqPowerCurveUnnormalized(float a, float b, float x) {
    return pow(x, a) * pow(1. - x, b);
}

vec3 iqCosinePalette(vec3 t, vec3 base, vec3 amplitude, vec3 frequency, vec3 phase) {
	return base + amplitude * cos(Tau * (frequency * t + phase));
}

vec3 iqCosinePalette(float t, vec3 base, vec3 amplitude, vec3 frequency, vec3 phase) {
	return base + amplitude * cos(Tau * (frequency * t + phase));
}

float rgb2luma(vec3 rgb) {
	return dot(rgb, vec3(.2126, .7152, .0722)); // magic luminance formular
}

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float lin2srgb(float c) {
	float float_if = step(0.00313066844250063, c);
	float float_smaller = c * 12.92;
	float float_greater = 1.055 * pow(c, 1./2.4) - 0.055;
	float float_result = mix(float_smaller, float_greater, float_if);
	return float_result;
}

vec3 lin2srgb(vec3 c) {
	vec3 vec_if = step(0.00313066844250063, c);
	vec3 vec_smaller = c * 12.92;
	vec3 vec_greater = 1.055 * pow(c, vec3(1./2.4)) - 0.055;
	vec3 vec_result = mix(vec_smaller, vec_greater, vec_if);
	return vec_result;
}

float srgb2lin(float c) {
	float float_if = step(0.0404482362771082f, c);
	float float_smaller = c / 12.92;
	float float_greater = pow( (c + 0.055) / 1.055, 2.4);
	float float_result = mix(float_smaller, float_greater, float_if);
	return float_result;
}

vec3 srgb2lin(vec3 c) {
	vec3 vec_if = step(0.0404482362771082f, c);
	vec3 vec_smaller = c / 12.92;
	vec3 vec_greater = pow( (c + 0.055) / 1.055, vec3(2.4));
	vec3 vec_result = mix(vec_smaller, vec_greater, vec_if);
	return vec_result;
}

vec2 bezier(vec2 p1, vec2 p2, vec2 p3, vec2 p4, float t) {
	vec2 p12 = mix(p1, p2, t);
	vec2 p23 = mix(p2, p3, t);
	vec2 p34 = mix(p3, p4, t);

	vec2 p123 = mix(p12, p23, t);
	vec2 p234 = mix(p23, p34, t);

	vec2 p1234 = mix(p123, p234, t);

	return p1234;
}

#line 5

layout(location = 69) uniform vec3 camera_position;
layout(location = 70) uniform vec4 camera_rotation; // quat
layout(location = 71) uniform float camera_focal_length;

layout(location = 72) uniform bool camera_crane_active;
layout(location = 73) uniform vec3 camera_crane_base;
layout(location = 74) uniform float camera_crane_length;
layout(location = 75) uniform float camera_crane_phi;
layout(location = 76) uniform float camera_crane_theta;

layout(location = 77) uniform bool camera_tracking_active;
layout(location = 78) uniform vec3 camera_tracking_target;
layout(location = 79) uniform vec2 camera_tracking_screen_pos;
layout(location = 80) uniform float camera_tracking_roll;

layout(location = 81) uniform bool spectatormode_active;
layout(location = 82) uniform vec3 spectatormode_position;
layout(location = 83) uniform vec4 spectatormode_rotation; // quat
layout(location = 84) uniform float spectatormode_focal_length;

// gets set with camGetDirection
const float camera_sensor_width = 0.024; // also hardcoded in CameraController.cpp
float screen_distance = camera_focal_length / camera_sensor_width;

float camGetFocalLength() {
	if (spectatormode_active) {
		return spectatormode_focal_length;
	}
	return camera_focal_length;
}

vec3 camGetPositionTimeline() {
	if (camera_crane_active) {
		vec3 crane_head_relative = camera_crane_length * unitVector(camera_crane_phi * Tau, camera_crane_theta * Tau);
		vec3 crane_head_absolute = camera_crane_base + crane_head_relative;
		return crane_head_absolute;
	}
	return camera_position;
}

vec3 camGetPosition() {
	if (spectatormode_active) {
		return spectatormode_position;
	}
	return camGetPositionTimeline();
}

mat3 camGetTrackingRotationMat() {
	// construct the camera coordinate system in a way that it is only dutch angled if wanted
	vec3 view_direction = normalize(camera_tracking_target - camGetPositionTimeline());
	vec3 view_right = -cross(view_direction, vec3(0., 1., 0.));

	if (view_right == vec3(0.)) {
		// view_direction parallel to y-axis
		view_right = vec3(1., 0., 0.);
	} else {
		view_right = normalize(view_right);
	}

	// rotate view_right around view_direction
	// from https://en.wikipedia.org/wiki/Rodrigues'_rotation_formula
	// simplified, because view_right and view_direction are perpendicular
	view_right = cos(camera_tracking_roll * Tau) * view_right + sin(camera_tracking_roll * Tau) * cross(view_direction, view_right);

	vec3 view_up = normalize(cross(view_direction, view_right));
	mat3 rotation_matrix = mat3(-view_right, view_up, -view_direction);


	// rotate in such a way that target is at camera_tracking_screen_pos
	vec2 screen_pos_normalized = (camera_tracking_screen_pos - .5) * res / res.x;
	vec2 screen_pos_tangens = screen_pos_normalized / (camGetFocalLength() / camera_sensor_width);

	vec2 c_arg = 1. / sqrt(1. + square(screen_pos_tangens)); // cos(arctan(arg));
	vec2 s_arg = screen_pos_tangens * c_arg; // sin(arctan(arg));

	// rotation around x axis with arg.y as angle
	mat3 rot_x_axis = mat3(
			1., 0., 0.,
			0., c_arg.y, -s_arg.y,
			0., s_arg.y, c_arg.y
			);

	// rotation around y axis with arg.x as angle
	// transpose to get the left-handed rotation
	mat3 rot_y_axis = transpose(mat3(
				c_arg.x, 0., s_arg.x,
				0., 1., 0.,
				-s_arg.x, 0., c_arg.x
				));

	// do x axis rotation in camera space and y axis rotation in world space
	rotation_matrix = rot_y_axis * rotation_matrix * rot_x_axis;
	return rotation_matrix;
}

vec3 camGetDirectionSS(vec2 coord, out float screen_d) {
	vec3 dir = vec3((coord - .5 * res) / res.x,
		camGetFocalLength() / camera_sensor_width);
	screen_d = length(dir.xz);
	dir = normalize(dir);
	return dir;
}

vec3 camGetDirection() {
	vec2 shake = vec2(
		smoothNoise(vec2(time * camera_shake_freq_rt_float, 23)),
		smoothNoise(vec2(time * camera_shake_freq_rt_float, 283))
	) * camera_shake_intensity_rt_float;
	vec3 dir = camGetDirectionSS(gl_FragCoord.xy + shake * res, screen_distance);
	dir.z = -dir.z;

	if (spectatormode_active) {
		pQuatRotate(dir, spectatormode_rotation);
	} else {
		if (camera_tracking_active) {
			dir = camGetTrackingRotationMat() * dir;
		} else {
			pQuatRotate(dir, camera_rotation);
		}
	}

	return dir;
}

float camGetPixelSize(float dist) {
	return .5 * dist / (screen_distance * res.x);
}

// rotates the object the same way the camera is rotated
void pRotateLikeCamera(inout vec3 v) {
	if (camera_tracking_active) {
		mat3 inverse_rotation_matrix = transpose(camGetTrackingRotationMat());
		v = inverse_rotation_matrix * v;
	} else {
		vec4 inverse_camera_rotation = vec4(-camera_rotation.xyz, camera_rotation.w);
		pQuatRotate(v, inverse_camera_rotation);
	}
}





#line 4

void pTrans(inout float p, float d) {
	p -= d;
}

void pTrans(inout vec2 p, vec2 d) {
	p -= d;
}

void pTrans(inout vec2 p, float x, float y) {
	pTrans(p, vec2(x, y));
}

void pTrans(inout vec3 p, vec3 d) {
	p -= d;
}

void pTrans(inout vec3 p, float x, float y, float z) {
	pTrans(p, vec3(x, y, z));
}

// rotates clockwise when looking in the direction given by the right-hand rule
// don't use this directly (unless for 2d coordinates)!
void pRot(inout vec2 p, float phi) {
	p *= rotMat2D(-phi);
}

// rotate clockwise around X axis
void pRotX(inout vec3 p, float phi) {
	pRot(p.yz, phi);
}

// rotate clockwise around Y axis
void pRotY(inout vec3 p, float phi) {
	pRot(p.zx, phi);
}

// rotate clockwise around Z axis
void pRotZ(inout vec3 p, float phi) {
	pRot(p.xy, phi);
}

float pDomrep(inout float p, float c) {
	p += .5 * c;
	float i = floor(p/c);
	p = mod(p, c) - .5 * c;
	return i;
}

vec2 pDomrep(inout vec2 p, vec2 c) {
	p += .5 * c;
	vec2 i = floor(p/c);
	p = mod(p, c) - .5 * c;
	return i;
}

vec2 pDomrep(inout vec2 p, float cx, float cy) {
	return pDomrep(p, vec2(cx, cy));
}

vec3 pDomrep(inout vec3 p, vec3 c) {
	p += .5 * c;
	vec3 i = floor(p/c);
	p = mod(p, c) - .5 * c;
	return i;
}

vec3 pDomrep(inout vec3 p, float cx, float cy, float cz) {
	return pDomrep(p, vec3(cx, cy, cz));
}

void pMirrorDomrepped(inout float p, float i) {
	p *= mod(i, 2.) * 2. - 1.;
}

void pMirrorDomrepped(inout vec2 p, vec2 i) {
	p *= mod(i, 2.) * 2. - 1.;
}

void pMirrorDomrepped(inout vec3 p, vec3 i) {
	p *= mod(i, 2.) * 2. - 1.;
}

float pDomrepMirror(inout float p, float c) {
	float i = pDomrep(p, c);
	p *= mod(i, 2.) * 2. - 1.;
	return i;
}

vec2 pDomrepMirror(inout vec2 p, vec2 c) {
	vec2 i = pDomrep(p, c);
	p *= mod(i, 2.) * 2. - 1.;
	return i;
}

vec3 pDomrepMirror(inout vec3 p, vec3 c) {
	vec3 i = pDomrep(p, c);
	p *= mod(i, 2.) * 2. - 1.;
	return i;
}

vec2 pDomrepGrid(inout vec2 p, vec2 c) {
	vec2 i = pDomrepMirror(p, c);
	p -= .5 * c;
	if (p.x > p.y) {
		p.xy = p.yx;
	}
	return floor(.5 * i);
}

vec3 pDomrepGrid(inout vec3 p, vec3 c) {
	vec3 i = pDomrepMirror(p, c);
	p -= .5 * c;
	if (p.x < p.y) {
		if (p.y > p.z) {
			if (p.x < p.z) {
				p = p.xzy;
			} else {
				p = p.zxy;
			}
		}
	} else {
		if (p.z < p.y) {
			p = p.zyx;
		} else {
			if (p.z < p.x) {
				p = p.yzx;
			} else {
				p = p.yxz;
			}
		}
	}
	return floor(.5 * i);
}

// domrep only in positive half
float pDomrepSingle(inout float p, float c) {
	float halfC = .5 * c;
	float i = 0.;
	if (p > halfC) {
		p += halfC;
		i = floor(p/c);
		p = mod(p, c) - halfC;
	}
	return i;
}

// domrep only in positive half
vec2 pDomrepSingle(inout vec2 p, vec2 c) {
	vec2 halfC = .5 * c;
	vec2 i = vec2(0.);

	if (p.x > halfC.x) {
		p.x += halfC.x;
		i.x = floor(p.x/c.x);
		p.x = mod(p.x, c.x) - halfC.x;
	}

	if (p.y > halfC.y) {
		p.y += halfC.y;
		i.y = floor(p.y/c.y);
		p.y = mod(p.y, c.y) - halfC.y;
	}

	return i;
}

// domrep only in positive half
vec3 pDomrepSingle(inout vec3 p, vec3 c) {
	vec3 halfC = .5 * c;
	vec3 i = vec3(0.);

	if (p.x > halfC.x) {
		p.x += halfC.x;
		i.x = floor(p.x/c.x);
		p.x = mod(p.x, c.x) - halfC.x;
	}

	if (p.y > halfC.y) {
		p.y += halfC.y;
		i.y = floor(p.y/c.y);
		p.y = mod(p.y, c.y) - halfC.y;
	}

	if (p.z > halfC.z) {
		p.z += halfC.z;
		i.z = floor(p.z/c.z);
		p.z = mod(p.z, c.z) - halfC.z;
	}

	return i;
}

// domrep from cell start to (inclusively) end
float pDomrepInterval(inout float p, float c, float start, float end) {
	p += .5 * c;
	float i = floor(p/c);
	p = mod(p, c) - .5 * c;

	if (i > end) {
		p += c * (i - end);
		i = end;
	} else if (i < start) {
		p += c * (i - start);
		i = start;
	}

	return i;
}

// domrep from cell start to (inclusively) end
vec2 pDomrepInterval(inout vec2 p, vec2 c, vec2 start, vec2 end) {
	p += .5 * c;
	vec2 i = floor(p/c);
	p = mod(p, c) - .5 * c;

	if (i.x > end.x) {
		p.x += c.x * (i.x - end.x);
		i.x = end.x;
	} else if (i.x < start.x) {
		p.x += c.x * (i.x - start.x);
		i.x = start.x;
	}

	if (i.y > end.y) {
		p.y += c.y * (i.y - end.y);
		i.y = end.y;
	} else if (i.y < start.y) {
		p.y += c.y * (i.y - start.y);
		i.y = start.y;
	}

	return i;
}

// domrep from cell start to (inclusively) end
vec3 pDomrepInterval(inout vec3 p, vec3 c, vec3 start, vec3 end) {
	p += .5 * c;
	vec3 i = floor(p/c);
	p = mod(p, c) - .5 * c;

	if (i.x > end.x) {
		p.x += c.x * (i.x - end.x);
		i.x = end.x;
	} else if (i.x < start.x) {
		p.x += c.x * (i.x - start.x);
		i.x = start.x;
	}

	if (i.y > end.y) {
		p.y += c.y * (i.y - end.y);
		i.y = end.y;
	} else if (i.y < start.y) {
		p.y += c.y * (i.y - start.y);
		i.y = start.y;
	}

	if (i.z > end.z) {
		p.z += c.z * (i.z - end.z);
		i.z = end.z;
	} else if (i.z < start.z) {
		p.z += c.z * (i.z - start.z);
		i.z = start.z;
	}

	return i;
}

float pDomrepAngleWithAtan(inout vec2 p, float repetitions, float radius, float preCalcAtan) {
	float at = preCalcAtan;
	float cAngle = Tau / repetitions;
	float i = pDomrep(at, cAngle);

	float r = length(p);
	p = r * unitVector(at);
	pTrans(p.x, radius);
	return i;
}

float pDomrepAngle(inout vec2 p, float repetitions, float radius) {
	return pDomrepAngleWithAtan(p, repetitions, radius, atan(p.y, p.x));
}

float pDomrepRadiusWithAtan(inout vec2 p, float c, float preCalcAtan) {
	float r = length(p);
	float i = floor(r / c);
	r = mod(r, c);
	p = r * unitVector(preCalcAtan);
	return i;
}

float pDomrepRadius(inout vec2 p, float c) {
	return pDomrepRadiusWithAtan(p, c, atan(p.y, p.x));
}

float pMirror(inout float p) {
	float s = sgn(p);
	p = abs(p);
	return s;
}

vec2 pMirror(inout vec2 p) {
	vec2 s = sgn(p);
	p = abs(p);
	return s;
}

vec3 pMirror(inout vec3 p) {
	vec3 s = sgn(p);
	p = abs(p);
	return s;
}

float pMirrorTrans(inout float p, float c) {
	float s = pMirror(p);
	pTrans(p, c);
	return s;
}

vec2 pMirrorTrans(inout vec2 p, vec2 c) {
	vec2 s = pMirror(p);
	pTrans(p, c);
	return s;
}

vec3 pMirrorTrans(inout vec3 p, vec3 c) {
	vec3 s = pMirror(p);
	pTrans(p, c);
	return s;
}

vec3 pMirrorLoco(inout vec2 p, vec2 c) {
	vec3 s;
	s.xy = pMirrorTrans(p, c);
	if (p.y > p.x) {
		s.z = -1.;
		p.xy = p.yx;
	} else {
		s.z = 1.;
	}
	return s;
}

vec3 pMirrorLoco(inout vec3 p, vec3 c) {
	p = p.zyx;
	vec3 s = pMirrorTrans(p, c);
	if (p.x < p.y) {
		if (p.y > p.z) {
			if (p.x < p.z) {
				p = p.xzy;
			} else {
				p = p.zxy;
			}
		}
	} else {
		if (p.z < p.y) {
			p = p.zyx;
		} else {
			if (p.z < p.x) {
				p = p.yzx;
			} else {
				p = p.yxz;
			}
		}
	}
	p = p.zyx;
	return s;
}

// see pMirrorGrid(vec3, float)
vec2 pMirrorGrid(inout vec2 p, float c) {
	vec2 q = p;
	pMirror(q);
	vec2 s = vec2(0);
	if (q.y > q.x) {
		p = p.yx;
		s.x = 1;
	}
	s.y = pMirrorTrans(p.x, c);
	p.y *= s.y;
	return s;
}

// s.x: axis (0: x, 1: y, 2: z)
// s.y: mirroring (-1 or 1)
vec2 pMirrorGrid(inout vec3 p, float c) {
	vec3 q = p;
	pMirror(q);
	vec2 s = vec2(0);
	if (q.z > q.x || q.y > q.x) {
		if (q.y > q.z) {
			p = p.yxz;
			s.x = 1;
		} else {
			p = p.zyx;
			s.x = 2;
		}
	}
	s.y = pMirrorTrans(p.x, c);
	p.y *= s.y;
	return s;
}

float pMirrorAtPlane(inout vec3 p, vec3 planeNormal, float offset) {
	float dist = dot(p, planeNormal) + offset;
	if (dist < 0.) {
		p -= 2. * dist * planeNormal;
	}
	return sgn(dist);
}

// cut through a plane at point a with normal normalize(v) and translate positive
// halfspace by v, filling the gap with the extruded cut plane
float pCutAndExtrude(inout vec3 p, vec3 a, vec3 v) {
	pTrans(p, a);
	float h = saturate(dot(p, v)/ dot(v, v));
	p -= v * h;
	pTrans(p, -a);
	return h;
}

void pFlip(inout float p) {
	p = -p;
}

void pFlip(inout vec2 p) {
	p = -p;
}

void pFlip(inout vec3 p) {
	p = -p;
}

#line 3

// unions two object and produces a very smooth transition
// affects an area of r between the meet point, underestimates the distance
// can be used with any kind of objects
float opUnionSmooth(float f1, float f2, float r) {
	// maybe remove this min() and if() stuff
	//float f_min = min(f1, f2);
	if (f1 < r && f2 < r) {
		float h = clamp(.5 + .5 * (f2 - f1) / r, 0., 1. );
		float f_smooth = mix(f2, f1, h) - r * h * (1. - h);
		return f_smooth;
		//return min(f_smooth, f_min);
	}
	float f_min = min(f1, f2);
	return f_min;
}

// intersects two object and produces a very smooth transition
// affects an area of r between the meet point, underestimates the distance
// can be used with any kind of objects
float opIntersectSmooth(float f1, float f2, float r) {
	// maybe remove this max() and if() stuff
	//float f_max = max(f1, f2);
	//if (f1 > -r && f2 > -r) {
	if (abs(f1) < r && abs(f2) < r) {
		float h = clamp(.5 - .5 * (f2 - f1) / r, 0., 1. );
		float f_smooth = mix(f2, f1, h) + r * h * (1. - h);
		return f_smooth;
		//return max(f_smooth, f_max);
	}
	float f_max = max(f1, f2);
	return f_max;
}

// subtracts f2 from f1 and produces a very smooth transition
// affects an area of r between the meet point, underestimates the distance
// can be used with any kind of objects
float opSubtractSmooth(float f1, float f2, float r) {
	return opIntersectSmooth(f1, -f2, r);
}

// all hail the smin
float smin(float f1, float f2, float r) {
	return opUnionSmooth(f1, f2, r);
}

float smax(float f1, float f2, float r) {
	return opIntersectSmooth(f1, f2, r);
}

// unions two objects and adds rounded edges with radius r
// use only with orthogonal objects
float opUnionRounded(float f1, float f2, float r) {
	float f_min = min(f1, f2);
	if (f1 < r && f2 < r) {
		vec2 q = vec2(f1, f2);
		vec2 center = vec2(r);
		float f_round = r - distance(center, q); // inside of fSphere2
		return min(f_round, f_min);
	}
	return f_min;
}

// intersects two objects and adds rounded edges with radius r
// use only with orthogonal objects
float opIntersectRounded(float f1, float f2, float r) {
	float f_max = max(f1, f2);
	if (f1 > -r && f2 > -r) {
		vec2 q = vec2(f1, f2);
		vec2 center = vec2(-r);
		float f_round = distance(center, q) - r;
		return max(f_round, f_max);
	}
	return f_max;
}

// subtracts f2 from f1 and adds rounded edges with radius r
// use only with orthogonal objects
float opSubtractRounded(float f1, float f2, float r) {
	return opIntersectRounded(f1, -f2, r);
}

// unions two objects and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opUnionChamfer(float f1, float f2, float r) {
	float f_min = min(f1, f2);
	//if (f1 < r && f2 < r) {
		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
		return min(f_chamfer, f_min);
	//}
	//return f_min;
}

// intersects two objects and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opIntersectChamfer(float f1, float f2, float r) {
	float f_max = max(f1, f2);
	//if (f1 > -r && f2 > -r) {
		float f_chamfer = sqrt(.5) * (f1 + f2 + r);
		return max(f_chamfer, f_max);
	//}
	//return f_max;
}

// subtracts f2 from f1 and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opSubtractChamfer(float f1, float f2, float r) {
	return opIntersectChamfer(f1, -f2, r);
}

// unions two objects and adds some kind of smooth chamfer with radius r
// use only with orthogonal objects
float opUnionChamferSmooth(float f1, float f2, float r) {
	float f_min = min(f1, f2);
	//if (f1 < r && f2 < r) {
		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
		return opUnionSmooth(f_chamfer, f_min, .1 * r);
	//}
	//return f_min;
}

// intersects two objects and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opIntersectChamferSmooth(float f1, float f2, float r) {
	float f_max = max(f1, f2);
	//if (f1 > -r && f2 > -r) {
		float f_chamfer = sqrt(.5) * (f1 + f2 + r);
		return opIntersectSmooth(f_chamfer, f_max, .1 * r);
	//}
	//return f_max;
}

// subtracts f2 from f1 and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opSubtractChamferSmooth(float f1, float f2, float r) {
	return opIntersectChamferSmooth(f1, -f2, r);
}

// unions two objects and adds n columns within a radius of r
// use only with orthogonal objects
float opUnionColumns(float f1, float f2, float r, float n) {
	// Speckröllchen
	float f_min = min(f1, f2);
	if (f1 < 2.*r && f2 < 2.*r) {
		vec2 q = vec2(f1, f2);
		//float radius = sqrt(2.) * .5 * r / (n + sqrt(2.) - 1.);
		//q.x -= radius;
		float radius = sqrt(.5) * r / (n + sqrt(.5) - 1.);
		q.y -= r - radius;
		pRot(q, -Tau / 8.);
		pDomrepInterval(q.x, 2. * radius, 0., n-1.);
		float f_columns = length(q) - radius;
		f_columns = min(f_columns, q.y);
		return min(f_columns, f_min);
	}
	return f_min;
}

// intersects two objects and adds n columns within a radius of r
// use only with orthogonal objects
float opIntersectColumns(float f1, float f2, float r, float n) {
	// Speckröllchen
	float f_max = max(f1, f2);
	if (f1 > -r && f2 > -r) {
		vec2 q = vec2(f1, f2);
		//float radius = sqrt(2.) * .5 * r / (n + sqrt(2.) - 1.);
		//q.x -= radius;
		float radius = sqrt(.5) * r / (n + sqrt(.5) - 1.);
		q.x -= -r;
		q.y -= -radius;
		pRot(q, -Tau / 8.);
		pDomrepInterval(q.x, 2. * radius, 0., n-1.);
		float f_columns = length(q) - radius;
		f_columns = min(f_columns, q.y);
		return max(f_columns, f_max);
	}
	return f_max;
}

// subtracts f2 from f1 and adds n columns within a radius of r
// use only with orthogonal objects
float opSubtractColumns(float f1, float f2, float r, float n) {
	return opIntersectColumns(f1, -f2, r, n);
}

// unions two objects and adds n stairs within a radius of r
// use only with orthogonal objects
float opUnionStairs(float f1, float f2, float r, float n) {
	// Speckröllchen
	float f_min = min(f1, f2);
	//if (f1 < 2.*r && f2 < 2.*r) {
		vec2 q = vec2(f1, f2);
		float radius = r / n * sqrt(.5);
		q.y -= r - sqrt(.5) * radius;
		q.x -= sqrt(.5) * radius;
		pRot(q, -Tau / 8.);
		pDomrep(q.x, 2. * radius);

		q.x = abs(q.x);
		float offset = radius * sqrt(.5);
		float f_columns = dot(q, vec2(sqrt(.5))) - offset;

		f_columns = min(f_columns, q.y);
		return min(f_columns, f_min);
	//}
	//return f_min;
}

// intersects two objects and adds n stairs within a radius of r
// use only with orthogonal objects
float opIntersectStairs(float f1, float f2, float r, float n) {
	// stairs are symmetric, so we can use opUnionStairs
	return -opUnionStairs(-f1, -f2, r, n);
}

// subtracts f2 from f1 and adds n stairs within a radius of r
// use only with orthogonal objects
float opSubtractStairs(float f1, float f2, float r, float n) {
	return opIntersectStairs(f1, -f2, r, n);
}

// like normal min()-union but with correct distance at corners
// gives correct interior
// use only with orthogonal objects
float opUnionEuclid(float f1, float f2) {
	vec2 q = vec2(f1, f2);
	return max(minV(q), 0.) - length(min(q, 0.));
}

// like normal max()-intersection but with correct distance at corners
// use only with orthogonal objects
float opIntersectEuclid(float f1, float f2) {
	vec2 q = vec2(f1, f2);
	return min(maxV(q), 0.) + length(max(q, 0.));
}

// like normal max()-subtraction but with correct distance at corners
// use only with orthogonal objects
float opSubtractEuclid(float f1, float f2) {
	return opIntersectEuclid(f1, -f2);
}

#line 4

struct MaterialId {
	float id;
	vec3 coord;
	vec4 misc;
};

struct MatWrap {
	float f;
	MaterialId m;
};

struct Material {
	vec3 color;
	float roughness;
	float metallic;
	float height;
	vec3 emission;
};

bool calculate_material = false;
float current_dist = Inf;
MaterialId current_material = MaterialId(0., vec3(0.), vec4(0.));

// der witz: jetzt in einmal komplett neu!
void mUnion(float f, MaterialId m) {
	if (calculate_material) {
		if (f < current_dist) {
			current_dist = f;
			current_material = m;
		}
	} else {
		current_dist = min(current_dist, f);
	}
}

void mUnion(MatWrap w) {
	mUnion(w.f, w.m);
}

MatWrap mUnion(MatWrap w1, MatWrap w2) {
	return w1.f < w2.f ? w1 : w2;
}

MatWrap mIntersect(MatWrap w1, MatWrap w2) {
	return w1.f > w2.f ? w1 : w2;
}

MatWrap mSubtract(MatWrap w1, MatWrap w2) {
	w2.f = -w2.f;
	return mIntersect(w1, w2);
}

// und hier der andere neue witz
void mIntersect(float f, MaterialId m) {
	if (calculate_material) {
		if (f > current_dist) {
			current_dist = f;
			current_material = m;
		}
	} else {
		current_dist = max(current_dist, f);
	}
}

MaterialId newMaterialId(float id, vec3 coord) {
	return MaterialId(id, coord, vec4(0.));
}

Material defaultMaterial(vec3 color) {
	return Material(
		color,
		.5,
		0,
		0,
		vec3(0)
	);
}


void mUnionChamfer(float f2, MaterialId m, float r, float id_chamfer) {
	//if (calculate_material) {
		float f1 = current_dist;
		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
		mUnion(f2, m);
		mUnion(f_chamfer, newMaterialId(id_chamfer, vec3(f1, f2, 0.)));
	//} else {
		//current_dist = opUnionChamfer(current_dist, f2, r);
	//}
}

void mUnionStairs(float f2, MaterialId m, float r, float n, float id_stairs) {
	//if (calculate_material) {
		float f1 = current_dist;
		//float f_min = min(f1, f2);
		//if (f1 < 2.*r && f2 < 2.*r) {
			vec2 q = vec2(f1, f2);
			float radius = r / n * sqrt(.5);
			q.y -= r - sqrt(.5) * radius;
			q.x -= sqrt(.5) * radius;
			pRot(q, -Tau / 8.);
			pDomrep(q.x, 2. * radius);

			q.x = abs(q.x);
			float offset = radius * sqrt(.5);
			float f_columns = dot(q, vec2(sqrt(.5))) - offset;

			f_columns = min(f_columns, q.y);

			mUnion(f2, m);
			mUnion(f_columns, newMaterialId(id_stairs, vec3(q, 0.)));
			//return min(f_columns, f_min);
		//}
		//return f_min;
	//} else {
		//current_dist = opUnionStairs(current_dist, f2, r, n);
	//}
}

void mUnionStairs(MatWrap w, float r, float n, float id_stairs) {
	mUnionStairs(w.f, w.m, r, n, id_stairs);
}

void mUnionSmooth(float f2, MaterialId m, float r, float id_smooth) {
	float f1 = current_dist;
	if (f1 < r && f2 < r) {
		float h = clamp(.5 + .5 * (f2 - f1) / r, 0., 1. );
		float f_smooth = mix(f2, f1, h) - r * h * (1. - h);
		current_dist = f_smooth;
		current_material = newMaterialId(id_smooth, vec3(f1, f2, h));
	}
	mUnion(f2, m);
}

MatWrap mUnionChamfer(MatWrap w1, MatWrap w2, float r, float id_chamfer) {
	//if (calculate_material) {
		MatWrap w_min = mUnion(w1, w2);
		float f_chamfer = sqrt(.5) * (w1.f + w2.f - r);
		return mUnion(w_min, MatWrap(f_chamfer, newMaterialId(id_chamfer, vec3(w1.f, w2.f, 0.))));
	//} else {
		//current_dist = opUnionChamfer(current_dist, f2, r);
	//}
}

MatWrap mUnionSmooth(MatWrap w1, MatWrap w2, float r, float id_smooth) {
	float f1 = w1.f;
	float f2 = w2.f;
	if (f1 < r && f2 < r) {
		float h = clamp(.5 + .5 * (f2 - f1) / r, 0., 1. );
		float f_smooth = mix(f2, f1, h) - r * h * (1. - h);
		return MatWrap(f_smooth, newMaterialId(id_smooth, vec3(f1, f2, 0.)));
	}
	MatWrap w_min = mUnion(w1, w2);
	return w_min;
}

MatWrap mUnionStairs(MatWrap w1, MatWrap w2, float r, float n, float id_stairs) {
	//if (calculate_material) {
		float f1 = w1.f;
		float f2 = w2.f;
		//float f_min = min(f1, f2);
		//if (f1 < 2.*r && f2 < 2.*r) {
			vec2 q = vec2(f1, f2);
			float radius = r / n * sqrt(.5);
			q.y -= r - sqrt(.5) * radius;
			q.x -= sqrt(.5) * radius;
			pRot(q, -Tau / 8.);
			pDomrep(q.x, 2. * radius);

			q.x = abs(q.x);
			float offset = radius * sqrt(.5);
			float f_columns = dot(q, vec2(sqrt(.5))) - offset;

			f_columns = min(f_columns, q.y);
			MatWrap w_columns = MatWrap(f_columns, newMaterialId(id_stairs, vec3(q, 0.)));

			MatWrap w_min = mUnion(w1, w2);
			w_min = mUnion(w_min, w_columns);
			return w_min;

			//mUnion(f2, m);
			//mUnion(f_columns, newMaterialId(id_stairs, vec3(q, 0.)));
			//return min(f_columns, f_min);
		//}
		//return f_min;
	//} else {
		//current_dist = opUnionStairs(current_dist, f2, r, n);
	//}
}






#line 5

float fSphere(vec3 p, float r) {
	return length(p) - r;
}

float f2Sphere(vec2 p, float r) {
	return length(p) - r;
}

// capped cylinder, h is half height
float fCylinder(vec3 p, float r, float h) {
	float sp2 = f2Sphere(p.xz, r);
	float y = abs(p.y) - h;
	return opIntersectEuclid(sp2, y);
}

// capped cylinder, h is half height
float fCylinderEdge(vec3 p, float r, float h) {
	float sp2 = f2Sphere(p.xz, r);
	float y = abs(p.y) - h;
	return max(sp2, y);
}

float fCorner(vec3 p) {
	return min(maxV(p), 0.) + length(max(p, 0.));
}

float fCornerRounded(vec3 p, float r) {
	return fCorner(p + r) - r;
}

float fCornerEdge(vec3 p) {
	return maxV(p);
}

float f2Corner(vec2 p) {
	return min(maxV(p), 0.) + length(max(p, 0.));
}

float fCornerRounded2(vec2 p, float r) {
	return f2Corner(p + r) - r;
}

float f2CornerEdge(vec2 p) {
	return maxV(p);
}

float fBox(vec3 p, vec3 r) {
	vec3 q = abs(p) - r;
	return min(maxV(q), 0.) + length(max(q, 0.));
}

float fBox(vec3 p, float rx, float ry, float rz) {
	return fBox(p, vec3(rx, ry, rz));
}

float fBox(vec3 p, float r) {
	return fBox(p, vec3(r));
}

float fBoxRounded(vec3 p, vec3 r, float rCorner) {
	return fBox(p, r - rCorner) - rCorner;
}

float fBoxRounded(vec3 p, float r, float rCorner) {
	return fBoxRounded(p, vec3(r), rCorner);
}

float fBoxEdge(vec3 p, vec3 r) {
	return maxV(abs(p) - r);
}

float fBoxEdge(vec3 p, float rx, float ry, float rz) {
	return fBoxEdge(p, vec3(rx, ry, rz));
}

float fBoxEdge(vec3 p, float r) {
	return fBoxEdge(p, vec3(r));
}

float f2Box(vec2 p, vec2 r) {
	vec2 q = abs(p) - r;
	return min(maxV(q), 0.) + length(max(q, 0.));
}

float f2Box(vec2 p, float rx, float ry) {
	return f2Box(p, vec2(rx, ry));
}

float f2Box(vec2 p, float r) {
	return f2Box(p, vec2(r));
}

float f2BoxRounded(vec2 p, vec2 r, float rCorner) {
	return f2Box(p, r - rCorner) - rCorner;
}

float f2BoxRounded(vec2 p, float r, float rCorner) {
	return f2BoxRounded(p, vec2(r), rCorner);
}

float f2BoxEdge(vec2 p, vec2 r) {
	return maxV(abs(p) - r);
}

float f2BoxEdge(vec2 p, float rx, float ry) {
	return f2BoxEdge(p, vec2(rx, ry));
}

float f2BoxEdge(vec2 p, float r) {
	return f2BoxEdge(p, vec2(r));
}

// generic box
// need to define func_box_1(q) first and undefine afterwards
// p, q and r must be of the same type
#define fBoxGeneric(p, r) func_box_1(abs(p) - r)

float fPlane(vec3 p, vec3 n) {
	// n must be normalized
	return dot(p, n);
}

float fPlaneAngle(vec3 p, float phi, float theta) {
	return fPlane(p, unitVector(phi, theta));
}

float f2Plane(vec2 p, vec2 n) {
	// n must be normalized
	return dot(p, n);
}

float f2PlaneAngle(vec2 p, float phi) {
	return f2Plane(p, unitVector(phi));
}

// r is the radius from the origin to the vertices
float f2Triprism(vec2 p, float r) {
	return max(f2PlaneAngle(vec2(abs(p.x), p.y), radians(30)), -p.y) - .5 * r;
}

// capped triprism, h is half height
float fTriprism(vec3 p, float r, float h) {
	float tri2 = f2Triprism(p.xz, r);
	float y = abs(p.y) - h;
	return opIntersectEuclid(tri2, y);
}

// capped triprism, h is half height
float fTriprismEdge(vec3 p, float r, float h) {
	float tri2 = f2Triprism(p.xz, r);
	float y = abs(p.y) - h;
	return max(tri2, y);
}

// r is the radius from the origin to the vertices
// just like a rotated f2BoxEdge
float f2Quadprism(vec2 p, float r) {
	float offset = r * sqrt(.5);
	vec2 q = abs(p);
	float quad = f2Plane(q, vec2(sqrt(.5))) - offset;
	return quad;
}

// r is the radius from the origin to the vertices
float f2Pentaprism(vec2 p, float r) {
	float phi1 = radians(108. / 2.);
	float phi2 = radians(-18.);
	float offset = r * cos(Tau / 5. / 2.);

	vec2 q = vec2(abs(p.x), p.y);
	float side1 = f2PlaneAngle(q, phi1);
	float side2 = -p.y;
	float side3 = f2PlaneAngle(q, phi2);

	float pentagon = max3(side1, side2, side3) - offset;

	return pentagon;
}

// capped pentaprism, h is half height
float fPentaprism(vec3 p, float r, float h) {
	float penta2 = f2Pentaprism(p.xz, r);
	float y = abs(p.y) - h;
	return opIntersectEuclid(penta2, y);
}

// capped pentaprism, h is half height
float fPentaprismEdge(vec3 p, float r, float h) {
	float penta2 = f2Pentaprism(p.xz, r);
	float y = abs(p.y) - h;
	return max(penta2, y);
}

// r is the radius from the origin to the vertices
float f2Hexprism(vec2 p, float r) {
	float offset = r * cos(Tau / 6. / 2.);
    vec2 q = abs(p);
	float side1 = f2PlaneAngle(q, radians(30.));
	float side2 = q.y;
	float hexagon = max(side1, side2) - offset;
    return hexagon;
}

// capped hexprism, h is half height
float fHexprism(vec3 p, float r, float h) {
	float hex2 = f2Hexprism(p.xz, r);
	float y = abs(p.y) - h;
	return opIntersectEuclid(hex2, y);
}

// capped hexprism, h is half height
float fHexprismEdge(vec3 p, float r, float h) {
	float hex2 = f2Hexprism(p.xz, r);
	float y = abs(p.y) - h;
	return max(hex2, y);
}

// awesome supershapes directly at your hands!
// a and b control the total size
// m is the number of spikes
// n1, n2, n3 control the exact shape
// http://paulbourke.net/geometry/supershape/
// http://de.wikipedia.org/wiki/Superformel
// have fun playing around!
float f2Supershape(vec2 p, float a, float b, float m, float n1, float n2, float n3) {
	float phi = atan(p.y, p.x);
	float d = length(p);

	float m4 = m / 4.;

	float c = cos(m4 * phi);
	float s = sin(m4 * phi);

	float ca = c / a;
	float sb = s / b;

	float gc = ca < 0. ? -1. : 1.;
	float gs = sb < 0. ? -1. : 1.;

	float absc = ca * gc;
	float abss = sb * gs;

	float ab2 = pow(absc, n2);
	float ab3 = pow(abss, n3);

	//float ab21 = pow(absc, n2 - 1.);
	//float ab31 = pow(abss, n3 - 1.);
	float ab21 = ab2 / absc;
	float ab31 = ab3 / abss;

	float rw = ab2 + ab3;
	float r = pow(rw, -1./n1);

	float k = -n2 * ab21 * gc / a * s;
	float l =  n3 * ab31 * gs / b * c;

	//float drpre = m4 / n1 * pow(rw, -1./n1 - 1.);
	float drpre = m4 / n1 * r / rw;
	float dr2 = drpre * drpre * (k * k + 2. * k * l + l * l);

	float f = (d - r) / sqrt(1 + dr2);
	return f;
}

float fTorus(vec3 p, float rBig, float rSmall) {
	// also try replacing f2Sphere by something like f2Box/f2BoxEdge/f2BoxRounded
	vec2 q = vec2(f2Sphere(p.xz, rBig), p.y);
	return f2Sphere(q, rSmall);
}

float fTorus(vec3 p, vec2 r) {
	return fTorus(p, r.x, r.y);
}

float fTorusBox(vec3 p, float rBig, float rSmall) {
	vec2 q = vec2(f2BoxEdge(p.xz, rBig), p.y);
	return f2Box(q, rSmall);
}

float fTorusSphereBox(vec3 p, float rBig, float rSmall) {
	vec2 q = vec2(f2Sphere(p.xz, rBig), p.y);
	return f2Box(q, rSmall);
}

float fTorusPartial(vec3 p, float rBig, float rSmall, float halfAngle) {
	float r = length(p.xz);
	float angle = atan(p.z, p.x);

	angle -= clamp(angle, -halfAngle, halfAngle);

	p.xz = r * unitVector(angle);
	p.x -= rBig;

	return fSphere(p, rSmall);
}

// generic torus
// need to define func_torus_1(q) and func_torus_2(q) first and undefine afterwards
// p must be a vec3
// q must be a vec2
#define fTorusGeneric(p) func_torus_2(vec2(func_torus_1(p.xz), p.y))

// n is plane normal and must be normalized
float fCone(vec3 p, vec2 n) {
	vec2 q = vec2(length(p.xz), p.y);
	return f2Plane(q, n);
}

float fConeAngle(vec3 p, float angle) {
	vec2 q = vec2(length(p.xz), p.y);
	return f2PlaneAngle(q, angle);
}

float fConeCapped(vec3 p, float rBig, float rSmall, float h) {
	float a = rBig - rSmall;
	float b = 2 * h;
	float side = sqrt(a * a + b * b);
	vec2 n = vec2(b/side, a/side);
	vec3 q = p;
	pTrans(q.y, rSmall * n.x / n.y + h);
	float cone = fCone(q, n);
	float y = abs(p.y) - h;
	return max(cone, y);
}

// capped cone with box as cross-section (a pyramid)
// h is height of capping
// r is x radius of box at capping
// r * ratio is y radius at capping
float fConeBoxCapped(vec3 p, float r, float h, float ratio) {
	vec2 n1 = normalize(vec2(h, r));
	vec2 n2 = normalize(vec2(h, r * ratio));
	pMirror(p.xz);
	float cone_plane1 = f2Plane(p.xy, n1);
	float cone_plane2 = f2Plane(p.zy, n2);
	float cone = max(cone_plane1, cone_plane2);
	float cut = p.y + h;
	return max(cone, -cut);
}

// line from origin to v, inflated by r
float fLine(vec3 p, float r, vec3 v) {
	float h = saturate(dot(p, v)/ dot(v, v));
	return fSphere(p - v*h, r);
}

// line along x-axis, from -h to h
float f2LineX(vec2 p, float r, float h) {
	p.x -= clamp(p.x, -h, h);
	return f2Sphere(p, r);
}

// line along x-axis, from -h to h
float fLineX(vec3 p, float r, float h) {
	p.x -= clamp(p.x, -h, h);
	return fSphere(p, r);
}

// line along x-axis with cones as caps, n is cone normal
float fLineCone(vec3 p, float r, float h, vec2 n) {
	float a = r * n.x / n.y; // r / tan(phi)
	p.x = abs(p.x);
	p.x -= min(h, p.x);
	p.x -= a;
	return fCone(p.yxz, n);
}

// line along x-axis with cones as caps, angle is cone angle
float fLineConeAngle(vec3 p, float r, float h, float angle) {
	return fLineCone(p, r, h, unitVector(angle));
}

// r is the radius from the origin to the vertices
float fOctahedron(vec3 p, float r) {
	vec3 q = abs(p);
	q.y -= r;
	//q.y -= .707106781 * s; // sqrt(2) / 2 * s
	//float phi = TAU/4. - acos(-1./3.) * .5;
	float plane1 = f2Plane(q.xy, vec2(.816496581, .577350269)); // cos(phi), sin(phi)
	float plane2 = f2Plane(q.zy, vec2(.816496581, .577350269));
	float f_oktaeder = max(plane1, plane2);
	return f_oktaeder;
}

// spiral starting at the orgin
// dist is distance between tubes, r is radius of tubes
float fSpiral(vec3 p, float dist, float r) {
	float radius = length(p.xz);
	float angle = atan(p.z, p.x);
	radius -= angle * dist / Tau;
	vec2 q = vec2(radius, p.y);
	pDomrep(q.x, dist);
	float d = f2Sphere(q, r);
	return d;
}


// l: half distance of focus points, r: radius
float f2Spheroid(vec2 p, float l, float r) {
	vec2 q1 = p, q2 = p;
	q1.x -= l;
	q2.x -= -l;

	float f = length(q1) + length(q2);
	f = f * .5 - r;
	return f;
}

// l: half distance of focus points, r: radius
float fSpheroid(vec3 p, float l, float r) {
	vec3 q1 = p, q2 = p;
	q1.x -= l;
	q2.x -= -l;

	float f = length(q1) + length(q2);
	f = f * .5 - r;
	return f;
}

// four sided pyramid, h is height, phi is angle of planes
float fPyramid(vec3 p, float h, float phi) {
	vec3 q = p;
	q.xz = abs(q.xz);
	q.y -= h;
	float plane1 = f2PlaneAngle(q.xy, phi);
	float plane2 = f2PlaneAngle(q.zy, phi);
	float f_oktaeder = max(plane1, plane2);
	return f_oktaeder;
}

float fTetrahedron(vec3 p, float r) {
	pTrans(p.y, r);
	vec4 q = p * mat4x3(
		vec3(0., -1., 0.),
		vec3(0., .333, .943),
		vec3(.816, .333, -.471),
		vec3(-.816, .333, -.471)
	);
	return maxV(q) - r;
}




#line 3

mat2 rM2D(float phi) {
	float c = cos(phi);
	float s = sin(phi);
	return mat2(c, -s, s, c);
}

vec3 envmap(vec3 d) {
	float angle = atan(d.z, d.x);

	vec3 color_pal_1 = iqCosinePalette((d.y * d.x * d.z), pal_base_rt_color, pal_amplitude_rt_color, pal_frequency_rt_vec3, pal_phase_rt_vec3);
	color_pal_1 *= pow(abs(1. - smoothstep(0., .03 + 0.01 * sin(angle * 16. + .25*Tau), d.y)), bg_fade_pow_rt_float);

	vec3 color_pal_2 = mix(bg_color1_rt_color, bg_color2_rt_color, lin2srgb(square(sin(4. * angle))));
	color_pal_2 *= pow(smoothstep(-.01, .0, d.y), bg_fade_pow_rt_float);
	color_pal_2 *= 0.5;

	vec3 color = color_pal_1 + color_pal_2;


	float y = d.y / sqrt(1. - square(d.y)); // tan(arcsin(d.y))
	float pattern_angle = angle + bg_pattern_tilt_rt_float * y + bg_pattern_offset_rt_float;
	float pattern_domrepped = mod(pattern_angle, bg_pattern_domrep_rt_float) / bg_pattern_domrep_rt_float;
	color += step(1. - bg_pattern_width_rt_float, pattern_domrepped) * 0.005;

	color *= .01;
	return color;
}

#line 5

layout(binding = 20) uniform sampler2D brdf;
layout(binding = 21) uniform samplerCube environment;
layout(binding = 22) uniform samplerCube filteredDiffuse;
layout(binding = 23) uniform samplerCube filteredSpecular;

struct SphereLight {
	vec3 position;
	float radius;
	vec3 color;
	float power;
};

struct PointLight {
	vec3 position;
	vec3 direction;
	float angle;
	vec3 color;
	float power;
};

vec3 getSpecularDominantDir(vec3 N, vec3 R, float NdotV, float roughness) {
	float smoothness = saturate(1 - roughness);
	return normalize(mix(N, R, smoothness * (sqrt(smoothness) + roughness)));
}

// n: normal
// v: vector from hit to camera (for example -dir)
// color: base color of object
// roughness: between 0 and 1
vec3 approximateSpecular(vec3 n, vec3 v, vec3 color, float roughness) {
	float NoV = saturate(dot(n, v));
	vec3 r = 2. * dot(n, v) * n - v;

	vec3 dir = getSpecularDominantDir(n, r, NoV, sqrt(roughness));

	vec3 prefiltered = textureLod(filteredSpecular, dir, sqrt(roughness) * 5.).rgb;
	vec2 envBRDF = textureLod(brdf, vec2(roughness, NoV), 0.).rg;

	return prefiltered * (color * envBRDF.x + envBRDF.y);
}

// n: normal
// v: vector from hit to camera (for example -dir)
// color: base color of object
// roughness: between 0 and 1
// metallic: only 0 or 1
vec3 ambientColor(vec3 n, vec3 v, Material mat) {
	vec3 diffuse = textureLod(filteredDiffuse, n, 0.).rgb;
	vec3 dielectric = mat.color * diffuse
		+ approximateSpecular(n, v, vec3(.04), mat.roughness);
	vec3 metal = approximateSpecular(n, v, mat.color, mat.roughness);
	vec3 color = mix(dielectric, metal, mat.metallic);

	return color;
}

// o: camera origin
// d: camera view direction
// r: radius of "bounding sphere"
vec3 environmentColor(vec3 o, vec3 d, float r) {
	// hmmmmm…
	o.xz -= camGetPosition().xz;
	float radicand = square(dot(d, o)) - dot(o, o) + r * r;
	float t = -dot(d, o) + sqrt(radicand);
	//vec3 color = textureLod(environment, normalize(o + t * d), 0.).rgb;
	vec3 color = envmap(normalize(o + t * d));

	return color;
}

// handy standard applyLights() function at your hands, just copy this into yout applyLights() function
/*
	return applyNormalLights(origin, marched, direction, hit, normal, material);
// */
vec3 applyNormalLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	material.color = .5 * normal + .5;
	return ambientColor(normal, -direction, material);
}

float diffuseSphereLight(vec3 hit, vec3 N, SphereLight light) {
	vec3 L_unormalized = light.position - hit;
	float light_distance2 = dot(L_unormalized, L_unormalized);
	vec3 L = L_unormalized * inversesqrt(light_distance2);

	float Beta = acos(dot(N, L));
	float H = sqrt(light_distance2);
	float h = H / light.radius;
	float x = sqrt(square(h) - 1);
	float y = -x / tan(Beta);

	float illuminance = 0;
	if (h * cos(Beta) > 1) {
		illuminance = cos(Beta) / square(h);
	} else {
		illuminance = (1. / (Pi * square(h))) *
			(cos(Beta) * acos(y) - x * sin(Beta) * sqrt(1 - y * y)) +
			(1 / Pi) * atan(sin(Beta) * sqrt(1 - square(y)) / x);
	}

	illuminance *= Pi;
	return illuminance;
}

vec3 specularSphereLightClosestPoint(vec3 hit, vec3 N, vec3 V, float roughness, SphereLight light) {
	vec3 R  = reflect(-V, N);
	vec3 dominantDir = getSpecularDominantDir(N, R, pdot(N, V), roughness);
	vec3 L_unnorm = light.position - hit;
	vec3 centerToRay = dot(L_unnorm, dominantDir) * dominantDir - L_unnorm;
	vec3 closestPoint = L_unnorm + centerToRay * saturate(light.radius / length(centerToRay));
	return closestPoint;
}

// not actually the delta function, adapted for use in brdfSpecularEpicSmith
float deltaGGX(float alpha2, float NdotV) {
	float NdotV2 = square(NdotV);
	float tan_theta_alpha2 = (1 - NdotV2) / NdotV2 * alpha2;
	return sqrt(1 + tan_theta_alpha2);
}

float brdfSpecularEpicSmithWithoutFresnel(vec3 V, vec3 L, vec3 N, float roughness) {
	vec3 H = normalize(V + L);
	float NdotL = pdot(N, L);
	float NdotV = pdot(N, V);
	float NdotH = pdot(N, H);
	float alpha = square(roughness);
	float alpha2 = square(alpha);

	float divisor = max(Pi * square(square(NdotH) * (alpha2 - 1) + 1), 1e-8); // specular D part
	divisor *= deltaGGX(alpha2, NdotV) + deltaGGX(alpha2, NdotL); // height-correlated smith masking-shadowing * 2
	divisor *= 2 * NdotL * NdotV; // 2 moved to G term
	return max(alpha2 / divisor, 0);
}

vec3 fresnel(vec3 V, vec3 L, vec3 F0) {
	vec3 H = normalize(V + L);
	float VdotH = pdot(V, H);
	return F0 + (1 - F0) * exp2((-5.55473 * VdotH - 6.98316) * VdotH);
}

vec3 applySphereLight(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, SphereLight light) {
	// diffuse part
	vec3 L = light.position - hit;
	float lightDistance2 = dot(L, L);
	L /= sqrt(lightDistance2);
	float NoL = pdot(L, normal);
	float incomingLight = light.power / (4 * square(Pi) * square(light.radius));
	incomingLight *= diffuseSphereLight(hit, normal, light);
	vec3 diffuse = material.color * (max(0, NoL * incomingLight) / Pi);

	// specular part
	vec3 closestPoint = specularSphereLightClosestPoint(hit, normal, -direction, material.roughness, light);
	float roughnessAlpha = saturate(material.roughness + light.radius / (2 * sqrt(lightDistance2)));
	float closestPointDistance2 = dot(closestPoint, closestPoint);
	closestPoint /= sqrt(closestPointDistance2);
	float normalization = square(material.roughness / roughnessAlpha);
	float incomingLightSpecular = light.power * normalization / (4 * Pi * closestPointDistance2);
	vec3 F_metal = fresnel(-direction, closestPoint, material.color);
	float specular = incomingLightSpecular * NoL * brdfSpecularEpicSmithWithoutFresnel(-direction, closestPoint, normal, material.roughness);

	vec3 F_dielectric = fresnel(-direction, closestPoint, vec3(.04));
	vec3 light_result = mix(diffuse + F_dielectric * specular, specular * F_metal, material.metallic);
	light_result *= light.color;
	return light_result;
}

float getDistanceAttenuation(vec3 lightVector) {
	return rcp(Pi * dot(lightVector, lightVector));
}

float getAngleAttenuation(vec3 L, PointLight light) {
	float outer = light.angle;
	float foo = 1 - smoothstep(0, outer, acos(-dot(light.direction, L)));
	return square(foo);
}

vec3 applyPointLight(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, PointLight light) {
	vec3 lightVector = light.position - hit;
	vec3 L = normalize(lightVector);
	float attenuation = 1;
	attenuation *= getDistanceAttenuation(lightVector);
	attenuation *= getAngleAttenuation(L, light);
	float brdfSpecular = brdfSpecularEpicSmithWithoutFresnel(-direction, L, normal, material.roughness);
	vec3 dielectric = material.color / Pi + brdfSpecular * fresnel(-direction, L, vec3(.04));
	vec3 metallic = brdfSpecular * fresnel(-direction, L, material.color);
	return mix(dielectric, metallic, material.metallic) * saturate(dot(normal, L)) * attenuation * light.color * light.power;
}


const float mat_id_ground = 34.;

bool traceScenePlane(vec3 o, vec3 d, float t_max, out float marched, out vec3 normal) {
	o.x -= extbg_ground_offset_rt_float;

	float radius = extbg_ground_round_r_rt_float;
	vec2 center = vec2(radius);
	vec2 l = d.xy;
	float l_square = dot(l, l);
	vec2 OminusC = o.xy - center;
	float LdotOminusC = dot(l, OminusC);
	float radicand = square(LdotOminusC) - l_square * (dot(OminusC, OminusC) - square(radius));

	float local_inf = t_max;
	float current_d = local_inf;
	vec3 current_normal;
	if (radicand > 0) {
		float root = sqrt(radicand);
		float summand = - dot(l, OminusC);
		float d1 = (summand - root) / l_square;
		float d2 = (summand + root) / l_square;
		float d = d2;
		if (d > 0) {
			vec2 point = o.xy + d * l;
			vec2 pointLocal = point - center;
			if (pointLocal.x < 0 && pointLocal.y < 0 && d < current_d) {
				current_d = d;
				current_normal = -normalize(vec3(pointLocal, 0));
			}
		}
	}

	float d_plane1 = -o.x / d.x;
	if (d_plane1 > 0 && d_plane1 < current_d) {
		current_d = d_plane1;
		current_normal = vec3(1, 0, 0);
	}

	float d_plane2 = -o.y / d.y;
	if (d_plane2 > 0 && d_plane2 < current_d) {
		current_d = d_plane2;
		current_normal = vec3(0, 1, 0);
	}

	marched = current_d;
	normal = current_normal;
	return current_d != local_inf;
}

#line 11

/// marchingloopkram.glsl

layout(location = 197) uniform float main_normal_epsilon_bias;

layout(location = 198) uniform float debug_mode;
layout(location = 199) uniform vec3 debug_plane_normal;
layout(location = 200) uniform float debug_plane_height;
layout(location = 201) uniform bool debug_camera_visualization;

const float debug_plane_material_id = 423.;
const float debug_camera_id = 424.;
const float debug_crane_base_id = 425.;
const float debug_crane_arm_id = 426.;
const float debug_tracking_target_id = 427.;

bool debug_default_pass_scene_visible = true;
bool debug_default_pass_plane_visible = false;
bool debug_default_pass_camera_visible = false;
bool debug_isoline_pass_scene_visible = false;
bool debug_isoline_pass_plane_visible = false;
bool debug_isoline_pass_camera_visible = false;
bool debug_gradient_visualization = false;
bool debug_gradient_pass_scene_visible = false;
bool debug_gradient_pass_plane_visible = false;
bool debug_gradient_pass_camera_visible = false;

bool scene_visible = debug_default_pass_scene_visible;
bool debug_plane_visible = debug_default_pass_plane_visible;
bool debug_camera_visible = debug_default_pass_camera_visible;

float fScene(vec3 p);

float fDebugPlane(vec3 p) {
	return abs(fPlane(p, normalize(debug_plane_normal)) - debug_plane_height);
}

MatWrap wDebugCameraVisualization(vec3 p) {
	// camera
	vec3 p_camera = p;
	pTrans(p_camera, camGetPositionTimeline());
	pRotateLikeCamera(p_camera);
	float f_camera = fConeBoxCapped(p_camera.xzy, .3, .5, res.y/res.x);
	//float f_camera = Inf;
	MatWrap w_camera = MatWrap(f_camera, newMaterialId(debug_camera_id, p_camera));

	if (camera_crane_active) {
		// crane
		vec3 p_crane = p;
		pTrans(p_crane, camera_crane_base);
		vec3 relative_head = camera_crane_length * unitVector(camera_crane_phi * Tau, camera_crane_theta * Tau);

		float f_crane_base = fSphere(p_crane, .5);
		MatWrap w_crane_base = MatWrap(f_crane_base, newMaterialId(debug_crane_base_id, p_crane));
		w_camera = mUnion(w_camera, w_crane_base);

		float f_crane_arm = fLine(p_crane, .05, relative_head);
		MatWrap w_crane_arm = MatWrap(f_crane_arm, newMaterialId(debug_crane_arm_id, p_crane));
		w_camera = mUnion(w_camera, w_crane_arm);
	}

	if (camera_tracking_active) {
		// target tracking
		vec3 p_target = p;
		pTrans(p_target, camera_tracking_target);
		float f_target = fSphere(p_target, .2);
		MatWrap w_target = MatWrap(f_target, newMaterialId(debug_tracking_target_id, p_target));
		w_camera = mUnion(w_camera, w_target);
	}

	// remove all geometry in sphere around spectator camera
	vec3 p_eyes_free = p;
	pTrans(p_eyes_free, camGetPosition());
	float f_eyes_free = fSphere(p_eyes_free, 1.);
	w_camera.f = max(w_camera.f, -f_eyes_free);

	return w_camera;
}

float fMain(vec3 p, bool calc_m) {
	current_dist = Inf;
	calculate_material = calc_m;

	if (debug_plane_visible) {
		mUnion(fDebugPlane(p), newMaterialId(debug_plane_material_id, p));
	}

	if (debug_camera_visible) {
		mUnion(wDebugCameraVisualization(p));
	}

	if (scene_visible) {
		fScene(p);
	}

	return current_dist;
}

vec3 sdfNormalRaw(vec3 p, float e) {
	// writing the gradient this way, causes the compiler not to inline f six times
	// thanks to mercury, stupid compilers
	vec3 s[6] = vec3[6](vec3(e,0,0), vec3(0,e,0), vec3(0,0,e), vec3(-e,0,0), vec3(0,-e,0), vec3(0,0,-e));
	float d[6] = float[6](0,0,0,0,0,0);
	for(int i = 0; i < 6; i++) {
		d[i] = fMain(p+s[i], false);
	}
	return vec3(d[0]-d[3], d[1]-d[4], d[2]-d[5]);
}

vec3 sdfNormal(vec3 p, float epsilon) {
	return normalize(sdfNormalRaw(p, epsilon));
}

vec3 sdfNormal(vec3 p) {
	float epsilon = max(.00005, camGetPixelSize(distance(p, camera_position)));
	epsilon *= main_normal_epsilon_bias;
	return sdfNormal(p, epsilon);
}

vec3 sdfNormalForeward(vec3 p, vec3 direction) {
	vec3 normal = sdfNormal(p);
	// try eliminating backfacing normals
	if (dot(direction, normal) > 0.) {
		normal = normalize(dFdx(normal) + normal);
		if (dot(direction, normal) > 0.) {
			normal = normalize(dFdy(normal) + normal);
		}
	}
	return normal;
}

vec3 sdfGradient(vec3 p, float epsilon) {
	return sdfNormalRaw(p, epsilon) / (2. * epsilon);
}

vec3 sdfGradient(vec3 p) {
	float epsilon = max(.00005, camGetPixelSize(distance(p, camera_position)));
	epsilon *= main_normal_epsilon_bias;
	return sdfGradient(p, epsilon);
}

// ein fachmenschich kopierter marchingloop
bool sdfMarchAdvanced(vec3 o, vec3 d, float t_min, float t_max, float pixelRadius, int max_iterations, float omega, bool forceHit, out float candidate_t) {
	// o, d : ray origin, direction (normalized)
	// t_min, t_max: minimum, maximum t values
	// pixelRadius: radius of a pixel at t = 1
	// forceHit: boolean enforcing to use the
	//           candidate_t value as result
	float t = t_min;
	float candidate_error = Inf;
	candidate_t = t_min;
	float previousRadius = 0;
	float stepLength = 0;
	float functionSign = sgn(fMain(o, false));

	for (int i = 0; i < max_iterations; ++i) {
		float signedRadius = functionSign * fMain(d*t + o, false);
		float radius = abs(signedRadius);

		bool sorFail = omega > 1 && (radius + previousRadius) < stepLength;
		if (sorFail) {
			stepLength -= omega * stepLength;
			omega = 1;
		} else {
			stepLength = signedRadius * omega;
		}

		previousRadius = radius;
		float error = radius / t;
		if (!sorFail && error < candidate_error) {
			candidate_t = t;
			candidate_error = error;
		}

		if (!sorFail && error < pixelRadius || t > t_max) {
			break;
		}

		t += stepLength;
	}

	return !((t > t_max || candidate_error > pixelRadius) && !forceHit);
}

bool sdfMarch(vec3 o, vec3 d, float t_max, out float marched) {
	bool hit = sdfMarchAdvanced(o, d, .001, t_max, camGetPixelSize(1), 256, 1.2, false, marched);

	if (!hit) {
		return false;
	} else {
		// discontinuity reduction
		for (int i = 0; i < 3; i++) {
			marched += fMain(o + marched * d, false) - camGetPixelSize(marched);
		}
		return true;
	}
}

void setDebugParameters() {
	int mode = int(debug_mode);
	if (mode == 0) {
		// default
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			debug_gradient_pass_scene_visible = false;
			debug_gradient_pass_plane_visible = false;
	} else if(mode == 1) { // debug plane
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			debug_gradient_pass_scene_visible = false;
			debug_gradient_pass_plane_visible = false;
	} else if(mode == 2) { // debug plane without scene geometry
			debug_default_pass_scene_visible = false;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			debug_gradient_pass_scene_visible = false;
			debug_gradient_pass_plane_visible = false;
	} else if(mode == 3) { // visualize gradient length
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = true;
			debug_gradient_pass_scene_visible = true;
			debug_gradient_pass_plane_visible = false;
	} else if(mode == 4) { // visualize gradient length with debug plane
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = true;
			debug_gradient_pass_scene_visible = true;
			debug_gradient_pass_plane_visible = false;
	} else if(mode == 5) { // visualize gradient length with debug plane and without geometry
			debug_default_pass_scene_visible = false;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = true;
			debug_gradient_pass_scene_visible = true;
			debug_gradient_pass_plane_visible = false;
	} else { // same as default
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			debug_gradient_pass_scene_visible = false;
			debug_gradient_pass_plane_visible = false;
	}

	debug_default_pass_camera_visible = debug_camera_visualization;
	debug_isoline_pass_camera_visible = false;
	debug_gradient_pass_camera_visible = false;

	scene_visible = debug_default_pass_scene_visible;
	debug_plane_visible = debug_default_pass_plane_visible;
	debug_camera_visible = debug_default_pass_camera_visible;
}



////// beleuchtungskram.glsl

vec3 debugIsolineTexture(float sdf_dist, vec3 camera_pos, float camera_dist) {
	float small_lines = abs(sin(Pi * 10. * sdf_dist));
	small_lines = 1. - (1. - smoothstep(8., 15., camera_dist)) * (1. - small_lines);

	float medium_lines = abs(sin(Pi * 1. * sdf_dist));
	medium_lines = 1. - (.8 + .2 * smoothstep(6., 10., camera_dist)) * (1. - smoothstep(60., 80., camera_dist)) * (1. - medium_lines);

	float big_lines = abs(sin(Pi * 1./10. * sdf_dist));
	big_lines = 1. - (.8 + .2 * smoothstep(30., 50., camera_dist)) * (1. - smoothstep(80., 150., camera_dist)) * (1. - big_lines);

	float height = fDebugPlane(camera_pos);

	vec3 lines_color = vec3(0.);
	vec3 near_color = vec3(0.29804, 0.18824, 0.43922);
	vec3 far_color = vec3(0.12549, 0.52941, 0.36078);
	vec3 inner_color = vec3(0.02095, 0.19046, 0.60548);

	vec3 base_color = mix(near_color, far_color, smoothstep(.1 * height, height, sdf_dist));
	if (sdf_dist < 0.) {
		base_color = inner_color;
	}
	base_color = rgb2hsv(base_color);
	base_color.y *= 1. - smoothstep(height, 10. * height, abs(sdf_dist)); // desaturate
	base_color = hsv2rgb(base_color);

	base_color = mix(lines_color, base_color, small_lines);
	base_color = mix(lines_color, base_color, medium_lines);
	base_color = mix(lines_color, base_color, big_lines);

	return base_color;
}

vec3 debugIsolineTextureFiltered(vec3 p, vec3 camera_pos, float camera_dist) {
	scene_visible = debug_isoline_pass_scene_visible;
	debug_plane_visible = debug_isoline_pass_plane_visible;
	debug_camera_visible = debug_isoline_pass_camera_visible;

	float sdf_dist = fMain(p, false);
	vec3 sdf_normal = sdfNormal(p);

	scene_visible = debug_default_pass_scene_visible;
	debug_plane_visible = debug_default_pass_plane_visible;
	debug_camera_visible = debug_default_pass_camera_visible;

	vec3 pX = dFdx(p);
	vec3 pY = dFdy(p);

	/*
	float detail = 100.;
	int MaxSamples = 10;
	int sx = 1 + clamp( int( detail*length(pX) ), 0, MaxSamples-1 );
	int sy = 1 + clamp( int( detail*length(pY) ), 0, MaxSamples-1 );
	// */

	//*
	// fuck it - just supersample everything!
	int sx = 5;
	int sy = 5;
	// */

	vec3 no = vec3(0);
	for(int j = 0; j < sy; j++ ) {
		for(int i = 0; i < sx; i++ ) {
			vec2 st = (vec2(i, j) + .5)/vec2(sx, sy) - .5;
			vec3 delta = st.x * pX + st.y * pY;
			float f_dist = sdf_dist + dot(sdf_normal, delta);
			no += debugIsolineTexture(f_dist, camera_pos, camera_dist);
		}
	}

	return no / float(sx*sy);
}

vec3 debugColorIsolines(vec3 origin, float marched, vec3 hit) {
	return debugIsolineTextureFiltered(hit, origin, marched);
}

vec3 debugColorGradient(vec3 p) {
	scene_visible = debug_gradient_pass_scene_visible;
	debug_plane_visible = debug_gradient_pass_plane_visible;
	debug_camera_visible = debug_gradient_pass_camera_visible;

	vec3 gradient = sdfGradient(p);
	float len_grad = length(gradient);

	scene_visible = debug_default_pass_scene_visible;
	debug_plane_visible = debug_default_pass_plane_visible;
	debug_camera_visible = debug_default_pass_camera_visible;

	vec3 under_color = vec3(0.18014, 0.74453, 0.03288);
	vec3 over_color = vec3(0.71547, 0.03995, 0.02537);

	vec3 base_color = vec3(1.);
	base_color = mix(base_color, under_color, 1. - smoothstep(.8, 1., len_grad));
	base_color = mix(base_color, over_color, smoothstep(1., 1.2, len_grad));

	return base_color;
}

vec3 applyLightsDebugCamera(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId) {
	Material mat = defaultMaterial(vec3(0.));
	mat.metallic = 1.;
	mat.roughness = 0.076;

	vec3 color_camera = vec3(0.076, 0.434, 0.024);
	vec3 color_crane = vec3(0.015, 0.065, 0.045);
	vec3 color_target = vec3(0.783, 0.035, 0.011);

	if (materialId.id == debug_camera_id) {
		mat.color = color_camera;
		mat.emission = color_camera;
	} else if (materialId.id == debug_crane_base_id) {
		mat.color = color_crane;
		mat.emission = color_crane;
	} else if (materialId.id == debug_crane_arm_id) {
		mat.color = color_crane;
		mat.emission = color_crane;
	} else if (materialId.id == debug_tracking_target_id) {
		mat.color = color_target;
		mat.emission = color_target;
	}

	vec3 result = vec3(0.);
	result += ambientColor(normal, -direction, mat);
	result += mat.emission;

	result *= 0.200;
	result /= exp2(camera_exposure_rt_float);
	return result;
}

Material getMaterial(MaterialId materialId);
vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material);
vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color);

layout(location = 202) uniform float main_marching_distance;

// Bump Mapping Unparametrized Surfaces on the GPU [Mikkelsen2010]
void perturbNormal(vec3 p, inout vec3 n, float height) {
	vec3 sigma_s = dFdx(p);
	vec3 sigma_t = dFdy(p);
	vec3 a = cross(sigma_t, n);
	vec3 b = cross(n, sigma_s);
	float determinant = dot(sigma_s, a);
	float d_beta_s = dFdx(height);
	float d_beta_t = dFdy(height);
	vec3 surface_gradient = (d_beta_s * a + d_beta_t * b) * sign(determinant);
	vec3 n_perturbed = normalize(abs(determinant) * n - surface_gradient);
	n = n_perturbed;
}

void main() {
	setDebugParameters();

	vec3 origin = camGetPosition();
	vec3 direction = camGetDirection();
	float marched;
	bool hit = sdfMarch(origin, direction, main_marching_distance, marched);

	float trace_marched;
	vec3 trace_normal;
	bool trace_hit = false;

	if (extbg_enabled_rt_bool) {
		trace_hit = traceScenePlane(origin, direction, main_marching_distance, trace_marched, trace_normal);

		if (trace_hit) {
			if (!hit) {
				hit = true;
				marched = trace_marched;
			} else {
				if (trace_marched < marched) {
					marched = trace_marched;
				} else {
					trace_hit = false;
				}
			}
		}
	}

	if (!hit) {
		out_color = environmentColor(origin, direction, main_marching_distance);
		out_depth = main_marching_distance;
	} else {
		vec3 hit = origin + marched * direction;

		// next line is needed for calculating materials
		float marching_error = trace_hit ? 0. : fMain(hit, true);
		MaterialId materialId = trace_hit ? newMaterialId(mat_id_ground, hit) : current_material;

		vec3 normal = trace_hit ? trace_normal : sdfNormalForeward(hit, direction);

		if (materialId.id == debug_plane_material_id) {
			vec3 c_isoline = debugColorIsolines(origin, marched, hit);
			if (debug_gradient_visualization) {
				vec3 c_gradient = debugColorGradient(hit);
				c_isoline = mix(c_isoline, c_gradient, .5);
			}
			Material m_isoline = defaultMaterial(c_isoline);
			m_isoline.roughness = 1.;
			out_color = ambientColor(normal, -direction, m_isoline);
		} else if (materialId.id >= debug_camera_id && materialId.id <= debug_tracking_target_id) {
			out_color = applyLightsDebugCamera(origin, marched, direction, hit, normal, materialId);
		} else {
			if (debug_gradient_visualization) {
				vec3 c_gradient = debugColorGradient(hit);
				Material m_gradient = defaultMaterial(c_gradient);
				m_gradient.roughness = 0.;
				out_color = ambientColor(normal, -direction, m_gradient);
			} else {
				Material material = getMaterial(materialId);
				// TODO move this somewhere else
				// should be done for debug materials too to avoid false differentials
				//perturbNormal(hit, normal, material.height);
				material.roughness = square(material.roughness);
				out_color = applyLights(origin, marched, direction, hit, normal, materialId, material);
			}
		}
		out_depth = marched;
	}
	out_color = applyAfterEffects(origin, marched, direction, out_color);
	out_color *= exp2(camera_exposure_rt_float);
}

float shadowMarchAdvanced(vec3 o, vec3 d, float t_min, float t_max, int max_iterations, float omega) {
	// o, d : ray origin, direction (normalized)
	// t_min, t_max: minimum, maximum t values
	// pixelRadius: radius of a pixel at t = 1
	// forceHit: boolean enforcing to use the
	//           candidate_t value as result
	float t = t_min;
	float candidate_error = Inf;
	float previousRadius = 0;
	float stepLength = 0;
	float functionSign = sgn(fMain(o + t_min*d, false));

	for (int i = 0; i < max_iterations; ++i) {
		float signedRadius = functionSign * fMain(d*t + o, false);
		float radius = abs(signedRadius);

		bool sorFail = omega > 1 && (radius + previousRadius) < stepLength;
		if (sorFail) {
			stepLength -= omega * stepLength;
			omega = 1;
		} else {
			stepLength = signedRadius * omega;

			float error = radius / t;
			candidate_error = min(candidate_error, error);

			if (error < 0.0001) {
				return 0.;
			}
		}

		previousRadius = radius;
		t += stepLength;

		if (t > t_max) {
			break;
		}
	}

	return candidate_error;
}

float shadowMarch(vec3 o, vec3 d, float t_max, float softness) {
	float shadowing_value = shadowMarchAdvanced(o, d, .01, t_max, 256, 1.2);

	return min(0.244, softness * shadowing_value);
}

#line 3

vec3 applyPointLightWithShadow(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, PointLight light, float shadow_softness) {
	vec3 light_result = applyPointLight(origin, marched, direction, hit, normal, material, light);

	// shadows
	vec3 lightVector = normalize(light.position - hit);
	float lightDistance = distance(light.position, hit);
	float shadowing_value = shadowMarch(hit, lightVector, lightDistance, shadow_softness);
	light_result *= shadowing_value;

	return light_result;
}

vec3 applySphereLightWithShadow(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, SphereLight light, float shadow_softness) {
	vec3 light_result = applySphereLight(origin, marched, direction, hit, normal, material, light);

	// shadows
	vec3 lightVector = normalize(light.position - hit);
	float lightDistance = distance(light.position, hit);
	float shadowing_value = shadowMarch(hit, lightVector, lightDistance, shadow_softness);
	light_result *= shadowing_value;

	return light_result;
}

vec3 applyExtruderLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = vec3(0.);

	vec3 emission = material.emission;
	result += emission;

	vec3 ambient_lighting = ambientColor(normal, -direction, material);
	//if (materialId.id == mat_id_ground) {
		result += ambient_lighting;
	//}

	SphereLight light1 = SphereLight(extlight_1_pos_rt_vec3, extlight_1_radius_rt_float, extlight_1_color_rt_color, extlight_1_power_rt_float*1000.);
	vec3 sphere_lighting1 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light1, extlight_shadow_softness_rt_float);
	result += sphere_lighting1;

	SphereLight light2 = SphereLight(extlight_2_pos_rt_vec3, extlight_2_radius_rt_float, extlight_2_color_rt_color, extlight_2_power_rt_float*1000.);
	vec3 sphere_lighting2 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light2, extlight_shadow_softness_rt_float);
	result += sphere_lighting2;

	SphereLight light3 = SphereLight(extlight_3_pos_rt_vec3, extlight_3_radius_rt_float, extlight_3_color_rt_color, extlight_3_power_rt_float*1000.);
	vec3 sphere_lighting3 = applySphereLightWithShadow(origin, marched, direction, hit, normal, material, light3, extlight_shadow_softness_rt_float);
	result += sphere_lighting3;

	return result;
}

float f_ext_background(vec3 p) {
	vec3 p_bg = p;
	pTrans(p_bg, extbg_offset_rt_vec3);
	vec2 i_bg = pDomrep(p_bg.xz, extbg_domrep_rt_vec2);
	pRotY(p_bg, 9781 * i_bg.y + 31 + extbg_rot_anim_rt_float);
	pRotX(p_bg, 4514 * i_bg.x + 89 + extbg_rot_anim_rt_float);
	float f_bg = fTetrahedron(p_bg, extbg_tetra_r_rt_float);
	return f_bg;
}

#line 4

const float mat_id_plane = 0.;
const float mat_id_ext = 1.;

float pDomrepGridOnlyX(inout vec2 p, float c) {
	float i = pDomrepMirror(p.x, c);
	p.x -= .5 * c;
	if (p.x > p.y) {
		p.xy = p.yx;
	}
	return floor(.5 * i);
}

float rot_cell = mand_rot_cell_rt_float;
vec2 trans_inner = mand_trans_inner_rt_vec2;

MatWrap wMandala(vec3 p) {
	// domain geschwurbel
	vec3 p_cyl = p;

	pRotY(p_cyl, Tau * mand_rot_global_rt_float);

	// domrep angle
	float c_circle = pDomrepAngle(p_cyl.zx, mand_domrep_angle_num_rt_float, mand_domrep_angle_r_rt_float);

	pTrans(p_cyl.x, 4.);

	// grid domrep
	float c_grid = pDomrepGridOnlyX(p_cyl.xz, 8.);

	pRotY(p_cyl, Tau * rot_cell);
	pTrans(p_cyl.xz, trans_inner);

	// distance function geschwurbel
	pRotX(p_cyl, 2. * mand_rot_3D_rt_float * Tau);
	pRotZ(p_cyl,      mand_rot_3D_rt_float * Tau);
	float f_cyl = f2Box(p_cyl.zx, 1., 2.*Golden_Ratio);
	float c_distfunc = pDomrepInterval(f_cyl, mand_f_domrep_rt_float, 0., 1.);
	f_cyl = f2Box(vec2(f_cyl, p_cyl.y), mand_f_thickness_rt_float);

	MatWrap w_cyl = MatWrap(f_cyl, MaterialId(mat_id_ext, p_cyl, vec4(c_circle, c_grid, c_distfunc, 0.)));
	return w_cyl;
}

float fScene(vec3 p) {
	vec3 p_ext = p;
	//pTrans(p_ext.y, 10.);

	// extruder geschwurbel
	pTrans(p_ext.x, -ext_extrude_h_rt_float);
	float px_before = p_ext.x;
	float px_clamped = clamp(p_ext.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_ext.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	pRotX(p_ext, .5 * (px_param + ext_rot_rt_float) * Tau);
	/*
	pTrans(p_ext.z, 10 * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_ext,      (px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_ext, 4. * (px_param + ext_rot_rt_float) * Tau);
	// */

	rot_cell = .3 * sin(1. * (px_param + ext_rot_rt_float) * Tau) + mand_rot_cell_rt_float;
	trans_inner = vec2(1., 3.6) * sin(1. * (px_param + ext_rot_rt_float) * Tau) + mand_trans_inner_rt_vec2;


	// assemble object
	MatWrap w_ext = wMandala(p_ext.yxz);

	// ground plane
	float f_ground = p.x - mandext_plane_offset_rt_float;
	MatWrap w_ground = MatWrap(f_ground, newMaterialId(mat_id_plane, p));

	// combine everything
	MatWrap w = w_ext;
	w = mUnion(w, w_ground);

	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = applyExtruderLights(origin, marched, direction, hit, normal, materialId, material);
	return result;
}

float fGeo(vec2 p) {
	float rot_angle = (-mand_rot_global_rt_float + .5 * (ext_extrude_h_rt_float + ext_rot_rt_float) + geo_rotation_offset_rt_float) * Tau;
	pRot(p, rot_angle);

	float radius = geo_radius_rt_float * mand_domrep_angle_r_rt_float;
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

		pMirrorTrans(f, geo_thickness_rt_float);

	} else {
		float box_offset = geo_box_offset_rt_float * mand_domrep_angle_r_rt_float;
		float f1 = f2Quadprism(p - vec2(box_offset, 0.), radius - box_offset);
		float f2 = f2Quadprism(p + vec2(box_offset, 0.), radius - box_offset);

		pMirrorTrans(f1, geo_thickness_rt_float);
		pMirrorTrans(f2, geo_thickness_rt_float);

		f = min(f1, f2);
	}

	f = smoothstep(-geo_thickness_rt_float * .01, 0., -f);

	return f;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	if (geo_enabled_rt_bool) {
		for (int i = 0; i < geo_plane_num_rt_float; i++) {
			// http://geomalgorithms.com/a05-_intersect-1.html
			float intersection_dist = ((geo_plane_xpos_rt_float + i * geo_plane_distance_rt_float) - origin.x) / direction.x;
			if (intersection_dist > 0. && intersection_dist < marched) {
				vec2 p = (origin + intersection_dist * direction).zy;
				float hipster_shit = fGeo(p);

				float brightness = geo_brightness_rt_float * .001 * square(float(i+1)/geo_plane_num_rt_float);
				color += hipster_shit * brightness * geo_color_rt_color;
			}
		}
	}

	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));

	if (materialId.id == mat_id_ext) {
		float c_circle = (materialId.misc.x + floor(mand_domrep_angle_num_rt_float/2.));// / (mand_domrep_angle_num_rt_float-1);
		float c_grid = materialId.misc.y / 2. + .5; // 0,1
		float c_distfunc = materialId.misc.z; // null oder 1 nicer wert

		if (c_distfunc > 0) {
			mat.color = 2* mand_color_circle_rt_color;
		} else {
			mat.color = mand_color_2_rt_color;
		}
		if (c_grid > 0) {
			mat.color += 0.5;
		}

		mat.metallic = 1.;
		mat.roughness = mandlight_obj_roughness_rt_float;
	} else if (materialId.id == mat_id_plane) {
		mat.color = extbg_ground_color_rt_color;
		mat.metallic = 0.;
		mat.roughness = extbg_ground_roughness_rt_float;
	}

	return mat;
}
