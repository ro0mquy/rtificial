#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector.h"

typedef struct {
	vec3 v;
	float w;
} quat;

quat quat_new(vec3 v, float w);

quat quat_normalize(quat q);

quat quat_mult(quat q1, quat q2);

vec3 quat_rotate_vec3(quat q, vec3 v);

#endif
