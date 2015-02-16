#version 430
uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat

uniform float camera_focal_length;

layout(location = 0) uniform vec2 res;

float TAU = 6.28318530718;

// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
vec3 quat_rotate(vec3 v, vec4 q) {
	vec3 t = 2 * cross(q.xyz, v);
	return v + q.w * t + cross(q.xyz, t);
	// *hex hex*
}

vec3 get_direction(out float screenDist) {
	vec3 dir = vec3((gl_FragCoord.xy - .5 * res) / res.x , -camera_focal_length / .03);
	screenDist = length(vec2(dir.xz));
	dir = normalize(dir);
	return quat_rotate(dir, camera_rotation);
}

vec2 f(vec3 p, bool last_step);

float g(vec3 p) {
	return f(p, false)[0];
}

// ein fachmenschich kopierter marchingloop
float march_adv(vec3 o, vec3 d, float t_min, float t_max, float pixelRadius, int max_iterations, float omega, bool forceHit) {
	// o, d : ray origin, direction (normalized)
	// t_min, t_max: minimum, maximum t values
	// pixelRadius: radius of a pixel at t = 1
	// forceHit: boolean enforcing to use the
	//           candidate_t value as result
	float t = t_min;
	float candidate_error = 1.0/0.0;
	float candidate_t = t_min;
	float previousRadius = 0;
	float stepLength = 0;
	float functionSign = g(o) < 0 ? -1 : +1;
	for (int i = 0; i < max_iterations; ++i) {
		float signedRadius = functionSign * g(d*t + o);
		float radius = abs(signedRadius);
		bool sorFail = omega > 1 &&
			(radius + previousRadius) < stepLength;
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
		if (!sorFail && error < pixelRadius || t > t_max)
			break;
		t += stepLength;
	}
	if ((t > t_max || candidate_error > pixelRadius) &&
			!forceHit) return 1./0.;
	return candidate_t;
}

float march(vec3 o, vec3 d, float t_max, float screenDistX) {
	return march_adv(o, d, .001, t_max, screenDistX/res.x, 128, 1.2, false);
}

vec3 calc_normal(vec3 p, bool last_step) {
	vec2 e = vec2(.001, 0.); // no epilepsilon this time
	return normalize(vec3(
		f(p + e.xyy, last_step)[0] - f(p - e.xyy, last_step)[0],
		f(p + e.yxy, last_step)[0] - f(p - e.yxy, last_step)[0],
		f(p + e.yyx, last_step)[0] - f(p - e.yyx, last_step)[0]
	));
}

float sphere(vec3 p, float s) {
	return length(p) - s;
}

float torus(vec3 p, vec2 t) {
	vec2 q = vec2(length(p.xz) - t.x, p.y);
	return length(q) - t.y;
}

float length8(vec2 p) {
	p *= p;
	p *= p;
	p *= p;
	return pow(p.x + p.y, 1./8.);
}

float torus82(vec3 p, vec2 t) {
	vec2 q = vec2(length(p.xz) - t.x, p.y);
	return length8(q) - t.y;
}

// h.x: height of zweieck, h.y: extruding length
float zweieck(vec3 p , vec2 h) {
	vec3 q = abs(p);
	q.x -= -h.x / 2.;
	float sp = length(q.xy) - h.x;

	float extrude = q.z - h.y;

	return max(sp, extrude);
}

// h.x: (fast) height of eineck, h.y: extruding length
float eineck(vec3 p, vec2 h) {
	float halbkreis = min(length(p.xy) - h.x * .5, -p.y);
	halbkreis = max(halbkreis, zweieck(p, h));

	float extrude = abs(p.z) - h.y;

	return max(halbkreis, extrude);
}

// h.x: width of triangle, h.y: extruding length
float triprism(vec3 p, vec2 h) {
    vec3 q = abs(p);
    return max(q.z - h.y, max(q.x * .866025 + p.y * .5, -p.y) - h.x);
}

// h.x: width of pentagon, h.y: extruding length
float pentaprism(vec3 p, vec2 h) {
	float phi1 = radians(108. / 2.);
	float phi2 = radians(-18.);

	vec3 q = abs(p);
	float side1 = q.x * cos(phi1) + p.y * sin(phi1);
	float side2 = -p.y;
	float side3 = q.x * cos(phi2) + p.y * sin(phi2);

	float pentagon = max(max(side1, side2), side3) - h.x;

	float extrude = q.z - h.y;

	return max(pentagon, extrude);
}

// h.x: width of hexagon, h.y: extruding length
float hexprism(vec3 p, vec2 h) {
    vec3 q = abs(p);
    return max(q.z - h.y, max((q.x * .866025 + q.y * .5), q.y) - h.x);
}

// smooth minimum, k is the difference between the two values for which to smooth (eg. k = 0.1)
float smin(float a, float b, float k) {
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );
	return mix(b, a, h) - k * h * (1.0 - h);
}

// smooth maximum, k is the difference between the two values for which to smooth (eg. k = 0.1)
float smax(float a, float b, float k) {
	float h = clamp(0.5 - 0.5 * (b - a) / k, 0.0, 1.0 );
	return mix(b, a, h) + k * h * (1.0 - h);
}

float box(vec3 p, vec3 b) {
	p = abs(p) - b;
	return max(p.x, max(p.y, p.z));
}

float box2(vec2 p, vec2 b) {
	p = abs(p) - b;
	return max(p.x, p.y);
}

// more accurate than box(), but slower
float slowbox(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));
}

// box with rounded corners, r is radius of corners
float roundbox(vec3 p, vec3 b, float r) {
	return slowbox(p, b - r) - r;
}

vec3 domrep(vec3 p, vec3 c) {
	return mod(p, c) - .5 * c;
}

// repeat things
vec3 domrep(vec3 p, float x, float y, float z) {
	return domrep(p, vec3(x, y, z));
}

// hier kommt der witz!
vec2 min_material(vec2 a, vec2 b) {
	return mix(a, b, float(a.x > b.x));
}
// kam er?

// f: distance function to object
// p: evaluation point
// s: scale factor
#define scale(f, p, s) f((p)/(s))*(s)

// trans*late things - using vectors!!
// p: point
// v: translation vector
vec3 trans(vec3 p, vec3 v) {
	return p - v;
}

// trans*late things
// p: point
// x: x
// y: y
// z: z
vec3 trans(vec3 p, float x, float y, float z) {
	return trans(p, vec3(x, y, z));
}

mat3 rX(float theta) {
	return mat3(
		1., 0., 0.,
		0., cos(theta), sin(theta),
		0., -sin(theta), cos(theta)
	);
}

mat3 rY(float theta) {
	return mat3(
		cos(theta), 0., -sin(theta),
		0., 1., 0.,
		sin(theta), 0., cos(theta)
	);
}

mat3 rZ(float theta) {
	return mat3(
		cos(theta), sin(theta), 0.,
		-sin(theta), cos(theta), 0.,
		0., 0., 1.
	);
}

mat2 rot2D(float theta) {
	return mat2(cos(theta), -sin(theta), sin(theta), cos(theta));
}