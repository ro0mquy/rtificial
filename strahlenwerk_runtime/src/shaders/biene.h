#ifndef biene_H
#define biene_H
const char biene_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(layout(location = 77) uniform float biene_fluegel_musterung_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 78) uniform float biene_fluegel_musterung_anim_rt_float;
)shader_source"
R"shader_source(layout(location = 79) uniform float biene_fluegel_musterung_space_rt_float;
)shader_source"
R"shader_source(layout(location = 80) uniform float biene_tunnel_size_rt_float;
)shader_source"
R"shader_source(layout(location = 81) uniform float biene_fluegel_musterung_phi_pow_rt_float;
)shader_source"
R"shader_source(layout(location = 82) uniform float biene_fluegel_musterung_start_rt_float;
)shader_source"
R"shader_source(layout(location = 83) uniform float biene_fluegel_end_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 84) uniform float biene_fluegel_asym_rt_float;
)shader_source"
R"shader_source(layout(location = 85) uniform vec2 biene_fluegel_c_rt_vec2;
)shader_source"
R"shader_source(layout(location = 86) uniform float biene_fluegel_start_rt_float;
)shader_source"
R"shader_source(layout(location = 87) uniform float biene_fluegel_raise_rt_float;
)shader_source"
R"shader_source(layout(location = 88) uniform float biene_fluegel_angle_rt_float;
)shader_source"
R"shader_source(layout(location = 89) uniform float biene_fluegel_length_rt_float;
)shader_source"
R"shader_source(layout(location = 90) uniform float biene_fluegel_anim_rt_float;
)shader_source"
R"shader_source(layout(location = 91) uniform float biene_fluegel_rot_rt_float;
)shader_source"
R"shader_source(layout(location = 92) uniform float biene_body_head_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 93) uniform float biene_dellen_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 94) uniform float biene_dellen_depth_rt_float;
)shader_source"
R"shader_source(layout(location = 95) uniform float biene_body_thorax_scale_rt_float;
)shader_source"
R"shader_source(layout(location = 96) uniform float biene_mouth_front_rt_float;
)shader_source"
R"shader_source(layout(location = 97) uniform float biene_body_max_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 98) uniform float biene_mouth_smooth_bottom_rt_float;
)shader_source"
R"shader_source(layout(location = 99) uniform float biene_dellen_num_rt_float;
)shader_source"
R"shader_source(layout(location = 100) uniform float biene_body_head_offset_rt_float;
)shader_source"
R"shader_source(layout(location = 101) uniform float bg_boden_height_rt_float;
)shader_source"
R"shader_source(layout(location = 102) uniform float biene_body_radius_rt_float;
)shader_source"
R"shader_source(layout(location = 103) uniform float biene_body_head_radius_rt_float;
)shader_source"
R"shader_source(layout(location = 104) uniform float biene_body_head_fuel_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 105) uniform vec3 bg_kristall_offset_rt_vec3;
)shader_source"
R"shader_source(layout(location = 106) uniform float biene_body_head_fuel_bend_rt_float;
)shader_source"
R"shader_source(layout(location = 107) uniform float biene_body_head_w_stretch_rt_float;
)shader_source"
R"shader_source(layout(location = 108) uniform float biene_fluegel_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 109) uniform float biene_fluegel_thick_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 110) uniform float biene_body_abdomen_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 111) uniform float biene_body_abdomen_offset_rt_float;
)shader_source"
R"shader_source(layout(location = 112) uniform float biene_mouth_bend_power_rt_float;
)shader_source"
R"shader_source(layout(location = 113) uniform float bg_kristall_h_rt_float;
)shader_source"
R"shader_source(layout(location = 114) uniform float biene_body_thorax_stretch_rt_float;
)shader_source"
R"shader_source(layout(location = 115) uniform float biene_body_head_stretch_rt_float;
)shader_source"
R"shader_source(layout(location = 116) uniform float bg_kristall_r_rt_float;
)shader_source"
R"shader_source(layout(location = 117) uniform float biene_fluegel_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 118) uniform float bg_kristall_cap_rt_float;
)shader_source"
R"shader_source(layout(location = 119) uniform float biene_body_bend_rt_float;
)shader_source"
R"shader_source(layout(location = 120) uniform float bg_smin_felsen_rt_float;
)shader_source"
R"shader_source(layout(location = 121) uniform float biene_body_head_eye_height_rt_float;
)shader_source"
R"shader_source(layout(location = 122) uniform float bg_smin_kristall_rt_float;
)shader_source"
R"shader_source(layout(location = 123) uniform vec2 biene_mouth_c_rt_vec2;
)shader_source"
R"shader_source(layout(location = 124) uniform float biene_body_anim_rt_float;
)shader_source"
R"shader_source(layout(location = 125) uniform float bg_smin_boden_rt_float;
)shader_source"
R"shader_source(layout(location = 126) uniform float biene_body_head_eye_offset_rt_float;
)shader_source"
R"shader_source(layout(location = 127) uniform float biene_body_thorax_height_rt_float;
)shader_source"
R"shader_source(layout(location = 128) uniform float biene_mouth_dist_rt_float;
)shader_source"
R"shader_source(layout(location = 129) uniform float biene_mouth_height_rt_float;
)shader_source"
R"shader_source(layout(location = 130) uniform float biene_mouth_bottom_rt_float;
)shader_source"
R"shader_source(layout(location = 131) uniform float biene_fluegel_musterung_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 132) uniform float biene_mouth_bend_rt_float;
)shader_source"
R"shader_source(layout(location = 133) uniform float biene_fluegel_musterung_phi_factor_rt_float;
)shader_source"
R"shader_source(layout(location = 134) uniform float biene_body_head_fuel_dist_rt_float;
)shader_source"
R"shader_source(layout(location = 135) uniform float biene_mouth_offset_rt_float;
)shader_source"
R"shader_source(layout(location = 136) uniform float biene_dellen_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 137) uniform float biene_body_length_rt_float;
)shader_source"
R"shader_source(layout(location = 138) uniform float biene_body_head_eye_dist_rt_float;
)shader_source"
R"shader_source(layout(location = 139) uniform float biene_body_head_fuel_bend_power_rt_float;
)shader_source"
R"shader_source(layout(location = 140) uniform float biene_body_head_eye_stretch_rt_float;
)shader_source"
R"shader_source(layout(location = 141) uniform float biene_body_head_eye_radius_rt_float;
)shader_source"
R"shader_source(layout(location = 142) uniform float biene_mouth_smooth_rt_float;
)shader_source"
R"shader_source(layout(location = 143) uniform float biene_Body_head_fuel_height_rt_float;
)shader_source"
R"shader_source(layout(location = 144) uniform float biene_body_head_fuel_length_rt_float;
)shader_source"
R"shader_source(layout(location = 145) uniform float biene_body_head_fuel_thick_rt_float;
)shader_source"
R"shader_source(layout(location = 146) uniform float biene_body_min_thick_rt_float;
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
R"shader_source(#line 5
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
R"shader_source(		vec3 p = o + t * d;
)shader_source"
R"shader_source(		vec3 normal = calc_normal(p, false);
)shader_source"
R"shader_source(		float material = f(p, true)[1];
)shader_source"
R"shader_source(		float rough = .2;
)shader_source"
R"shader_source(		float metallic = 1.;
)shader_source"
R"shader_source(		vec3 col = vec3(1.);
)shader_source"
R"shader_source(		out_color.rgb = ambientColor(normal, -d, col, rough, metallic);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	output_color(out_color, t);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 bee_body(vec3 p, bool last_step) {
)shader_source"
R"shader_source(	float biene_body_thorax_height_rt_float = mix(biene_body_thorax_height_rt_float, 0.,
)shader_source"
R"shader_source(			smoothstep(0., 1., biene_body_anim_rt_float));
)shader_source"
R"shader_source(	float biene_body_thorax_stretch_rt_float = mix(1., biene_body_thorax_stretch_rt_float,
)shader_source"
R"shader_source(			smoothstep(0., 1., biene_body_anim_rt_float));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float biene_body_head_radius_rt_float = mix(0., biene_body_head_radius_rt_float,
)shader_source"
R"shader_source(			smoothstep(1., 2., biene_body_anim_rt_float));
)shader_source"
R"shader_source(	float biene_body_head_stretch_rt_float = mix(1., biene_body_head_stretch_rt_float,
)shader_source"
R"shader_source(			smoothstep(1., 2., biene_body_anim_rt_float));
)shader_source"
R"shader_source(	float biene_body_head_offset_rt_float = mix(biene_body_head_offset_rt_float, 0.,
)shader_source"
R"shader_source(			smoothstep(1., 2., biene_body_anim_rt_float));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float biene_body_radius_rt_float = mix(0., biene_body_radius_rt_float,
)shader_source"
R"shader_source(			smoothstep(2., 3., biene_body_anim_rt_float));
)shader_source"
R"shader_source(	float biene_body_abdomen_offset_rt_float = mix(-.1, biene_body_abdomen_offset_rt_float,
)shader_source"
R"shader_source(			smoothstep(2., 3., biene_body_anim_rt_float));
)shader_source"
R"shader_source(	float biene_body_bend_rt_float = mix(0., biene_body_bend_rt_float,
)shader_source"
R"shader_source(			smoothstep(2., 3., biene_body_anim_rt_float));
)shader_source"
R"shader_source(	float biene_body_length_rt_float = mix(1e-6, biene_body_length_rt_float,
)shader_source"
R"shader_source(			smoothstep(2., 3., biene_body_anim_rt_float));
)shader_source"
R"shader_source(	float biene_body_abdomen_smooth_rt_float = mix(0., biene_body_abdomen_smooth_rt_float,
)shader_source"
R"shader_source(			smoothstep(2., 2.5, biene_body_anim_rt_float));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float dellen_anim = mix(0., 1., smoothstep(3., 4., biene_body_anim_rt_float));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_thorax = p;
)shader_source"
R"shader_source(	p_thorax.y -= biene_body_thorax_height_rt_float;
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
R"shader_source(	p_head.z += thorax_length + head_length + biene_body_head_offset_rt_float;
)shader_source"
R"shader_source(	vec3 p_fuel = p_head;
)shader_source"
R"shader_source(	p_head.z /= biene_body_head_stretch_rt_float;
)shader_source"
R"shader_source(	vec3 p_mouth = p_head;
)shader_source"
R"shader_source(	p_head.x /= biene_body_head_w_stretch_rt_float;
)shader_source"
R"shader_source(	float d_head = sphere(p_head, head_radius);
)shader_source"
R"shader_source(	d_head *= min(min(biene_body_head_stretch_rt_float, biene_body_head_w_stretch_rt_float), 1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_mouth.x = abs(p_mouth.x);
)shader_source"
R"shader_source(	p_mouth.x -= biene_mouth_dist_rt_float;
)shader_source"
R"shader_source(	//p_mouth.z += head_length;
)shader_source"
R"shader_source(	p_mouth.z += biene_mouth_front_rt_float * head_length;
)shader_source"
R"shader_source(	float mouth_height = biene_mouth_height_rt_float;
)shader_source"
R"shader_source(	float t_mouth = clamp(-p.y / mouth_height, 0., 1.);
)shader_source"
R"shader_source(	p_mouth.xy *= rot2D(radians(1.) * biene_mouth_bend_rt_float * pow(t_mouth, biene_mouth_bend_power_rt_float));
)shader_source"
R"shader_source(	p_mouth.y += mouth_height + head_radius * biene_mouth_offset_rt_float;
)shader_source"
R"shader_source(	float d_mouth = cone(vec3(p_mouth.x, p_mouth.z, -p_mouth.y), normalize(biene_mouth_c_rt_vec2));
)shader_source"
R"shader_source(	d_mouth = max(d_mouth, p_mouth.y - mouth_height + .5); // cap top
)shader_source"
R"shader_source(	d_mouth = smax(d_mouth,
)shader_source"
R"shader_source(		mouth_height * biene_mouth_bottom_rt_float - p_mouth.y,
)shader_source"
R"shader_source(		biene_mouth_smooth_bottom_rt_float); // cap bottom
)shader_source"
R"shader_source(	d_head = smin(d_head, d_mouth, head_radius * biene_mouth_smooth_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// eyes TODO
)shader_source"
R"shader_source(	vec3 p_eyes = p_head;
)shader_source"
R"shader_source(	p_eyes.x = abs(p_eyes.x);
)shader_source"
R"shader_source(	p_eyes.zx *= rot2D(radians(biene_body_head_eye_dist_rt_float));
)shader_source"
R"shader_source(	p_eyes.zy *= rot2D(radians(biene_body_head_eye_height_rt_float));
)shader_source"
R"shader_source(	p_eyes.z += head_length - biene_body_head_eye_offset_rt_float;
)shader_source"
R"shader_source(	p_eyes.y /= biene_body_head_eye_stretch_rt_float;
)shader_source"
R"shader_source(	float d_eyes = sphere(p_eyes, biene_body_head_eye_radius_rt_float * head_radius);
)shader_source"
R"shader_source(	d_head = min(d_head, d_eyes);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// fuel TODO
)shader_source"
R"shader_source(	p_fuel.x = abs(p_fuel.x);
)shader_source"
R"shader_source(	p_fuel.zx *= rot2D(radians(biene_body_head_fuel_dist_rt_float));
)shader_source"
R"shader_source(	p_fuel.zy *= rot2D(radians(biene_Body_head_fuel_height_rt_float));
)shader_source"
R"shader_source(	p_fuel.z += head_length;
)shader_source"
R"shader_source(	float t_fuel = clamp(-p_fuel.z, 0., biene_body_head_fuel_length_rt_float);
)shader_source"
R"shader_source(	p_fuel.yz *= rot2D(radians(1.) * pow(t_fuel, biene_body_head_fuel_bend_power_rt_float)
)shader_source"
R"shader_source(		* biene_body_head_fuel_bend_rt_float);
)shader_source"
R"shader_source(	float fuel_thick = biene_body_head_fuel_thick_rt_float;
)shader_source"
R"shader_source(	float d_fuel = line(p_fuel, vec3(0., 0., -biene_body_head_fuel_length_rt_float), fuel_thick);
)shader_source"
R"shader_source(	d_head = smin(d_head, d_fuel, fuel_thick * biene_body_head_fuel_smooth_rt_float);
)shader_source"
R"shader_source(	// immer nach gefühl
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
R"shader_source(	float dellen_space = 2. * biene_body_length_rt_float / biene_dellen_num_rt_float;
)shader_source"
R"shader_source(	vec3 p_dellen = p_abdomen;
)shader_source"
R"shader_source(	float dellen_cell = -ceil(p_dellen.z / dellen_space); // +floor() for other direction
)shader_source"
R"shader_source(	dellen_anim -= .5;
)shader_source"
R"shader_source(	float dellen_current_anim = clamp(dellen_anim * biene_dellen_num_rt_float - dellen_cell, 0., 1.);
)shader_source"
R"shader_source(	float biene_dellen_depth_rt_float = mix(0., biene_dellen_depth_rt_float,
)shader_source"
R"shader_source(			smoothstep(0., 1., dellen_current_anim));
)shader_source"
R"shader_source(	p_dellen.z = domrep(p_dellen.z, dellen_space);
)shader_source"
R"shader_source(	float d_dellen = smax(
)shader_source"
R"shader_source(		cylinder(p_dellen.xy, biene_body_radius_rt_float * (1. + biene_dellen_depth_rt_float)),
)shader_source"
R"shader_source(		abs(p_dellen.z) - biene_dellen_thick_rt_float,
)shader_source"
R"shader_source(		biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(	d_dellen = smax(d_dellen, abs(p_abdomen.z) - biene_body_length_rt_float, biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float);
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
R"shader_source(	float biene_fluegel_rot_rt_float = 20. * sin(TAU * biene_fluegel_anim_rt_float);
)shader_source"
R"shader_source(	float biene_fluegel_angle_rt_float = 20. * sin(TAU * (biene_fluegel_anim_rt_float - .25)) - 20.;
)shader_source"
R"shader_source(	float biene_fluegel_raise_rt_float = 20. * sin(TAU * biene_fluegel_anim_rt_float);
)shader_source"
R"shader_source(
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
R"shader_source(	d_fluegel = smax(d_fluegel,
)shader_source"
R"shader_source(		cylinder(p_fluegel.xz - vec2(fluegel_length, -fluegel_length * biene_fluegel_asym_rt_float), fluegel_length),
)shader_source"
R"shader_source(		biene_fluegel_end_smooth_rt_float * fluegel_thick); // cap ends
)shader_source"
R"shader_source(	d_fluegel = smax(d_fluegel, abs(p_fluegel.y) - fluegel_thick,
)shader_source"
R"shader_source(		biene_fluegel_thick_smooth_rt_float * fluegel_thick); // cap bottom/top
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// musterung
)shader_source"
R"shader_source(	vec3 p_filling = p_fluegel;
)shader_source"
R"shader_source(	p_filling.x -= biene_fluegel_musterung_start_rt_float;
)shader_source"
R"shader_source(	float phi = atan(p_filling.z, p_filling.x);
)shader_source"
R"shader_source(	float r = length(p_filling.xz);
)shader_source"
R"shader_source(	phi += pow(r, biene_fluegel_musterung_phi_pow_rt_float) * biene_fluegel_musterung_phi_factor_rt_float;
)shader_source"
R"shader_source(	float dr_phi = TAU / 6.;
)shader_source"
R"shader_source(	phi = domrep(phi, dr_phi);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_filling.xz = r * vec2(cos(phi), sin(phi));
)shader_source"
R"shader_source(	p_filling.x -= biene_fluegel_musterung_space_rt_float * -.25;
)shader_source"
R"shader_source(	p_filling.x = squarerep(p_filling.x, biene_fluegel_musterung_space_rt_float, biene_fluegel_musterung_anim_rt_float, 1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f_filling = slowbox2(p_filling.xy, vec2(biene_fluegel_musterung_space_rt_float * .25, fluegel_thick * 2.));
)shader_source"
R"shader_source(	f_filling = smax(f_filling, d_fluegel - biene_fluegel_musterung_thick_rt_float, fluegel_thick * biene_fluegel_musterung_smooth_rt_float);
)shader_source"
R"shader_source(	d_fluegel = smin(d_fluegel, f_filling, fluegel_thick * biene_fluegel_musterung_smooth_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return vec2(d_fluegel, 0.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rmk_tunnel(vec3 p) {
)shader_source"
R"shader_source(    float f_tri = max(abs(p.x) * .866025 + p.z * .5, -p.z) - biene_tunnel_size_rt_float;
)shader_source"
R"shader_source(	return -f_tri;
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
R"shader_source(	vec2 m_bee = smin_material(body, fluegel, biene_fluegel_smooth_rt_float * biene_fluegel_thick_rt_float);
)shader_source"
R"shader_source(	vec2 m_bg = vec2(background(p), 0.);
)shader_source"
R"shader_source(	vec2 m_tunnel = vec2(rmk_tunnel(p), 0.);
)shader_source"
R"shader_source(	m_bg = max_material(m_bg, m_tunnel);
)shader_source"
R"shader_source(	m_bg = min_material(m_bg, m_bee);
)shader_source"
R"shader_source(	return m_bg;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
