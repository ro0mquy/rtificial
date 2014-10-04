#ifndef syntakischerspalt_H
#define syntakischerspalt_H
const char syntakischerspalt_source[] =R"shader_source(#version 430
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
R"shader_source(#line 5
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 88) uniform vec3 synapse_color; // color
)shader_source"
R"shader_source(layout(location = 89) uniform vec3 synapse_color_bobbel; // color
)shader_source"
R"shader_source(layout(location = 90) uniform float synapse_gap; // float
)shader_source"
R"shader_source(layout(location = 91) uniform float synapse_aa; // float
)shader_source"
R"shader_source(layout(location = 92) uniform float synapse_bb; // float
)shader_source"
R"shader_source(layout(location = 93) uniform float synapse_cc; // float
)shader_source"
R"shader_source(layout(location = 94) uniform float synapse_dd; // float
)shader_source"
R"shader_source(layout(location = 95) uniform float synapse_dist_fog_a; // float
)shader_source"
R"shader_source(layout(location = 96) uniform float synapse_dist_fog_b; // float
)shader_source"
R"shader_source(layout(location = 97) uniform float synapse_bobbel_progress; // float
)shader_source"
R"shader_source(layout(location = 98) uniform float synapse_transmitter_r; // float
)shader_source"
R"shader_source(layout(location = 99) uniform float synapse_bobbel_roughness;
)shader_source"
R"shader_source(layout(location = 100) uniform float synapse_bobbel_metalicness;;
)shader_source"
R"shader_source(layout(location = 101) uniform float synapse_fade; // flaot
)shader_source"
R"shader_source(layout(location = 102) uniform float synapse_min_darkness_dist; // flaot
)shader_source"
R"shader_source(layout(location = 103) uniform bool synapse_last_fire; //  bool
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(Material materials[4] = Material[4](
)shader_source"
R"shader_source(	Material(vec3(1.), .5, 0.),
)shader_source"
R"shader_source(	Material(synapse_color, 0.7, 1.),
)shader_source"
R"shader_source(	Material(synapse_color_bobbel, synapse_bobbel_roughness, synapse_bobbel_metalicness),
)shader_source"
R"shader_source(	Material(synapse_color_bobbel, .2, 1.)
)shader_source"
R"shader_source();
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(const float MATERIAL_ID_BOUNDING    = 0.;
)shader_source"
R"shader_source(const float MATERIAL_ID_SYNAPSE     = 1.;
)shader_source"
R"shader_source(const float MATERIAL_ID_BOBBEL      = 2.;
)shader_source"
R"shader_source(const float MATERIAL_ID_TRANSMITTER = 3.;
)shader_source"
R"shader_source(
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
R"shader_source(	vec3 hit = march_adv(camera_position, direction, i, 100, .7);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 color = vec3(0.);
)shader_source"
R"shader_source(	if(i < 150) {
)shader_source"
R"shader_source(		int material = int(f(hit).y);
)shader_source"
R"shader_source(		vec3 normal = calc_normal(hit);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		Material mat = materials[material];
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(		if(MATERIAL_ID_TRANSMITTER == material){
)shader_source"
R"shader_source(			color = emit_light(mat.color, 3 * synapse_transmitter_r);
)shader_source"
R"shader_source(		} else {
)shader_source"
R"shader_source(			color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
)shader_source"
R"shader_source(		}
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	color *= smoothstep(synapse_dist_fog_a, synapse_dist_fog_b, -distance(hit, camera_position));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	color *= synapse_fade;
)shader_source"
R"shader_source(	if (synapse_min_darkness_dist > 0.) {
)shader_source"
R"shader_source(		color *= synapse_min_darkness_dist * 1. / distance(hit, vec3(25., -25., -25.));
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	output_color(color, distance(hit, camera_position));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 f(vec3 p) {
)shader_source"
R"shader_source(	// domrep with rotation
)shader_source"
R"shader_source(	vec3 dr_factor = vec3(50.);
)shader_source"
R"shader_source(	vec3 q = domrepv(p, dr_factor);
)shader_source"
R"shader_source(	q *= rZ(2*rand(3*floor(p/dr_factor)));
)shader_source"
R"shader_source(	q *= rY(1.5*rand(floor(p/dr_factor)));
)shader_source"
R"shader_source(	vec3 q_p = q; // save for bobbel cutout
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// mirror
)shader_source"
R"shader_source(	q.x = abs(q.x);
)shader_source"
R"shader_source(	q.x -= 10 * synapse_gap;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// main body
)shader_source"
R"shader_source(	float sphere1 = sphere(q, 1.);
)shader_source"
R"shader_source(	q.x -= 2.;
)shader_source"
R"shader_source(	float sphere2 = sphere(q, 2.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// tail
)shader_source"
R"shader_source(	q = rZ(-TAU*sin(q.x/5.)/30.) * q;
)shader_source"
R"shader_source(	q = rY(-TAU*sin(q.x/2.+time*0.5)/50.)*q;
)shader_source"
R"shader_source(	float capsule = line(q, vec3(2.,0.,0.), vec3(20.,0.,0.), mix(synapse_aa, synapse_bb, smoothstep(10.*synapse_cc, 10*synapse_dd, q.x)));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 synapse = vec2(smin(smax(-sphere1, sphere2, 1.), capsule, 1.), MATERIAL_ID_SYNAPSE);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// fake neurotransmitter
)shader_source"
R"shader_source(	vec3 p_t = q_p;
)shader_source"
R"shader_source(	float transm = sphere(p_t, 1.35 * synapse_transmitter_r);
)shader_source"
R"shader_source(	float t_cutout = sphere(trans(p, dr_factor.x/2., -dr_factor.y/2., -dr_factor.z/2.), 10.);
)shader_source"
R"shader_source(	if(synapse_last_fire){
)shader_source"
R"shader_source(		t_cutout *= -1;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	vec2 transmitter = vec2(max(transm, -t_cutout), MATERIAL_ID_TRANSMITTER);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// bobbel
)shader_source"
R"shader_source(	// positioning
)shader_source"
R"shader_source(	vec3 p_bbl = p;
)shader_source"
R"shader_source(	p_bbl = trans(p_bbl, dr_factor.x/2., -dr_factor.y/2., -dr_factor.z/2.);
)shader_source"
R"shader_source(	p_bbl *= rY(-0.11*TAU);
)shader_source"
R"shader_source(	p_bbl = trans(p_bbl, synapse_bobbel_progress, 0., 0.);
)shader_source"
R"shader_source(	// domrep
)shader_source"
R"shader_source(	vec3 p_bbl_p = p_bbl;
)shader_source"
R"shader_source(	vec3 bbl_dr_factor = vec3(.7, .5, .5);
)shader_source"
R"shader_source(	p_bbl = domrepv(p_bbl, bbl_dr_factor);
)shader_source"
R"shader_source(	// scale & add
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 cell = floor(p_bbl_p / bbl_dr_factor);
)shader_source"
R"shader_source(	cell.x += 90.;
)shader_source"
R"shader_source(	cell.y += 150.;
)shader_source"
R"shader_source(	cell.z += 250.;
)shader_source"
R"shader_source(	vec3 translation_vector = .04 * vec3(
)shader_source"
R"shader_source(			sin(cell.y * cell.z) * 4.,
)shader_source"
R"shader_source(			sin(cell.z * cell.x) * 2.,
)shader_source"
R"shader_source(			sin(cell.x * cell.y) * 2.6);
)shader_source"
R"shader_source(	p_bbl = transv(p_bbl, translation_vector);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	float scale_factor = 0.15;
)shader_source"
R"shader_source(	p_bbl = trans(p_bbl, -2. * scale_factor, 0., 0.);
)shader_source"
R"shader_source(	float bbl = conicbobbel(p_bbl / scale_factor, 2.) * scale_factor;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec3 p_cylinder = trans(q_p, synapse_bobbel_progress, 0., 0.);
)shader_source"
R"shader_source(	float f_cylinder = cylinder(p_cylinder.zyx, 1.35, 1.1);
)shader_source"
R"shader_source(	vec2 bobbel = vec2(smax(bbl, f_cylinder, .05), MATERIAL_ID_BOBBEL);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	vec2 bounding = vec2(-sphere(p - camera_position, -synapse_dist_fog_a), MATERIAL_ID_BOUNDING);
)shader_source"
R"shader_source(	if(!synapse_last_fire){
)shader_source"
R"shader_source(		bounding = min_material(bounding, bobbel);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return min_material(min_material(synapse, transmitter), bounding);
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
