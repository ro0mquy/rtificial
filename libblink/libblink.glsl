#version 400

uniform vec3 view_position;
uniform vec3 view_up;
uniform vec3 view_direction;
uniform vec2 res;
uniform float time;
uniform int notes[32];
uniform float envelopes[32];
uniform float aenvelopes[32];
uniform float senvelopes[32];

layout(location = 0) out vec4 out_color;
layout(location = 1) out float out_depth;

float TAU = 6.28318530718;

mat3 get_camera() {
	vec3 view_right = cross(view_direction, view_up);
	return mat3(view_right, view_up, -view_direction);
}

vec3 get_direction() {
	return get_camera() * normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y , -1.));
}

#define DECLARE_MARCH(name) vec3 name(vec3, vec3, out int);
#define DEFINE_MARCH(name, f) \
vec3 name(vec3 p, vec3 direction, out int i) {\
	float walked = 0.;\
	for (i=0; i < 150; i++) {\
		float dist = f(p)[0];\
		p += direction * dist;\
		dist = abs(dist);\
		walked += dist;\
\
		if (dist < .001 * walked) break;\
	}\
	return p;\
}

#define DECLARE_SHADOW_RAY(name) float name(vec3, vec3);
#define DEFINE_SHADOW_RAY(name, f) \
float name(vec3 hit, vec3 light) {\
	vec3 dir = normalize(hit - light);\
	vec3 p = light;\
	float walked = 0.;\
	for(int i = 0; i < 100; i++) {\
		float dist = f(p)[0];\
		p += dir * dist;\
		dist = abs(dist);\
		walked += dist;\
		if(dist < .001 * walked) break;\
	}\
	return float((distance(hit, light) - distance(light, p)) < .1);\
}

// iq's softshadows with penumbra
// light: position of light
// hit: the point hit by raymarching
// hardness: the hardness of the shadow (2, 8, 32, 128 are good values)
#define DECLARE_SOFTSHADOW(name) float name(vec3, vec3, float);
#define DEFINE_SOFTSHADOW(name, f) \
float softshadow(vec3 hit, vec3 light, float hardness) {\
	vec3 direction = normalize(light - hit);\
	float max_travel = .95 * distance(hit, light);\
	float traveled = .001;\
	float res = 1.;\
\
	for (; traveled < max_travel;) {\
		float dist = f(hit + traveled*direction)[0];\
\
		/* if doing small steps, we hit something */ \
		if (dist < 0.01 * traveled) {\
			return 0.;\
		}\
\
		/* save the smallest distance to an object */ \
		res = min(res, hardness * dist / traveled);\
		traveled += dist;\
	}\
	return res;\
}

#define DECLARE_NORMAL(name) vec3 name(vec3);
#define DEFINE_NORMAL(name, f) \
vec3 name(vec3 p) {\
	vec2 epilepsilon = vec2(.001, 0.);\
	return normalize(vec3(\
		f(p + epilepsilon.xyy)[0] - f(p - epilepsilon.xyy)[0],\
		f(p + epilepsilon.yxy)[0] - f(p - epilepsilon.yxy)[0],\
		f(p + epilepsilon.yyx)[0] - f(p - epilepsilon.yyx)[0]\
	));\
}

