#include "quat_rotate.glsl"
#include "helper.glsl"
#line 4 "sdf/domain"

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

// cut through a plane at point a with normal normalize(v) and translate positive
// halfspace by v, filling the gap with the extruded cut plane
float pCutAndExtrude(inout vec3 p, vec3 a, vec3 v) {
	pTrans(p, a);
	float h = saturate(dot(p, v)/ dot(v, v));
	p -= v * h;
	pTrans(p, -a);
	return h;
}
