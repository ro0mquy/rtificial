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
/* Given a position, this function generates a 3D co-ordinates based,
 * reconstructible static noise. */
float noise(vec3 position)
{
	position.x += position.y * 57. + position.z * 21.;
	return sin(cos(position.x) * position.x);

	/* The following is an alternative for the previous line:
	 * return fract(position.x * position.x * .0013) * 2. - 1.; */
}

/* Given a position, this function generates a 3D co-ordinates based,
 * reconstructible linearly interpolated smooth noise.
 *
 * This function uses the noise() function above for its underlying
 * noise texture. */
float smooth_noise(vec3 position)
{
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

#line 1
