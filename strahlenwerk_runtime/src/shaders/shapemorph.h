#ifndef shapemorph_H
#define shapemorph_H
const char shapemorph_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(const float morph_smooth_rt_float = 0.9000000000000000222;
)shader_source"
R"shader_source(const float morph_hexshape_smooth_rt_float = 0.8000000000000682121;
)shader_source"
R"shader_source(const float mk_kristall_r_rt_float = 0.30100000000004456524;
)shader_source"
R"shader_source(const float morph_trillant_smooth_rt_float = 0.89999999999997726263;
)shader_source"
R"shader_source(const float morph_trishape_smooth_rt_float = 0.70000000000004547474;
)shader_source"
R"shader_source(const float boden_hoehe_rt_float = 14.752000000000066393;
)shader_source"
R"shader_source(layout(location = 87) uniform vec3 mk_fels_color1_rt_color; // color
)shader_source"
R"shader_source(const float morph_rotation_rt_float = 3.8599999999999998757;
)shader_source"
R"shader_source(const float mk_fels_freq_rt_float = 0.03300000000001546141;
)shader_source"
R"shader_source(layout(location = 113) uniform vec3 mk_fels_color2_rt_color; // color
)shader_source"
R"shader_source(layout(location = 130) uniform float font_size_rt_float;
)shader_source"
R"shader_source(const float morph_kantenklumpen_smooth_rt_float = 0.3000000000000682121;
)shader_source"
R"shader_source(const vec4 morph_rot_rt_quat = vec4(0.2348449999999999982, 0.77163499999999995982, -0.20199500000000000788, 0.55554000000000003379); // quat
)shader_source"
R"shader_source(layout(location = 131) uniform vec3 greetings_position_rt_vec3;
)shader_source"
R"shader_source(layout(location = 132) uniform float greetings_where_rt_float;
)shader_source"
R"shader_source(const float morph_opal_stretch_x_rt_float = 0.60000000000002273737;
)shader_source"
R"shader_source(const float bg_kristall_h_rt_float = 15;
)shader_source"
R"shader_source(const float morph_schwurbel_freq_rt_float = 6.8469999999999799911;
)shader_source"
R"shader_source(layout(location = 104) uniform float mk_height_color_rt_float;
)shader_source"
R"shader_source(const float bg_smin_kristall_rt_float = 5;
)shader_source"
R"shader_source(const float bg_boden_height_rt_float = 0.57100000000002637535;
)shader_source"
R"shader_source(layout(location = 95) uniform float morph_rough_rt_float;
)shader_source"
R"shader_source(layout(location = 133) uniform float font_brightness_rt_float;
)shader_source"
R"shader_source(const float bg_kristall_cap_rt_float = 2;
)shader_source"
R"shader_source(const float morph_schwurbel_intens_rt_float = 0.15300000000002000888;
)shader_source"
R"shader_source(const float mk_fels_noise_power_rt_float = 0.2630000000000336513;
)shader_source"
R"shader_source(const float bg_kristall_r_rt_float = 2.5;
)shader_source"
R"shader_source(const float morph_opal_smooth_plane_rt_float = 0.20000000000004547474;
)shader_source"
R"shader_source(const float mk_kristall_cap_rt_float = 0.2870000000000345608;
)shader_source"
R"shader_source(const float mk_kristall_h_rt_float = 1.8720000000000709406;
)shader_source"
R"shader_source(const float morph_opal_stretch_y_rt_float = 0.3000000000000682121;
)shader_source"
R"shader_source(const vec3 bg_kristall_offset_rt_vec3 = vec3(25, 7, 15);
)shader_source"
R"shader_source(const float morph_octahedron_smooth_rt_float = -0.1999999999999317879;
)shader_source"
R"shader_source(layout(location = 134) uniform vec3 font_color_rt_color; // color
)shader_source"
R"shader_source(const float mk_smin_felsen_rt_float = 7;
)shader_source"
R"shader_source(layout(location = 96) uniform float morph_helligkeit_rt_float;
)shader_source"
R"shader_source(const float morph_opal_smooth_rt_float = 0.17700000000002091838;
)shader_source"
R"shader_source(layout(location = 98) uniform vec3 morph_rt_color; // color
)shader_source"
R"shader_source(layout(location = 100) uniform float morph_reflectivness_rt_float;
)shader_source"
R"shader_source(layout(location = 135) uniform float morph_mix_rt_float;
)shader_source"
R"shader_source(layout(location = 103) uniform float mk_fels_rough_rt_float;
)shader_source"
R"shader_source(const float morph_noise_freq_rt_float = 1.9759999999999990905;
)shader_source"
R"shader_source(layout(location = 136) uniform float font_thickness_rt_float;
)shader_source"
R"shader_source(const float bg_smin_boden_rt_float = 1.4200000000000727596;
)shader_source"
R"shader_source(layout(location = 137) uniform float greetings_rt_float;
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
R"shader_source(layout(location = 118) uniform float camera_focus_dist;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 119) uniform float camera_focal_length;
)shader_source"
R"shader_source(#define FOCAL_LENGTH
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 120) uniform float camera_f_stop;
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
R"shader_source(layout(location = 121) uniform vec3 camera_position;
)shader_source"
R"shader_source(layout(location = 122) uniform vec4 camera_rotation; // quat
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#ifndef FOCAL_LENGTH
)shader_source"
R"shader_source(layout(location = 119) uniform float camera_focal_length;
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 67) uniform float time;
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
R"shader_source(	//vec2 e = vec2(.001, 0.); // no epilepsilon this time
)shader_source"
R"shader_source(	//return normalize(vec3(
)shader_source"
R"shader_source(	//	f(p + e.xyy, last_step)[0] - f(p - e.xyy, last_step)[0],
)shader_source"
R"shader_source(	//	f(p + e.yxy, last_step)[0] - f(p - e.yxy, last_step)[0],
)shader_source"
R"shader_source(	//	f(p + e.yyx, last_step)[0] - f(p - e.yyx, last_step)[0]
)shader_source"
R"shader_source(	//));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float e = .001;
)shader_source"
R"shader_source(	vec3 s[6] = vec3[6](vec3(e,0,0), vec3(0,e,0), vec3(0,0,e), vec3(-e,0,0), vec3(0,-e,0), vec3(0,0,-e));
)shader_source"
R"shader_source(	float d[6] = float[6](0,0,0,0,0,0);
)shader_source"
R"shader_source(	for(int i = 0; i<6; i++)
)shader_source"
R"shader_source(		d[i] = f(p+s[i], last_step)[0];
)shader_source"
R"shader_source(	return normalize(vec3(d[0]-d[3],d[1]-d[4],d[2]-d[5]));
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
R"shader_source(// be careful when nesting! (just don't)
)shader_source"
R"shader_source(vec2 smax_smaterial(vec2 a, vec2 b, float k) {
)shader_source"
R"shader_source(	float h = clamp(0.5 - 0.5 * (b - a) / k, 0.0, 1.0 );
)shader_source"
R"shader_source(	return vec2(mix(b.x, a.x, h) + k * h * (1.0 - h), mix(b.y, a.y, h));
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
R"shader_source(vec2 smax_material(vec2 a, vec2 b, float k) {
)shader_source"
R"shader_source(	return vec2(smax(a.x, b.x, k), a.x < b.x ? b.y : a.y);
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
R"shader_source(vec2 cellular(vec3 P) {
)shader_source"
R"shader_source(	const float K = 0.142857142857; // 1/7
)shader_source"
R"shader_source(	const float Ko = 0.428571428571; // 1/2-K/2
)shader_source"
R"shader_source(	const float K2 = 0.020408163265306; // 1/(7*7)
)shader_source"
R"shader_source(	const float Kz = 0.166666666667; // 1/6
)shader_source"
R"shader_source(	const float Kzo = 0.416666666667; // 1/2-1/6*2
)shader_source"
R"shader_source(	const float jitter = 1.0; // smaller jitter gives more regular pattern
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 Pi = mod(floor(P), 289.0);
)shader_source"
R"shader_source(	vec3 Pf = fract(P) - 0.5;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 Pfx = Pf.x + vec3(1.0, 0.0, -1.0);
)shader_source"
R"shader_source(	vec3 Pfy = Pf.y + vec3(1.0, 0.0, -1.0);
)shader_source"
R"shader_source(	vec3 Pfz = Pf.z + vec3(1.0, 0.0, -1.0);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p = permute(Pi.x + vec3(-1.0, 0.0, 1.0));
)shader_source"
R"shader_source(	vec3 p1 = permute(p + Pi.y - 1.0);
)shader_source"
R"shader_source(	vec3 p2 = permute(p + Pi.y);
)shader_source"
R"shader_source(	vec3 p3 = permute(p + Pi.y + 1.0);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p11 = permute(p1 + Pi.z - 1.0);
)shader_source"
R"shader_source(	vec3 p12 = permute(p1 + Pi.z);
)shader_source"
R"shader_source(	vec3 p13 = permute(p1 + Pi.z + 1.0);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p21 = permute(p2 + Pi.z - 1.0);
)shader_source"
R"shader_source(	vec3 p22 = permute(p2 + Pi.z);
)shader_source"
R"shader_source(	vec3 p23 = permute(p2 + Pi.z + 1.0);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p31 = permute(p3 + Pi.z - 1.0);
)shader_source"
R"shader_source(	vec3 p32 = permute(p3 + Pi.z);
)shader_source"
R"shader_source(	vec3 p33 = permute(p3 + Pi.z + 1.0);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox11 = fract(p11*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy11 = mod(floor(p11*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz11 = floor(p11*K2)*Kz - Kzo; // p11 < 289 guaranteed
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox12 = fract(p12*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy12 = mod(floor(p12*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz12 = floor(p12*K2)*Kz - Kzo;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox13 = fract(p13*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy13 = mod(floor(p13*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz13 = floor(p13*K2)*Kz - Kzo;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox21 = fract(p21*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy21 = mod(floor(p21*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz21 = floor(p21*K2)*Kz - Kzo;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox22 = fract(p22*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy22 = mod(floor(p22*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz22 = floor(p22*K2)*Kz - Kzo;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox23 = fract(p23*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy23 = mod(floor(p23*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz23 = floor(p23*K2)*Kz - Kzo;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox31 = fract(p31*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy31 = mod(floor(p31*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz31 = floor(p31*K2)*Kz - Kzo;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox32 = fract(p32*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy32 = mod(floor(p32*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz32 = floor(p32*K2)*Kz - Kzo;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 ox33 = fract(p33*K) - Ko;
)shader_source"
R"shader_source(	vec3 oy33 = mod(floor(p33*K), 7.0)*K - Ko;
)shader_source"
R"shader_source(	vec3 oz33 = floor(p33*K2)*Kz - Kzo;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx11 = Pfx + jitter*ox11;
)shader_source"
R"shader_source(	vec3 dy11 = Pfy.x + jitter*oy11;
)shader_source"
R"shader_source(	vec3 dz11 = Pfz.x + jitter*oz11;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx12 = Pfx + jitter*ox12;
)shader_source"
R"shader_source(	vec3 dy12 = Pfy.x + jitter*oy12;
)shader_source"
R"shader_source(	vec3 dz12 = Pfz.y + jitter*oz12;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx13 = Pfx + jitter*ox13;
)shader_source"
R"shader_source(	vec3 dy13 = Pfy.x + jitter*oy13;
)shader_source"
R"shader_source(	vec3 dz13 = Pfz.z + jitter*oz13;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx21 = Pfx + jitter*ox21;
)shader_source"
R"shader_source(	vec3 dy21 = Pfy.y + jitter*oy21;
)shader_source"
R"shader_source(	vec3 dz21 = Pfz.x + jitter*oz21;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx22 = Pfx + jitter*ox22;
)shader_source"
R"shader_source(	vec3 dy22 = Pfy.y + jitter*oy22;
)shader_source"
R"shader_source(	vec3 dz22 = Pfz.y + jitter*oz22;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx23 = Pfx + jitter*ox23;
)shader_source"
R"shader_source(	vec3 dy23 = Pfy.y + jitter*oy23;
)shader_source"
R"shader_source(	vec3 dz23 = Pfz.z + jitter*oz23;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx31 = Pfx + jitter*ox31;
)shader_source"
R"shader_source(	vec3 dy31 = Pfy.z + jitter*oy31;
)shader_source"
R"shader_source(	vec3 dz31 = Pfz.x + jitter*oz31;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx32 = Pfx + jitter*ox32;
)shader_source"
R"shader_source(	vec3 dy32 = Pfy.z + jitter*oy32;
)shader_source"
R"shader_source(	vec3 dz32 = Pfz.y + jitter*oz32;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dx33 = Pfx + jitter*ox33;
)shader_source"
R"shader_source(	vec3 dy33 = Pfy.z + jitter*oy33;
)shader_source"
R"shader_source(	vec3 dz33 = Pfz.z + jitter*oz33;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 d11 = dx11 * dx11 + dy11 * dy11 + dz11 * dz11;
)shader_source"
R"shader_source(	vec3 d12 = dx12 * dx12 + dy12 * dy12 + dz12 * dz12;
)shader_source"
R"shader_source(	vec3 d13 = dx13 * dx13 + dy13 * dy13 + dz13 * dz13;
)shader_source"
R"shader_source(	vec3 d21 = dx21 * dx21 + dy21 * dy21 + dz21 * dz21;
)shader_source"
R"shader_source(	vec3 d22 = dx22 * dx22 + dy22 * dy22 + dz22 * dz22;
)shader_source"
R"shader_source(	vec3 d23 = dx23 * dx23 + dy23 * dy23 + dz23 * dz23;
)shader_source"
R"shader_source(	vec3 d31 = dx31 * dx31 + dy31 * dy31 + dz31 * dz31;
)shader_source"
R"shader_source(	vec3 d32 = dx32 * dx32 + dy32 * dy32 + dz32 * dz32;
)shader_source"
R"shader_source(	vec3 d33 = dx33 * dx33 + dy33 * dy33 + dz33 * dz33;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// Sort out the two smallest distances (F1, F2)
)shader_source"
R"shader_source(#if 0
)shader_source"
R"shader_source(	// Cheat and sort out only F1
)shader_source"
R"shader_source(	vec3 d1 = min(min(d11,d12), d13);
)shader_source"
R"shader_source(	vec3 d2 = min(min(d21,d22), d23);
)shader_source"
R"shader_source(	vec3 d3 = min(min(d31,d32), d33);
)shader_source"
R"shader_source(	vec3 d = min(min(d1,d2), d3);
)shader_source"
R"shader_source(	d.x = min(min(d.x,d.y),d.z);
)shader_source"
R"shader_source(	return sqrt(d.xx); // F1 duplicated, no F2 computed
)shader_source"
R"shader_source(#else
)shader_source"
R"shader_source(	// Do it right and sort out both F1 and F2
)shader_source"
R"shader_source(	vec3 d1a = min(d11, d12);
)shader_source"
R"shader_source(	d12 = max(d11, d12);
)shader_source"
R"shader_source(	d11 = min(d1a, d13); // Smallest now not in d12 or d13
)shader_source"
R"shader_source(	d13 = max(d1a, d13);
)shader_source"
R"shader_source(	d12 = min(d12, d13); // 2nd smallest now not in d13
)shader_source"
R"shader_source(	vec3 d2a = min(d21, d22);
)shader_source"
R"shader_source(	d22 = max(d21, d22);
)shader_source"
R"shader_source(	d21 = min(d2a, d23); // Smallest now not in d22 or d23
)shader_source"
R"shader_source(	d23 = max(d2a, d23);
)shader_source"
R"shader_source(	d22 = min(d22, d23); // 2nd smallest now not in d23
)shader_source"
R"shader_source(	vec3 d3a = min(d31, d32);
)shader_source"
R"shader_source(	d32 = max(d31, d32);
)shader_source"
R"shader_source(	d31 = min(d3a, d33); // Smallest now not in d32 or d33
)shader_source"
R"shader_source(	d33 = max(d3a, d33);
)shader_source"
R"shader_source(	d32 = min(d32, d33); // 2nd smallest now not in d33
)shader_source"
R"shader_source(	vec3 da = min(d11, d21);
)shader_source"
R"shader_source(	d21 = max(d11, d21);
)shader_source"
R"shader_source(	d11 = min(da, d31); // Smallest now in d11
)shader_source"
R"shader_source(	d31 = max(da, d31); // 2nd smallest now not in d31
)shader_source"
R"shader_source(	d11.xy = (d11.x < d11.y) ? d11.xy : d11.yx;
)shader_source"
R"shader_source(	d11.xz = (d11.x < d11.z) ? d11.xz : d11.zx; // d11.x now smallest
)shader_source"
R"shader_source(	d12 = min(d12, d21); // 2nd smallest now not in d21
)shader_source"
R"shader_source(	d12 = min(d12, d22); // nor in d22
)shader_source"
R"shader_source(	d12 = min(d12, d31); // nor in d31
)shader_source"
R"shader_source(	d12 = min(d12, d32); // nor in d32
)shader_source"
R"shader_source(	d11.yz = min(d11.yz,d12.xy); // nor in d12.yz
)shader_source"
R"shader_source(	d11.y = min(d11.y,d12.z); // Only two more to go
)shader_source"
R"shader_source(	d11.y = min(d11.y,d11.z); // Done! (Phew!)
)shader_source"
R"shader_source(	return sqrt(d11.xy); // F1, F2
)shader_source"
R"shader_source(#endif
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
R"shader_source(	//vec3 p_fels_5 = trans(p_fels,  domrep_size.x, 0., -domrep_size.y);
)shader_source"
R"shader_source(	//vec3 p_fels_6 = trans(p_fels,  domrep_size.x, 0.,  domrep_size.y);
)shader_source"
R"shader_source(	//vec3 p_fels_7 = trans(p_fels, -domrep_size.x, 0.,  domrep_size.y);
)shader_source"
R"shader_source(	//vec3 p_fels_8 = trans(p_fels, -domrep_size.x, 0., -domrep_size.y);
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
R"shader_source(	//vec2 cell_fels_5 = cell_fels + vec2( 1., -1.);
)shader_source"
R"shader_source(	//vec2 cell_fels_6 = cell_fels + vec2( 1.,  1.);
)shader_source"
R"shader_source(	//vec2 cell_fels_7 = cell_fels + vec2(-1.,  1.);
)shader_source"
R"shader_source(	//vec2 cell_fels_8 = cell_fels + vec2(-1., -1.);
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
R"shader_source(	//p_fels_5.xy *= rot2D(cell_fels_5.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	//p_fels_6.xy *= rot2D(cell_fels_6.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	//p_fels_7.xy *= rot2D(cell_fels_7.x * domrep_size.x * 32.73101);
)shader_source"
R"shader_source(	//p_fels_8.xy *= rot2D(cell_fels_8.x * domrep_size.x * 32.73101);
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
R"shader_source(	//p_fels_5.xz *= rot2D(cell_fels_5.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	//p_fels_6.xz *= rot2D(cell_fels_6.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	//p_fels_7.xz *= rot2D(cell_fels_7.y * domrep_size.y * 49.29012);
)shader_source"
R"shader_source(	//p_fels_8.xz *= rot2D(cell_fels_8.y * domrep_size.y * 49.29012);
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
R"shader_source(	//float fels_5 = box(p_fels_5, box_size);
)shader_source"
R"shader_source(	//float fels_6 = box(p_fels_6, box_size);
)shader_source"
R"shader_source(	//float fels_7 = box(p_fels_7, box_size);
)shader_source"
R"shader_source(	//float fels_8 = box(p_fels_8, box_size);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float fels_12 = min(fels_1, fels_2);
)shader_source"
R"shader_source(	float fels_34 = min(fels_3, fels_4);
)shader_source"
R"shader_source(	//float fels_56 = min(fels_5, fels_6);
)shader_source"
R"shader_source(	//float fels_78 = min(fels_7, fels_8);
)shader_source"
R"shader_source(	float fels_1234 = min(fels_12, fels_34);
)shader_source"
R"shader_source(	//float fels_5678 = min(fels_56, fels_78);
)shader_source"
R"shader_source(	//float fels_12345678 = min(fels_1234, fels_5678);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//float fels_012345678 = smin(fels_0, fels_12345678, mk_smin_felsen_rt_float * box_size.x);
)shader_source"
R"shader_source(	//return fels_012345678;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float fels_01234 = smin(fels_0, fels_1234, mk_smin_felsen_rt_float * box_size.x); // TODO: maybe remove this line for less/more? artefacts
)shader_source"
R"shader_source(	return fels_01234;
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
R"shader_source(const float background_fels_id = 23.;
)shader_source"
R"shader_source(const float background_kristall_id = 24.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 background(vec3 p) {
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
R"shader_source(	return smin_material(vec2(f_kristall, background_kristall_id), vec2(f_fels, background_fels_id), bg_smin_kristall_rt_float);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float opal(vec3 p) {
)shader_source"
R"shader_source(	const float size = 1.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_sphere = p;
)shader_source"
R"shader_source(	p_sphere.x /= morph_opal_stretch_x_rt_float;
)shader_source"
R"shader_source(	p_sphere.y /= morph_opal_stretch_y_rt_float;
)shader_source"
R"shader_source(	float f_sphere = sphere(p_sphere, size);
)shader_source"
R"shader_source(	f_sphere *= min(1., min(morph_opal_stretch_x_rt_float, morph_opal_stretch_y_rt_float));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_box1 = p;
)shader_source"
R"shader_source(	p_box1.xz *= rot2D(TAU / 12.);
)shader_source"
R"shader_source(	float f_box1 = box(p_box1, .5*vec3(morph_opal_stretch_x_rt_float, morph_opal_stretch_y_rt_float, size));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_box2 = p;
)shader_source"
R"shader_source(	p_box2.xz *= rot2D(-TAU / 12.);
)shader_source"
R"shader_source(	float f_box2 = box(p_box2, .5*vec3(morph_opal_stretch_x_rt_float, morph_opal_stretch_y_rt_float, size));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f = min(f_box1, f_box2);
)shader_source"
R"shader_source(	f = mix(f_sphere, f, morph_opal_smooth_rt_float);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float smooth_plane = size * morph_opal_smooth_plane_rt_float;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f_plane = abs(p.y);
)shader_source"
R"shader_source(	f_plane = smax(f, f_plane, smooth_plane);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f_plane1 = abs(p_box1.x);
)shader_source"
R"shader_source(	f_plane1 = smax(f, f_plane1, smooth_plane);
)shader_source"
R"shader_source(	f_plane = min(f_plane, f_plane1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float f_plane2 = abs(p_box2.x);
)shader_source"
R"shader_source(	f_plane2 = smax(f, f_plane2, smooth_plane);
)shader_source"
R"shader_source(	f_plane = min(f_plane, f_plane2);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	f = mix(f, f_plane, smooth_plane);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return f;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 augenlicht(vec3 p, vec3 d, vec3 normal) {
)shader_source"
R"shader_source(	if (dot(d, normal) >= 0.) {
)shader_source"
R"shader_source(		return morph_rt_color;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 n2 = vec3(1.1, 1.104, 1.106) * morph_reflectivness_rt_float;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 refraction_dir_r = refract(d, normal, n2.x);
)shader_source"
R"shader_source(	vec3 refraction_dir_g = refract(d, normal, n2.y);
)shader_source"
R"shader_source(	vec3 refraction_dir_b = refract(d, normal, n2.z);
)shader_source"
R"shader_source(	float refraction_red = length(refraction_dir_r) < 1e-3 ? 0. : textureLod(environment, refraction_dir_r, 0.).r;
)shader_source"
R"shader_source(	float refraction_green = length(refraction_dir_g) < 1e-3 ? 0. : textureLod(environment, refraction_dir_g, 0.).g;
)shader_source"
R"shader_source(	float refraction_blue = length(refraction_dir_b) < 1e-3 ? 0. : textureLod(environment, refraction_dir_b, 0.).b;
)shader_source"
R"shader_source(	vec3 refraction_color = vec3(refraction_red, refraction_green, refraction_blue);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 r0 = (1. - n2) / (1. + n2);
)shader_source"
R"shader_source(	r0 *= r0;
)shader_source"
R"shader_source(	float HdotV = -dot(normal, d);
)shader_source"
R"shader_source(	vec3 r = r0 + (1. - r0) * pow(1. - HdotV, 5.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 out_color;
)shader_source"
R"shader_source(	//out_color.rgb = vec3(max(dot(normal, normalize(vec3(1., .5, .5))), 0.) + .1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	//// TODO: maybe use the normal sss (ao) function
)shader_source"
R"shader_source(	vec3 n = -d;
)shader_source"
R"shader_source(	float ao_factor = 0.;
)shader_source"
R"shader_source(	float l = -.3;
)shader_source"
R"shader_source(	float i = 5.;
)shader_source"
R"shader_source(	for(; i > 0; i--) {
)shader_source"
R"shader_source(		vec3 p_i = p + n * i * l;
)shader_source"
R"shader_source(		// vor gebrauch kurz durchschwurbeln
)shader_source"
R"shader_source(		// nicht für den rohverzehr geeignet
)shader_source"
R"shader_source(		vec2 F = cellular(p_i * morph_noise_freq_rt_float + morph_schwurbel_intens_rt_float * vnoise(p_i * morph_schwurbel_freq_rt_float));
)shader_source"
R"shader_source(		float noise = F.y - F.x;
)shader_source"
R"shader_source(		//float noise = vnoise(p_i * morph_noise_freq_rt_float);
)shader_source"
R"shader_source(		//ao_factor -= (i * l - noise * f(p_i, false)[0]) / exp2(i);
)shader_source"
R"shader_source(		ao_factor += noise * abs(f(p_i, false)[0]) / exp2(i);
)shader_source"
R"shader_source(		//ao_factor += noise / exp2(i);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	//out_color.rgb *= .1;
)shader_source"
R"shader_source(	//refraction_color *= pow(clamp(ao_factor, 0., 1.), 10.);
)shader_source"
R"shader_source(	//out_color = mix(reflection_color, refraction_color, r);
)shader_source"
R"shader_source(	//out_color *= morph_rt_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 col = morph_rt_color;
)shader_source"
R"shader_source(	float rough = morph_rough_rt_float;
)shader_source"
R"shader_source(	float metallic = 1.;
)shader_source"
R"shader_source(	vec3 reflection_color = ambientColor(normal, -d, col, rough, metallic);
)shader_source"
R"shader_source(	// ganz viel spucke!
)shader_source"
R"shader_source(	//out_color.rgb *= .05;
)shader_source"
R"shader_source(	//out_color.rgb += ao_factor * 2.;
)shader_source"
R"shader_source(	refraction_color = ao_factor * morph_rt_color * max(vec3(morph_helligkeit_rt_float), refraction_color);
)shader_source"
R"shader_source(	out_color = mix(refraction_color, reflection_color, r);
)shader_source"
R"shader_source(	out_color = max(vec3(0.), out_color);
)shader_source"
R"shader_source(	return out_color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#define A_ vec2(0.,0.)
)shader_source"
R"shader_source(#define B_ vec2(1.,0.)
)shader_source"
R"shader_source(#define C_ vec2(2.,0.)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(//#define D_ vec2(0.,1.)
)shader_source"
R"shader_source(#define E_ vec2(1.,1.)
)shader_source"
R"shader_source(//#define F_ vec2(2.,1.)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#define G_ vec2(0.,2.)
)shader_source"
R"shader_source(#define H_ vec2(1.,2.)
)shader_source"
R"shader_source(#define I_ vec2(2.,2.)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#define J_ vec2(0.,3.)
)shader_source"
R"shader_source(#define K_ vec2(1.,3.)
)shader_source"
R"shader_source(#define L_ vec2(2.,3.)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#define M_ vec2(0.,4.)
)shader_source"
R"shader_source(#define N_ vec2(1.,4.)
)shader_source"
R"shader_source(#define O_ vec2(2.,4.)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(//#define P_ vec2(0.,5.)
)shader_source"
R"shader_source(//#define Q_ vec2(1.,5.)
)shader_source"
R"shader_source(//#define R_ vec2(1.,5.)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#define S_ vec2(0.,6.)
)shader_source"
R"shader_source(#define T_ vec2(1.,6.)
)shader_source"
R"shader_source(#define U_ vec2(2.0,6.)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#define A(p) min(min(min(t(G_,I_,p), t(I_,O_,p)), min(t(O_,M_, p), t(M_,J_,p))), t(J_,L_,p))
)shader_source"
R"shader_source(#define B(p) min(min(t(A_,M_,p), t(M_,O_,p)), min(t(O_,I_, p), t(I_,G_,p)))
)shader_source"
R"shader_source(#define C(p) min(min(t(I_,G_,p), t(G_,M_,p)), t(M_,O_,p))
)shader_source"
R"shader_source(#define D(p) min(min(t(C_,O_,p), t(O_,M_,p)), min(t(M_,G_,p), t(G_,I_,p)))
)shader_source"
R"shader_source(#define E(p) min(min(min(t(O_,M_,p), t(M_,G_,p)), min(t(G_,I_,p), t(I_,L_,p))), t(L_,J_,p))
)shader_source"
R"shader_source(#define F(p) min(min(t(C_,B_,p), t(B_,N_,p)), t(G_,I_,p))
)shader_source"
R"shader_source(#define G(p) min(min(min(t(O_,M_,p), t(M_,G_,p)), min(t(G_,I_,p), t(I_,U_,p))), t(U_,S_,p))
)shader_source"
R"shader_source(#define H(p) min(min(t(A_,M_,p), t(G_,I_,p)), t(I_,O_,p))
)shader_source"
R"shader_source(#define I(p) min(t(E_,E_,p), t(H_,N_,p))
)shader_source"
R"shader_source(#define J(p) min(min(t(E_,E_,p), t(H_,T_,p)), t(T_,S_,p))
)shader_source"
R"shader_source(#define K(p) min(min(t(A_,M_,p), t(M_,I_,p)), t(K_,O_,p))
)shader_source"
R"shader_source(#define L(p) t(B_,N_,p)
)shader_source"
R"shader_source(#define M(p) min(min(t(M_,G_,p), t(G_,I_,p)), min(t(H_,N_,p), t(I_,O_,p)))
)shader_source"
R"shader_source(#define N(p) min(min(t(M_,G_,p), t(G_,I_,p)), t(I_,O_,p))
)shader_source"
R"shader_source(#define O(p) min(min(t(G_,I_,p), t(I_,O_,p)), min(t(O_,M_, p), t(M_,G_,p)))
)shader_source"
R"shader_source(#define P(p) min(min(t(S_,G_,p), t(G_,I_,p)), min(t(I_,O_,p), t(O_,M_, p)))
)shader_source"
R"shader_source(#define Q(p) min(min(t(U_,I_,p), t(I_,G_,p)), min(t(G_,M_,p), t(M_,O_, p)))
)shader_source"
R"shader_source(#define R(p) min(t(M_,G_,p), t(G_,I_,p))
)shader_source"
R"shader_source(#define S(p) min(min(min(t(I_,G_,p), t(G_,J_,p)), min(t(J_,L_,p), t(L_,O_,p))), t(O_,M_,p))
)shader_source"
R"shader_source(#define T(p) min(min(t(B_,N_,p), t(N_,O_,p)), t(G_,I_,p))
)shader_source"
R"shader_source(#define U(p) min(min(t(G_,M_,p), t(M_,O_,p)), t(O_,I_,p))
)shader_source"
R"shader_source(#define V(p) min(min(t(G_,J_,p), t(J_,N_,p)), min(t(N_,L_,p), t(L_,I_,p)))
)shader_source"
R"shader_source(#define W(p) min(min(t(G_,M_,p), t(M_,O_,p)), min(t(N_,H_,p), t(O_,I_,p)))
)shader_source"
R"shader_source(#define X(p) min(t(G_,O_,p), t(I_,M_,p))
)shader_source"
R"shader_source(#define Y(p) min(min(t(G_,M_,p), t(M_,O_,p)), min(t(I_,U_,p), t(U_,S_,p)))
)shader_source"
R"shader_source(#define Z(p) min(min(t(G_,I_,p), t(I_,M_,p)), t(M_,O_,p))
)shader_source"
R"shader_source(#define STOP(p) t(N_,N_,p)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(//-----------------------------------------------------------------------------------
)shader_source"
R"shader_source(float t(vec2 v, vec2 w, vec2 p)
)shader_source"
R"shader_source({
)shader_source"
R"shader_source(	// Return minimum distance between line segment vw and point p
)shader_source"
R"shader_source(	float l2 = (v.x - w.x)*(v.x - w.x) + (v.y - w.y)*(v.y - w.y); //length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
)shader_source"
R"shader_source(	if (l2 == 0.0) {
)shader_source"
R"shader_source(		return distance(p, v);   // v == w case
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// Consider the line extending the segment, parameterized as v + t (w - v).
)shader_source"
R"shader_source(	// We find projection of point p onto the line.  It falls where t = [(p-v) . (w-v)] / |w-v|^2
)shader_source"
R"shader_source(	float t = dot(p - v, w - v) / l2;
)shader_source"
R"shader_source(	if(t < 0.0) {
)shader_source"
R"shader_source(		// Beyond the 'v' end of the segment
)shader_source"
R"shader_source(		return distance(p, v);
)shader_source"
R"shader_source(	} else if (t > 1.0) {
)shader_source"
R"shader_source(		return distance(p, w);  // Beyond the 'w' end of the segment
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	vec2 projection = v + t * (w - v);  // Projection falls on the segment
)shader_source"
R"shader_source(	return distance(p, projection);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// you need #include "font.glsl"
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void greetings(vec2 p, inout vec3 out_color, inout float zbuffer){
)shader_source"
R"shader_source(	p.y = -p.y;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 positions[4] = vec2[](
)shader_source"
R"shader_source(		/* top    left  */ vec2(-.8, -.7),
)shader_source"
R"shader_source(		/* top    right */ vec2( .0, -.7),
)shader_source"
R"shader_source(		/* bottom left  */ vec2(-.8,  .5),
)shader_source"
R"shader_source(		/* bottom right */ vec2( .0,  .5)
)shader_source"
R"shader_source(	);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 font_color = font_color_rt_color * font_brightness_rt_float;
)shader_source"
R"shader_source(	int which = int(greetings_rt_float);
)shader_source"
R"shader_source(	int where = int(greetings_where_rt_float);
)shader_source"
R"shader_source(	vec2 position_correction = greetings_position_rt_vec3.xy;
)shader_source"
R"shader_source(	float carret_inc = 3.;
)shader_source"
R"shader_source(	float carret;
)shader_source"
R"shader_source(	vec2 ap;
)shader_source"
R"shader_source(	float d;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	switch(which){
)shader_source"
R"shader_source(	case 1: /* XayaX */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = X(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, Y(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, X(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 2: /* stroboholics */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = S(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, T(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, B(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, H(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, L(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 3: /* still */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = S(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, T(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, L(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, L(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 4: /* peisik */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = P(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, K(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 5: /* nerdarzt */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = N(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, D(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, Z(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, T(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 6: /* Mercury */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = M(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, U(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, Y(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 7: /* LNX */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = L(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, N(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, X(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 8: /* farbrausch */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = F(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, B(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, U(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, H(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 9: /* conspiracy */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = C(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, N(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, P(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, Y(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 10: /* code red */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = C(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, D(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, D(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 11: /* brain control */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = B(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, N(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, N(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, T(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, L(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	case 12: /* ASD */
)shader_source"
R"shader_source(		ap = (p - positions[where] - position_correction) * font_size_rt_float;
)shader_source"
R"shader_source(		carret = 0.;
)shader_source"
R"shader_source(		d = A(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		d = min(d, D(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
)shader_source"
R"shader_source(		break;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if(which != 0){
)shader_source"
R"shader_source(		out_color = mix(out_color, font_color, step(d, 0.));
)shader_source"
R"shader_source(		zbuffer = mix(zbuffer, camera_focus_dist, step(d, 0.));
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void fels_color(vec3 p, out vec3 color, out float rough) {
)shader_source"
R"shader_source(	rough = mk_fels_rough_rt_float;
)shader_source"
R"shader_source(	float fels_noise = pow(cfbm(p * mk_fels_freq_rt_float) * .5 + .5, mk_fels_noise_power_rt_float);
)shader_source"
R"shader_source(	fels_noise *= smoothstep(0., mk_kristall_h_rt_float, p.y * mk_height_color_rt_float);
)shader_source"
R"shader_source(	color = mix(mk_fels_color1_rt_color, mk_fels_color2_rt_color, fels_noise);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 kristAll_color(vec3 p, vec3 fels_color, vec3 augenlicht_color) {
)shader_source"
R"shader_source(	vec2 F = cellular(p * .2 + vnoise(p * 4.) * .1 + vnoise(p) * .3);
)shader_source"
R"shader_source(	float cellnoise = F.y - F.x;
)shader_source"
R"shader_source(	cellnoise = 1. - clamp(cellnoise, 0., 1.);
)shader_source"
R"shader_source(	cellnoise = pow(cellnoise, 20.);
)shader_source"
R"shader_source(	return mix(fels_color, augenlicht_color, cellnoise);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 10
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(const float kristall_id = 1.;
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
R"shader_source(		if (material == kristall_id) {
)shader_source"
R"shader_source(			out_color.rgb = augenlicht(p, d, normal);
)shader_source"
R"shader_source(		} else if (material == background_fels_id || material == background_kristall_id) {
)shader_source"
R"shader_source(			vec3 col;
)shader_source"
R"shader_source(			float rough;
)shader_source"
R"shader_source(			float metallic = 0.;
)shader_source"
R"shader_source(			fels_color(p + vec3(0., boden_hoehe_rt_float, 0.), col, rough);
)shader_source"
R"shader_source(			out_color.rgb = ambientColor(normal, -d, col, rough, metallic);
)shader_source"
R"shader_source(			out_color.rgb *= clamp(ao(o + t * d, normal, .4, 5), 0., 1.);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (material == background_kristall_id) {
)shader_source"
R"shader_source(			vec3 kristAllColor = augenlicht(p, d, normal);
)shader_source"
R"shader_source(			out_color = kristAll_color(p, out_color, kristAllColor);
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		//out_color.rgb = abs(normal);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 position = gl_FragCoord.xy / res * 2. - 1.;
)shader_source"
R"shader_source(	greetings(position, out_color, t);
)shader_source"
R"shader_source(
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
R"shader_source(		f = mix(kristall(p), hexshape(p), smoothstep(0., 1., morph_mix_rt_float));
)shader_source"
R"shader_source(	} else if (morph_mix_rt_float <= 2.) {
)shader_source"
R"shader_source(		f = mix(hexshape(p), octahedronthingie(p), smoothstep(1., 2., morph_mix_rt_float));
)shader_source"
R"shader_source(	} else if (morph_mix_rt_float <= 3.) {
)shader_source"
R"shader_source(		f = mix(octahedronthingie(p), trishape(p), smoothstep(2., 3., morph_mix_rt_float));
)shader_source"
R"shader_source(	} else if (morph_mix_rt_float <= 4.) {
)shader_source"
R"shader_source(		f = mix(trishape(p), trillant(p), smoothstep(3., 4., morph_mix_rt_float));
)shader_source"
R"shader_source(	} else if (morph_mix_rt_float <= 5.) {
)shader_source"
R"shader_source(		f = mix(trillant(p), opal(p), smoothstep(4., 5., morph_mix_rt_float));
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
R"shader_source(	vec2 m_kristall = vec2(f, kristall_id);
)shader_source"
R"shader_source(	vec2 m_bg = background(p);
)shader_source"
R"shader_source(	m_kristall = min_material(m_bg, m_kristall);
)shader_source"
R"shader_source(	return m_kristall;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
