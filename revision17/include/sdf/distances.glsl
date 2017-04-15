#include "helper.glsl"
#include "sdf/operators.glsl"
#include "sdf/domain.glsl"
#line 5

float fSphere(vec3 p, float r) {
	return length(p) - r;
}

float f2Sphere(vec2 p, float r) {
	return length(p) - r;
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

// r is the radius from the origin to the vertices
float f2Hexprism(vec2 p, float r) {
	float offset = r * cos(Tau / 6. / 2.);
    vec2 q = abs(p);
	float side1 = f2PlaneAngle(q, radians(30.));
	float side2 = q.y;
	float hexagon = max(side1, side2) - offset;
    return hexagon;
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
//
// l: half distance of focus points, r: radius
float fSpheroid(vec3 p, float l, float r) {
	vec3 q1 = p, q2 = p;
	q1.x -= l;
	q2.x -= -l;

	float f = length(q1) + length(q2);
	f = f * .5 - r;
	return f;
}
