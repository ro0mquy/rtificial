#include <math.h>

#include "vector.h"

vec3 vec3_new(float x, float y, float z) {
	vec3 v = { x, y, z };
	return v;
}

float vec3_length(vec3 v) {
	return sqrt(vec3_dot(v, v));
}

float vec3_dot(vec3 v, vec3 w) {
	return v.x * w.x + v.y * w.y + v.z * w.z;
}

float vec3_distance(vec3 v, vec3 w) {
	return vec3_length(vec3_sub(v, w));
}

vec3 vec3_normalize(vec3 v) {
	float l = vec3_length(v);
	if (l == 0) {
		return vec3_new(0, 0, 0);
	}
	return vec3_s_mult(1./vec3_length(v), v);
}

vec3 vec3_inv(vec3 v) {
	return vec3_new(
		1. / v.x,
		1. / v.y,
		1. / v.z
	);
}

vec3 vec3_add(vec3 v, vec3 w) {
	return vec3_new(
		v.x + w.x,
		v.y + w.y,
		v.z + w.z
	);
}

vec3 vec3_sub(vec3 v, vec3 w) {
	return vec3_add(v, vec3_s_mult(-1, w));
}

vec3 vec3_mult(vec3 v, vec3 w) {
	return vec3_new(
		v.x * w.x,
		v.y * w.y,
		v.z * w.z
	);
}

vec3 vec3_div(vec3 v, vec3 w) {
	return vec3_mult(v, vec3_inv(w));
}

vec3 vec3_s_add(float s, vec3 v) {
	return vec3_new(
		v.x + s,
		v.y + s,
		v.z + s
	);
}

vec3 vec3_s_mult(float s, vec3 v) {
	return vec3_new(
		v.x * s,
		v.y * s,
		v.z * s
	);
}

vec3 vec3_cross(vec3 v, vec3 w) {
	return vec3_new(
		v.y * w.z - v.z * w.y,
		v.z * w.x - v.x * w.z,
		v.x * w.y - v.y * w.x
	);
}

vec3 vec3_lerp(vec3 v, vec3 w, float t) {
	return vec3_add(vec3_s_mult(1. - t, v), vec3_s_mult(t, w));
}

vec3 vec3_cubic_bezier(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t) {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        return vec3_add(
			vec3_add(
				vec3_s_mult(uuu, p0),
				vec3_s_mult(3 * uu * t, p1)
				),
			vec3_add(
				vec3_s_mult(3 * u * tt, p2),
				vec3_s_mult(ttt, p3)
				)
			);
}
