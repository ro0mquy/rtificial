#include <math.h>

#include "quaternion.h"

static quat quat_conjugate(quat q);

quat quat_new(vec3 v, float w) {
	quat q = { .v = v, .w = w };
	return q;
}

quat quat_normalize(quat q) {
	float length = sqrt(vec3_dot(q.v, q.v) + q.w * q.w);
	q.v = vec3_s_mult(1./length, q.v);
	q.w /= length;
	return q;
}

quat quat_mult(quat q1, quat q2) {
	float w = q1.w * q2.w - vec3_dot(q1.v, q2.v);
	vec3 v = vec3_add(
		vec3_add(
			vec3_s_mult(q1.w, q2.v),
			vec3_s_mult(q2.w, q1.v)
		),
		vec3_cross(q1.v, q2.v)
	);
	return quat_new(v, w);
}

vec3 quat_rotate_vec3(quat q, vec3 v) {
	return quat_mult(q, quat_mult(quat_new(v, 0.), quat_conjugate(q))).v;
}

static quat quat_conjugate(quat q) {
	q.v = vec3_s_mult(-1., q.v);
	return q;
}

quat quat_nlerp(quat q, quat r, float t) {
	q.v = vec3_lerp(q.v, r.v, t);
	q.w = q.w * (1. - t) + r.w * t;
	return quat_normalize(q);
}
