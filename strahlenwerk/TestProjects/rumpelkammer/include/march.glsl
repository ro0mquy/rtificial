#version 430
#line 3 "march"
float smin(float a, float b, float k) {
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );
	return mix(b, a, h) - k * h * (1.0 - h);
}

////////////// helper.glsl

out vec3 out_color;

uniform float time;

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

float linstep(float edge0, float edge1, float x) {
	return clamp((x - edge0) / (edge1 - edge0), 0., 1.);
}

float smootherstep(float edge0, float edge1, float x) {
	// directly from master Ken
	float t = clamp((x - edge0) / (edge1 - edge0), 0., 1.);
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


//////// camera.glsl


layout(location = 0) uniform vec2 res;

uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat
uniform float camera_focal_length;

// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
vec3 pQuatRotate(vec3 v, vec4 q) {
	vec3 t = 2 * cross(q.xyz, v);
	return v + q.w * t + cross(q.xyz, t);
	// *hex hex*
}

float camGetDirection(out vec3 dir) {
	dir.xy = (gl_FragCoord.xy - .5 * res) / res.x;
	dir.z = -camera_focal_length / .03;
	float screen_dist = length(vec2(dir.xz));
	dir = normalize(dir);
	dir = pQuatRotate(dir, camera_rotation);
	return screen_dist;
}


//////// raymarchingkram.glsl

/// materialkram.glsl

struct Material {
	float id;
	vec3 coord;
};

bool calculate_material = false;
float current_dist = Inf;
Material current_material = Material(0., vec3(0.));

void mUnion(float f, Material m) {
	if (calculate_material) {
		if (f < current_dist) {
			current_dist = f;
			current_material = m;
		}
	} else {
		current_dist = min(current_dist, f);
	}
}

void mIntersect(float f, Material m) {
	if (calculate_material) {
		if (f > current_dist) {
			current_dist = f;
			current_material = m;
		}
	} else {
		current_dist = max(current_dist, f);
	}
}


/// sdfkram.glsl

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

vec2 pMirrorLoco(inout vec2 p, vec2 c) {
	vec2 s = pMirrorTrans(p, c);
	if (p.y > p.x) {
		p.xy = p.yx;
	}
	return s;
}

vec3 pMirrorLoco(inout vec3 p, vec3 c) {
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
	return s;
}

float pMirrorAtPlane(inout vec3 p, vec3 planeNormal, float offset) {
	float dist = dot(p, planeNormal) + offset;
	if (dist < 0.) {
		p -= 2. * dist * planeNormal;
	}
	return sgn(dist);
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

// r is the radius from the origin to the vertices
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

// r is the radius from the origin to the vertices
// just like a rotated fBoxEdge2
float fQuadprism2(vec2 p, float r) {
	float offset = r * sqrt(.5);
	vec2 q = abs(p);
	float quad = fPlane2(q, vec2(sqrt(.5))) - offset;
	return quad;
}

// r is the radius from the origin to the vertices
float fPentaprism2(vec2 p, float r) {
	float phi1 = radians(108. / 2.);
	float phi2 = radians(-18.);
	float offset = r * cos(Tau / 5. / 2.);

	vec2 q = vec2(abs(p.x), p.y);
	float side1 = fPlaneAngle2(q, phi1);
	float side2 = -p.y;
	float side3 = fPlaneAngle2(q, phi2);

	float pentagon = max3(side1, side2, side3) - offset;

	return pentagon;
}

// capped pentaprism, h is half height
float fPentaprism(vec3 p, float r, float h) {
	float penta2 = fPentaprism2(p.xz, r);
	float y = abs(p.y) - h;
	return opIntersectEuclid(penta2, y);
}

// capped pentaprism, h is half height
float fPentaprismEdge(vec3 p, float r, float h) {
	float penta2 = fPentaprism2(p.xz, r);
	float y = abs(p.y) - h;
	return max(penta2, y);
}

// r is the radius from the origin to the vertices
float fHexprism2(vec2 p, float r) {
	float offset = r * cos(Tau / 6. / 2.);
    vec2 q = abs(p);
	float side1 = fPlaneAngle2(q, radians(30.));
	float side2 = q.y;
	float hexagon = max(side1, side2) - offset;
    return hexagon;
}

// capped hexprism, h is half height
float fHexprism(vec3 p, float r, float h) {
	float hex2 = fHexprism2(p.xz, r);
	float y = abs(p.y) - h;
	return opIntersectEuclid(hex2, y);
}

// capped hexprism, h is half height
float fHexprismEdge(vec3 p, float r, float h) {
	float hex2 = fHexprism2(p.xz, r);
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
float fSupershape2(vec2 p, float a, float b, float m, float n1, float n2, float n3) {
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

float fTorusPartial(vec3 p, float rBig, float rSmall, float halfAngle) {
	float r = length(p.xz);
	float angle = atan(p.z, p.x);

	angle -= clamp(angle, -halfAngle, halfAngle);

	p.xz = r * unitVector(angle);
	p.x -= rBig;

	return fSphere(p, rSmall);
}

// n is plane normal and must be normalized
float fCone(vec3 p, vec2 n) {
	vec2 q = vec2(length(p.xz), p.y);
	return fPlane2(q, n);
}

float fConeAngle(vec3 p, float angle) {
	vec2 q = vec2(length(p.xz), p.y);
	return fPlaneAngle2(q, angle);
}

// line from origin to v, inflated by r
float fLine(vec3 p, float r, vec3 v) {
	float h = saturate(dot(p, v)/ dot(v, v));
	return fSphere(p - v*h, r);
}

// line along x-axis, form -h to h
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
	float plane1 = fPlane2(q.xy, vec2(.816496581, .577350269)); // cos(phi), sin(phi)
	float plane2 = fPlane2(q.zy, vec2(.816496581, .577350269));
	float f_oktaeder = max(plane1, plane2);
	return f_oktaeder;
}





/// marchingloopkram.glsl

uniform float debug_mode;
uniform vec3 debug_plane_normal;
uniform float debug_plane_height;

const float debug_plane_material_id = 42.;

bool debug_default_pass_scene_visible = true;
bool debug_default_pass_plane_visible = false;
bool debug_isoline_pass_scene_visible = false;
bool debug_isoline_pass_plane_visible = false;
bool debug_gradient_visualization = false;

bool scene_visible = debug_default_pass_scene_visible;
bool debug_plane_visible = debug_default_pass_plane_visible;

float fScene(vec3 p);

float fDebugPlane(vec3 p) {
	return abs(fPlane(p, normalize(debug_plane_normal)) - debug_plane_height);
}

float fMain(vec3 p, bool calc_m) {
	current_dist = Inf;
	calculate_material = calc_m;

	if (debug_plane_visible) {
		mUnion(fDebugPlane(p), Material(debug_plane_material_id, p));
	}

	if (scene_visible) {
		fScene(p);
	}

	return current_dist;
}

vec3 sdfRawNormal(vec3 p, float e) {
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
	return normalize(sdfRawNormal(p, epsilon));
}

vec3 sdfNormal(vec3 p) {
	return sdfNormal(p, .001);
}

vec3 sdfGradient(vec3 p, float epsilon) {
	return sdfRawNormal(p, epsilon) / (2. * epsilon);
}

vec3 sdfGradient(vec3 p) {
	return sdfGradient(p, .001);
}

// ein fachmenschich kopierter marchingloop
float sdfMarchAdvanced(vec3 o, vec3 d, float t_min, float t_max, float pixelRadius, int max_iterations, float omega, bool forceHit) {
	// o, d : ray origin, direction (normalized)
	// t_min, t_max: minimum, maximum t values
	// pixelRadius: radius of a pixel at t = 1
	// forceHit: boolean enforcing to use the
	//           candidate_t value as result
	float t = t_min;
	float candidate_error = Real_Inf;
	float candidate_t = t_min;
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

	if ((t > t_max || candidate_error > pixelRadius) && !forceHit) {
		return Real_Inf;
	}

	return candidate_t;
}

float sdfMarch(vec3 o, vec3 d, float t_max, float screenDistX) {
	return sdfMarchAdvanced(o, d, .001, t_max, screenDistX/res.x*.5, 128, 1.2, false);
}

void setDebugParameters() {
	int mode = int(debug_mode);
	switch (mode) {
		case 0: // default
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			break;
		case 1: // debug plane
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			break;
		case 2: // debug plane without scene geometry
			debug_default_pass_scene_visible = false;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			break;
		case 3: // visualize gradient length
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = true;
			break;
		default: // same as default
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			break;
	}

	scene_visible = debug_default_pass_scene_visible;
	debug_plane_visible = debug_default_pass_plane_visible;
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
	vec3 near_color = debug_color_iso_near_rt_color; // vec3(0.47044, 0.07593, 0.00259) // vec3(0.13035, 0.00080, 0.35865)
	vec3 far_color = debug_color_iso_far_rt_color; // vec3(0.30663, 0.72992, 0.01794) // vec3(0.01794, 0.72992, 0.21204)
	vec3 inner_color = debug_color_iso_inner_rt_color;

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

	float sdf_dist = fMain(p, false);
	vec3 sdf_normal = sdfNormal(p);

	scene_visible = debug_default_pass_scene_visible;
	debug_plane_visible = debug_default_pass_plane_visible;

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

vec3 debugColorIsolines(vec3 p, vec3 camera_pos, float camera_dist) {
	return debugIsolineTextureFiltered(p, camera_pos, camera_dist);
}

vec3 debugColorGradient(vec3 p) {
	vec3 gradient = sdfGradient(p);
	float len_grad = length(gradient);

	vec3 under_color = debug_color_grad_under_rt_color;
	vec3 over_color = debug_color_grad_over_rt_color;

	vec3 base_color = vec3(1.);
	base_color = mix(base_color, under_color, 1. - smoothstep(.9, 1., len_grad));
	base_color = mix(base_color, over_color, smoothstep(1., 1.1, len_grad));

	return base_color;
}
