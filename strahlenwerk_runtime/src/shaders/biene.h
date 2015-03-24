#ifndef biene_H
#define biene_H
const char biene_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(layout(location = 77) uniform float biene_fluegel_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 78) uniform float biene_fluegel_end_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 79) uniform float biene_fluegel_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 80) uniform vec2 biene_fluegel_c_rt_vec2;
)shader_source"
R"shader_source(layout(location = 81) uniform float biene_body_abdomen_offset_rt_float;
)shader_source"
R"shader_source(layout(location = 82) uniform float biene_body_head_stretch_rt_float;
)shader_source"
R"shader_source(layout(location = 83) uniform float biene_dellen_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 84) uniform float biene_body_radius_rt_float;
)shader_source"
R"shader_source(layout(location = 85) uniform float biene_dellen_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 86) uniform float biene_fluegel_angle_rt_float;
)shader_source"
R"shader_source(layout(location = 87) uniform float biene_body_head_radius_rt_float;
)shader_source"
R"shader_source(layout(location = 88) uniform float biene_fluegel_thick_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 89) uniform float biene_body_length_rt_float;
)shader_source"
R"shader_source(layout(location = 90) uniform float biene_body_max_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 91) uniform float biene_body_bend_rt_float;
)shader_source"
R"shader_source(layout(location = 92) uniform float biene_fluegel_raise_rt_float;
)shader_source"
R"shader_source(layout(location = 93) uniform float biene_body_thorax_stretch_rt_float;
)shader_source"
R"shader_source(layout(location = 94) uniform float biene_dellen_space_rt_float;
)shader_source"
R"shader_source(layout(location = 95) uniform float biene_dellen_depth_rt_float;
)shader_source"
R"shader_source(layout(location = 96) uniform float biene_body_head_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 97) uniform float biene_body_abdomen_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 98) uniform float biene_body_thorax_scale_rt_float;
)shader_source"
R"shader_source(layout(location = 99) uniform float biene_fluegel_rot_rt_float;
)shader_source"
R"shader_source(layout(location = 100) uniform float biene_fluegel_start_rt_float;
)shader_source"
R"shader_source(layout(location = 101) uniform float biene_body_min_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 102) uniform float biene_fluegel_length_rt_float;
)shader_source"
R"shader_source(layout(location = 103) uniform vec3 camera_position;
)shader_source"
R"shader_source(layout(location = 104) uniform vec4 camera_rotation; // quat
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 105) uniform float camera_focal_length;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 66) uniform float time;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float TAU = 6.28318530718;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
)shader_source"
R"shader_source(vec3 quat_rotate(vec3 v, vec4 q) {
)shader_source"
R"shader_source(	vec3 t = 2 * cross(q.xyz, v);
)shader_source"
R"shader_source(	return v + q.w * t + cross(q.xyz, t);
)shader_source"
R"shader_source(	// *hex hex*
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 get_direction(out float screenDist) {
)shader_source"
R"shader_source(	vec3 dir = vec3((gl_FragCoord.xy - .5 * res) / res.x , -camera_focal_length / .03);
)shader_source"
R"shader_source(	screenDist = length(vec2(dir.xz));
)shader_source"
R"shader_source(	dir = normalize(dir);
)shader_source"
R"shader_source(	return quat_rotate(dir, camera_rotation);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p, bool last_step);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float g(vec3 p) {
)shader_source"
R"shader_source(	return f(p, false)[0];
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// ein fachmenschich kopierter marchingloop
)shader_source"
R"shader_source(float march_adv(vec3 o, vec3 d, float t_min, float t_max, float pixelRadius, int max_iterations, float omega, bool forceHit) {
)shader_source"
R"shader_source(	// o, d : ray origin, direction (normalized)
)shader_source"
R"shader_source(	// t_min, t_max: minimum, maximum t values
)shader_source"
R"shader_source(	// pixelRadius: radius of a pixel at t = 1
)shader_source"
R"shader_source(	// forceHit: boolean enforcing to use the
)shader_source"
R"shader_source(	//           candidate_t value as result
)shader_source"
R"shader_source(	float t = t_min;
)shader_source"
R"shader_source(	float candidate_error = 1.0/0.0;
)shader_source"
R"shader_source(	float candidate_t = t_min;
)shader_source"
R"shader_source(	float previousRadius = 0;
)shader_source"
R"shader_source(	float stepLength = 0;
)shader_source"
R"shader_source(	float functionSign = g(o) < 0 ? -1 : +1;
)shader_source"
R"shader_source(	for (int i = 0; i < max_iterations; ++i) {
)shader_source"
R"shader_source(		float signedRadius = functionSign * g(d*t + o);
)shader_source"
R"shader_source(		float radius = abs(signedRadius);
)shader_source"
R"shader_source(		bool sorFail = omega > 1 &&
)shader_source"
R"shader_source(			(radius + previousRadius) < stepLength;
)shader_source"
R"shader_source(		if (sorFail) {
)shader_source"
R"shader_source(			stepLength -= omega * stepLength;
)shader_source"
R"shader_source(			omega = 1;
)shader_source"
R"shader_source(		} else {
)shader_source"
R"shader_source(			stepLength = signedRadius * omega;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(		previousRadius = radius;
)shader_source"
R"shader_source(		float error = radius / t;
)shader_source"
R"shader_source(		if (!sorFail && error < candidate_error) {
)shader_source"
R"shader_source(			candidate_t = t;
)shader_source"
R"shader_source(			candidate_error = error;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(		if (!sorFail && error < pixelRadius || t > t_max)
)shader_source"
R"shader_source(			break;
)shader_source"
R"shader_source(		t += stepLength;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	if ((t > t_max || candidate_error > pixelRadius) &&
)shader_source"
R"shader_source(			!forceHit) return 1./0.;
)shader_source"
R"shader_source(	return candidate_t;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float march(vec3 o, vec3 d, float t_max, float screenDistX) {
)shader_source"
R"shader_source(	return march_adv(o, d, .001, t_max, screenDistX/res.x*.5, 128, 1.2, false);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 calc_normal(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	vec2 e = vec2(.001, 0.); // no epilepsilon this time
)shader_source"
R"shader_source(	return normalize(vec3(
)shader_source"
R"shader_source(		f(p + e.xyy, last_step)[0] - f(p - e.xyy, last_step)[0],
)shader_source"
R"shader_source(		f(p + e.yxy, last_step)[0] - f(p - e.yxy, last_step)[0],
)shader_source"
R"shader_source(		f(p + e.yyx, last_step)[0] - f(p - e.yyx, last_step)[0]
)shader_source"
R"shader_source(	));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float sphere(vec3 p, float s) {
)shader_source"
R"shader_source(	return length(p) - s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float cylinder(vec2 p, float s) {
)shader_source"
R"shader_source(	return length(p) - s;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float torus(vec3 p, vec2 t) {
)shader_source"
R"shader_source(	vec2 q = vec2(length(p.xz) - t.x, p.y);
)shader_source"
R"shader_source(	return length(q) - t.y;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float length8(vec2 p) {
)shader_source"
R"shader_source(	p *= p;
)shader_source"
R"shader_source(	p *= p;
)shader_source"
R"shader_source(	p *= p;
)shader_source"
R"shader_source(	return pow(p.x + p.y, 1./8.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float torus82(vec3 p, vec2 t) {
)shader_source"
R"shader_source(	vec2 q = vec2(length(p.xz) - t.x, p.y);
)shader_source"
R"shader_source(	return length8(q) - t.y;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// c must be normalized
)shader_source"
R"shader_source(float cone(vec3 p, vec2 c) {
)shader_source"
R"shader_source(	float q = length(p.xy);
)shader_source"
R"shader_source(	return dot(c, vec2(q, p.z));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// h.x: height of zweieck, h.y: extruding length
)shader_source"
R"shader_source(float zweieck(vec3 p , vec2 h) {
)shader_source"
R"shader_source(	vec3 q = abs(p);
)shader_source"
R"shader_source(	q.x -= -h.x / 2.;
)shader_source"
R"shader_source(	float sp = length(q.xy) - h.x;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float extrude = q.z - h.y;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return max(sp, extrude);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// h.x: (fast) height of eineck, h.y: extruding length
)shader_source"
R"shader_source(float eineck(vec3 p, vec2 h) {
)shader_source"
R"shader_source(	float halbkreis = min(length(p.xy) - h.x * .5, -p.y);
)shader_source"
R"shader_source(	halbkreis = max(halbkreis, zweieck(p, h));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float extrude = abs(p.z) - h.y;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return max(halbkreis, extrude);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// h.x: width of triangle, h.y: extruding length
)shader_source"
R"shader_source(float triprism(vec3 p, vec2 h) {
)shader_source"
R"shader_source(    vec3 q = abs(p);
)shader_source"
R"shader_source(    return max(q.z - h.y, max(q.x * .866025 + p.y * .5, -p.y) - h.x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// h.x: width of pentagon, h.y: extruding length
)shader_source"
R"shader_source(float pentaprism(vec3 p, vec2 h) {
)shader_source"
R"shader_source(	float phi1 = radians(108. / 2.);
)shader_source"
R"shader_source(	float phi2 = radians(-18.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 q = abs(p);
)shader_source"
R"shader_source(	float side1 = q.x * cos(phi1) + p.y * sin(phi1);
)shader_source"
R"shader_source(	float side2 = -p.y;
)shader_source"
R"shader_source(	float side3 = q.x * cos(phi2) + p.y * sin(phi2);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float pentagon = max(max(side1, side2), side3) - h.x;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float extrude = q.z - h.y;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return max(pentagon, extrude);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// h.x: width of hexagon, h.y: extruding length
)shader_source"
R"shader_source(float hexprism(vec3 p, vec2 h) {
)shader_source"
R"shader_source(    vec3 q = abs(p);
)shader_source"
R"shader_source(    return max(q.z - h.y, max((q.x * .866025 + q.y * .5), q.y) - h.x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// s: side length
)shader_source"
R"shader_source(float oktaeder(vec3 p, float s) {
)shader_source"
R"shader_source(	vec3 q = abs(p);
)shader_source"
R"shader_source(	q.y -= .707106781 * s; // sqrt(2) / 2 * s
)shader_source"
R"shader_source(	//float phi = TAU/4. - acos(-1./3.) * .5;
)shader_source"
R"shader_source(	float plane1 = dot(q.xy, vec2(.816496581, .577350269)); // cos(phi), sin(phi)
)shader_source"
R"shader_source(	float plane2 = dot(q.zy, vec2(.816496581, .577350269));
)shader_source"
R"shader_source(	float f_oktaeder = max(plane1, plane2);
)shader_source"
R"shader_source(	return f_oktaeder;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// awesome supershapes directly at your hands!
)shader_source"
R"shader_source(// a and b control the total size
)shader_source"
R"shader_source(// m is the number of spikes
)shader_source"
R"shader_source(// n1, n2, n3 control the exact shape
)shader_source"
R"shader_source(// http://paulbourke.net/geometry/supershape/
)shader_source"
R"shader_source(// http://de.wikipedia.org/wiki/Superformel
)shader_source"
R"shader_source(// have fun playing around!
)shader_source"
R"shader_source(float supershape(vec2 p, float a, float b, float m, float n1, float n2, float n3) {
)shader_source"
R"shader_source(	const float phi = atan(p.y, p.x);
)shader_source"
R"shader_source(	const float d = length(p);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float m4 = m / 4.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float c = cos(m4 * phi);
)shader_source"
R"shader_source(	const float s = sin(m4 * phi);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float ca = c / a;
)shader_source"
R"shader_source(	const float sb = s / b;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float gc = ca < 0. ? -1. : 1.;
)shader_source"
R"shader_source(	const float gs = sb < 0. ? -1. : 1.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float absc = ca * gc;
)shader_source"
R"shader_source(	const float abss = sb * gs;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float ab2 = pow(absc, n2);
)shader_source"
R"shader_source(	const float ab3 = pow(abss, n3);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//const float ab21 = pow(absc, n2 - 1.);
)shader_source"
R"shader_source(	//const float ab31 = pow(abss, n3 - 1.);
)shader_source"
R"shader_source(	const float ab21 = ab2 / absc;
)shader_source"
R"shader_source(	const float ab31 = ab3 / abss;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float rw = ab2 + ab3;
)shader_source"
R"shader_source(	const float r = pow(rw, -1./n1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float k = -n2 * ab21 * gc / a * s;
)shader_source"
R"shader_source(	const float l =  n3 * ab31 * gs / b * c;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//const float drpre = m4 / n1 * pow(rw, -1./n1 - 1.);
)shader_source"
R"shader_source(	const float drpre = m4 / n1 * r / rw;
)shader_source"
R"shader_source(	const float dr2 = drpre * drpre * (k * k + 2. * k * l + l * l);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	const float f = (d - r) / sqrt(1 + dr2);
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// smooth minimum, k is the difference between the two values for which to smooth (eg. k = 0.1)
)shader_source"
R"shader_source(float smin(float a, float b, float k) {
)shader_source"
R"shader_source(	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );
)shader_source"
R"shader_source(	return mix(b, a, h) - k * h * (1.0 - h);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 smin_material(vec2 a, vec2 b, float k) {
)shader_source"
R"shader_source(	return vec2(smin(a.x, b.x, k), a.x > b.x ? b.y : a.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// be careful when nesting! (just don't)
)shader_source"
R"shader_source(vec2 smin_smaterial(vec2 a, vec2 b, float k) {
)shader_source"
R"shader_source(	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );
)shader_source"
R"shader_source(	return vec2(mix(b.x, a.x, h) - k * h * (1.0 - h), mix(b.y, a.y, h));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// smooth maximum, k is the difference between the two values for which to smooth (eg. k = 0.1)
)shader_source"
R"shader_source(float smax(float a, float b, float k) {
)shader_source"
R"shader_source(	float h = clamp(0.5 - 0.5 * (b - a) / k, 0.0, 1.0 );
)shader_source"
R"shader_source(	return mix(b, a, h) + k * h * (1.0 - h);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float box(vec3 p, vec3 b) {
)shader_source"
R"shader_source(	p = abs(p) - b;
)shader_source"
R"shader_source(	return max(p.x, max(p.y, p.z));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float box2(vec2 p, vec2 b) {
)shader_source"
R"shader_source(	p = abs(p) - b;
)shader_source"
R"shader_source(	return max(p.x, p.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// more accurate than box(), but slower
)shader_source"
R"shader_source(float slowbox(vec3 p, vec3 b) {
)shader_source"
R"shader_source(	vec3 d = abs(p) - b;
)shader_source"
R"shader_source(	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// box with rounded corners, r is radius of corners
)shader_source"
R"shader_source(float roundbox(vec3 p, vec3 b, float r) {
)shader_source"
R"shader_source(	return slowbox(p, b - r) - r;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// n must be normalized
)shader_source"
R"shader_source(float plane(vec3 p, vec3 n) {
)shader_source"
R"shader_source(	return dot(p, n.xyz);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 domrep(vec3 p, vec3 c) {
)shader_source"
R"shader_source(	return mod(p, c) - .5 * c;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// repeat things
)shader_source"
R"shader_source(vec3 domrep(vec3 p, float x, float y, float z) {
)shader_source"
R"shader_source(	return domrep(p, vec3(x, y, z));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 domrep(vec2 p, vec2 c) {
)shader_source"
R"shader_source(	return mod(p, c) - .5 * c;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 domrep(vec2 p, float x, float y) {
)shader_source"
R"shader_source(	return domrep(p, vec2(x, y));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float domrep(float p, float c) {
)shader_source"
R"shader_source(	return mod(p, c) - .5 * c;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// hier kommt der witz!
)shader_source"
R"shader_source(vec2 min_material(vec2 a, vec2 b) {
)shader_source"
R"shader_source(	return mix(a, b, float(a.x > b.x));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(// kam er?
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// f: distance function to object
)shader_source"
R"shader_source(// p: evaluation point
)shader_source"
R"shader_source(// s: scale factor
)shader_source"
R"shader_source(#define scale(f, p, s) f((p)/(s))*(s)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// trans*late things - using vectors!!
)shader_source"
R"shader_source(// p: point
)shader_source"
R"shader_source(// v: translation vector
)shader_source"
R"shader_source(vec3 trans(vec3 p, vec3 v) {
)shader_source"
R"shader_source(	return p - v;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// trans*late things
)shader_source"
R"shader_source(// p: point
)shader_source"
R"shader_source(// x: x
)shader_source"
R"shader_source(// y: y
)shader_source"
R"shader_source(// z: z
)shader_source"
R"shader_source(vec3 trans(vec3 p, float x, float y, float z) {
)shader_source"
R"shader_source(	return trans(p, vec3(x, y, z));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(mat3 rX(float theta) {
)shader_source"
R"shader_source(	return mat3(
)shader_source"
R"shader_source(		1., 0., 0.,
)shader_source"
R"shader_source(		0., cos(theta), sin(theta),
)shader_source"
R"shader_source(		0., -sin(theta), cos(theta)
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(mat3 rY(float theta) {
)shader_source"
R"shader_source(	return mat3(
)shader_source"
R"shader_source(		cos(theta), 0., -sin(theta),
)shader_source"
R"shader_source(		0., 1., 0.,
)shader_source"
R"shader_source(		sin(theta), 0., cos(theta)
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(mat3 rZ(float theta) {
)shader_source"
R"shader_source(	return mat3(
)shader_source"
R"shader_source(		cos(theta), sin(theta), 0.,
)shader_source"
R"shader_source(		-sin(theta), cos(theta), 0.,
)shader_source"
R"shader_source(		0., 0., 1.
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(mat2 rot2D(float theta) {
)shader_source"
R"shader_source(	return mat2(cos(theta), -sin(theta), sin(theta), cos(theta));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float line(vec3 pa, vec3 ba, float r) {
)shader_source"
R"shader_source(    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
)shader_source"
R"shader_source(    return length( pa - ba*h ) - r;
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
R"shader_source(#line 3
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(out vec4 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	vec3 o = camera_position;
)shader_source"
R"shader_source(	float screenDist;
)shader_source"
R"shader_source(	vec3 d = get_direction(screenDist);
)shader_source"
R"shader_source(	float t = march(o, d, 50., screenDist);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (isinf(t)) {
)shader_source"
R"shader_source(		out_color.rgb = vec3(0.);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 0.))), 0.) + .1);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 bee_body(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	vec3 p_thorax = p;
)shader_source"
R"shader_source(	p_thorax.z /= biene_body_thorax_stretch_rt_float;
)shader_source"
R"shader_source(	float thorax_radius = biene_body_max_thick_rt_float * biene_body_thorax_scale_rt_float;
)shader_source"
R"shader_source(	float thorax_length = thorax_radius * biene_body_thorax_stretch_rt_float;
)shader_source"
R"shader_source(	float d_thorax = sphere(p_thorax, thorax_radius);
)shader_source"
R"shader_source(	d_thorax *= min(biene_body_thorax_stretch_rt_float, 1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_head = p;
)shader_source"
R"shader_source(	float head_radius = thorax_radius * biene_body_head_radius_rt_float;
)shader_source"
R"shader_source(	float head_length = head_radius * biene_body_head_stretch_rt_float;
)shader_source"
R"shader_source(	p_head.z += thorax_length + head_length;
)shader_source"
R"shader_source(	p_head.z /= biene_body_head_stretch_rt_float;
)shader_source"
R"shader_source(	float d_head = sphere(p_head, head_radius);
)shader_source"
R"shader_source(	d_head *= min(biene_body_head_stretch_rt_float, 1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_abdomen = p;
)shader_source"
R"shader_source(	float abdomen_length = biene_body_length_rt_float + .5 * biene_body_radius_rt_float;
)shader_source"
R"shader_source(	p_abdomen.z -= (abdomen_length + thorax_length) * (1. + biene_body_abdomen_offset_rt_float);
)shader_source"
R"shader_source(	float t_body = clamp(p_abdomen.z / (biene_body_length_rt_float + .5 * biene_body_radius_rt_float), -1., 1.);
)shader_source"
R"shader_source(	p_abdomen.zy *= rot2D(radians(biene_body_bend_rt_float) * (t_body * .5 + .5));
)shader_source"
R"shader_source(	t_body = clamp(p_abdomen.z / (biene_body_length_rt_float + .5 * biene_body_radius_rt_float), -1., 1.);
)shader_source"
R"shader_source(	p_abdomen.xy /= (biene_body_max_thick_rt_float - biene_body_min_thick_rt_float) * (1. - t_body * t_body) + biene_body_min_thick_rt_float;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 a = vec3(0., 0., biene_body_length_rt_float);
)shader_source"
R"shader_source(	vec3 b = -a;
)shader_source"
R"shader_source(	float d_abdomen = line(p_abdomen - a, b - a, biene_body_radius_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	d_abdomen *= min(biene_body_min_thick_rt_float, 1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_dellen = p_abdomen;
)shader_source"
R"shader_source(	p_dellen.z = domrep(p_dellen.z, biene_dellen_space_rt_float);
)shader_source"
R"shader_source(	float d_dellen = max(
)shader_source"
R"shader_source(		cylinder(p_dellen.xy, biene_body_radius_rt_float * (1. + biene_dellen_depth_rt_float)),
)shader_source"
R"shader_source(		abs(p_dellen.z) - biene_dellen_thick_rt_float
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(	d_dellen = max(d_dellen, abs(p_abdomen.z) - biene_body_length_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	d_abdomen = smin(d_abdomen, d_dellen, biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float d_body = smin(d_abdomen, smin(d_thorax, d_head, biene_body_head_smooth_rt_float * head_radius),
)shader_source"
R"shader_source(		biene_body_abdomen_smooth_rt_float * biene_body_max_thick_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return vec2(d_body, 0.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 bee_fluegel(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	vec3 p_fluegel = p;
)shader_source"
R"shader_source(	p_fluegel.x = abs(p_fluegel.x);
)shader_source"
R"shader_source(	p_fluegel.zx *= rot2D(radians(1.) * biene_fluegel_rot_rt_float);
)shader_source"
R"shader_source(	p_fluegel.yz *= rot2D(radians(1.) * biene_fluegel_angle_rt_float);
)shader_source"
R"shader_source(	p_fluegel.yx *= rot2D(radians(1.) * biene_fluegel_raise_rt_float);
)shader_source"
R"shader_source(	p_fluegel.x -= biene_fluegel_start_rt_float;
)shader_source"
R"shader_source(	vec2 c = normalize(biene_fluegel_c_rt_vec2);
)shader_source"
R"shader_source(	float d_fluegel = cone(vec3(p_fluegel.z, p_fluegel.y, -p_fluegel.x), c);
)shader_source"
R"shader_source(	float fluegel_length = biene_fluegel_length_rt_float;
)shader_source"
R"shader_source(	float fluegel_thick = biene_fluegel_thick_rt_float;
)shader_source"
R"shader_source(	d_fluegel = smax(d_fluegel, cylinder(p_fluegel.xz - vec2(fluegel_length, 0.), fluegel_length),
)shader_source"
R"shader_source(		biene_fluegel_end_smooth_rt_float * fluegel_thick); // cap ends
)shader_source"
R"shader_source(	d_fluegel = smax(d_fluegel, abs(p_fluegel.y) - fluegel_thick,
)shader_source"
R"shader_source(		biene_fluegel_thick_smooth_rt_float * fluegel_thick); // cap bottom/top
)shader_source"
R"shader_source(	return vec2(d_fluegel, 0.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	vec2 body = bee_body(p, last_step);
)shader_source"
R"shader_source(	vec2 fluegel = bee_fluegel(p, last_step);
)shader_source"
R"shader_source(	return smin_material(body, fluegel, biene_fluegel_smooth_rt_float * biene_fluegel_thick_rt_float);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*
)shader_source"
R"shader_source(float expStep( float x, float k, float n ) {
)shader_source"
R"shader_source(    return exp( -k*pow(x,n) );
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 fluegel(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	p.x = abs(p.x);
)shader_source"
R"shader_source(	p.y /= .1;
)shader_source"
R"shader_source(	vec2 c = normalize(vec2(2., .4));
)shader_source"
R"shader_source(	float d = cone(vec3(p.z, p.y, -p.x), c);
)shader_source"
R"shader_source(	d = max(d , cylinder(p.xz - vec2(3.5, 0.), 3.));
)shader_source"
R"shader_source(	d *= .1;
)shader_source"
R"shader_source(	return vec2(d, 0.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 body(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	vec3 p_body = p;
)shader_source"
R"shader_source(	float t = clamp(p_body.z, -1. / .4, 1. / .4);
)shader_source"
R"shader_source(	float dellen = expStep(abs(fract(t * 3.) * 2. - 1.), 20., 5.);
)shader_source"
R"shader_source(	p_body.z -= (1. - pow(min(length(p_body.xy), 1.), 1.47)) * .6;
)shader_source"
R"shader_source(	p_body.z *= .4;
)shader_source"
R"shader_source(	float d_body = sphere(p_body, 1. - dellen * .03);
)shader_source"
R"shader_source(	return vec2(d_body, 0.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	float d = body(p, last_step)[0];
)shader_source"
R"shader_source(	return vec2(d, 0.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(*/
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*
)shader_source"
R"shader_source(vec2 f(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	float scale = (p.z < 0.) ? .5: 1.;
)shader_source"
R"shader_source(	p.z /= (1. + scale * smoothstep(0., 4., p.x));
)shader_source"
R"shader_source(	float d = cylinder(p.yz, 1.);
)shader_source"
R"shader_source(	d = max(d, abs(p.y) - .1);
)shader_source"
R"shader_source(	d = max(d, -p.x);
)shader_source"
R"shader_source(	return vec2(d, 0.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(*/
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*
)shader_source"
R"shader_source(vec2 f(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	float d;
)shader_source"
R"shader_source(	vec3 p_fluegel = p;
)shader_source"
R"shader_source(	p_fluegel.y /= .1;
)shader_source"
R"shader_source(	float scale = (p.z < 0.) ? .6: .3;
)shader_source"
R"shader_source(	p_fluegel.z /= 1. + scale * smoothstep(.0, 5., p_fluegel.x);
)shader_source"
R"shader_source(	p_fluegel.z /= 1. - scale * smoothstep(5., 7., p_fluegel.x);
)shader_source"
R"shader_source(	d = cylinder(p_fluegel.yz, 1.);
)shader_source"
R"shader_source(	d *= .1;
)shader_source"
R"shader_source(	d = max(d, -p.x);
)shader_source"
R"shader_source(	d = max(d, sphere(p, 6.5));
)shader_source"
R"shader_source(	return vec2(d, 0.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(*/
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*
)shader_source"
R"shader_source(vec2 f(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	vec3 p_body = p;
)shader_source"
R"shader_source(	float t = clamp(p_body.z, -1. / .4, 1. / .4);
)shader_source"
R"shader_source(	float dellen = expStep(abs(fract(t * 3.) * 2. - 1.), 20., 5.);
)shader_source"
R"shader_source(	p_body.z -= (1. - pow(min(length(p_body.xy), 1.), 1.47)) * .6;
)shader_source"
R"shader_source(	p_body.z *= .4;
)shader_source"
R"shader_source(	float d_body = sphere(p_body, 1. - dellen * .03);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_fluegel = p;
)shader_source"
R"shader_source(	p_fluegel.x = abs(p_fluegel.x);
)shader_source"
R"shader_source(	p_fluegel.x -= 1.2;
)shader_source"
R"shader_source(	p_fluegel.xy *= rot2D(radians(30.));
)shader_source"
R"shader_source(	p_fluegel.xz *= rot2D(radians(20.));
)shader_source"
R"shader_source(	p_fluegel.y -= .5;
)shader_source"
R"shader_source(	p_fluegel.z /= 2.3;
)shader_source"
R"shader_source(	float d_fluegel = length(p_fluegel.xz) - 1.;
)shader_source"
R"shader_source(	p_fluegel.y = abs(p_fluegel.y);
)shader_source"
R"shader_source(	float biegung = 3.;
)shader_source"
R"shader_source(	p_fluegel.y += biegung - .2;
)shader_source"
R"shader_source(	d_fluegel = max(d_fluegel, sphere(p_fluegel, biegung));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_kopf = p;
)shader_source"
R"shader_source(	p_kopf.z += 2.2;
)shader_source"
R"shader_source(	float kopf_radius = .7;
)shader_source"
R"shader_source(	float d_kopf = sphere(p_kopf, kopf_radius);
)shader_source"
R"shader_source(	vec3 p_fuelerchen = p_kopf; // ordentlich fuel
)shader_source"
R"shader_source(	p_fuelerchen.z += .4;
)shader_source"
R"shader_source(	p_fuelerchen.y -= .35;
)shader_source"
R"shader_source(	float t_fueler = clamp(-p_fuelerchen.z, 0., 1.);
)shader_source"
R"shader_source(	p_fuelerchen.yz *= rot2D(radians(20.) * p_fuelerchen.z);
)shader_source"
R"shader_source(	p_fuelerchen.x = abs(p_fuelerchen.x);
)shader_source"
R"shader_source(	p_fuelerchen.x -= .3;
)shader_source"
R"shader_source(	p_fuelerchen.xz *= rot2D(radians(-30.) * t_fueler);
)shader_source"
R"shader_source(	float d_fuelerchen = line(p_fuelerchen, vec3(0., 0., -1.), .1);
)shader_source"
R"shader_source(	d_kopf = smin(d_kopf, d_fuelerchen, .1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float d = min(d_body, d_fluegel);
)shader_source"
R"shader_source(	d = min(d, d_kopf);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return vec2(d, .0);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(*/
)shader_source"
;
#endif