// ambient occlusion and subsurface scattering
#define DECLARE_AO(name) float name(vec3, vec3, float, float);
#define DEFINE_AO(name, f) \
float ao(vec3 p, vec3 n, float d, float i) {\
	float o, s = sign(d);\
	for(o = s * .5 + .5; i > 0; i--) {\
		o -= (i * d - f(p + n * i * d * s)[0]) / exp2(i);\
	}\
	return o;\
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

// hier kommt der witz!
vec2 min_material(vec2 a, vec2 b) {
	return mix(a, b, a.x > b.x);
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

// c: x value of maximum
// w: pulse width / 2
// x: x
// see http://www.iquilezles.org/www/articles/functions/functions.htm
float cubic_pulse(float c, float w, float x) {
    x = abs(x - c);
    x /= w;
	return 1. - smoothstep(0., 1., x);
}

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// (x^n + y^n + z^n)^1/n
float length_n(vec3 p, float n) {
	return pow(dot(pow(abs(p), vec3(n)), vec3(1)), 1./n);
}

float sphere(vec3 p, float s) {
	return length(p) - s;
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
	return slowbox(p, b) - r;
}

float torus(vec3 p, vec2 t) {
	vec2 q = vec2(length(p.xz) - t.x, p.y);
	return length(q) - t.y;
}

float tri_prism(vec3 p, vec2 h) {
	vec3 q = abs(p);
	return max(q.z - h.y, max(q.x * 0.866025 + p.y * 0.5, -p.y) -h.x * 0.5);
}

float hex_prism(vec3 p, vec2 h) {
	vec3 q = abs(p);
	return max(q.z - h.y, max(q.x + q.y * 0.57735, q.y * 1.1547) - h.x);
}

// c must be normalized
float cone(vec3 p, vec2 c) {
	float q = length(p.xy);
	return dot(c, vec2(q, p.z));
}

// n must be normalized
float plane(vec3 p, vec3 n) {
	return dot(p, n.xyz);
}

// a and b are the endpoints
// r is the radius if you want some kind of capsule
float line(vec3 p, vec3 a, vec3 b, float r) {
	vec3 pa = p - a;
	vec3 ba = b - a;
	float h = clamp(dot(pa,ba) / dot(ba,ba), 0., 1. );
	return length(pa - ba*h) - r;
}

// f: distance function to object
// p: evaluation point
// s: scale factor
#define scale(f, p, s) f((p)/(s))*(s)

// trans*late things - using vectors!!
// p: point
// v: translation vector
vec3 transv(vec3 p, vec3 v) {
	return p - v;
}

// trans*late things
// p: point
// x: x
// y: y
// z: z
vec3 trans(vec3 p, float x, float y, float z) {
	return transv(p, vec3(x, y, z));
}

vec3 domrepv(vec3 p, vec3 c) {
	return mod(p, c) - .5 * c;
}

// repeat things
vec3 domrep(vec3 p, float x, float y, float z) {
	return domrepv(p, vec3(x, y, z));
}

// Given a position, this function generates a 3D co-ordinates based,
// reconstructible static noise.
float noise(vec3 position) {
	position.x += position.y * 57. + position.z * 21.;
	return sin(cos(position.x) * position.x);

	/* The following is an alternative for the previous line:
	 * return fract(position.x * position.x * .0013) * 2. - 1.; */
}

// Given a position, this function generates a 3D co-ordinates based,
// reconstructible linearly interpolated smooth noise.
//
// This function uses the noise() function above for its underlying
// noise texture.
float smooth_noise(vec3 position) {
	vec3 integer = floor(position);
	vec3 fractional = position - integer;

	return mix(mix(mix(noise(integer),
			   noise(integer + vec3(1, 0, 0)),
			   fractional.x),
		       mix(noise(integer + vec3(0, 1, 0)),
			   noise(integer + vec3(1, 1, 0)),
			   fractional.x),
		       fractional.y),
		   mix(mix(noise(integer + vec3(0, 0, 1)),
			   noise(integer + vec3(1, 0, 1)),
			   fractional.x),
		       mix(noise(integer + vec3(0, 1, 1)),
			   noise(integer + 1.), fractional.x),
		       fractional.y),
		   fractional.z) * .5 + .5;
}

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 g(float v) {
	v *= 2.0 * 3.1415926;
	return vec2(cos(v), sin(v));
}

vec2 fade(vec2 t) {
	return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float classic_noise(vec2 co) {
	vec2 c = fract(co);
	vec2 C = floor(co);
	vec2 e = vec2(0.0, 1.0);

	vec4 n = vec4(
		dot(g(rand(C + e.xx)), c - e.xx),
		dot(g(rand(C + e.xy)), c - e.xy),
		dot(g(rand(C + e.yx)), c - e.yx),
		dot(g(rand(C + e.yy)), c - e.yy)
	);

	vec2 u = fade(c);

	return mix(
		mix(n[0], n[2], u.x),
		mix(n[1], n[3], u.x),
		u.y);
}

// positive dot product
float pdot(vec3 a, vec3 b) {
	return max(0., dot(a, b));
}

// calculate lambertian reflectance (diffuse term of phong model)
// input vectors have to be of unit length
float lambert_norm(vec3 to_light, vec3 normal) {
	return pdot(to_light, normal);
}

// calculate lambertian reflectance (diffuse term of phong model)
float lambert(vec3 to_light, vec3 normal) {
	return lambert_norm(normalize(to_light), normalize(normal));
}

// calculate specular term of phong model
// input vectors have to be of unit length
float phong_norm(vec3 to_light, vec3 normal, vec3 to_view, float exponent) {
	vec3 reflection = 2. * normal * dot(normal, to_light) - to_light;
	return pow(pdot(reflection, to_view), exponent);
}

// calculate specular term of phong model
float phong(vec3 to_light, vec3 normal, vec3 to_view, float exponent) {
	return phong_norm(normalize(to_light), normalize(normal), normalize(to_view), exponent);
}

// see oren_nayar()
// sigma2 = sigma * sigma
// input vectors have to be of unit length
float oren_nayar_norm(vec3 to_light, vec3 normal, vec3 to_view, float sigma2) {
	float A = 1 - sigma2 / (2 * (sigma2 + .33));
	float B = .45 * sigma2 / (sigma2 + .09);
	// orthogonalize to_light and to_view with normal to calculate angle
	float cos_l_n = dot(to_light, normal);
	float cos_v_n = dot(to_view, normal);
	float cos_sigma_i_minus_sigma_r = dot(
		normalize(to_light - normal * cos_l_n),
		normalize(to_view - normal * cos_v_n)
	);
	// calculating sin and tan from cos avoids case differentiation
	float sin_alpha_tan_beta = sqrt((1 - cos_l_n * cos_l_n) * (1 - cos_v_n * cos_v_n)) / max(cos_l_n, cos_v_n);
	return max(0, cos_l_n) * (A + B * max(0, cos_sigma_i_minus_sigma_r) * sin_alpha_tan_beta);
}

// calculate oren-nayar reflectance
// sigma is roughness (sigma = 0 equal to lambert)
float oren_nayar(vec3 to_light, vec3 normal, vec3 to_view, float sigma) {
	return oren_nayar_norm(normalize(to_light), normalize(normal), normalize(to_view), sigma * sigma);
}

// see cook_torrance()
// m2 = m * m
// R0 is from schlick's approximation
// input vectors have to be of unit length
float cook_torrance_norm(vec3 to_light, vec3 normal, vec3 to_view, float m2, float R0) {
	vec3 H = .5 * (to_light + to_view);
	float NdotH = dot(normal, H);
	float cos2_alpha = NdotH * NdotH;
	// beckmann distribution
	float distribution = exp(-(1. - cos2_alpha)/(cos2_alpha * m2)) / (m2 * cos2_alpha * cos2_alpha);
	// schlick's approximation
	float fresnel = R0 + (1. - R0) * pow(1 - dot(H, to_view), 5.);
	float VdotH = dot(to_view, H);
	float LdotN = dot(to_light, normal);
	float VdotN = dot(to_view, normal);
	// geometric attenuation
	float geometric = min(1., 2. * NdotH / VdotH * min(LdotN, VdotN));
	return distribution * fresnel * geometric / (4. * LdotN * VdotN);
}

// calculate cook-torrance reflectance
// m is roughness (rms slope of the surface microfasets)
// n is the refraction index of the material
float cook_torrance(vec3 to_light, vec3 normal, vec3 to_view, float m, float n) {
	float R0 = (1. - n) / (1. + n);
	R0 *= R0;
	return max(0, cook_torrance_norm(normalize(to_light), normalize(normal), normalize(to_view), m * m, R0));
}

float rgb2luma(vec3 rgb) {
	return dot(rgb, vec3(.2126, .7152, .0722)); // magic luminance formular
}

// change contrast - k changes the intensity
// k > 1: more contrast
// k < 1: less contrast
// k == 0: black-white
// k == 1: the input color
// k < 0: invert color & contrast(abs(k))
vec3 contrast(vec3 color, float k) {
	//float luma = rgb2luma(color);
	return mix(vec3(.5), color, k);
}

// add some kind of vignette
// 0 < intensity < 1
// intensity = .7 is good default
float vignette(float intensity) {
	float one_minus_intesity = 1. - intensity;
	vec2 position = gl_FragCoord.xy / res;
	position.y *= -1.;
	return one_minus_intesity + intensity * 16. * position.x *position.y * (1. - position.x) * (-1. - position.y);
}


// light, normal, view, roughness, specular color (F0)
vec3 specular(vec3 L, vec3 N, vec3 V, float r, vec3 c) {
	vec3 H = .5 * (L + V);
	float NdotL = dot(N, L);
	float NdotV = dot(N, V);
	float NdotH = dot(N, H);

	float alpha2 = r * r;
	alpha2 *= alpha2;
	float stuff = NdotH * NdotH * (alpha2 - 1.) + 1.;

	float r1 = r + 1.;
	float k = r1 * r1 / 8.;

	float VdotH = dot(V, H);
	vec3 F = c + (1. - c) * exp2((-5.55473 * VdotH - 6.98316) * VdotH);
	return F * (alpha2 / (4. * mix(NdotL, 1., k) * mix(NdotV, 1., k) * 3.14 * stuff * stuff));
}

#line 1
