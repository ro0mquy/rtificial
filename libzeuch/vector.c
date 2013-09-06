#include <math.h>

#include "vector.h"

vec3 vec3_new(float x, float y, float z) {
	vec3 v = { .x = x, .y = y, .z = z };
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

vec3 vec3_rotate(vec3 v, vec3 a, float angle) {
	const float sin_angle = sin(angle);
	const float cos_angle = cos(angle);
	const float one_min_cos = 1. - cos_angle;
	vec3 column_x = vec3_new(
		a.x * a.x * one_min_cos + cos_angle,
		a.x * a.y * one_min_cos - a.z * sin_angle,
		a.x * a.z * one_min_cos + a.y * sin_angle
	);
	vec3 column_y = vec3_new(
		a.y * a.x * one_min_cos + a.z * sin_angle,
		a.y * a.y * one_min_cos + cos_angle,
		a.y * a.z * one_min_cos - a.x * sin_angle
	);
	vec3 column_z = vec3_new(
		a.z * a.x * one_min_cos - a.y * sin_angle,
		a.z * a.y * one_min_cos + a.x * sin_angle,
		a.z * a.z * one_min_cos + cos_angle
	);
	return vec3_new(
		vec3_dot(v, column_x),
		vec3_dot(v, column_y),
		vec3_dot(v, column_z)
	);
}

vec3 vec3_lerp(vec3 v, vec3 w, float t) {
	return vec3_add(vec3_s_mult(1. - t, v), vec3_s_mult(t, w));
}
