#include <math.h>

#include "vector.h"

vec2 vec2_new(float x, float y) {
	vec2 v = { .x = x, .y = y };
	return v;
}

vec3 vec3_new(float x, float y, float z) {
	vec3 v = { .x = x, .y = y, .z = z };
	return v;
}

vec4 vec4_new(float x, float y, float z, float w) {
	vec4 v = { .x = x, .y = y, .z = z, .w = w};
	return v;
}


float vec2_length(vec2 v) {
	return sqrt(vec2_dot(v, v));
}

float vec3_length(vec3 v) {
	return sqrt(vec3_dot(v, v));
}

float vec4_length(vec4 v) {
	return sqrt(vec4_dot(v, v));
}


float vec2_dot(vec2 v, vec2 w) {
	return v.x * w.x + v.y * w.y;
}

float vec3_dot(vec3 v, vec3 w) {
	return v.x * w.x + v.y * w.y + v.z * w.z;
}

float vec4_dot(vec4 v, vec4 w) {
	return v.x * w.x + v.y * w.y + v.z * w.z + v.w * w.w;
}


float vec2_distance(vec2 v, vec2 w) {
	return vec2_length(vec2_sub(v, w));
}

float vec3_distance(vec3 v, vec3 w) {
	return vec3_length(vec3_sub(v, w));
}

float vec4_distance(vec4 v, vec4 w) {
	return vec4_length(vec4_sub(v, w));
}


vec2 vec2_normalize(vec2 v) {
	return vec2_s_mult(1./vec2_length(v), v);
}

vec3 vec3_normalize(vec3 v) {
	return vec3_s_mult(1./vec3_length(v), v);
}

vec4 vec4_normalize(vec4 v) {
	return vec4_s_mult(1./vec4_length(v), v);
}


vec2 vec2_inv(vec2 v) {
	vec2 w = {
		.x = 1. / v.x,
		.y = 1. / v.y,
	};
	return w;
}

vec3 vec3_inv(vec3 v) {
	vec3 w = {
		.x = 1. / v.x,
		.y = 1. / v.y,
		.z = 1. / v.z,
	};
	return w;
}

vec4 vec4_inv(vec4 v) {
	vec4 w = {
		.x = 1. / v.x,
		.y = 1. / v.y,
		.z = 1. / v.z,
		.w = 1. / v.w,
	};
	return w;
}



vec2 vec2_add(vec2 v, vec2 w) {
	vec2 u = {
		.x = v.x + w.x,
		.y = v.y + w.y,
	};
	return u;
}
vec3 vec3_add(vec3 v, vec3 w) {
	vec3 u = {
		.x = v.x + w.x,
		.y = v.y + w.y,
		.z = v.z + w.z,
	};
	return u;
}
vec4 vec4_add(vec4 v, vec4 w) {
	vec4 u = {
		.x = v.x + w.x,
		.y = v.y + w.y,
		.z = v.z + w.z,
		.w = v.w + w.w,
	};
	return u;
}

vec2 vec2_sub(vec2 v, vec2 w) {
	return vec2_add(v, vec2_s_mult(-1, w));
}
vec3 vec3_sub(vec3 v, vec3 w) {
	return vec3_add(v, vec3_s_mult(-1, w));
}
vec4 vec4_sub(vec4 v, vec4 w) {
	return vec4_add(v, vec4_s_mult(-1, w));
}

vec2 vec2_mult(vec2 v, vec2 w) {
	vec2 u = {
		.x = v.x * w.x,
		.y = v.y * w.y,
	};
	return u;
}
vec3 vec3_mult(vec3 v, vec3 w) {
	vec3 u = {
		.x = v.x * w.x,
		.y = v.y * w.y,
		.z = v.z * w.z,
	};
	return u;
}
vec4 vec4_mult(vec4 v, vec4 w) {
	vec4 u = {
		.x = v.x * w.x,
		.y = v.y * w.y,
		.z = v.z * w.z,
		.w = v.w * w.w,
	};
	return u;
}

vec2 vec2_div(vec2 v, vec2 w) {
	return vec2_mult(v, vec2_inv(w));
}
vec3 vec3_div(vec3 v, vec3 w) {
	return vec3_mult(v, vec3_inv(w));
}
vec4 vec4_div(vec4 v, vec4 w) {
	return vec4_mult(v, vec4_inv(w));
}

vec2 vec2_s_add(float s, vec2 v) {
	vec2 u = {
		.x = v.x + s,
		.y = v.y + s,
	};
	return u;
}

vec3 vec3_s_add(float s, vec3 v) {
	vec3 u = {
		.x = v.x + s,
		.y = v.y + s,
		.z = v.z + s,
	};
	return u;
}

vec4 vec4_s_add(float s, vec4 v) {
	vec4 u = {
		.x = v.x + s,
		.y = v.y + s,
		.z = v.z + s,
		.w = v.w + s,
	};
	return u;
}


vec2 vec2_s_mult(float s, vec2 v) {
	vec2 u = {
		.x = v.x * s,
		.y = v.y * s,
	};
	return u;
}

vec3 vec3_s_mult(float s, vec3 v) {
	vec3 u = {
		.x = v.x * s,
		.y = v.y * s,
		.z = v.z * s,
	};
	return u;
}

vec4 vec4_s_mult(float s, vec4 v) {
	vec4 u = {
		.x = v.x * s,
		.y = v.y * s,
		.z = v.z * s,
		.w = v.w * s,
	};
	return u;
}


vec3 vec3_cross(vec3 v, vec3 w) {
	vec3 u = {
		.x = v.y * w.z - v.z * w.y,
		.y = v.z * w.x - v.x * w.z,
		.z = v.x * w.y - v.y * w.x,
	};
	return u;
}
