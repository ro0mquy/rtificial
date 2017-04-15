#version 430

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

void min_max(float a, float b, out float min_out, out float max_out) {
	min_out = min(a, b);
	max_out = max(a, b);
}

float median(float elements[9]) {
	float min10, max10;
	min_max(elements[1], elements[2], min10, max10);
	float min11, max11;
	min_max(elements[4], elements[5], min11, max11);
	float min12, max12;
	min_max(elements[7], elements[8], min12, max12);

	float min20, max20;
	min_max(elements[0], min10, min20, max20);
	float min21, max21;
	min_max(elements[3], min11, min21, max21);
	float min22, max22;
	min_max(elements[6], min12, min22, max22);

	float min30, max30;
	min_max(max20, max10, min30, max30);
	float min31, max31;
	min_max(max21, max11, min31, max31);
	float min32, max32;
	min_max(max22, max12, min32, max32);

	float max40 = max(min20, min21);
	float min41 = min(max31, max32);

	float min50, max50;
	min_max(min31, min32, min50, max50);

	float max60 = max(max40, min22);
	float max61 = max(min30, min50);
	float min62 = min(max30, min41);

	float min70 = min(max61, max50);

	float min80, max80;
	min_max(min70, min62, min80, max80);

	float max90 = max(max60, min80);

	return min(max90, max80);
}


#line 5

layout(binding = 27) uniform sampler2D color; // vec3
layout(location = 0) out vec3 out_color;

layout(location = 144) uniform bool post_disable_median;

void main() {
	if (post_disable_median) {
		out_color = textureLod(color, tc, 0).rgb;
		return;
	}

	vec2 pixelSize = 1. / textureSize(color, 0);
	vec2 base = tc - pixelSize;
	vec3 colors[9];
	float values[9];
	float avg_value = 0;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			vec3 sampleValue = textureLod(color, base + vec2(x, y) * pixelSize, 0).rgb;
			colors[y * 3 + x] = sampleValue;
			values[y * 3 + x] = rgb2luma(colors[y * 3 + x]);
			avg_value += values[y * 3 + x] * (1./9);
		}
	}

	out_color = colors[5];
	if (values[5] > avg_value * 2 || isinf(values[5]) || isnan(values[5])) {
		float median_luma = median(values);
		for (int i = 0; i < 9; i++) {
			if ((values[i] - median_luma) < 1e-6) {
				out_color = colors[i];
			}
		}
	}
}