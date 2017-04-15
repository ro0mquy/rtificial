#version 430
layout(location = 67) uniform float camera_shake_intensity_rt_float;
layout(location = 68) uniform float camera_shake_freq_rt_float;

layout(location = 0) uniform vec2 res;
vec2 tc = gl_FragCoord.xy / res;

layout(location = 64) uniform float time;

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

#line 5

layout(binding = 29) uniform sampler2D color; // vec3
layout(binding = 28) uniform sampler2D depth; // float

layout(location = 85) uniform vec3 camera_focus_target;
layout(location = 255) uniform float camera_focus_stop;
layout(location = 87) uniform bool post_disable_dof;

float focus_dist = distance(camGetPosition(), camera_focus_target);

layout(location = 0) out vec4 out_color_and_coc;

void main() {
	if (post_disable_dof) {
		return;
	}
	float depthValue = textureLod(depth, tc, 0).x;

	float c = (abs(depthValue - focus_dist) / depthValue)
		* (square(camera_focal_length) / (camera_focus_stop * (focus_dist - camera_focal_length)));
	float percentOfSensor = c / .024; // sensor height
	float maxCoC = .05;
	float blurFactor = clamp(percentOfSensor, 0., maxCoC);

	out_color_and_coc.rgb = textureLod(color, tc, 0).rgb;
	out_color_and_coc.a = blurFactor;
}
