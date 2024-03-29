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


const int numSamples = 13;

vec4 dof(sampler2D colorSampler, sampler2D depthSampler, float angle) {
	float bleedingBias = 0.02;
	float bleedingMult = 30.;

	vec4 centerPixel = textureLod(colorSampler, tc, 0);
	float centerDepth = textureLod(depthSampler, tc, 0).r;

	vec4 color = vec4(0);
	float totalWeight = 0;

	float radius = .5;
	float aspectRatio = res.x / res.y;
	vec2 pt = radius * vec2(cos(angle), sin(angle));
	pt.x /= aspectRatio;

	for (int i = 0; i < numSamples; i++) {
		float t = float(i) / (numSamples - 1.);
		vec2 offset = mix(-pt, pt, t);
		vec2 sampleCoords = tc + offset * centerPixel.a;
		vec4 samplePixel = textureLod(colorSampler, sampleCoords, 0);
		float sampleDepth = textureLod(depthSampler, sampleCoords, 0).r;

		float weight = sampleDepth < centerDepth ? samplePixel.a * bleedingMult : 1.;
		weight = (centerPixel.a > samplePixel.a + bleedingBias) ? weight : 1.;

		// karis average for better stability
		weight /= 1. + rgb2luma(samplePixel.rgb);

		color += samplePixel * weight;
		totalWeight += weight;
	}

	return color / totalWeight;
}

#line 4

layout(binding = 32) uniform sampler2D color_and_coc; // vec4
layout(binding = 28) uniform sampler2D depth; // float
layout(location = 0) out vec4 out_color_and_coc;
layout(location = 87) uniform bool post_disable_dof;

void main() {
	if (post_disable_dof) {
		return;
	}
	out_color_and_coc = dof(color_and_coc, depth, Tau / 4.);
}
