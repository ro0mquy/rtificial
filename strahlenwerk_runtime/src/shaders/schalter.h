#ifndef schalter_H
#define schalter_H
const char schalter_source[] =R"shader_source(#version 430
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
R"shader_source(layout(location = 73) uniform float zapfen_leit_freq;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float leitungen(vec3 p, float rotation, float radius, float freq, float domrep_size) {
)shader_source"
R"shader_source(	p.xz *= rot2D(rotation);
)shader_source"
R"shader_source(	float t = vnoise(vec2(p.z, floor(p.x/80.) * floor(p.z/80.)) * zapfen_leit_freq + 333. * freq) * .5 + .5;
)shader_source"
R"shader_source(	//float t = vnoise(vec2(p.z, 0.) * zapfen_leit_freq + 333. * freq) * .5 + .5;
)shader_source"
R"shader_source(	//p.xz += rot2D(radians(20.) * t) * vec2(10.) - 10.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 q = p;
)shader_source"
R"shader_source(	q.xz = mod(q.xz, domrep_size) + 5.;
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
R"shader_source(
)shader_source"
R"shader_source(#line 5
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 74) uniform float pyramid_h; // float
)shader_source"
R"shader_source(layout(location = 75) uniform float pyramid_s; // float
)shader_source"
R"shader_source(layout(location = 76) uniform float pyramid_animation; // float
)shader_source"
R"shader_source(layout(location = 77) uniform float pyramid_wave_animation; // float
)shader_source"
R"shader_source(layout(location = 78) uniform float pyramid_bottom; // float
)shader_source"
R"shader_source(layout(location = 79) uniform vec3 pyramid_color; // color
)shader_source"
R"shader_source(layout(location = 80) uniform vec3 pyramid_color2; // color
)shader_source"
R"shader_source(layout(location = 81) uniform vec3 pyramid_color_leitungen; // color
)shader_source"
R"shader_source(layout(location = 82) uniform vec3 pyramid_color_boden; // color
)shader_source"
R"shader_source(layout(location = 83) uniform vec3 schalter_light1_pos;
)shader_source"
R"shader_source(layout(location = 84) uniform vec3 schalter_light2_pos;
)shader_source"
R"shader_source(layout(location = 85) uniform vec3 schalter_background_color; // color
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(Material materials[6] = Material[6](
)shader_source"
R"shader_source(	Material(schalter_background_color, .5, 0.),
)shader_source"
R"shader_source(	Material(pyramid_color, 1., 1.),
)shader_source"
R"shader_source(	Material(pyramid_color_boden, .5, 0.),
)shader_source"
R"shader_source(	Material(vec3(.7, .3, .2), 1., 0.),
)shader_source"
R"shader_source(	Material(vec3(1.), .5, 0.),
)shader_source"
R"shader_source(	Material(pyramid_color_leitungen, 1., 0.)
)shader_source"
R"shader_source();
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(const int MATERIAL_ID_BOUNDING  = 0;
)shader_source"
R"shader_source(const int MATERIAL_ID_PYRAMID   = 1;
)shader_source"
R"shader_source(const int MATERIAL_ID_FLOOR     = 2;
)shader_source"
R"shader_source(const int MATERIAL_ID_CUBE      = 3;
)shader_source"
R"shader_source(const int MATERIAL_ID_LIGHTBALL = 4;
)shader_source"
R"shader_source(const int MATERIAL_ID_LEITUNGEN = 5;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pyramid(vec3 p, float s, float h);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 p_pyramid;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 apply_lights(vec3 p, vec3 N, vec3 V, Material mat) {
)shader_source"
R"shader_source(	vec3 color = vec3(0.);
)shader_source"
R"shader_source(	float radius = 2.;
)shader_source"
R"shader_source(	float intensity = 50.;
)shader_source"
R"shader_source(	color += apply_light(p, N, V, mat, SphereLight(
)shader_source"
R"shader_source(		schalter_light1_pos,
)shader_source"
R"shader_source(		vec3(1.),
)shader_source"
R"shader_source(		radius,
)shader_source"
R"shader_source(		intensity));
)shader_source"
R"shader_source(	color += apply_light(p, N, V, mat, SphereLight(
)shader_source"
R"shader_source(		schalter_light2_pos,
)shader_source"
R"shader_source(		vec3(1.),
)shader_source"
R"shader_source(		radius,
)shader_source"
R"shader_source(		intensity));
)shader_source"
R"shader_source(	return color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main(void) {
)shader_source"
R"shader_source(	vec3 direction = get_direction();
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	int i;
)shader_source"
R"shader_source(	vec3 hit = march(camera_position, direction, i);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 color = vec3(0.);
)shader_source"
R"shader_source(	float materialFloat = f(hit).y;
)shader_source"
R"shader_source(	int material = int(materialFloat);
)shader_source"
R"shader_source(	vec3 normal = calc_normal(hit);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	Material mat = materials[material];
)shader_source"
R"shader_source(	SphereLight light1 = SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float factor = 1.;
)shader_source"
R"shader_source(	if(material == MATERIAL_ID_PYRAMID) {
)shader_source"
R"shader_source(		float t = vnoise(p_pyramid * .4) * cfbm(p_pyramid * 3.);
)shader_source"
R"shader_source(		mat.roughness = .5 + .2 * sign(t) * pow(abs(t), .7) + .3 * cnoise(p_pyramid * .2 + 726.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		Material rust = Material(pyramid_color2, .9, .8);
)shader_source"
R"shader_source(		float rustiness = .1 + .06 * cnoise(p_pyramid * .5);
)shader_source"
R"shader_source(		rustiness *= 3.;
)shader_source"
R"shader_source(		color = apply_lights(hit, normal, -direction, rust) * rustiness;
)shader_source"
R"shader_source(		factor = 1. - rustiness;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	if (MATERIAL_ID_LIGHTBALL == material || MATERIAL_ID_CUBE == material) {
)shader_source"
R"shader_source(		color = emit_light(materials[MATERIAL_ID_LIGHTBALL].color, 5. * (smoothstep(-6., 0., pdot(-direction, normal)) * 10));
)shader_source"
R"shader_source(	} else if (material >= MATERIAL_ID_FLOOR && material <= MATERIAL_ID_CUBE) {
)shader_source"
R"shader_source(		vec3 nonglowing_floor = factor * apply_lights(hit, normal, -direction, mat);
)shader_source"
R"shader_source(		vec3 glowing_floor = emit_light(mat.color, 5.);
)shader_source"
R"shader_source(		float wave_height = hit.y + 2.;
)shader_source"
R"shader_source(		vec3 color_floor = mix(nonglowing_floor, glowing_floor, smoothstep(0., 2., wave_height));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		vec3 color_cube = emit_light(materials[MATERIAL_ID_LIGHTBALL].color, 5.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		color += mix(color_floor, color_cube, material - MATERIAL_ID_FLOOR);
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		color += factor * apply_lights(hit, normal, -direction, mat);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float dist = distance(hit, camera_position);
)shader_source"
R"shader_source(	output_color(color, dist);
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float pyramid(vec3 p, float s, float h){
)shader_source"
R"shader_source(	float alpha = atan(s/(2*h));
)shader_source"
R"shader_source(	// float alpha = s/(2*h*sqrt(s*s/(4*h*h)+1)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 n1 = vec3( cos(alpha), sin(alpha), 0.);
)shader_source"
R"shader_source(	vec3 n2 = vec3(-cos(alpha), sin(alpha), 0.);
)shader_source"
R"shader_source(	vec3 n3 = vec3(0., sin(alpha),  cos(alpha));
)shader_source"
R"shader_source(	vec3 n4 = vec3(0., sin(alpha), -cos(alpha));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float pyr;
)shader_source"
R"shader_source(	pyr = -p.y - h;
)shader_source"
R"shader_source(	pyr = max(pyr, plane(p, n1));
)shader_source"
R"shader_source(	pyr = max(pyr, plane(p, n2));
)shader_source"
R"shader_source(	pyr = max(pyr, plane(p, n3));
)shader_source"
R"shader_source(	pyr = max(pyr, plane(p, n4));
)shader_source"
R"shader_source(	return pyr;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p) {
)shader_source"
R"shader_source(	vec3 q = rY(TAU * 0.05 * time) * p;
)shader_source"
R"shader_source(	//q = trans(q, 0., 1.*sin(0.75 * time), 0.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// double pyramid
)shader_source"
R"shader_source(	float pyramid_size = 10.;
)shader_source"
R"shader_source(	vec3 qq = q;
)shader_source"
R"shader_source(	qq = trans(qq, 0., pyramid_h*pyramid_size, 0.); // y-axis translation
)shader_source"
R"shader_source(	qq = trans(qq, 0., pyramid_bottom, 0.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float t_fall_closing = 1. - pow((max(0.1,min(.9, pyramid_animation))-0.)/.9, 2.);
)shader_source"
R"shader_source(	float t_fall_opening = pow((max(0.,min(.1, pyramid_animation))-0.)/.1, 2.);
)shader_source"
R"shader_source(	float t_fall = t_fall_opening*(1-step(0.1,pyramid_animation)) + t_fall_closing*step(0.1,pyramid_animation);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float pyr_closed = 4. * t_fall;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	qq = trans(qq, 0., pyr_closed/2., 0.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float t_stampf = linstep(.9,1.,pyramid_animation);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	qq = trans(qq, 0.,  -  (1-pow(1.-t_stampf/2., 20.)), 0.);
)shader_source"
R"shader_source(	qq.y = -abs(qq.y);
)shader_source"
R"shader_source(	qq = trans(qq, 0., -pyr_closed/2., 0.); // pyramid distance
)shader_source"
R"shader_source(	qq.y = -pyramid_size*pyramid_h-qq.y;
)shader_source"
R"shader_source(	float pyr = pyramid(qq, pyramid_size * pyramid_s, pyramid_size * pyramid_h);
)shader_source"
R"shader_source(	float b = pyramid_size * pyramid_s * .5 * .9;
)shader_source"
R"shader_source(	pyr = smax(pyr, -roundbox(trans(qq, 0., -pyramid_size * pyramid_h, 0.), vec3(b, .1, b), .3), .05);
)shader_source"
R"shader_source(	p_pyramid = qq;
)shader_source"
R"shader_source(	vec2 pyramid = vec2(pyr, MATERIAL_ID_PYRAMID);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// box-torus-morph
)shader_source"
R"shader_source(	q = trans(q, 0., pyramid_size* pyramid_h + 1., 0.);
)shader_source"
R"shader_source(	q = trans(q, 0., pyramid_bottom, 0.);
)shader_source"
R"shader_source(	// .2 fuer schale
)shader_source"
R"shader_source(	vec3 cube_dimension = vec3(smoothstep(0., 0.1, pyramid_animation) + .2);
)shader_source"
R"shader_source(	float cube = roundbox(q, cube_dimension, .5);
)shader_source"
R"shader_source(	float d = .75 + smoothstep(.70,1., pyramid_animation)*7.5;
)shader_source"
R"shader_source(	float down_anim = - (pyramid_size * pyramid_h + 3. + pyramid_bottom) * smoothstep(0.75, 1., pyramid_animation);
)shader_source"
R"shader_source(	float mytorus = torus(trans(q, 0.,-.5 + down_anim,0.), vec2(.5 + d, .5));
)shader_source"
R"shader_source(	float f_schalter_cube = vec2(mix(cube, mytorus, smoothstep(.65, .75, pyramid_animation)), MATERIAL_ID_CUBE);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	/*
)shader_source"
R"shader_source(	float torus_2_down = -(pyramid_size * pyramid_h + 2. + pyramid_bottom) - 2.*linstep(.5,1.,pyramid_wave_animation);
)shader_source"
R"shader_source(	float torus_2_d = (.75+7.5 ) + pyramid_wave_animation * 10;
)shader_source"
R"shader_source(	float torus_2_dicke = 0.5 - 0.5 * linstep(.5,1.,pyramid_wave_animation);
)shader_source"
R"shader_source(	float torus_2 = torus(trans(q, 0.,-.5 + torus_2_down,0.), vec2(.5 + torus_2_d, torus_2_dicke));
)shader_source"
R"shader_source(	torus_2 += 100 * (1. - step(0.001, pyramid_wave_animation));
)shader_source"
R"shader_source(	//vec2 schalter_cube = vec2(min(f_schalter_cube, torus_2), MATERIAL_ID_CUBE);
)shader_source"
R"shader_source(	*/
)shader_source"
R"shader_source(	vec2 schalter_cube = vec2(f_schalter_cube, MATERIAL_ID_CUBE);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_lb = trans(p, 0., -2., 0.);
)shader_source"
R"shader_source(	vec3 dr_factor = vec3(7.,10.,8.);
)shader_source"
R"shader_source(	vec3 p_lb_dr = domrepv(p_lb, dr_factor);
)shader_source"
R"shader_source(	p_lb_dr = trans(p_lb_dr, 0.,2*vnoise(floor(p_lb/dr_factor)),0.);
)shader_source"
R"shader_source(	float lightballs_dist = sphere(p_lb_dr, 2+0.75*vnoise(floor(p_lb/dr_factor)));
)shader_source"
R"shader_source(	float no_lightballs = sphere(p, 20.);
)shader_source"
R"shader_source(	vec2 lightballs = vec2(max(lightballs_dist, -no_lightballs), MATERIAL_ID_LIGHTBALL);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float wave_position =
)shader_source"
R"shader_source(		+ 9. * step(.85, pyramid_animation)
)shader_source"
R"shader_source(		+ 1.5 * linstep(.9, 1., pyramid_animation)
)shader_source"
R"shader_source(		+ 10.5 * step(.00001, pyramid_wave_animation)
)shader_source"
R"shader_source(		+ 15. * pyramid_wave_animation
)shader_source"
R"shader_source(		;
)shader_source"
R"shader_source(	float wave_amplitude = 0.
)shader_source"
R"shader_source(		+ 2. * smoothstep(.87, .95, pyramid_animation)
)shader_source"
R"shader_source(		+ 2. * step(.00001, pyramid_wave_animation)
)shader_source"
R"shader_source(		- 2. * smoothstep(.00001, 1., pyramid_wave_animation)
)shader_source"
R"shader_source(		;
)shader_source"
R"shader_source(	float wave_width = 2.;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_boden = trans(p, 0., -2., 0.);
)shader_source"
R"shader_source(	float wave = length(p_boden.xz);
)shader_source"
R"shader_source(	wave -= wave_position;
)shader_source"
R"shader_source(	wave /= wave_width;
)shader_source"
R"shader_source(	wave = wave_amplitude / (wave * wave + 1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	p_boden.y -= wave;
)shader_source"
R"shader_source(	p_boden = trans(p_boden, 10., 0., 5.);
)shader_source"
R"shader_source(	float smoothing_leitungen = .5;
)shader_source"
R"shader_source(	float f_leitungen = 0.;
)shader_source"
R"shader_source(	f_leitungen = leitungen(p_boden, 0., .2, 1.7, 20.);
)shader_source"
R"shader_source(	f_leitungen = smin(f_leitungen, leitungen(p_boden, 80., .2, 27.3, 15.), smoothing_leitungen);
)shader_source"
R"shader_source(	f_leitungen = smin(f_leitungen, leitungen(p_boden, 20., .2, 549.2, 30.), smoothing_leitungen);
)shader_source"
R"shader_source(	vec2 m_leitungen = vec2(f_leitungen, MATERIAL_ID_LEITUNGEN);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 bottom = vec2(p_boden.y, MATERIAL_ID_FLOOR);
)shader_source"
R"shader_source(	vec2 m_cube_bottom = smin_smaterial(schalter_cube, bottom, 3.);
)shader_source"
R"shader_source(	vec2 m_cube_bottom_leitungen = min_material(m_cube_bottom, m_leitungen);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 bounding = vec2(-sphere(p - camera_position, 50.), MATERIAL_ID_BOUNDING);
)shader_source"
R"shader_source(	return min_material(m_cube_bottom_leitungen, min_material(lightballs, min_material(pyramid, bounding)));
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
