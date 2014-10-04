#ifndef zapfen_H
#define zapfen_H
const char zapfen_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(layout(location = 1) uniform float envelopes[32];
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color; // alpha = CoC
)shader_source"
R"shader_source(layout(location = 1) out float coc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 49) uniform float focus_dist;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 39) uniform float focal_length;
)shader_source"
R"shader_source(#define FOCAL_LENGTH
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 36) uniform float f_stop;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void output_color(vec3 color, float dist) {
)shader_source"
R"shader_source(	float focus_dist = focus_dist;
)shader_source"
R"shader_source(	float f = focal_length;
)shader_source"
R"shader_source(	float N = f_stop;
)shader_source"
R"shader_source(	coc = (dist - focus_dist)/dist * (f * f) / (N * (focus_dist - f)) / 0.03 * res.x;
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
R"shader_source(// fancy functions TODO
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 37) uniform vec3 camera_position;
)shader_source"
R"shader_source(layout(location = 38) uniform vec4 camera_rotation;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 33) uniform float time;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#ifndef FOCAL_LENGTH
)shader_source"
R"shader_source(layout(location = 39) uniform float focal_length;
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float TAU = 6.28318530718;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*
)shader_source"
R"shader_source(mat3 get_camera() {
)shader_source"
R"shader_source(	vec3 view_right = cross(camera_direction, camera_up);
)shader_source"
R"shader_source(	return mat3(view_right, camera_up, -camera_direction);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(*/
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
R"shader_source(vec3 get_direction() {
)shader_source"
R"shader_source(	vec3 dir = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.x , -focal_length / .03));
)shader_source"
R"shader_source(	return quat_rotate(dir, camera_rotation);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 march_adv(vec3 p, vec3 direction, out int i, int iterations, float stepsize) {
)shader_source"
R"shader_source(	float walked = 0.;
)shader_source"
R"shader_source(	for (i=0; i < iterations; i++) {
)shader_source"
R"shader_source(		float dist = f(p)[0] * stepsize;
)shader_source"
R"shader_source(		p += direction * dist;
)shader_source"
R"shader_source(		dist = abs(dist);
)shader_source"
R"shader_source(		walked += dist;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if (dist < .001 * walked) break;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return p;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 march(vec3 p, vec3 direction, out int i) {
)shader_source"
R"shader_source(	return march_adv(p, direction, i, 100, 1.);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 calc_normal(vec3 p) {
)shader_source"
R"shader_source(	vec2 epilepsilon = vec2(.001, 0.);
)shader_source"
R"shader_source(	return normalize(vec3(
)shader_source"
R"shader_source(		f(p + epilepsilon.xyy)[0] - f(p - epilepsilon.xyy)[0],
)shader_source"
R"shader_source(		f(p + epilepsilon.yxy)[0] - f(p - epilepsilon.yxy)[0],
)shader_source"
R"shader_source(		f(p + epilepsilon.yyx)[0] - f(p - epilepsilon.yyx)[0]
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
R"shader_source(// a and b are the endpoints
)shader_source"
R"shader_source(// r is the radius if you want some kind of capsule
)shader_source"
R"shader_source(float line(vec3 p, vec3 a, vec3 b, float r) {
)shader_source"
R"shader_source(	vec3 pa = p - a;
)shader_source"
R"shader_source(	vec3 ba = b - a;
)shader_source"
R"shader_source(	float h = clamp(dot(pa,ba) / dot(ba,ba), 0., 1. );
)shader_source"
R"shader_source(	return length(pa - ba*h) - r;
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
R"shader_source(float cylinder(vec3 p, float radius, float thicknesshalf) {
)shader_source"
R"shader_source(	float circle = length(p.xy) - radius;
)shader_source"
R"shader_source(	return max(circle, abs(p.z) - thicknesshalf);
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
R"shader_source(struct Material {
)shader_source"
R"shader_source(	vec3 color;
)shader_source"
R"shader_source(	float roughness;
)shader_source"
R"shader_source(	float metallic;
)shader_source"
R"shader_source(};
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(struct SphereLight {
)shader_source"
R"shader_source(	vec3 center;
)shader_source"
R"shader_source(	vec3 color;
)shader_source"
R"shader_source(	float radius;
)shader_source"
R"shader_source(	float intensity;
)shader_source"
R"shader_source(};
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// positive dot product
)shader_source"
R"shader_source(float pdot(vec3 a, vec3 b) {
)shader_source"
R"shader_source(	return max(0., dot(a, b));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float square(float a) {
)shader_source"
R"shader_source(	return a * a;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 apply_light(vec3 p, vec3 N, vec3 V, Material material, SphereLight light) {
)shader_source"
R"shader_source(	vec3 L = light.center - p;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// calculate representative point
)shader_source"
R"shader_source(	vec3 r = reflect(-V, N);
)shader_source"
R"shader_source(	vec3 centerToRay = dot(L, r) * r - L;
)shader_source"
R"shader_source(	vec3 closestPoint = L + centerToRay * clamp(light.radius/length(centerToRay), 0., 1.);
)shader_source"
R"shader_source(	vec3 L_spec = normalize(closestPoint);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float light_distance = distance(p, light.center);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// specular
)shader_source"
R"shader_source(	float PI = acos(-1.);
)shader_source"
R"shader_source(	vec3 H = .5 * (L_spec + V);
)shader_source"
R"shader_source(	float NdotH = dot(N, H);
)shader_source"
R"shader_source(	float NdotL= pdot(N, L_spec);
)shader_source"
R"shader_source(	float NdotV = dot(N, V);
)shader_source"
R"shader_source(	float alpha = square(material.roughness);
)shader_source"
R"shader_source(	float alpha2 = square(alpha);
)shader_source"
R"shader_source(	float k = square(material.roughness + 1.);
)shader_source"
R"shader_source(	float specular_without_F = alpha2 / (4. * mix(NdotL, 1., k) * mix(NdotV, 1., k) * PI * square(square(NdotH) * (alpha2 - 1.) + 1.));
)shader_source"
R"shader_source(	float alphastrich = clamp(alpha + light.radius * .5 / light_distance, 0., 1.);
)shader_source"
R"shader_source(	float spec_norm = alpha2 / (alphastrich * alphastrich);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float VdotH = dot(V, H);
)shader_source"
R"shader_source(	float fresnel_power = exp2((-5.55473 * VdotH - 6.98316) * VdotH);
)shader_source"
R"shader_source(	float F_dielectric = .04 + (1. - .04) * fresnel_power;
)shader_source"
R"shader_source(	vec3 F_metal = material.color + (1. - material.color) * fresnel_power;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 dielectric_color = .5 * (material.color / PI + specular_without_F * F_dielectric);
)shader_source"
R"shader_source(	vec3 metal_color = specular_without_F * F_metal;
)shader_source"
R"shader_source(	float foo = square(1. - square(square(light_distance / light.radius)));
)shader_source"
R"shader_source(	float falloff = clamp(foo, 0., 1.) / (square(light_distance) + 1.);
)shader_source"
R"shader_source(	return NdotL * mix(dielectric_color, metal_color, material.metallic) * falloff * light.intensity * light.color * spec_norm;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 emit_light(vec3 color, float intensity) {
)shader_source"
R"shader_source(	return color * intensity;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 domrepv(vec3 p, vec3 c) {
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
R"shader_source(	return domrepv(p, vec3(x, y, z));
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
R"shader_source(vec3 transv(vec3 p, vec3 v) {
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
R"shader_source(	return transv(p, vec3(x, y, z));
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
R"shader_source(// impulse from iq's website
)shader_source"
R"shader_source(// k is the sharpness of the curve
)shader_source"
R"shader_source(float impulse(float k, float x) {
)shader_source"
R"shader_source(	const float h = k * x;
)shader_source"
R"shader_source(	return h * exp(1. - h);
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
R"shader_source(float ao(vec3 p, vec3 n, float d, float i) {
)shader_source"
R"shader_source(	float o, s = sign(d);
)shader_source"
R"shader_source(	for(o = s * .5 + .5; i > 0; i--) {
)shader_source"
R"shader_source(		o -= (i * d - f(p + n * i * d * s)[0]) / exp2(i);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return o;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 50) uniform float conic_bobbel_noifreq;
)shader_source"
R"shader_source(layout(location = 51) uniform float conic_bobbel_roughness;
)shader_source"
R"shader_source(layout(location = 52) uniform float conic_ring_animation;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// l_body is size of bobbel, function is optimized for 2.
)shader_source"
R"shader_source(float conicbobbel(vec3 p_cone, float l_body) {
)shader_source"
R"shader_source(	////// distfunctions for body (some day in the past it was a cone)
)shader_source"
R"shader_source(	float radius_mixer = smoothstep(l_body * .3, l_body * 1., p_cone.x)
)shader_source"
R"shader_source(		+ .3 * (1. - smoothstep(l_body * .0, l_body * .3, p_cone.x)); // used to mix between min_ and max_radius
)shader_source"
R"shader_source(	float max_radius = .5;
)shader_source"
R"shader_source(	float min_radius = .15;
)shader_source"
R"shader_source(	float line_radius = mix(min_radius, max_radius, radius_mixer);;
)shader_source"
R"shader_source(	float f_cone = line(p_cone, vec3(l_body, 0., 0.), vec3(0., 0., 0.), line_radius);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	////// akward spikes
)shader_source"
R"shader_source(	float num_spikes = 8.;
)shader_source"
R"shader_source(	float height_spikes = .8;
)shader_source"
R"shader_source(	float sharpness_spikes = 7.;
)shader_source"
R"shader_source(	// move where the center of the spikes should be
)shader_source"
R"shader_source(	vec3 p_spike = trans(p_cone, l_body * .9, 0., 0.);
)shader_source"
R"shader_source(	// make cylindrical domrep; x -> x, y -> radius, z -> angle
)shader_source"
R"shader_source(	p_spike.yz = vec2(length(p_spike.yz), atan(p_spike.y, p_spike.z) / TAU);
)shader_source"
R"shader_source(	vec3 p_pre_spike = p_spike;
)shader_source"
R"shader_source(	p_spike = domrep(p_spike, 1., 1., 1./num_spikes);
)shader_source"
R"shader_source(	p_spike.xy = p_pre_spike.xy; // don't domrep x and radius
)shader_source"
R"shader_source(	p_spike.y -= height_spikes;
)shader_source"
R"shader_source(	p_spike.z *= 3.2;
)shader_source"
R"shader_source(	// you may uncomment following rotation line, but then there will be lots of ugly artefacts, if you do, then change the center of the spikes to x = l_body * .9
)shader_source"
R"shader_source(	p_spike = rZ(TAU * -(min(p_spike.y, .0) + 1.) / 7.)* p_spike;
)shader_source"
R"shader_source(	float f_spike = cone(p_spike.xzy, normalize(vec2(sharpness_spikes, 1.)));
)shader_source"
R"shader_source(	f_cone = smin(f_cone, f_spike, .1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	////// perlin make some noise!
)shader_source"
R"shader_source(	vec2 surface_coord = vec2(p_cone.x * 3. + time, atan(p_cone.y, p_cone.z));
)shader_source"
R"shader_source(	float noise_amplitude = smoothstep(-.6, 0., p_cone.x) * (1. - smoothstep(l_body + .3, l_body + .5, p_cone.x)) * .03;
)shader_source"
R"shader_source(	f_cone -= noise_amplitude * smoothstep(conic_bobbel_roughness, 1., cnoise(surface_coord * conic_bobbel_noifreq)); // smoothstep cuts the lower half of the noise to zero, -1 and 0 are both good values for conic_bobbel_roughness
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return f_cone;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// l_body is size of bobbel, function is optimized for 2.
)shader_source"
R"shader_source(// this one totally noiseless
)shader_source"
R"shader_source(float conicbobbel_fast(vec3 p_cone, float l_body) {
)shader_source"
R"shader_source(	////// distfunctions for body (some day in the past it was a cone)
)shader_source"
R"shader_source(	float radius_mixer = smoothstep(l_body * .3, l_body * 1., p_cone.x)
)shader_source"
R"shader_source(		+ .3 * (1. - smoothstep(l_body * .0, l_body * .3, p_cone.x)); // used to mix between min_ and max_radius
)shader_source"
R"shader_source(	float max_radius = .5;
)shader_source"
R"shader_source(	float min_radius = .15;
)shader_source"
R"shader_source(	float line_radius = mix(min_radius, max_radius, radius_mixer);;
)shader_source"
R"shader_source(	float f_cone = line(p_cone, vec3(l_body, 0., 0.), vec3(0., 0., 0.), line_radius);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	////// akward spikes
)shader_source"
R"shader_source(	float num_spikes = 8.;
)shader_source"
R"shader_source(	float height_spikes = .8;
)shader_source"
R"shader_source(	float sharpness_spikes = 7.;
)shader_source"
R"shader_source(	// move where the center of the spikes should be
)shader_source"
R"shader_source(	vec3 p_spike = trans(p_cone, l_body * .9, 0., 0.);
)shader_source"
R"shader_source(	// make cylindrical domrep; x -> x, y -> radius, z -> angle
)shader_source"
R"shader_source(	p_spike.yz = vec2(length(p_spike.yz), atan(p_spike.y, p_spike.z) / TAU);
)shader_source"
R"shader_source(	vec3 p_pre_spike = p_spike;
)shader_source"
R"shader_source(	p_spike = domrep(p_spike, 1., 1., 1./num_spikes);
)shader_source"
R"shader_source(	p_spike.xy = p_pre_spike.xy; // don't domrep x and radius
)shader_source"
R"shader_source(	p_spike.y -= height_spikes;
)shader_source"
R"shader_source(	p_spike.z *= 3.2;
)shader_source"
R"shader_source(	// you may uncomment following rotation line, but then there will be lots of ugly artefacts, if you do, then change the center of the spikes to x = l_body * .9
)shader_source"
R"shader_source(	p_spike = rZ(TAU * -(min(p_spike.y, .0) + 1.) / 7.)* p_spike;
)shader_source"
R"shader_source(	float f_spike = cone(p_spike.xzy, normalize(vec2(sharpness_spikes, 1.)));
)shader_source"
R"shader_source(	f_cone = smin(f_cone, f_spike, .1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	////// perlin make some noise!
)shader_source"
R"shader_source(	//vec2 surface_coord = vec2(p_cone.x * 3. + time, atan(p_cone.y, p_cone.z));
)shader_source"
R"shader_source(	//float noise_amplitude = smoothstep(-.6, 0., p_cone.x) * (1. - smoothstep(l_body + .3, l_body + .5, p_cone.x)) * .03;
)shader_source"
R"shader_source(	//f_cone -= noise_amplitude * smoothstep(conic_bobbel_roughness, 1., cnoise(surface_coord * conic_bobbel_noifreq)); // smoothstep cuts the lower half of the noise to zero, -1 and 0 are both good values for conic_bobbel_roughness
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return f_cone;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float bobbelring(vec3 p_cone, float l_body, float factorDeltaT) {
)shader_source"
R"shader_source(	////// big, bright rings
)shader_source"
R"shader_source(	float T = 1.; // duration of one animation cyclus
)shader_source"
R"shader_source(	float deltaT = T * factorDeltaT; // delta between first and second ring
)shader_source"
R"shader_source(	float bgn_rng_anim = l_body + .7; // start point of ring animation
)shader_source"
R"shader_source(	float end_rng_anim = -1.; // end point of ring anmation
)shader_source"
R"shader_source(	float max_ring_amp = 1.5; // maximum ring amplitude
)shader_source"
R"shader_source(	vec3 p_ring = trans(p_cone, bgn_rng_anim + (end_rng_anim - bgn_rng_anim) * mod(conic_ring_animation + deltaT, T) / T, 0., 0.);
)shader_source"
R"shader_source(	float ring_radius = max_ring_amp * (impulse(8., mod(conic_ring_animation + deltaT, T) / T) - .025); // first arg is impulse shape
)shader_source"
R"shader_source(	float ring_r_thickness = .2 * ring_radius; // thickness of ring in r direction
)shader_source"
R"shader_source(	float ring_x_thickness = .03; // thickness of ring in x direction
)shader_source"
R"shader_source(	float f_ring = 0.;
)shader_source"
R"shader_source(	f_ring = abs(sphere(p_ring, ring_radius)) - ring_r_thickness;
)shader_source"
R"shader_source(	f_ring = smax(f_ring, abs(p_ring.x) - ring_x_thickness, .02);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return f_ring;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 4
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 67) uniform vec3 zapfen_background_color; // color
)shader_source"
R"shader_source(layout(location = 101) uniform float zapfen_kreise;
)shader_source"
R"shader_source(layout(location = 73) uniform float zapfen_leit_freq;
)shader_source"
R"shader_source(layout(location = 102) uniform vec3 zapfen_color1; // color
)shader_source"
R"shader_source(layout(location = 103) uniform vec3 zapfen_color2; // color
)shader_source"
R"shader_source(layout(location = 104) uniform float zapfen_mat_freq;
)shader_source"
R"shader_source(layout(location = 105) uniform float zapfen_rough1;
)shader_source"
R"shader_source(layout(location = 106) uniform float zapfen_rough2;
)shader_source"
R"shader_source(layout(location = 107) uniform vec3 zapfen_col_boden; // color
)shader_source"
R"shader_source(layout(location = 108) uniform vec3 zapfen_col_leit_glow; // color
)shader_source"
R"shader_source(layout(location = 109) uniform vec3 zapfen_col_leit_non; // color
)shader_source"
R"shader_source(layout(location = 110) uniform vec3 zapfen_col_bobbel; // color
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 111) uniform vec3 zapfen_light1_pos;
)shader_source"
R"shader_source(layout(location = 112) uniform vec3 zapfen_light2_pos;
)shader_source"
R"shader_source(layout(location = 113) uniform vec3 zapfen_light1_col; // color
)shader_source"
R"shader_source(layout(location = 114) uniform vec3 zapfen_light2_col; // color
)shader_source"
R"shader_source(layout(location = 115) uniform float zapfen_light_radius;
)shader_source"
R"shader_source(layout(location = 116) uniform float zapfen_light_intensity;
)shader_source"
R"shader_source(layout(location = 117) uniform float zapfen_bobbel_progress;
)shader_source"
R"shader_source(layout(location = 118) uniform vec3 zapfen_bobbel_displacement;
)shader_source"
R"shader_source(layout(location = 119) uniform bool zapfen_bobbels_enabled;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(bool add_boden = false;
)shader_source"
R"shader_source(bool normal_mapping = false;
)shader_source"
R"shader_source(//const bool add_boden = true;
)shader_source"
R"shader_source(//const bool normal_mapping = true;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 apply_lights(vec3 p, vec3 N, vec3 V, Material mat) {
)shader_source"
R"shader_source(	vec3 color = vec3(0.);
)shader_source"
R"shader_source(	color += apply_light(p, N, V, mat, SphereLight(zapfen_light1_pos, zapfen_light1_col, zapfen_light_radius, zapfen_light_intensity));
)shader_source"
R"shader_source(	color += apply_light(p, N, V, mat, SphereLight(zapfen_light2_pos, zapfen_light2_col, zapfen_light_radius, zapfen_light_intensity));
)shader_source"
R"shader_source(	return color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	vec3 dir = get_direction();
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	int i;
)shader_source"
R"shader_source(	vec3 p = march_adv(camera_position, dir, i, 100, .9);
)shader_source"
R"shader_source(	if(abs(f(p)[1]) == 4.) {
)shader_source"
R"shader_source(		if(dir.y != 0) {
)shader_source"
R"shader_source(			float t = (-(camera_position.y - 20.) / dir.y);
)shader_source"
R"shader_source(			if(t > 0.) {
)shader_source"
R"shader_source(				add_boden = true;
)shader_source"
R"shader_source(				p = march_adv(camera_position + t * dir, dir, i, 20, .9);
)shader_source"
R"shader_source(			}
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float materialId = f(p)[1];
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 color = vec3(0.);
)shader_source"
R"shader_source(	normal_mapping = true;
)shader_source"
R"shader_source(	vec3 normal = calc_normal(p);
)shader_source"
R"shader_source(	vec3 bounding_color = .01 * zapfen_background_color;
)shader_source"
R"shader_source(	if(materialId == 4.) {
)shader_source"
R"shader_source(		color = bounding_color;
)shader_source"
R"shader_source(	} else if(materialId >= 0. && materialId < 1.) {
)shader_source"
R"shader_source(		Material material_bobbel = Material(zapfen_col_bobbel, .3, 1.);
)shader_source"
R"shader_source(		vec3 color_bobbel = 1000. * apply_lights(p, normal, -dir, material_bobbel);
)shader_source"
R"shader_source(		//vec3 color_bobbel = emit_light(zapfen_col_bobbel, 10.) * pdot(normal, -dir);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float foo = pow(smoothstep(-80., -20., -p.y), 7.) * smoothstep(.3, .9, cfbm(p * zapfen_mat_freq * vec3(vnoise(.2 * p + 7.) * .01 + 1., .3, vnoise(.2 * p + 3.) * .01 + 1.)) * .5 + .5);
)shader_source"
R"shader_source(		Material material1 = Material(mix(zapfen_color1, zapfen_color2, foo), zapfen_rough1, .0);
)shader_source"
R"shader_source(		vec3 color_zapfen = apply_lights(p, normal, -dir, material1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		color = mix(color_bobbel, color_zapfen, pow(materialId - 0., .005));
)shader_source"
R"shader_source(	} else if(materialId >= 1. && materialId <= 2.) {
)shader_source"
R"shader_source(		float foo = pow(smoothstep(-80., -20., -p.y), 7.) * smoothstep(.3, .9, cfbm(p * zapfen_mat_freq * vec3(vnoise(.2 * p + 7.) * .01 + 1., .3, vnoise(.2 * p + 3.) * .01 + 1.)) * .5 + .5);
)shader_source"
R"shader_source(		Material material1 = Material(mix(zapfen_color1, zapfen_color2, foo), zapfen_rough1, .0);
)shader_source"
R"shader_source(		vec3 color1 = apply_lights(p, normal, -dir, material1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		Material material2 = Material(zapfen_col_boden, 1., 0.);
)shader_source"
R"shader_source(		vec3 color2 = apply_lights(p, normal, -dir, material2);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		color = mix(color1, color2, pow(materialId - 1., 6.));
)shader_source"
R"shader_source(	} else if(materialId > 2. && materialId <= 3.) {
)shader_source"
R"shader_source(		Material material = Material(zapfen_col_leit_non, .9, 0.);
)shader_source"
R"shader_source(		vec3 nonglowing = apply_lights(p, normal, -dir, material);
)shader_source"
R"shader_source(		vec3 glowing = emit_light(zapfen_col_leit_glow, .005);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		float glow;// = cnoise(p.xz * .001 + time) * .5 + .5;
)shader_source"
R"shader_source(		// estimate coordinate of next zapfen
)shader_source"
R"shader_source(		vec2 floored = floor(p.xz/65.) * 65.;
)shader_source"
R"shader_source(		vec2 ceiled = ceil(p.xz/65.) * 65.;
)shader_source"
R"shader_source(		vec2 next = floored;
)shader_source"
R"shader_source(		if(abs(p.x - floored.x) > abs(p.x - ceiled.x)) {
)shader_source"
R"shader_source(			next.x = ceiled.x;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(		if(abs(p.y - floored.y) > abs(p.y - ceiled.y)) {
)shader_source"
R"shader_source(			next.y = ceiled.y;
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(		glow = sin(distance(next, p.xz) * .5 + 6. * time) * (vnoise(floor(p.xz/65.)*20. + time * .4) * .5 + .5);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		color = mix(nonglowing, glowing, pow(glow * .5 + .5, 5.));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		Material material2 = Material(zapfen_col_boden, 1., 0.);
)shader_source"
R"shader_source(		vec3 color2 = apply_lights(p, normal, -dir, material2);
)shader_source"
R"shader_source(		color = mix(color2, color, pow(materialId - 2., .3));
)shader_source"
R"shader_source(		//color = nonglowing;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float dist = distance(camera_position, p);
)shader_source"
R"shader_source(	color = mix(color, bounding_color, smoothstep(220., 300., dist));
)shader_source"
R"shader_source(	output_color(color, dist);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float boden(vec3 p);
)shader_source"
R"shader_source(float leitungen(vec3 p, float rotation, float radius, float freq, float t);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p) {
)shader_source"
R"shader_source(	vec2 bounding = vec2(-sphere(camera_position - p, 300.), 4.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float height = 40.; // .5 * height
)shader_source"
R"shader_source(	vec3 q = domrepv(p, vec3(75.));
)shader_source"
R"shader_source(	q.y = p.y - height;
)shader_source"
R"shader_source(	float k = smoothstep(-height * 1.2, height, -q.y);
)shader_source"
R"shader_source(	vec2 ij = floor(p.xz / 50.);
)shader_source"
R"shader_source(	float rotation = sin(7. * dot(ij, ij) + zapfen_bobbel_progress * .25);
)shader_source"
R"shader_source(	float rotation2 = cos(11. * dot(ij, ij) + zapfen_bobbel_progress * .25);
)shader_source"
R"shader_source(	q.xz *= rot2D((1. - k) * radians(100.));
)shader_source"
R"shader_source(	mat2 rot_matrix = rot2D(rotation * (1. - k) * radians(7.));
)shader_source"
R"shader_source(	q.xy *= rot_matrix;
)shader_source"
R"shader_source(	q.yz *= rot_matrix;
)shader_source"
R"shader_source(	float radius = 10. * k;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float d = max(length(q.xz) - radius, abs(q.y) - height);
)shader_source"
R"shader_source(	if(normal_mapping) {
)shader_source"
R"shader_source(		vec2 polar = vec2(q.y, atan(q.x, q.z));
)shader_source"
R"shader_source(		polar *= vec2(1., 3.);
)shader_source"
R"shader_source(		d += .06 * cnoise((2. * vnoise(polar) + polar) * .5);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	d = smin(d, sphere(trans(q, 0., height, 0), 3.), 7.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 object = vec2(d, 1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if(add_boden) {
)shader_source"
R"shader_source(		vec3 q = trans(p, 0., 10. * (vnoise(.05 * p.xz) * .5 + .5), 0.);
)shader_source"
R"shader_source(		vec2 ground = vec2(q.y, 2.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		vec2 boden_object = smin_smaterial(object, ground, 10.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		vec2 leit;
)shader_source"
R"shader_source(		//if(p.y < 20.) {
)shader_source"
R"shader_source(			float t = vnoise(vec2(q.z, floor(q.x/80.) * floor(q.z/80.)) * zapfen_leit_freq + 333. * 38.) * .5 + .5;
)shader_source"
R"shader_source(			float leit1 = leitungen(q, 0., .6, 1.7, t);
)shader_source"
R"shader_source(			//float leit2 = leitungen(q, 50., .6, 27.3);
)shader_source"
R"shader_source(			//float leit3 = leitungen(q, 20., .6, 549.2);
)shader_source"
R"shader_source(			float leit4 = leitungen(q, 80., .6, 123.7, t);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(			//vec2 leit1 = leitungen(q, 0., .2, 1.7);
)shader_source"
R"shader_source(			//vec2 leit2 = leitungen(q, 50., .2, 2.3);
)shader_source"
R"shader_source(			//vec2 leit3 = leitungen(q, 30., .2, 3.2);
)shader_source"
R"shader_source(			//vec2 leit4 = leitungen(q, 70., .2, 2.7);
)shader_source"
R"shader_source(			float k = .5;
)shader_source"
R"shader_source(			//leit = vec2(smin(smin(leit1, leit2, k), smin(leit3, leit4, k), k), 3.);
)shader_source"
R"shader_source(			//leit = vec2(smin(smin(leit1, leit2, k), leit4, k), 3.);
)shader_source"
R"shader_source(			leit = vec2(smin(leit1, leit4, k), 3.);
)shader_source"
R"shader_source(		//} else {
)shader_source"
R"shader_source(		//	leit = vec2(p.y, 3.);
)shader_source"
R"shader_source(		//}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		object = smin_smaterial(boden_object, leit, .5);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		object = vec2(smax(object.x, 20. - p.y, 2.), 1.);
)shader_source"
R"shader_source(		//object = vec2(max(object.x, 20. - p.y), 1.);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_bobbel = trans(p, 0., 30., 0.);
)shader_source"
R"shader_source(	vec3 p_bobbel_unrot = p_bobbel;
)shader_source"
R"shader_source(	p_bobbel = transv(p_bobbel, zapfen_bobbel_displacement);
)shader_source"
R"shader_source(	p_bobbel.xy *= rot2D(radians(70.));
)shader_source"
R"shader_source(	p_bobbel.x -= zapfen_bobbel_progress * 2.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if(zapfen_bobbels_enabled) {
)shader_source"
R"shader_source(		vec3 cellsize = vec3(60.);
)shader_source"
R"shader_source(		vec3 p_bobbel_mod = domrepv(p_bobbel, cellsize);
)shader_source"
R"shader_source(		float bobbel_d = conicbobbel_fast(p_bobbel_mod, 2.);
)shader_source"
R"shader_source(		bobbel_d = max(bobbel_d, p_bobbel.x + 10.);
)shader_source"
R"shader_source(		bobbel_d = max(bobbel_d, -p_bobbel_unrot.y + 10.);
)shader_source"
R"shader_source(		vec2 bobbel = vec2(bobbel_d, 0.);
)shader_source"
R"shader_source(		object = smin_smaterial(object, bobbel, 2.);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	return min_material(bounding, object);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float leitungen(vec3 p, float rotation, float radius, float freq, float t) {
)shader_source"
R"shader_source(	// estimate coordinate of next zapfen
)shader_source"
R"shader_source(	//vec2 floored = floor(p.xz/50.) * 50.;
)shader_source"
R"shader_source(	//vec2 ceiled = ceil(p.xz/50.) * 50.;
)shader_source"
R"shader_source(	//vec2 next = floored;
)shader_source"
R"shader_source(	//if(abs(p.x - floored.x) > abs(p.x - ceiled.x)) {
)shader_source"
R"shader_source(	//	next.x = ceiled.x;
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(	//if(abs(p.y - floored.y) > abs(p.y - ceiled.y)) {
)shader_source"
R"shader_source(	//	next.y = ceiled.y;
)shader_source"
R"shader_source(	//}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p.xz *= rot2D(rotation);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	////float glow = smoothstep(.2, .8, vnoise(vec2(time * 3. + p.z * .2, 2000. * floor(p.x / 10. /freq))));
)shader_source"
R"shader_source(	//float glow = smoothstep(.2, .8, vnoise(vec2(distance(next, p.xz) * .1 + 1.3 * time, 2000. * rand(floor((p.xz + 25.) / 50.)))));
)shader_source"
R"shader_source(	//glow = 1.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	t *= sin(freq) * .5 + .5;
)shader_source"
R"shader_source(	//float t = vnoise(vec2(p.z, 0.) * zapfen_leit_freq + 333. * freq) * .5 + .5;
)shader_source"
R"shader_source(	p.xz += rot2D(radians(20.) * t) * vec2(10.) - 10.;
)shader_source"
R"shader_source(	//p = trans(p, 0., 2. * (t * .5 + .5), 0.);
)shader_source"
R"shader_source(	//p.x = mod(p.x, 20. / freq) - 10. / freq;
)shader_source"
R"shader_source(	//return vec2(length(p.xy) - radius, glow);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 q = trans(p, freq * 17., 0., -freq * 27.);
)shader_source"
R"shader_source(	q.xz = mod(q.xz, 50.) + 5.;
)shader_source"
R"shader_source(	float angle = atan(q.x, q.z);
)shader_source"
R"shader_source(	angle = mod(angle, radians(20.));
)shader_source"
R"shader_source(	float r = length(q.xz);
)shader_source"
R"shader_source(	q.x = r * sin(angle);
)shader_source"
R"shader_source(	return length(q.xy) - radius;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
