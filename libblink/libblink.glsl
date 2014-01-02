#version 330

uniform vec3 view_position;
uniform vec3 view_up;
uniform vec3 view_direction;
uniform vec2 res;
uniform float time;

out vec4 out_color;

vec3 get_direction() {
	vec3 view_right = cross(view_direction, view_up);
	mat3 camera = mat3(view_right, view_up, -view_direction);
	return camera * normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y , -1.));
}

#define DECLARE_MARCH(name) vec3 name(vec3, vec3, out int);
#define DEFINE_MARCH(name, f) \
vec3 name(vec3 p, vec3 direction, out int i) {\
	float walked = 0.;\
	for (i=0; i < 100; i++) {\
		float dist = f(p)[0];\
		p += direction * dist;\
		dist = abs(dist);\
		walked += dist;\
\
		if (dist < .001 * walked) break;\
	}\
	return p;\
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

float sphere(vec3 p, float s) {
	return length(p) - s;
}

float box(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));
}

float torus(vec3 p, vec2 t) {
	vec2 q = vec2(length(p.xz) - t.x, p.y);
	return length(q) - t.y;
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

// f: distance function to object
// p: evaluation point
// s: scale factor
#define SCALE(f, p, s) f((p)/(s))*(s)

// trans*late things
// p: point
// x: x
// y: y
// z: z
#define TRANS(p, x, y, z) ((p)-vec3((x), (y), (z)))

// trans*late things - using vectors!!
// p: point
// v: translation vector
#define TRANSv(p, v) ((p)-(v))

#line 1
