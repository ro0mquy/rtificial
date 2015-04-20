//#version 430
#line 3

////////////// helper.glsl

//uniform float time;

#line 8

const float Pi = 3.14159265359;
const float Tau = 6.28318530718;
const float Euler = 2.71828182846;
const float GoldenRatio = 1.61803398875;
const float Inf = 1./0.;

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

vec2 unitVector(float phi) {
	return vec2(cos(phi), sin(phi));
}

vec3 unitVector(float phi, float theta) {
	float ct = cos(theta);
	float sp = sin(phi);
	float st = sin(theta);
	float cp = cos(phi);
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


//////// raymarchingkram.glsl


void pTrans(inout float p, float d) {
	p -= d;
}

void pTrans(inout vec2 p, vec2 d) {
	p -= d;
}

void pTrans(inout vec3 p, vec3 d) {
	p -= d;
}

// rotates clockwise when looking in the direction given by the right-hand rule
// don't use this directly (unless for 2d coordinates)!
void pRot(inout vec2 p, float phi) {
	p *= mat2(cos(phi), sin(phi), -sin(phi), cos(phi));
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

vec3 pDomrep(inout vec3 p, vec3 c) {
	p += .5 * c;
	vec3 i = floor(p/c);
	p = mod(p, c) - .5 * c;
	return i;
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

float pDomrepAngleWithAtan(inout vec2 p, float repetitions, float preCalcAtan) {
	float at = preCalcAtan;
	float cAngle = Tau / repetitions;
	float i = pDomrep(at, cAngle);

	float r = length(p);
	p = r * unitVector(at);
	return i;
}

float pDomrepAngle(inout vec2 p, float repetitions) {
	return pDomrepAngleWithAtan(p, repetitions, atan(p.y, p.x));
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

void pMirrorLoco(inout float p, float c) {
	p = abs(p) - c;
}

void pMirrorLoco(inout vec2 p, vec2 c) {
	p = abs(p) - c;
	if (p.y > p.x) {
		p.xy = p.yx;
	}
}

void pMirrorLoco(inout vec3 p, vec3 c) {
	vec3 d = abs(p) - c;
	if (d.x < d.y) {
		if (d.y < d.z) {
			p = d;
		} else {
			if (d.x < d.z) {
				p = d.xzy;
			} else {
				p = d.zxy;
			}
		}
	} else {
		if (d.z < d.y) {
			p = d.zyx;
		} else {
			if (d.z < d.x) {
				p = d.yzx;
			} else {
				p = d.yxz;
			}
		}
	}
}

// like normal max()-intersection but with correct distance at corners
// use only with orthogonal objects
float opIntersectEuclid(float f1, float f2) {
	vec2 q = vec2(f1, f2);
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
	float sp2 = fSphere2(p.xz, r);
	float y = abs(p.y) - h;
	return opIntersectEuclid(sp2, y);
}

// capped cylinder, h is half height
float fCylinderEdge(vec3 p, float r, float h) {
	float sp2 = fSphere2(p.xz, r);
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

float fCorner2(vec2 p) {
	return min(maxV(p), 0.) + length(max(p, 0.));
}

float fCornerRounded2(vec2 p, float r) {
	return fCorner2(p + r) - r;
}

float fCornerEdge2(vec2 p) {
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
	vec2 q = abs(p) - r;
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
	vec2 q = vec2(fSphere2(p.xz, rBig), p.y);
	return fSphere2(q, rSmall);
}

float fTorus(vec3 p, vec2 r) {
	return fTorus(p, r.x, r.y);
}

float fTorusBox(vec3 p, float rBig, float rSmall) {
	vec2 q = vec2(fBoxEdge2(p.xz, rBig), p.y);
	return fBox2(q, rSmall);
}

float fTorusSphereBox(vec3 p, float rBig, float rSmall) {
	vec2 q = vec2(fSphere2(p.xz, rBig), p.y);
	return fBox2(q, rSmall);
}

float fPlane(vec3 p, vec3 n) {
	// n must be normalized
	return dot(p, n);
}

float fPlaneAngle(vec3 p, float phi, float theta) {
	return fPlane(p, unitVector(phi, theta));
}

float fPlane2(vec2 p, vec2 n) {
	// n must be normalized
	return dot(p, n);
}

float fPlaneAngle2(vec2 p, float phi) {
	return fPlane2(p, unitVector(phi));
}

float fTriprism2(vec2 p, float r) {
	return max(fPlaneAngle2(vec2(abs(p.x), p.y), radians(30)), -p.y) - .5 * r;
}

// capped triprism, h is half height
float fTriprism(vec3 p, float r, float h) {
	float tri2 = fTriprism2(p.xz, r);
	float y = abs(p.y) - h;
	return opIntersectEuclid(tri2, y);
}

// capped triprism, h is half height
float fTriprismEdge(vec3 p, float r, float h) {
	float tri2 = fTriprism2(p.xz, r);
	float y = abs(p.y) - h;
	return max(tri2, y);
}
