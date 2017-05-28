#version 430
layout(location = 88) uniform vec3 post_credits_logo_color_rt_color; // color
layout(location = 89) uniform bool post_border_blend_overlay_rt_bool;
layout(location = 90) uniform float post_border_x_rt_float;
layout(location = 91) uniform float post_remap_value_top_rt_float;
layout(location = 92) uniform float post_remap_value_bottom_rt_float;
layout(location = 93) uniform vec3 post_remap_rgb_top_rt_vec3;
layout(location = 94) uniform vec3 post_remap_rgb_bottom_rt_vec3;
layout(location = 95) uniform float post_credits_alpha_rt_float;
layout(location = 96) uniform float post_credits_penta_r_rt_float;
layout(location = 97) uniform vec2 post_credits_penta_pos_rt_vec2;
layout(location = 98) uniform float extlogo_lt_down_bend_angle_rt_float;
layout(location = 99) uniform vec2 post_credits_logo_pos_rt_vec2;
layout(location = 100) uniform float post_credits_penta_dr_rt_float;
layout(location = 101) uniform vec3 bg_color2_rt_color; // color
layout(location = 102) uniform float extlogo_lt_down_bend_offset_rt_float;
layout(location = 103) uniform float extlogo_lt_down_thickness_factor_rt_float;
layout(location = 104) uniform float post_remap_value_skew_rt_float;
layout(location = 105) uniform float extlogo_lt_up_cut_angle_rt_float;
layout(location = 106) uniform float post_credits_penta_alpha_rt_float;
layout(location = 107) uniform float extlogo_lt_angle_rt_float;
layout(location = 108) uniform float extlogo_lt_down_bend_scale_rt_float;
layout(location = 109) uniform float extlogo_V_angle_rt_float;
layout(location = 110) uniform float post_credits_logo_scale_rt_float;
layout(location = 111) uniform float extlogo_thickness_rt_float;
layout(location = 112) uniform float post_border_y_rt_float;
layout(location = 113) uniform vec3 pal_amplitude_rt_color; // color
layout(location = 114) uniform bool post_credits_visible_rt_bool;
layout(location = 115) uniform vec2 extlogo_lt_origin_rt_vec2;
layout(location = 116) uniform vec3 post_credits_penta_color_rt_color; // color
layout(location = 117) uniform vec2 extlogo_lt_up_cut_pos_rt_vec2;
layout(location = 118) uniform vec3 post_border_color_rt_color; // color
layout(location = 119) uniform float extlogo_lt_cutout_right_rt_float;
layout(location = 120) uniform vec2 extlogo_lt_down_cut_pos_rt_vec2;
layout(location = 121) uniform float extlogo_V_height_rt_float;
layout(location = 122) uniform float extlogo_lt_down_cut_angle_rt_float;
layout(location = 123) uniform vec3 pal_phase_rt_vec3;
layout(location = 124) uniform vec3 pal_base_rt_color; // color
layout(location = 125) uniform float extlogo_lt_cutout_left_rt_float;
layout(location = 126) uniform vec3 pal_frequency_rt_vec3;
layout(location = 127) uniform float extlogo_lt_angle_global_rt_float;
layout(location = 128) uniform vec3 bg_color1_rt_color; // color

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

// TODO figure out how to get rid of this special include file

// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
void pQuatRotate(inout vec3 v, vec4 q) {
	vec3 t = 2 * cross(q.xyz, v);
	v += q.w * t + cross(q.xyz, t);
	// *hex hex*
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

#line 7
// lens distort, vignette, tonemapping, color grading, noise

layout(binding = 50) uniform sampler2D color; // vec3
layout(location = 0) out vec3 out_color;

layout(location = 129) uniform float post_image_distortion;
layout(location = 130) uniform float post_lens_distort_k;
layout(location = 131) uniform float post_lens_distort_kcube;
layout(location = 132) uniform float post_vignette_intensity;
layout(location = 133) uniform float post_vignette_distortion;
layout(location = 134) uniform float post_film_grain_intensity;
layout(location = 135) uniform float post_film_grain_frequency;
layout(location = 136) uniform float post_film_grain_power;
layout(location = 137) uniform bool post_disable_grain;

layout(location = 138) uniform vec3 post_colorgrading_slope; // color
layout(location = 139) uniform vec3 post_colorgrading_power; // color
layout(location = 140) uniform vec3 post_colorgrading_offset; // color

layout(location = 141) uniform vec3 post_colorgrading_lift; // color
layout(location = 142) uniform vec3 post_colorgrading_gamma; // color
layout(location = 143) uniform vec3 post_colorgrading_gain; // color

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
			out_color = mix(out_color, post_credits_penta_color_rt_color, post_credits_penta_alpha_rt_float);
		}
		if (f_penta_2 < 0) {
			out_color = mix(out_color, post_credits_penta_color_rt_color, post_credits_penta_alpha_rt_float);
		}

		vec2 p2_logo = p2;
		pTrans(p2_logo, post_credits_logo_pos_rt_vec2);
		p2_logo.x = -p2_logo.x;
		if (f2Logo(p2_logo*post_credits_logo_scale_rt_float*10) < 0) {
			out_color = mix(out_color, post_credits_logo_color_rt_color, post_credits_alpha_rt_float);
		}
	}

}
