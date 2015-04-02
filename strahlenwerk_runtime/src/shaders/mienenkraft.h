#ifndef mienenkraft_H
#define mienenkraft_H
const char mienenkraft_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(const vec3 mk_huegel_pos_rt_vec3 = vec3(10, 8, 10);
)shader_source"
R"shader_source(layout(location = 156) uniform float mk_hintergrund_kristall_cap_rt_float;
)shader_source"
R"shader_source(layout(location = 157) uniform float mk_hintergrund_kristall_r_rt_float;
)shader_source"
R"shader_source(layout(location = 158) uniform float mk_hintergrund_kristall_h_rt_float;
)shader_source"
R"shader_source(layout(location = 159) uniform float mk_bounding_offset_rt_float;
)shader_source"
R"shader_source(const float mk_kristall_cap_rt_float = 0.2870000000000345608;
)shader_source"
R"shader_source(layout(location = 160) uniform float mk_fog_amount_rt_float;
)shader_source"
R"shader_source(layout(location = 161) uniform float mk_kristall_h_rt_float;
)shader_source"
R"shader_source(layout(location = 162) uniform float mk_bounding_height_rt_float;
)shader_source"
R"shader_source(const vec3 mk_kristall3_trans_relative_rt_vec3 = vec3(-0.2369999999999663487, -0.47899999999992814992, -0.51499999999998635758);
)shader_source"
R"shader_source(layout(location = 163) uniform float mk_kristall1_fullrot_rt_float;
)shader_source"
R"shader_source(layout(location = 164) uniform float mk_smin_felsen_rt_float;
)shader_source"
R"shader_source(layout(location = 165) uniform float mk_kristall_r_rt_float;
)shader_source"
R"shader_source(const vec3 mk_kristall2_trans_relative_rt_vec3 = vec3(-0.74500000000000443645, -0.34699999999997999112, -0.24900000000002364686);
)shader_source"
R"shader_source(const float mk_kristall3_h_relative_rt_float = 0.42899999999997362465;
)shader_source"
R"shader_source(layout(location = 166) uniform vec3 mk_kristall_pos_rt_vec3;
)shader_source"
R"shader_source(layout(location = 167) uniform vec4 mk_kristall_rot_rt_quat; // quat
)shader_source"
R"shader_source(layout(location = 168) uniform vec3 mk_kristall1_trans_relative_rt_vec3;
)shader_source"
R"shader_source(layout(location = 169) uniform float mk_fog_invheight_rt_float;
)shader_source"
R"shader_source(layout(location = 170) uniform float mk_kristall1_eigenrot_rt_float;
)shader_source"
R"shader_source(const vec4 mk_kristall2_rot_relative_rt_quat = vec4(0.053422939032316207886, -0.97990703582763682977, 0.06456902623176574707, -0.18099492788314819336); // quat
)shader_source"
R"shader_source(const float mk_kristall2_h_relative_rt_float = 0.68299999999999272404;
)shader_source"
R"shader_source(const float mk_kristall2_r_relative_rt_float = 0.28300000000001546141;
)shader_source"
R"shader_source(const float mk_kristall2_cap_relative_rt_float = 0.36300000000005638867;
)shader_source"
R"shader_source(const vec4 mk_kristall3_rot_relative_rt_quat = vec4(-0.10300314426422120528, -0.15430708229541778564, 0.021007779985666275024, -0.98241466283798228876); // quat
)shader_source"
R"shader_source(const float mk_kristall3_r_relative_rt_float = 0.2870000000000345608;
)shader_source"
R"shader_source(const float mk_kristall3_cap_relative_rt_float = 0.31900000000007366907;
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
R"shader_source(#line 4
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(const float boden_id = 0.;
)shader_source"
R"shader_source(const float fels_id = 1.;
)shader_source"
R"shader_source(const float berg_id = 2.;
)shader_source"
R"shader_source(const float kristall_id = 3.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 applyFog( in vec3  rgb,      // original color of the pixel
)shader_source"
R"shader_source(               in float distance, // camera to point distance
)shader_source"
R"shader_source(               in vec3  rayOri,   // camera position
)shader_source"
R"shader_source(               in vec3  rayDir )  // camera to point vector
)shader_source"
R"shader_source({
)shader_source"
R"shader_source(	float b = mk_fog_invheight_rt_float;
)shader_source"
R"shader_source(	float c = mk_fog_amount_rt_float;
)shader_source"
R"shader_source(    float fogAmount = c * exp(-rayOri.y*b) * (1.0-exp( -distance*rayDir.y*b ))/rayDir.y;
)shader_source"
R"shader_source(	fogAmount = clamp(fogAmount, 0., 1.);
)shader_source"
R"shader_source(    vec3  fogColor  = vec3(0.5,0.6,0.7);
)shader_source"
R"shader_source(    return mix( rgb, fogColor, fogAmount );
)shader_source"
R"shader_source(}
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
R"shader_source(	float t = march(o, d, 200., screenDist);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float material_id = f(o+t*d, true)[1];
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (isinf(t)) {
)shader_source"
R"shader_source(		out_color.rgb = environmentColor(o, d, 200.);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		vec3 normal = calc_normal(o + t * d, false);
)shader_source"
R"shader_source(		out_color.rgb = vec3(1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (material_id == boden_id) {
)shader_source"
R"shader_source(			out_color *= 0.2;
)shader_source"
R"shader_source(		} else if (material_id == berg_id) {
)shader_source"
R"shader_source(			//out_color.r *= 0.2;
)shader_source"
R"shader_source(		} else if (material_id == kristall_id) {
)shader_source"
R"shader_source(			out_color.b *= 0.2;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float rough = 1.;
)shader_source"
R"shader_source(		float metallic = 0.;
)shader_source"
R"shader_source(		out_color.rgb = ambientColor(normal, -d, out_color.rgb, rough, metallic);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		//out_color.rgb = abs(normal);
)shader_source"
R"shader_source(		out_color.rgb = applyFog(out_color.rgb, t, o, d);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		out_color.rgb *= clamp(ao(o + t * d, normal, .4, 5), 0., 1.);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	output_color(out_color, t);
)shader_source"
R"shader_source(
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
R"shader_source(	float fels_012345678 = smin(fels_0, fels_12345678, mk_smin_felsen_rt_float * box_size.x);
)shader_source"
R"shader_source(	fels_012345678 = smin(fels_0, fels_1234, mk_smin_felsen_rt_float * box_size.x); // TODO: maybe remove this line for less/more? artefacts
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
R"shader_source(vec2 kristallgruppe(vec3 p){
)shader_source"
R"shader_source(	vec3 p_kristall1 = trans(p, mk_kristall1_trans_relative_rt_vec3);
)shader_source"
R"shader_source(	// invert rotation
)shader_source"
R"shader_source(	vec4 rot_quat = vec4(-mk_kristall_rot_rt_quat.xyz, mk_kristall_rot_rt_quat.w);
)shader_source"
R"shader_source(	rot_quat = normalize(mix(vec4(0.,0.,0.,1.), rot_quat, mk_kristall1_fullrot_rt_float));
)shader_source"
R"shader_source(	p_kristall1 = quat_rotate(p_kristall1, rot_quat);
)shader_source"
R"shader_source(	p_kristall1.xz *= rot2D(TAU * mk_kristall1_eigenrot_rt_float);
)shader_source"
R"shader_source(	float f_kristall = kristall(
)shader_source"
R"shader_source(		abs(p_kristall1),
)shader_source"
R"shader_source(		mk_kristall_h_rt_float,
)shader_source"
R"shader_source(		mk_kristall_r_rt_float,
)shader_source"
R"shader_source(		mk_kristall_cap_rt_float
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_kristall2 = trans(p, mk_kristall2_trans_relative_rt_vec3);
)shader_source"
R"shader_source(	float kristall2 = kristall(
)shader_source"
R"shader_source(		quat_rotate(p_kristall2, mk_kristall2_rot_relative_rt_quat),
)shader_source"
R"shader_source(		mk_kristall_h_rt_float * mk_kristall2_h_relative_rt_float,
)shader_source"
R"shader_source(		mk_kristall_r_rt_float * mk_kristall2_r_relative_rt_float,
)shader_source"
R"shader_source(		mk_kristall_cap_rt_float * mk_kristall2_cap_relative_rt_float
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_kristall3 = trans(p, mk_kristall3_trans_relative_rt_vec3);
)shader_source"
R"shader_source(	float kristall3 = kristall(
)shader_source"
R"shader_source(		quat_rotate(p_kristall3, mk_kristall3_rot_relative_rt_quat),
)shader_source"
R"shader_source(		mk_kristall_h_rt_float * mk_kristall3_h_relative_rt_float,
)shader_source"
R"shader_source(		mk_kristall_r_rt_float * mk_kristall3_r_relative_rt_float,
)shader_source"
R"shader_source(		mk_kristall_cap_rt_float * mk_kristall3_cap_relative_rt_float
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	f_kristall = min(f_kristall, kristall2);
)shader_source"
R"shader_source(	f_kristall = min(f_kristall, kristall3);
)shader_source"
R"shader_source(	return vec2(f_kristall, kristall_id);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 huegel(vec3 p) {
)shader_source"
R"shader_source(	float berg_smin = mk_smin_felsen_rt_float; // 5.0
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_berg_0 = p;
)shader_source"
R"shader_source(	//p_berg_0.xz *= rot2D(TAU * 0.5);
)shader_source"
R"shader_source(	p_berg_0.xy *= rot2D(TAU * 1.2);
)shader_source"
R"shader_source(	p_berg_0.yz *= rot2D(TAU * 0.7);
)shader_source"
R"shader_source(	float berg_0 = box(p_berg_0, vec3(1.5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_berg_1 = trans(p, 1.2,0.5,0.3);
)shader_source"
R"shader_source(	p_berg_1.xz *= rot2D(TAU * 0.2);
)shader_source"
R"shader_source(	p_berg_1.xy *= rot2D(TAU * 0.3);
)shader_source"
R"shader_source(	p_berg_1.yz *= rot2D(TAU * 0.1);
)shader_source"
R"shader_source(	float berg_1 = box(p_berg_1, vec3(3.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_berg_2 = trans(p, 3.,3.,5.);
)shader_source"
R"shader_source(	p_berg_2.xz *= rot2D(TAU * 0.53);
)shader_source"
R"shader_source(	p_berg_2.xy *= rot2D(TAU * 0.76);
)shader_source"
R"shader_source(	p_berg_2.yz *= rot2D(TAU * 0.35);
)shader_source"
R"shader_source(	float berg_2 = box(p_berg_2, vec3(3.,5.,2.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_berg_3 = trans(p, 3.,7.,8.);
)shader_source"
R"shader_source(	p_berg_3.xz *= rot2D(TAU * 0.2);
)shader_source"
R"shader_source(	p_berg_3.xy *= rot2D(TAU * 0.45);
)shader_source"
R"shader_source(	p_berg_3.yz *= rot2D(TAU * 0.1);
)shader_source"
R"shader_source(	float berg_3 = box(p_berg_3, vec3(.7,1.2,1.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_berg_4 = trans(p, 7.,4.,6.);
)shader_source"
R"shader_source(	p_berg_4.xz *= rot2D(TAU * 0.35);
)shader_source"
R"shader_source(	p_berg_4.xy *= rot2D(TAU * 0.48);
)shader_source"
R"shader_source(	p_berg_4.yz *= rot2D(TAU * 0.12);
)shader_source"
R"shader_source(	float berg_4 = box(p_berg_4, vec3(2.,2.5,1.5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	berg_3 = smin(berg_3, berg_4, berg_smin);
)shader_source"
R"shader_source(	berg_2 = smin(berg_2, berg_3, berg_smin);
)shader_source"
R"shader_source(	berg_1 = smin(berg_1, berg_2, berg_smin);
)shader_source"
R"shader_source(	float berg = smin(berg_0, berg_1, berg_smin);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 obj_berg = vec2(berg, berg_id);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 kristall_p = trans(p, mk_kristall_pos_rt_vec3);
)shader_source"
R"shader_source(	kristall_p = quat_rotate(kristall_p, mk_kristall_rot_rt_quat);
)shader_source"
R"shader_source(	vec2 obj_kristall = kristallgruppe(kristall_p);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return min_material(obj_berg, obj_kristall);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	float f_bounding = p.y - mk_bounding_height_rt_float;
)shader_source"
R"shader_source(	bool use_bounding = f_bounding > mk_bounding_offset_rt_float;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float fels = f_bounding;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (!use_bounding) {
)shader_source"
R"shader_source(		vec3 p_fels = p;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		vec2 domrep_size = vec2(10.);
)shader_source"
R"shader_source(		vec3 box_size = vec3(1.5);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float fels_klein = fels_noise(p_fels, domrep_size, box_size);
)shader_source"
R"shader_source(		p_fels.xz *= rot2D(TAU * .1);
)shader_source"
R"shader_source(		float fels_mittel = fels_noise(p_fels, 2. * domrep_size, 2. * box_size);
)shader_source"
R"shader_source(		p_fels.xz *= rot2D(TAU * .05);
)shader_source"
R"shader_source(		float fels_gross = fels_noise(p_fels, 4. * domrep_size, 4. * box_size);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		fels = smin(fels_klein, fels_mittel, mk_smin_felsen_rt_float);
)shader_source"
R"shader_source(		fels = smin(fels, fels_gross, mk_smin_felsen_rt_float);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_hintergrund_kristall1 = trans(p, -35., 10., -5.);
)shader_source"
R"shader_source(	p_hintergrund_kristall1.xy *= rot2D(TAU * .1);
)shader_source"
R"shader_source(	float f_hintergrund_kristall1 = kristall(p_hintergrund_kristall1, mk_hintergrund_kristall_h_rt_float, mk_hintergrund_kristall_r_rt_float, mk_hintergrund_kristall_cap_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_hintergrund_kristall2 = trans(p, 85., 10., -35.);
)shader_source"
R"shader_source(	p_hintergrund_kristall2.zy *= rot2D(TAU * .08);
)shader_source"
R"shader_source(	float f_hintergrund_kristall2 = kristall(p_hintergrund_kristall2, mk_hintergrund_kristall_h_rt_float, mk_hintergrund_kristall_r_rt_float, mk_hintergrund_kristall_cap_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_hintergrund_kristall3 = trans(p, 5., 10., -95.);
)shader_source"
R"shader_source(	p_hintergrund_kristall3.zy *= rot2D(TAU * -.12);
)shader_source"
R"shader_source(	float f_hintergrund_kristall3 = kristall(p_hintergrund_kristall3, mk_hintergrund_kristall_h_rt_float, mk_hintergrund_kristall_r_rt_float, mk_hintergrund_kristall_cap_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f_hintergrund_kristAlle = f_hintergrund_kristall1;
)shader_source"
R"shader_source(	f_hintergrund_kristAlle = min(f_hintergrund_kristAlle, f_hintergrund_kristall2);
)shader_source"
R"shader_source(	f_hintergrund_kristAlle = min(f_hintergrund_kristAlle, f_hintergrund_kristall3);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (use_bounding) {
)shader_source"
R"shader_source(		fels = min(fels, f_hintergrund_kristAlle);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		fels = smin(fels, f_hintergrund_kristAlle, mk_smin_felsen_rt_float);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 obj_fels = vec2(fels, fels_id);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_huegel = trans(p, mk_huegel_pos_rt_vec3);
)shader_source"
R"shader_source(	vec2 obj_huegel = huegel(p_huegel);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (use_bounding) {
)shader_source"
R"shader_source(		obj_fels = min_material(obj_fels, obj_huegel);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		obj_fels = smin_material(obj_fels, obj_huegel, mk_smin_felsen_rt_float);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return obj_fels;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
