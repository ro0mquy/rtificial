#version 330

#define DECLARE_MARCH(name) vec3 name(vec3, vec3, out int);
#define DEFINE_MARCH(name, f) \
vec3 name(vec3 p, vec3 direction, out int i) {\
	float walked = 0.;\
	for (i=0; i < 100; i++) {\
		float dist = f(p);\
		p += direction * dist;\
		dist = abs(dist);\
		walked += dist;\
\
		if (dist < .001 * walked) break;\
	}\
	return p;\
}

#define DECLARE_NORMAL(name, f) vec3 name(vec3);
#define DEFINE_NORMAL(name, f) \
vec3 name(vec3 p) {\
	vec2 epilepsilon = vec2(.001, 0.);\
	return normalize(vec3(\
		f(p + epilepsilon.xyy) - f(p - epilepsilon.xyy),\
		f(p + epilepsilon.yxy) - f(p - epilepsilon.yxy),\
		f(p + epilepsilon.yyx) - f(p - epilepsilon.yyx)\
	));\
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

#define SCALE(f, p, s) f((p)/(s))*(s)
