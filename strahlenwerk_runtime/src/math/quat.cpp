#include "quat.h"
#include <cmath>

quat::quat(double w_, double x_, double y_, double z_) :
	w(w_), x(x_), y(y_), z(z_)
{
}


quat operator+(quat const& q1, quat const& q2) {
	return quat(
			q1.w + q2.w,
			q1.x + q2.x,
			q1.y + q2.y,
			q1.z + q1.z);
}

quat operator-(quat const& q1, quat const& q2) {
	return quat(
			q1.w - q2.w,
			q1.x - q2.x,
			q1.y - q2.y,
			q1.z - q1.z);
}

quat operator*(double const& s, quat const& q) {
	return quat(
			s * q.w,
			s * q.x,
			s * q.y,
			s * q.z);
}

quat operator*(quat const& q, double const& s) {
	return quat(
			q.w * s,
			q.x * s,
			q.y * s,
			q.z * s);
}

double length(quat const& q) {
	return std::sqrt(dot(q, q));
}

double dot(quat const& q1, quat const& q2) {
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

quat normalize(quat const& q) {
	const double len = length(q);
	if (len <= 0.) {
		return quat(1., 0., 0., 0.);
	}
	return (1. / len) * q;
}

quat slerp(quat const& q1, quat const& q2, const double& t) {
	quat q2_ = q2;

	double cosTheta = dot(q1, q2);

	// If cosTheta < 0, the interpolation will take the long way around the sphere. 
	// To fix this, one quat must be negated.
	if (cosTheta < 0.)
	{
		q2_      = -1 * q2;
		cosTheta = -cosTheta;
	}

	double angle = acos(cosTheta);
	return (sin((1. - t) * angle) * q1 + sin(t * angle) * q2_) * (1.0 / sin(angle));
}

quat mirror(quat const& q0, quat const& q1) {
	return 2 * dot(q0, q1) * q1 - q0;
}

quat bisect(quat const& q1, quat const& q2) {
	return normalize(q1 + q2);
}
