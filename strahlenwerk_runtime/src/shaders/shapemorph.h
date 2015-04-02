#ifndef shapemorph_H
#define shapemorph_H
const char shapemorph_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(layout(location = 183) uniform float morph_mix_rt_float;
)shader_source"
R"shader_source(layout(location = 184) uniform float morph_octahedron_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 185) uniform float morph_hexshape_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 161) uniform float mk_kristall_cap_rt_float;
)shader_source"
R"shader_source(layout(location = 163) uniform float mk_kristall_h_rt_float;
)shader_source"
R"shader_source(layout(location = 186) uniform vec4 morph_rot_rt_quat; // quat
)shader_source"
R"shader_source(layout(location = 187) uniform float morph_trishape_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 188) uniform float morph_rotation_rt_float;
)shader_source"
R"shader_source(layout(location = 189) uniform vec3 morph_rt_color; // color
)shader_source"
R"shader_source(layout(location = 105) uniform vec3 bg_kristall_offset_rt_vec3;
)shader_source"
R"shader_source(layout(location = 168) uniform float mk_kristall_r_rt_float;
)shader_source"
R"shader_source(layout(location = 113) uniform float bg_kristall_h_rt_float;
)shader_source"
R"shader_source(layout(location = 116) uniform float bg_kristall_r_rt_float;
)shader_source"
R"shader_source(layout(location = 190) uniform float morph_kantenklumpen_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 118) uniform float bg_kristall_cap_rt_float;
)shader_source"
R"shader_source(layout(location = 101) uniform float bg_boden_height_rt_float;
)shader_source"
R"shader_source(layout(location = 120) uniform float bg_smin_felsen_rt_float;
)shader_source"
R"shader_source(layout(location = 122) uniform float bg_smin_kristall_rt_float;
)shader_source"
R"shader_source(layout(location = 125) uniform float bg_smin_boden_rt_float;
)shader_source"
R"shader_source(layout(location = 191) uniform float morph_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 192) uniform float morph_trillant_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 193) uniform float morph_noise_freq_rt_float;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(layout(location = 1) out float coc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 147) uniform float camera_focus_dist;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 148) uniform float camera_focal_length;
)shader_source"
R"shader_source(#define FOCAL_LENGTH
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 149) uniform float camera_f_stop;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void output_color(vec3 color, float dist) {
)shader_source"
R"shader_source(	dist = clamp(dist, 0., 1000.);
)shader_source"
R"shader_source(	float focus_dist = camera_focus_dist;
)shader_source"
R"shader_source(	float f = camera_focal_length;
)shader_source"
R"shader_source(	float N = camera_f_stop;
)shader_source"
R"shader_source(	coc = (dist - focus_dist)/dist * (f * f) / (N * (focus_dist - f)) / 0.035 * 1920.;
)shader_source"
R"shader_source(	if(any(isnan(color)) || any(isinf(color))) {
)shader_source"
R"shader_source(		color = vec3(0.);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	out_color = color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 1
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 150) uniform vec3 camera_position;
)shader_source"
R"shader_source(layout(location = 151) uniform vec4 camera_rotation; // quat
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#ifndef FOCAL_LENGTH
)shader_source"
R"shader_source(layout(location = 148) uniform float camera_focal_length;
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 66) uniform float time;
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
R"shader_source(	vec3 dir = vec3((gl_FragCoord.xy - .5 * res) / res.x , -camera_focal_length / .035);
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
R"shader_source(	// force hit except for far away points
)shader_source"
R"shader_source(	if (t > t_max) return 1./0.;
)shader_source"
R"shader_source(	//if ((t > t_max || candidate_error > pixelRadius) &&
)shader_source"
R"shader_source(	//		!forceHit) return 1./0.;
)shader_source"
R"shader_source(	return candidate_t;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float march(vec3 o, vec3 d, float t_max, float screenDistX) {
)shader_source"
R"shader_source(	return march_adv(o, d, .001, t_max, .5/(screenDistX*res.x), 128, 1.2, false);
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
R"shader_source(float slowbox2(vec2 p, vec2 b) {
)shader_source"
R"shader_source(	vec2 d = abs(p) - b;
)shader_source"
R"shader_source(	return min(max(d.x, d.y), 0.) + length(max(d, 0.));
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
R"shader_source(// und hier der andere witz
)shader_source"
R"shader_source(vec2 max_material(vec2 a, vec2 b) {
)shader_source"
R"shader_source(	return mix(a, b, float(a.x < b.x));
)shader_source"
R"shader_source(}
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
R"shader_source(// a: domrep cell size, b: parameter where the square spacing starts
)shader_source"
R"shader_source(float squarerep(float x, float a, float b, float min_cell) {
)shader_source"
R"shader_source(	if (x / a - floor(b) > 1.25) {
)shader_source"
R"shader_source(		b += .5;
)shader_source"
R"shader_source(		float cell = max(min_cell, floor(sqrt(abs(x / a - b)) + b));
)shader_source"
R"shader_source(		float cell_halfdist = a * (cell - b + .5);
)shader_source"
R"shader_source(		float cell_result = a * ((cell - b) * (cell - b) + b);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		x -= cell_result;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (x > cell_halfdist) {
)shader_source"
R"shader_source(			x = 2. * cell_halfdist - x;
)shader_source"
R"shader_source(		} else {
)shader_source"
R"shader_source(			if (cell - floor(b - .5) < 2.) {
)shader_source"
R"shader_source(				x = 2. * cell_halfdist - x;
)shader_source"
R"shader_source(			}
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		x = mod(x - .25 * a, a) - .5 * a;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return x;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 0) uniform sampler2D brdf;
)shader_source"
R"shader_source(layout(binding = 1) uniform samplerCube environment;
)shader_source"
R"shader_source(layout(binding = 2) uniform samplerCube filteredDiffuse;
)shader_source"
R"shader_source(layout(binding = 3) uniform samplerCube filteredSpecular;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 approximateSpecular(vec3 color, float roughness, vec3 N, vec3 V) {
)shader_source"
R"shader_source(	float NoV = clamp(dot(N, V), 0., 1.);
)shader_source"
R"shader_source(	vec3 R = 2. * dot(V, N) * N - V;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 prefiltered = textureLod(filteredSpecular, R, roughness * 5.).rgb;
)shader_source"
R"shader_source(	vec2 envBRDF = textureLod(brdf, vec2(roughness, NoV), 0.).rg;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return prefiltered * (color  * envBRDF.x + envBRDF.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 ambientColor(vec3 n, vec3 v, vec3 color, float rough, float metallic) {
)shader_source"
R"shader_source(	vec3 diffuse = textureLod(filteredDiffuse, n, 0.).rgb;
)shader_source"
R"shader_source(	vec3 dielectric = color * diffuse + approximateSpecular(vec3(.04), rough, n, v);
)shader_source"
R"shader_source(	vec3 metal = approximateSpecular(color, rough, n, v);
)shader_source"
R"shader_source(	return mix(dielectric, metal, metallic);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float ao(vec3 p, vec3 n, float d, float i) {
)shader_source"
R"shader_source(	float o, s = sign(d);
)shader_source"
R"shader_source(	for(o = s * .5 + .5; i > 0; i--) {
)shader_source"
R"shader_source(		o -= (i * d - f(p + n * i * d * s, false)[0]) / exp2(i);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return o;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 environmentColor(vec3 o, vec3 d, float r) {
)shader_source"
R"shader_source(	// hmmmmm…
)shader_source"
R"shader_source(	o.xz -= camera_position.xz;
)shader_source"
R"shader_source(	float radicand = dot(d, o) * dot(d, o) - dot(o, o) + r * r;
)shader_source"
R"shader_source(	if (radicand < 0.) discard; // hupsi
)shader_source"
R"shader_source(	float t = -dot(d, o) + sqrt(radicand);
)shader_source"
R"shader_source(	return textureLod(environment, normalize(o + t * d), 0.).rgb;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*
)shader_source"
R"shader_source(Noise - nützlich für fast alles! Daher auch gleich mal ne Menge verschiedenen.
)shader_source"
R"shader_source(Wir haben klassichen Perlin Noise (cnoise - classical noise), sowie Value Noise (vnoise), jeweils für 2D und 3D.
)shader_source"
R"shader_source(Perlin Noise ist schicker Gradient Noise, und sieht deshalb viel besser aus. Ist aber auch teurer.
)shader_source"
R"shader_source(Daher gibts auch noch den schnellen Value Noise, für wenn mans eh nicht sieht.
)shader_source"
R"shader_source(Außerdem noch fbm Varianten davon (cfbm, vfbm), die mehrere Oktaven kombinieren und ein wenig spannender sind.
)shader_source"
R"shader_source(Gefühlt kommt vfbm näher an cfbm, als vnoise an cnoise, und cfbm ist noch mal ordentlich teuer.
)shader_source"
R"shader_source(Der Wertebereich ist jeweils [-1, 1]! (das ist keine Fakultät)
)shader_source"
R"shader_source(*/
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float mod289(float x) {
)shader_source"
R"shader_source(  return x - floor(x * (1.0 / 289.0)) * 289.0;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 mod289(vec2 x) {
)shader_source"
R"shader_source(  return x - floor(x * (1.0 / 289.0)) * 289.0;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 mod289(vec3 x) {
)shader_source"
R"shader_source(  return x - floor(x * (1.0 / 289.0)) * 289.0;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec4 mod289(vec4 x) {
)shader_source"
R"shader_source(  return x - floor(x * (1.0 / 289.0)) * 289.0;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float permute(float x) {
)shader_source"
R"shader_source(  return mod289(((x*34.0)+1.0)*x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 permute(vec2 x) {
)shader_source"
R"shader_source(  return mod289(((x*34.0)+1.0)*x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 permute(vec3 x) {
)shader_source"
R"shader_source(  return mod289(((x*34.0)+1.0)*x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec4 permute(vec4 x) {
)shader_source"
R"shader_source(  return mod289(((x*34.0)+1.0)*x);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec4 taylorInvSqrt(vec4 r) {
)shader_source"
R"shader_source(  return 1.79284291400159 - 0.85373472095314 * r;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 fade(vec2 t) {
)shader_source"
R"shader_source(  return t*t*t*(t*(t*6.0-15.0)+10.0);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 fade(vec3 t) {
)shader_source"
R"shader_source(  return t*t*t*(t*(t*6.0-15.0)+10.0);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see noise section above
)shader_source"
R"shader_source(float cnoise(vec2 P) {
)shader_source"
R"shader_source(	vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
)shader_source"
R"shader_source(	vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
)shader_source"
R"shader_source(	Pi = mod289(Pi); // To avoid truncation effects in permutation
)shader_source"
R"shader_source(	vec4 ix = Pi.xzxz;
)shader_source"
R"shader_source(	vec4 iy = Pi.yyww;
)shader_source"
R"shader_source(	vec4 fx = Pf.xzxz;
)shader_source"
R"shader_source(	vec4 fy = Pf.yyww;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec4 i = permute(permute(ix) + iy);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
)shader_source"
R"shader_source(	vec4 gy = abs(gx) - 0.5 ;
)shader_source"
R"shader_source(	vec4 tx = floor(gx + 0.5);
)shader_source"
R"shader_source(	gx = gx - tx;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 g00 = vec2(gx.x,gy.x);
)shader_source"
R"shader_source(	vec2 g10 = vec2(gx.y,gy.y);
)shader_source"
R"shader_source(	vec2 g01 = vec2(gx.z,gy.z);
)shader_source"
R"shader_source(	vec2 g11 = vec2(gx.w,gy.w);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
)shader_source"
R"shader_source(	g00 *= norm.x;
)shader_source"
R"shader_source(	g01 *= norm.y;
)shader_source"
R"shader_source(	g10 *= norm.z;
)shader_source"
R"shader_source(	g11 *= norm.w;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float n00 = dot(g00, vec2(fx.x, fy.x));
)shader_source"
R"shader_source(	float n10 = dot(g10, vec2(fx.y, fy.y));
)shader_source"
R"shader_source(	float n01 = dot(g01, vec2(fx.z, fy.z));
)shader_source"
R"shader_source(	float n11 = dot(g11, vec2(fx.w, fy.w));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 fade_xy = fade(Pf.xy);
)shader_source"
R"shader_source(	vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
)shader_source"
R"shader_source(	float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
)shader_source"
R"shader_source(	return 2.3 * n_xy;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see noise section above
)shader_source"
R"shader_source(float cnoise(vec3 P) {
)shader_source"
R"shader_source(	vec3 Pi0 = floor(P); // Integer part for indexing
)shader_source"
R"shader_source(	vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
)shader_source"
R"shader_source(	Pi0 = mod289(Pi0);
)shader_source"
R"shader_source(	Pi1 = mod289(Pi1);
)shader_source"
R"shader_source(	vec3 Pf0 = fract(P); // Fractional part for interpolation
)shader_source"
R"shader_source(	vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
)shader_source"
R"shader_source(	vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
)shader_source"
R"shader_source(	vec4 iy = vec4(Pi0.yy, Pi1.yy);
)shader_source"
R"shader_source(	vec4 iz0 = Pi0.zzzz;
)shader_source"
R"shader_source(	vec4 iz1 = Pi1.zzzz;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec4 ixy = permute(permute(ix) + iy);
)shader_source"
R"shader_source(	vec4 ixy0 = permute(ixy + iz0);
)shader_source"
R"shader_source(	vec4 ixy1 = permute(ixy + iz1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec4 gx0 = ixy0 * (1.0 / 7.0);
)shader_source"
R"shader_source(	vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
)shader_source"
R"shader_source(	gx0 = fract(gx0);
)shader_source"
R"shader_source(	vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
)shader_source"
R"shader_source(	vec4 sz0 = step(gz0, vec4(0.0));
)shader_source"
R"shader_source(	gx0 -= sz0 * (step(0.0, gx0) - 0.5);
)shader_source"
R"shader_source(	gy0 -= sz0 * (step(0.0, gy0) - 0.5);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec4 gx1 = ixy1 * (1.0 / 7.0);
)shader_source"
R"shader_source(	vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
)shader_source"
R"shader_source(	gx1 = fract(gx1);
)shader_source"
R"shader_source(	vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
)shader_source"
R"shader_source(	vec4 sz1 = step(gz1, vec4(0.0));
)shader_source"
R"shader_source(	gx1 -= sz1 * (step(0.0, gx1) - 0.5);
)shader_source"
R"shader_source(	gy1 -= sz1 * (step(0.0, gy1) - 0.5);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
)shader_source"
R"shader_source(	vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
)shader_source"
R"shader_source(	vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
)shader_source"
R"shader_source(	vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
)shader_source"
R"shader_source(	vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
)shader_source"
R"shader_source(	vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
)shader_source"
R"shader_source(	vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
)shader_source"
R"shader_source(	vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
)shader_source"
R"shader_source(	g000 *= norm0.x;
)shader_source"
R"shader_source(	g010 *= norm0.y;
)shader_source"
R"shader_source(	g100 *= norm0.z;
)shader_source"
R"shader_source(	g110 *= norm0.w;
)shader_source"
R"shader_source(	vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
)shader_source"
R"shader_source(	g001 *= norm1.x;
)shader_source"
R"shader_source(	g011 *= norm1.y;
)shader_source"
R"shader_source(	g101 *= norm1.z;
)shader_source"
R"shader_source(	g111 *= norm1.w;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float n000 = dot(g000, Pf0);
)shader_source"
R"shader_source(	float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
)shader_source"
R"shader_source(	float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
)shader_source"
R"shader_source(	float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
)shader_source"
R"shader_source(	float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
)shader_source"
R"shader_source(	float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
)shader_source"
R"shader_source(	float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
)shader_source"
R"shader_source(	float n111 = dot(g111, Pf1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 fade_xyz = fade(Pf0);
)shader_source"
R"shader_source(	vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
)shader_source"
R"shader_source(	vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
)shader_source"
R"shader_source(	float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
)shader_source"
R"shader_source(	return 2.2 * n_xyz;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rand(vec2 c) {
)shader_source"
R"shader_source(    vec2 m = mod289(c);
)shader_source"
R"shader_source(	vec2 h = permute(m);
)shader_source"
R"shader_source(    return fract(permute(h.x * h.y + m.x + m.y)/41.) * 2. - 1.;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rand(vec3 c) {
)shader_source"
R"shader_source(    vec3 m = mod289(c);
)shader_source"
R"shader_source(	vec3 h = permute(m);
)shader_source"
R"shader_source(    return fract(permute(h.x * h.y * h.z + m.x + m.y + m.z)/41.) * 2. - 1.;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see noise section above
)shader_source"
R"shader_source(float vnoise(vec2 c) {
)shader_source"
R"shader_source(	vec2 c0 = floor(c);
)shader_source"
R"shader_source(    vec2 t = fract(c);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(    vec2 o = vec2(1., 0.);
)shader_source"
R"shader_source(    float v00 = rand(c0 + o.yy);
)shader_source"
R"shader_source(    float v01 = rand(c0 + o.yx);
)shader_source"
R"shader_source(    float v10 = rand(c0 + o.xy);
)shader_source"
R"shader_source(    float v11 = rand(c0 + o.xx);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(    t = fade(t);
)shader_source"
R"shader_source(    return mix(mix(v00, v10, t.x), mix(v01, v11, t.x), t.y);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see noise section above
)shader_source"
R"shader_source(float vnoise(vec3 c) {
)shader_source"
R"shader_source(	vec3 c0 = floor(c);
)shader_source"
R"shader_source(    vec3 t = fract(c);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(    vec2 o = vec2(1., 0.);
)shader_source"
R"shader_source(    float v000 = rand(c0 + o.yyy);
)shader_source"
R"shader_source(    float v001 = rand(c0 + o.yyx);
)shader_source"
R"shader_source(    float v010 = rand(c0 + o.yxy);
)shader_source"
R"shader_source(    float v011 = rand(c0 + o.yxx);
)shader_source"
R"shader_source(    float v100 = rand(c0 + o.xyy);
)shader_source"
R"shader_source(    float v101 = rand(c0 + o.xyx);
)shader_source"
R"shader_source(    float v110 = rand(c0 + o.xxy);
)shader_source"
R"shader_source(    float v111 = rand(c0 + o.xxx);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	t = fade(t);
)shader_source"
R"shader_source(	return mix(
)shader_source"
R"shader_source(		mix(
)shader_source"
R"shader_source(			mix(v000, v100, t.x),
)shader_source"
R"shader_source(			mix(v010, v110, t.x),
)shader_source"
R"shader_source(			t.y),
)shader_source"
R"shader_source(		mix(
)shader_source"
R"shader_source(			mix(v001, v101, t.x),
)shader_source"
R"shader_source(			mix(v011, v111, t.x),
)shader_source"
R"shader_source(			t.y),
)shader_source"
R"shader_source(		t.z);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see noise section above
)shader_source"
R"shader_source(float cfbm(vec2 c) {
)shader_source"
R"shader_source(	return (cnoise(c) + cnoise(c * 2.) * .5 + cnoise(c * 4.) * .25)/1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see noise section above
)shader_source"
R"shader_source(float cfbm(vec3 c) {
)shader_source"
R"shader_source(	return (cnoise(c) + cnoise(c * 2.) * .5 + cnoise(c * 4.) * .25)/1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see noise section above
)shader_source"
R"shader_source(float vfbm(vec2 c) {
)shader_source"
R"shader_source(	return (vnoise(c) + vnoise(c * 2.) * .5 + vnoise(c * 4.) * .25)/1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// see noise section above
)shader_source"
R"shader_source(float vfbm(vec3 c) {
)shader_source"
R"shader_source(	return (vnoise(c) + vnoise(c * 2.) * .5 + vnoise(c * 4.) * .25)/1.75;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float fels_noise(vec3 p_fels, vec2 domrep_size, vec3 box_size) {
)shader_source"
R"shader_source(	vec2 cell_fels = floor(p_fels.xz / domrep_size);
)shader_source"
R"shader_source(	//cell_fels = vec2(0.);
)shader_source"
R"shader_source(	p_fels.xz = domrep(p_fels.xz, domrep_size);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//  8 1 5
)shader_source"
R"shader_source(	//  4 0 2  >x
)shader_source"
R"shader_source(	//  7 3 6 vz
)shader_source"
R"shader_source(	vec3 p_fels_0 = p_fels;
)shader_source"
R"shader_source(	vec3 p_fels_1 = trans(p_fels, 0., 0., -domrep_size.y);
)shader_source"
R"shader_source(	vec3 p_fels_2 = trans(p_fels,  domrep_size.x, 0., 0.);
)shader_source"
R"shader_source(	vec3 p_fels_3 = trans(p_fels, 0., 0.,  domrep_size.y);
)shader_source"
R"shader_source(	vec3 p_fels_4 = trans(p_fels, -domrep_size.x, 0., 0.);
)shader_source"
R"shader_source(	vec3 p_fels_5 = trans(p_fels,  domrep_size.x, 0., -domrep_size.y);
)shader_source"
R"shader_source(	vec3 p_fels_6 = trans(p_fels,  domrep_size.x, 0.,  domrep_size.y);
)shader_source"
R"shader_source(	vec3 p_fels_7 = trans(p_fels, -domrep_size.x, 0.,  domrep_size.y);
)shader_source"
R"shader_source(	vec3 p_fels_8 = trans(p_fels, -domrep_size.x, 0., -domrep_size.y);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 cell_fels_0 = cell_fels;
)shader_source"
R"shader_source(	vec2 cell_fels_1 = cell_fels + vec2( 0., -1.);
)shader_source"
R"shader_source(	vec2 cell_fels_2 = cell_fels + vec2( 1.,  0.);
)shader_source"
R"shader_source(	vec2 cell_fels_3 = cell_fels + vec2( 0.,  1.);
)shader_source"
R"shader_source(	vec2 cell_fels_4 = cell_fels + vec2(-1.,  0.);
)shader_source"
R"shader_source(	vec2 cell_fels_5 = cell_fels + vec2( 1., -1.);
)shader_source"
R"shader_source(	vec2 cell_fels_6 = cell_fels + vec2( 1.,  1.);
)shader_source"
R"shader_source(	vec2 cell_fels_7 = cell_fels + vec2(-1.,  1.);
)shader_source"
R"shader_source(	vec2 cell_fels_8 = cell_fels + vec2(-1., -1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_fels_0.xy *= rot2D(cell_fels_0.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	p_fels_1.xy *= rot2D(cell_fels_1.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	p_fels_2.xy *= rot2D(cell_fels_2.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	p_fels_3.xy *= rot2D(cell_fels_3.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	p_fels_4.xy *= rot2D(cell_fels_4.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	p_fels_5.xy *= rot2D(cell_fels_5.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	p_fels_6.xy *= rot2D(cell_fels_6.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	p_fels_7.xy *= rot2D(cell_fels_7.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	p_fels_8.xy *= rot2D(cell_fels_8.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_fels_0.xz *= rot2D(cell_fels_0.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	p_fels_1.xz *= rot2D(cell_fels_1.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	p_fels_2.xz *= rot2D(cell_fels_2.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	p_fels_3.xz *= rot2D(cell_fels_3.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	p_fels_4.xz *= rot2D(cell_fels_4.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	p_fels_5.xz *= rot2D(cell_fels_5.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	p_fels_6.xz *= rot2D(cell_fels_6.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	p_fels_7.xz *= rot2D(cell_fels_7.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	p_fels_8.xz *= rot2D(cell_fels_8.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	/*
)shader_source"
R"shader_source(	p_fels_0.yz *= rot2D(cell_fels_0.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	p_fels_1.yz *= rot2D(cell_fels_1.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	p_fels_2.yz *= rot2D(cell_fels_2.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	p_fels_3.yz *= rot2D(cell_fels_3.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	p_fels_4.yz *= rot2D(cell_fels_4.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	p_fels_5.yz *= rot2D(cell_fels_5.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	p_fels_6.yz *= rot2D(cell_fels_6.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	p_fels_7.yz *= rot2D(cell_fels_7.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	p_fels_8.yz *= rot2D(cell_fels_8.y * domrep_size.x * 52.40165);
)shader_source"
R"shader_source(	// */
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float fels_0 = box(p_fels_0, box_size);
)shader_source"
R"shader_source(	float fels_1 = box(p_fels_1, box_size);
)shader_source"
R"shader_source(	float fels_2 = box(p_fels_2, box_size);
)shader_source"
R"shader_source(	float fels_3 = box(p_fels_3, box_size);
)shader_source"
R"shader_source(	float fels_4 = box(p_fels_4, box_size);
)shader_source"
R"shader_source(	float fels_5 = box(p_fels_5, box_size);
)shader_source"
R"shader_source(	float fels_6 = box(p_fels_6, box_size);
)shader_source"
R"shader_source(	float fels_7 = box(p_fels_7, box_size);
)shader_source"
R"shader_source(	float fels_8 = box(p_fels_8, box_size);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float fels_12 = min(fels_1, fels_2);
)shader_source"
R"shader_source(	float fels_34 = min(fels_3, fels_4);
)shader_source"
R"shader_source(	float fels_56 = min(fels_5, fels_6);
)shader_source"
R"shader_source(	float fels_78 = min(fels_7, fels_8);
)shader_source"
R"shader_source(	float fels_1234 = min(fels_12, fels_34);
)shader_source"
R"shader_source(	float fels_5678 = min(fels_56, fels_78);
)shader_source"
R"shader_source(	float fels_12345678 = min(fels_1234, fels_5678);
)shader_source"
R"shader_source(	float fels_012345678 = smin(fels_0, fels_12345678, bg_smin_felsen_rt_float * box_size.x);
)shader_source"
R"shader_source(	fels_012345678 = smin(fels_0, fels_1234, bg_smin_felsen_rt_float * box_size.x); // TODO: maybe remove this line for less/more? artefacts
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return fels_012345678;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float kristall(vec3 p_kristall, float height_kristall, float radius_kristall, float size_cap) {
)shader_source"
R"shader_source(	float r_kristall = radius_kristall * min((height_kristall - radius_kristall) - p_kristall.y, size_cap) / size_cap;
)shader_source"
R"shader_source(	p_kristall.y -= height_kristall * .5;
)shader_source"
R"shader_source(	return hexprism(p_kristall.xzy, vec2(r_kristall, height_kristall));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float background(vec3 p) {
)shader_source"
R"shader_source(	p.y -= -10.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_kristall = p;
)shader_source"
R"shader_source(	p_kristall.y -= bg_kristall_offset_rt_vec3.y;
)shader_source"
R"shader_source(	p_kristall.x = abs(p_kristall.x);
)shader_source"
R"shader_source(	p_kristall.x -= bg_kristall_offset_rt_vec3.x;
)shader_source"
R"shader_source(	p_kristall.z = abs(p_kristall.z);
)shader_source"
R"shader_source(	p_kristall.z -= bg_kristall_offset_rt_vec3.z;
)shader_source"
R"shader_source(	p_kristall.yz *= rot2D(TAU * .48);
)shader_source"
R"shader_source(	p_kristall.xy *= rot2D(TAU * .45);
)shader_source"
R"shader_source(	float f_kristall = kristall(p_kristall, bg_kristall_h_rt_float, bg_kristall_r_rt_float, bg_kristall_cap_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_fels = p;
)shader_source"
R"shader_source(	p_fels.xz -= vec2(237., 349.); // origin looks shitty
)shader_source"
R"shader_source(	p_fels.xz *= rot2D(TAU * .1);
)shader_source"
R"shader_source(	float f_fels = fels_noise(p_fels, vec2(10.), vec3(2.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_boden = p;
)shader_source"
R"shader_source(	p_boden.y -= bg_boden_height_rt_float;
)shader_source"
R"shader_source(	float f_boden = p_boden.y;
)shader_source"
R"shader_source(	f_fels = smin(f_fels, f_boden, bg_smin_boden_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f = smin(f_kristall, f_fels, bg_smin_kristall_rt_float);
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 6
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
R"shader_source(	float t = march(o, d, 100., screenDist);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 out_color;
)shader_source"
R"shader_source(	if (isinf(t)) {
)shader_source"
R"shader_source(		o.y += 10.;
)shader_source"
R"shader_source(		out_color.rgb = environmentColor(o, d, 100.);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		vec3 normal = calc_normal(o + t * d, false);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		//vec3 r = refract();
)shader_source"
R"shader_source(		vec3 reflection_dir = reflect(d, normal);
)shader_source"
R"shader_source(		vec3 reflection_color = textureLod(environment, reflection_dir, 0.).rgb;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		vec3 n2 = vec3(1.1, 1.104, 1.106);
)shader_source"
R"shader_source(		vec3 refraction_dir_r = refract(d, normal, n2.x);
)shader_source"
R"shader_source(		vec3 refraction_dir_g = refract(d, normal, n2.y);
)shader_source"
R"shader_source(		vec3 refraction_dir_b = refract(d, normal, n2.z);
)shader_source"
R"shader_source(		float refraction_red = textureLod(environment, refraction_dir_r, 0.).r;
)shader_source"
R"shader_source(		float refraction_green = textureLod(environment, refraction_dir_g, 0.).g;
)shader_source"
R"shader_source(		float refraction_blue = textureLod(environment, refraction_dir_b, 0.).b;
)shader_source"
R"shader_source(		vec3 refraction_color = vec3(refraction_red, refraction_green, refraction_blue);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		vec3 r0 = (1. - n2) / (1. + n2);
)shader_source"
R"shader_source(		r0 *= r0;
)shader_source"
R"shader_source(		vec3 H_r = .5 * (refraction_dir_r - d);
)shader_source"
R"shader_source(		vec3 H_g = .5 * (refraction_dir_g - d); // <3 mercury
)shader_source"
R"shader_source(		vec3 H_b = .5 * (refraction_dir_b - d);
)shader_source"
R"shader_source(		vec3 HdotV = -vec3(dot(H_r, d), dot(H_g, d), dot(H_b, d));
)shader_source"
R"shader_source(		vec3 r = r0 + (1. - r0) * pow(1. - HdotV, vec3(5.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		//out_color.rgb = vec3(max(dot(normal, normalize(vec3(1., .5, .5))), 0.) + .1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		//// TODO: maybe use the normal sss (ao) function
)shader_source"
R"shader_source(		vec3 p = o + t * d;
)shader_source"
R"shader_source(		vec3 n = -d;
)shader_source"
R"shader_source(		float ao_factor = 0.;
)shader_source"
R"shader_source(		float l = -.3;
)shader_source"
R"shader_source(		float i = 5.;
)shader_source"
R"shader_source(		for(; i > 0; i--) {
)shader_source"
R"shader_source(			vec3 p_i = p + n * i * l;
)shader_source"
R"shader_source(			float noise = cfbm(p_i * morph_noise_freq_rt_float);
)shader_source"
R"shader_source(			//ao_factor -= (i * l - noise * f(p_i, false)[0]) / exp2(i);
)shader_source"
R"shader_source(			ao_factor += noise * abs(f(p_i, false)[0]) / exp2(i);
)shader_source"
R"shader_source(			//ao_factor += noise / exp2(i);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(		//out_color.rgb *= .1;
)shader_source"
R"shader_source(		refraction_color *= pow(clamp(ao_factor, 0., 1.), 10.);
)shader_source"
R"shader_source(		out_color = mix(reflection_color, refraction_color, r);
)shader_source"
R"shader_source(		out_color *= morph_rt_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		vec3 col = morph_rt_color;
)shader_source"
R"shader_source(		float rough = 0.3;
)shader_source"
R"shader_source(		float metallic = 1.;
)shader_source"
R"shader_source(		out_color.rgb = ambientColor(normal, -d, col, rough, metallic);
)shader_source"
R"shader_source(		// ganz viel spucke!
)shader_source"
R"shader_source(		//out_color.rgb *= .05;
)shader_source"
R"shader_source(		//out_color.rgb += ao_factor * 2.;
)shader_source"
R"shader_source(		out_color.rgb = mix(out_color.rgb, (ao_factor * 5. + .5) * morph_rt_color, r);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		//out_color.rgb = abs(normal);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	output_color(out_color, t);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float trishape(vec3 p) {
)shader_source"
R"shader_source(	float height_hex = .3;
)shader_source"
R"shader_source(	vec3 p_hex = p;
)shader_source"
R"shader_source(	float r_hex = mix(1., .5, clamp(p_hex.y / height_hex, 0., 10.));
)shader_source"
R"shader_source(	p_hex.y -= height_hex * .5;
)shader_source"
R"shader_source(	float f_hex = triprism(p_hex.xzy, vec2(r_hex, height_hex * .5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_hex2 = p_hex;
)shader_source"
R"shader_source(	p_hex2.xz *= rot2D(TAU / 3. / 2.);
)shader_source"
R"shader_source(	float f_hex2 = triprism(p_hex2.xzy, vec2(.7 * r_hex, height_hex * .5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f = f_hex;
)shader_source"
R"shader_source(	f = smin(f, f_hex2, morph_trishape_smooth_rt_float); // 0.7 is a good value
)shader_source"
R"shader_source(	f *= .5;
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float kantenklumpen(vec3 p) {
)shader_source"
R"shader_source(	vec3 p_penta = p;
)shader_source"
R"shader_source(	float f_penta = pentaprism(p_penta.xzy, vec2(1., 1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_penta = quat_rotate(p_penta, morph_rot_rt_quat);
)shader_source"
R"shader_source(	float f_penta2 = pentaprism(p_penta.xzy, vec2(1., 1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_penta = quat_rotate(p_penta, morph_rot_rt_quat);
)shader_source"
R"shader_source(	float f_penta3 = pentaprism(p_penta.xzy, vec2(1., 1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_penta = quat_rotate(p_penta, morph_rot_rt_quat);
)shader_source"
R"shader_source(	float f_penta4 = pentaprism(p_penta.xzy, vec2(1., 1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f = f_penta;
)shader_source"
R"shader_source(	f = smax(f, f_penta2, morph_kantenklumpen_smooth_rt_float); // -.3 or 0.5 is a good value
)shader_source"
R"shader_source(	f = smax(f, f_penta3, morph_kantenklumpen_smooth_rt_float);
)shader_source"
R"shader_source(	f = smax(f, f_penta4, morph_kantenklumpen_smooth_rt_float);
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float trillant(vec3 p) {
)shader_source"
R"shader_source(	float height = .4;
)shader_source"
R"shader_source(	float width = mix(3., 1.5, clamp(abs(p.y / (height * .5)), 0., 1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 p_c1 = p.xz;
)shader_source"
R"shader_source(	p_c1 -= width * vec2(0., 1./sqrt(3.));
)shader_source"
R"shader_source(	float f_c1 = length(p_c1) - width;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 p_c2 = p.xz;
)shader_source"
R"shader_source(	p_c2 -= width * vec2(-.5, -.5/sqrt(3.));
)shader_source"
R"shader_source(	float f_c2 = length(p_c2) - width;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 p_c3 = p.xz;
)shader_source"
R"shader_source(	p_c3 -= width * vec2(.5, -.5/sqrt(3.));
)shader_source"
R"shader_source(	float f_c3 = length(p_c3) - width;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f = f_c1;
)shader_source"
R"shader_source(	f = max(f, f_c2);
)shader_source"
R"shader_source(	f = max(f, f_c3);
)shader_source"
R"shader_source(	f = max(f, abs(p.y) - height * .5);
)shader_source"
R"shader_source(	f *= .3;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_tri = p;
)shader_source"
R"shader_source(	p_tri.xz *= rot2D(TAU / 3. / 3.);
)shader_source"
R"shader_source(	float f_tri = triprism(p_tri.xzy, vec2(.7/3. * width, height * .5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_tri.xz *= rot2D(TAU / 3. / 3.);
)shader_source"
R"shader_source(	float f_tri2 = triprism(p_tri.xzy, vec2(.7/3. * width, height * .5));
)shader_source"
R"shader_source(	f_tri = min(f_tri, f_tri2);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	f = mix(f_tri, f, morph_trillant_smooth_rt_float); // 0.9 or 1.2 are nice shapes
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float kristall(vec3 p) {
)shader_source"
R"shader_source(	vec3 p_kristall = p;
)shader_source"
R"shader_source(	float height_kristall = mk_kristall_h_rt_float;
)shader_source"
R"shader_source(	float radius_kristall = mk_kristall_r_rt_float;
)shader_source"
R"shader_source(	float size_cap = mk_kristall_cap_rt_float;
)shader_source"
R"shader_source(	p_kristall.y = abs(p_kristall.y);
)shader_source"
R"shader_source(	float r_kristall = radius_kristall * min((height_kristall - radius_kristall) - p_kristall.y, size_cap) / size_cap;
)shader_source"
R"shader_source(	float f_kristall = hexprism(p_kristall.xzy, vec2(r_kristall, height_kristall));
)shader_source"
R"shader_source(	f_kristall *= .5;
)shader_source"
R"shader_source(	return f_kristall;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float hexshape(vec3 p) {
)shader_source"
R"shader_source(	float f_hex = hexprism(p.xzy, vec2(1., 1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_plane = p;
)shader_source"
R"shader_source(	p_plane.xz *= rot2D(TAU / 6. / 2.);
)shader_source"
R"shader_source(	float f_plane = hexprism(p_plane.xzy, vec2(1., .0));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f_shape = smin(f_hex, f_plane, morph_hexshape_smooth_rt_float); // something around .8
)shader_source"
R"shader_source(	return f_shape;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float octahedronthingie(vec3 p) {
)shader_source"
R"shader_source(	float f_oktaeder = oktaeder(p, 2.);
)shader_source"
R"shader_source(	float f_box = box(p, vec3(.5));
)shader_source"
R"shader_source(	float f = smin(f_oktaeder, f_box, .5);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p.xy *= rot2D(TAU * .1);
)shader_source"
R"shader_source(	float f_kanten1 = box(p, vec3(.5));
)shader_source"
R"shader_source(	p.xz *= rot2D(TAU * .15);
)shader_source"
R"shader_source(	float f_kanten2 = box(p, vec3(.5));
)shader_source"
R"shader_source(	p.zy *= rot2D(TAU * .2);
)shader_source"
R"shader_source(	float f_kanten3 = box(p, vec3(.5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f_kanten = min(f_kanten1, f_kanten2);
)shader_source"
R"shader_source(	f_kanten = min(f_kanten, f_kanten3);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	f = mix(f, f_kanten, morph_octahedron_smooth_rt_float); // -.2 or .2
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	float d;
)shader_source"
R"shader_source(	float f;
)shader_source"
R"shader_source(	/*
)shader_source"
R"shader_source(	{
)shader_source"
R"shader_source(		float f_kein = max(length(p.xy) - .5, abs(p.z) - 1.);
)shader_source"
R"shader_source(		float f_ein = eineck(p, vec2(1.));
)shader_source"
R"shader_source(		float f_zwei = zweieck(p, vec2(1.));
)shader_source"
R"shader_source(		float f_tri = triprism(trans(p, 0., -.2, 0.), vec2(.8, 1.));
)shader_source"
R"shader_source(		float f_cube = box(p, vec3(1.));
)shader_source"
R"shader_source(		float f_penta = pentaprism(p, vec2(1.));
)shader_source"
R"shader_source(		float f_hex = hexprism(p, vec2(1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (morph_mix_rt_float <= 1.) {
)shader_source"
R"shader_source(			f = mix(f_hex, f_penta, morph_mix_rt_float);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 2.) {
)shader_source"
R"shader_source(			f = mix(f_penta, f_cube, morph_mix_rt_float - 1.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 3.) {
)shader_source"
R"shader_source(			f = mix(f_cube, f_tri, morph_mix_rt_float - 2.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 4.) {
)shader_source"
R"shader_source(			f = mix(f_tri, f_zwei, morph_mix_rt_float - 3.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 5.) {
)shader_source"
R"shader_source(			f = mix(f_zwei, f_ein, morph_mix_rt_float - 4.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 6.) {
)shader_source"
R"shader_source(			f = mix(f_ein, f_kein, morph_mix_rt_float - 5.);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	d = f;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	{
)shader_source"
R"shader_source(		//vec3 p = p.zyx;
)shader_source"
R"shader_source(		vec3 p = p;
)shader_source"
R"shader_source(		p.xz *= rot2D(morph_rotation_rt_float);
)shader_source"
R"shader_source(		float f_kein = max(length(p.xy) - .5, abs(p.z) - 1.);
)shader_source"
R"shader_source(		float f_ein = eineck(p, vec2(1.));
)shader_source"
R"shader_source(		float f_zwei = zweieck(p, vec2(1.));
)shader_source"
R"shader_source(		float f_tri = triprism(trans(p, 0., -.2, 0.), vec2(.8, 1.));
)shader_source"
R"shader_source(		float f_cube = box(p, vec3(1.));
)shader_source"
R"shader_source(		float f_penta = pentaprism(p, vec2(1.));
)shader_source"
R"shader_source(		float f_hex = hexprism(p, vec2(1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (morph_mix_rt_float <= 1.) {
)shader_source"
R"shader_source(			f = mix(f_hex, f_penta, morph_mix_rt_float);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 2.) {
)shader_source"
R"shader_source(			f = mix(f_penta, f_cube, morph_mix_rt_float - 1.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 3.) {
)shader_source"
R"shader_source(			f = mix(f_cube, f_tri, morph_mix_rt_float - 2.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 4.) {
)shader_source"
R"shader_source(			f = mix(f_tri, f_zwei, morph_mix_rt_float - 3.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 5.) {
)shader_source"
R"shader_source(			f = mix(f_zwei, f_ein, morph_mix_rt_float - 4.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 6.) {
)shader_source"
R"shader_source(			f = mix(f_ein, f_kein, morph_mix_rt_float - 5.);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	d = smin(d, f, morph_smooth_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	{
)shader_source"
R"shader_source(		//vec3 p = p.xzy;
)shader_source"
R"shader_source(		vec3 p = p;
)shader_source"
R"shader_source(		p.yz *= rot2D(morph_rotation_rt_float);
)shader_source"
R"shader_source(		float f_kein = max(length(p.xy) - .5, abs(p.z) - 1.);
)shader_source"
R"shader_source(		float f_ein = eineck(p, vec2(1.));
)shader_source"
R"shader_source(		float f_zwei = zweieck(p, vec2(1.));
)shader_source"
R"shader_source(		float f_tri = triprism(trans(p, 0., -.2, 0.), vec2(.8, 1.));
)shader_source"
R"shader_source(		float f_cube = box(p, vec3(1.));
)shader_source"
R"shader_source(		float f_penta = pentaprism(p, vec2(1.));
)shader_source"
R"shader_source(		float f_hex = hexprism(p, vec2(1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (morph_mix_rt_float <= 1.) {
)shader_source"
R"shader_source(			f = mix(f_hex, f_penta, morph_mix_rt_float);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 2.) {
)shader_source"
R"shader_source(			f = mix(f_penta, f_cube, morph_mix_rt_float - 1.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 3.) {
)shader_source"
R"shader_source(			f = mix(f_cube, f_tri, morph_mix_rt_float - 2.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 4.) {
)shader_source"
R"shader_source(			f = mix(f_tri, f_zwei, morph_mix_rt_float - 3.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 5.) {
)shader_source"
R"shader_source(			f = mix(f_zwei, f_ein, morph_mix_rt_float - 4.);
)shader_source"
R"shader_source(		} else if (morph_mix_rt_float <= 6.) {
)shader_source"
R"shader_source(			f = mix(f_ein, f_kein, morph_mix_rt_float - 5.);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	d = smin(d, f, morph_smooth_rt_float); // 0.3 or -0.4
)shader_source"
R"shader_source(	f = d;
)shader_source"
R"shader_source(	// */
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	/*
)shader_source"
R"shader_source(	float height_hex = .3;
)shader_source"
R"shader_source(	vec3 p_hex = p;
)shader_source"
R"shader_source(	float r_hex = mix(1., .5, clamp(p_hex.y / height_hex, 0., 10.));
)shader_source"
R"shader_source(	p_hex.y -= height_hex * .5;
)shader_source"
R"shader_source(	float f_hex = hexprism(p_hex.xzy, vec2(r_hex, height_hex * .5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_hex2 = p_hex;
)shader_source"
R"shader_source(	p_hex2.xz *= rot2D(radians(30.));
)shader_source"
R"shader_source(	float f_hex2 = hexprism(p_hex2.xzy, vec2(r_hex, height_hex * .5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	f = f_hex;
)shader_source"
R"shader_source(	f = smax(f, f_hex2, morph_smooth_rt_float); // .3
)shader_source"
R"shader_source(	f *= .5;
)shader_source"
R"shader_source(	// */
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//*
)shader_source"
R"shader_source(	if (morph_mix_rt_float <= 1.) {
)shader_source"
R"shader_source(		f = mix(kristall(p), hexshape(p), morph_mix_rt_float);
)shader_source"
R"shader_source(	} else if (morph_mix_rt_float <= 2.) {
)shader_source"
R"shader_source(		f = mix(hexshape(p), kantenklumpen(p), morph_mix_rt_float - 1.);
)shader_source"
R"shader_source(	} else if (morph_mix_rt_float <= 3.) {
)shader_source"
R"shader_source(		f = mix(kantenklumpen(p), octahedronthingie(p), morph_mix_rt_float - 2.);
)shader_source"
R"shader_source(	} else if (morph_mix_rt_float <= 4.) {
)shader_source"
R"shader_source(		f = mix(octahedronthingie(p), trishape(p), morph_mix_rt_float - 3.);
)shader_source"
R"shader_source(	} else if (morph_mix_rt_float <= 5.) {
)shader_source"
R"shader_source(		f = mix(trishape(p), trillant(p), morph_mix_rt_float - 4.);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	// */
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//f = octahedronthingie(p);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 m_kristall = vec2(f, 0.);
)shader_source"
R"shader_source(	vec2 m_bg = vec2(background(p), 0.);
)shader_source"
R"shader_source(	m_bg = min_material(m_bg, m_kristall);
)shader_source"
R"shader_source(	return m_bg;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
