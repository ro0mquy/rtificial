//#version 430

////////////// helper.glsl

//uniform float time;

#line 8

const float Pi = 3.14159265359;
const float Tau = 6.28318530718;
const float Euler = 2.71828182846;
const float GoldenRatio = 1.61803398875;

float minV(vec2 v) {
	return min(v.x, v.y);
}

float minV(vec3 v) {
	return min(min(v.x, v.y), v.z);
}

float maxV(vec2 v) {
	return max(v.x, v.y);
}

float maxV(vec3 v) {
	return max(max(v.x, v.y), v.z);
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

float square(float x) {
	return x * x;
}

vec2 square(vec2 x) {
	return x * x;
}

vec3 square(vec3 x) {
	return x * x;
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

/*
float linstep(float edge0, float edge1, float x) {
	return clamp((x - edge0) / (edge1 - edge0), 0., 1.);
}
*/

float pdot(vec2 a, vec2 b) {
	return max(0., dot(a, b));
}

float pdot(vec3 a, vec3 b) {
	return max(0., dot(a, b));
}

float sgn(float x) {
	return x < 0. ? -1. : 1.;
}

// m: anything above stays unchanged
// n: value at x = 0
// x: x
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqAlmostIdentity(float m, float n, float x) {
	if(x > m) return x;

	const float a = 2. * n - m;
	const float b = 2. * m - 3. * n;
	const float t = x / m;

	return (a * t + b) * t * t + n;
}

// k: width of impulse, maximum happens at 1/k
// x: x
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqImpulse(float k, float x) {
	const float h = k * x;
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
    const float k = pow(a + b, a + b) / (pow(a, a) * pow(b, b));
    return k * pow(x, a) * pow(1. - x, b);
}

// a, b: how much to bend in left and right direction
// x: x
// not normalized version
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float iqPowerCurveUnnormalized(float a, float b, float x) {
    return pow(x, a) * pow(1. - x, b);
}

float rgb2luma(vec3 rgb) {
	return dot(rgb, vec3(.2126, .7152, .0722)); // magic luminance formular
}

vec3 rgb2hsv(vec3 c) {
    const vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    const vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    const vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    const float d = q.x - min(q.w, q.y);
    const float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c) {
    const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    const vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


//////// raymarchingkram.glsl

// like normal max()-intersection but with correct distance at corners
// use only with orthogonal objects
float opIntersectEuclid(float f1, float f2) {
	const vec2 q = vec2(f1, f2);
	return min(maxV(q), 0.) + length(max(q, 0.));
}

float fSphere(vec3 p, float r) {
	return length(p) - r;
}

float fSphere2(vec2 p, float r) {
	return length(p) - r;
}

// capped cylinder, h is half height
float fCylinder(vec3 p, float r, float h) {
	const float sp2 = fSphere2(p.xz, r);
	const float y = abs(p.y) - h;
	return opIntersectEuclid(sp2, y);
}

float fBox(vec3 p, vec3 r) {
	const vec3 q = abs(p) - r;
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

float fBoxEdge(vec3 p, vec3 r) {
	return maxV(abs(p) - r);
}

float fBoxEdge(vec3 p, float rx, float ry, float rz) {
	return fBoxEdge(p, vec3(rx, ry, rz));
}

float fBoxEdge(vec3 p, float r) {
	return fBoxEdge(p, vec3(r));
}

float fBox2(vec2 p, vec2 r) {
	const vec2 q = abs(p) - r;
	return min(maxV(q), 0.) + length(max(q, 0.));
}

float fBox2(vec2 p, float rx, float ry) {
	return fBox2(p, vec2(rx, ry));
}

float fBox2(vec2 p, float r) {
	return fBox2(p, vec2(r));
}

float fBoxRounded2(vec2 p, vec2 r, float rCorner) {
	return fBox2(p, r - rCorner) - rCorner;
}

float fBoxEdge2(vec2 p, vec2 r) {
	return maxV(abs(p) - r);
}

float fBoxEdge2(vec2 p, float rx, float ry) {
	return fBoxEdge2(p, vec2(rx, ry));
}

float fBoxEdge2(vec2 p, float r) {
	return fBoxEdge2(p, vec2(r));
}

float fTorus(vec3 p, float rBig, float rSmall) {
	// also try replacing fSphere2 by something like fBox2/fBoxEdge2/fBoxRounded2
	const vec2 q = vec2(fSphere2(p.xz, rBig), p.y);
	return fSphere2(q, rSmall);
}

float fTorus(vec3 p, vec2 r) {
	return fTorus(p, r.x, r.y);
}

float fTorusBox(vec3 p, float rBig, float rSmall) {
	const vec2 q = vec2(fBoxEdge2(p.xz, rBig), p.y);
	return fBox2(q, rSmall);
}

float fTorusSphereBox(vec3 p, float rBig, float rSmall) {
	const vec2 q = vec2(fSphere2(p.xz, rBig), p.y);
	return fBox2(q, rSmall);
}
