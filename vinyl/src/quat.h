#ifndef MATH_QUAT_H
#define MATH_QUAT_H

struct quat {
	float w, x, y, z;

	quat();
	quat(float w, float x, float y, float z);
};

quat operator+(quat const& q1, quat const& q2);
quat operator-(quat const& q1, quat const& q2);

quat operator*(float const& s, quat const& q);
quat operator*(quat const& q, float const& s);

float length(quat const& q);
float dot(quat const& q1, quat const& q2);
quat normalize(quat const& q);
quat slerp(quat const& q1, quat const& q2, const float& t);
quat mirror(quat const& q0, quat const& q1);

#endif // MATH_QUAT_H
