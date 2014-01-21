#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector.h"

typedef struct {
	vec3 v;
	float w;
} quat;

quat quat_new(vec3 v, float w);

quat quat_normalize(quat q);

quat quat_add(quat q1, quat q2);

quat quat_sub(quat q1, quat q2);

quat quat_mult(quat q1, quat q2);

quat quat_s_mult(float s, quat q);

float quat_dot(quat q1, quat q2);

vec3 quat_rotate_vec3(quat q, vec3 v);

quat quat_nlerp(quat q, quat r, float t);

quat quat_slerp(quat v0, quat v1, float t);

quat quat_cubic_bezier(quat q00, quat q10, quat q20, quat q30, float t);

#endif
