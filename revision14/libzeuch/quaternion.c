#include <math.h>

#include "quaternion.h"


quat quat_new(vec3 v, float w) {
	quat q = { v, w };
	return q;
}

quat quat_normalize(quat q) {
	float length = sqrt(vec3_dot(q.v, q.v) + q.w * q.w);
	q.v = vec3_s_mult(1./length, q.v);
	q.w /= length;
	return q;
}

quat quat_add(quat q1, quat q2) {
	return quat_new(
			vec3_add(q1.v, q2.v),
			q1.w + q2.w
		       );
}

quat quat_sub(quat q1, quat q2) {
	return quat_add(q1, quat_s_mult(-1., q2));
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

quat quat_s_mult(float s, quat q) {
	q.w *= s;
	q.v = vec3_s_mult(s, q.v);
	return q;
}

float quat_dot(quat q1, quat q2) {
	return vec3_dot(q1.v, q2.v) + q1.w * q2.w;
}

vec3 quat_rotate_vec3(quat q, vec3 v) {
	return quat_mult(q, quat_mult(quat_new(v, 0.), quat_conjugate(q))).v;
}

quat quat_conjugate(quat q) {
	q.v = vec3_s_mult(-1., q.v);
	return q;
}

quat quat_nlerp(quat q, quat r, float t) {
	q.v = vec3_lerp(q.v, r.v, t);
	q.w = q.w * (1. - t) + r.w * t;
	return quat_normalize(q);
}

// slerp, code from http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
quat quat_slerp(quat v0, quat v1, float t) {
	// v0 and v1 should be unit length or else
	// something broken will happen.

	// Compute the cosine of the angle between the two vectors.
	float dot = quat_dot(v0, v1);

	// if dot product ist negative the rotation is the long way
	if (dot < 0.) {
		dot = -dot;
		v1 = quat_s_mult(-1., v1);
	}

	// check for linear independance
	const float DOT_THRESHOLD = 0.9995;
	if (dot > DOT_THRESHOLD) {
		// If the inputs are too close for comfort, linearly interpolate
		// and normalize the result.

		quat result = quat_add(v0, quat_s_mult(t, quat_sub(v1, v0))); // v0 + t*(v1 - v0)
		result = quat_normalize(result);
		return result;
	}

	//Clamp(dot, -1, 1);           // Robustness: Stay within domain of acos()
	dot = dot > 1. ? 1. : dot < -1. ? -1. : dot;
	float theta_0 = acos(dot);  // theta_0 = angle between input vectors
	float theta = theta_0*t;    // theta = angle between v0 and result 

	quat v2 = quat_sub(v1, quat_s_mult(dot, v0)); // v1 - dot * v0
	v2 = quat_normalize(v2);              // { v0, v2 } is now an orthonormal basis

	return quat_add(quat_s_mult(cos(theta), v0), quat_s_mult(sin(theta), v2)); // v0*cos(theta) + v2*sin(theta)
}

// quaternion bezier curves
quat quat_cubic_bezier(quat q00, quat q10, quat q20, quat q30, float t) {
	quat q01 = quat_slerp(q00, q10, t);
	quat q11 = quat_slerp(q10, q20, t);
	quat q21 = quat_slerp(q20, q30, t);
	quat q02 = quat_slerp(q01, q11, t);
	quat q12 = quat_slerp(q11, q21, t);
	quat q03 = quat_slerp(q02, q12, t);
	return quat_normalize(q03);
}
