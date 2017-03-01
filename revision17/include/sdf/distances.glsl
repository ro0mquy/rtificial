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
