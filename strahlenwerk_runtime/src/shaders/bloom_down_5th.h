#ifndef bloom_down_5th_H
#define bloom_down_5th_H
const char bloom_down_5th_source[] =R"shader_source(#version 430
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
R"shader_source(vec3 karis_average(vec3 a, vec3 b, vec3 c, vec3 d) {
)shader_source"
R"shader_source(	float weights = 0.;
)shader_source"
R"shader_source(	float weight_a = 1/(1 + rgb2luma(a));
)shader_source"
R"shader_source(	float weight_b = 1/(1 + rgb2luma(b));
)shader_source"
R"shader_source(	float weight_c = 1/(1 + rgb2luma(c));
)shader_source"
R"shader_source(	float weight_d = 1/(1 + rgb2luma(d));
)shader_source"
R"shader_source(	return (a * weight_a + b * weight_b + c * weight_c + d * weight_d)
)shader_source"
R"shader_source(		/ (weight_a + weight_b + weight_c + weight_d);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 average(vec3 a, vec3 b, vec3 c, vec3 d, bool karis) {
)shader_source"
R"shader_source(	if (karis) {
)shader_source"
R"shader_source(		return karis_average(a, b, c, d);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		return .25 * a + .25 * b + .25 * c + .25 * d;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 downsample(sampler2D inputTexture, vec2 bigPixelCenter, vec2 smallPixelSize, bool karis) {
)shader_source"
R"shader_source(	vec3 topLeftOuter = textureLod(inputTexture, bigPixelCenter - 2. * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 topRightOuter = textureLod(inputTexture, bigPixelCenter + vec2(2., -2.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottomLeftOuter = textureLod(inputTexture, bigPixelCenter + vec2(-2., 2.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottomRightOuter = textureLod(inputTexture, bigPixelCenter + 2. * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 top = textureLod(inputTexture, bigPixelCenter - vec2(0., 1.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 right = textureLod(inputTexture, bigPixelCenter + vec2(1., 0.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 left = textureLod(inputTexture, bigPixelCenter - vec2(1., 0.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottom = textureLod(inputTexture, bigPixelCenter + vec2(0., 1.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 center = textureLod(inputTexture, bigPixelCenter, 0.).rgb;
)shader_source"
R"shader_source(	vec3 topLeftInner = textureLod(inputTexture, bigPixelCenter - smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 topRightInner = textureLod(inputTexture, bigPixelCenter + vec2(1., -1.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottomLeftInner = textureLod(inputTexture, bigPixelCenter + vec2(-1., 1.) * smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(	vec3 bottomRightInner = textureLod(inputTexture, bigPixelCenter + smallPixelSize, 0.).rgb;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 middleResult      = average(topLeftInner, topRightInner, bottomLeftInner, bottomRightInner, karis);
)shader_source"
R"shader_source(	vec3 topLeftResult     = average(topLeftOuter, top, left, center, karis);
)shader_source"
R"shader_source(	vec3 topRightResult    = average(topRightOuter, top, right, center, karis);
)shader_source"
R"shader_source(	vec3 bottomLeftResult  = average(bottomLeftOuter, bottom, left, center, karis);
)shader_source"
R"shader_source(	vec3 bottomRightResult = average(bottomRightOuter, bottom, right, center, karis);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return middleResult * .5 + .125 * topLeftResult + .125 * topRightResult + .125 * bottomLeftResult + .125 * bottomRightResult;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 5
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 17) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	out_color = downsample(color, tc, .5/res, false);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// level(5)
)shader_source"
;
#endif
