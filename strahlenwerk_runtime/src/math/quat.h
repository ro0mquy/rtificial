#ifndef MATH_QUAT_H
#define MATH_QUAT_H

struct quat {
	double x, y, z, w;

	quat(double w, double x, double y, double z);
};

quat operator+(quat const& q1, quat const& q2);
quat operator-(quat const& q1, quat const& q2);

quat operator*(double const& s, quat const& q);
quat operator*(quat const& q, double const& s);

double length(quat const& q);
double dot(quat const& q1, quat const& q2);
quat normalize(quat const& q);
quat slerp(quat const& q1, quat const& q2, const double& t);
quat mirror(quat const& q1, quat const& q2);
quat bisect(quat const& q1, quat const& q2);

#endif // MATH_QUAT_H
