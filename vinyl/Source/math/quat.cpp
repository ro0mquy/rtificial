#include "quat.h"
#include "stdmath.h"
#include "scalar.h"

quat::quat() :
	quat(1., 0., 0., 0.)
{ }

quat::quat(float w_, float x_, float y_, float z_) :
	w(w_), x(x_), y(y_), z(z_)
{ }

quat operator+(quat const& q1, quat const& q2) {
	return quat(
			q1.w + q2.w,
			q1.x + q2.x,
			q1.y + q2.y,
			q1.z + q2.z);
}

quat operator-(quat const& q1, quat const& q2) {
	return quat(
			q1.w - q2.w,
			q1.x - q2.x,
			q1.y - q2.y,
			q1.z - q2.z);
}

quat operator*(float const& s, quat const& q) {
	return quat(
			s * q.w,
			s * q.x,
			s * q.y,
			s * q.z);
}

quat operator*(quat const& q, float const& s) {
	return quat(
			q.w * s,
			q.x * s,
			q.y * s,
			q.z * s);
}

float length(quat const& q) {
	return sqrt(dot(q, q));
}

float dot(quat const& q1, quat const& q2) {
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

quat normalize(quat const& q) {
	const float len = length(q);
	if (len <= 0.f) {
		return quat(1., 0., 0., 0.);
	}
	return (1.f / len) * q;
}

quat slerp(quat const& q1, quat const& q2, const float& t) {
	quat q2_ = q2;

	float cosTheta = dot(q1, q2);

	// If cosTheta < 0, the interpolation will take the long way around the sphere. 
	// To fix this, one quat must be negated.
	if (cosTheta < 0.f)
	{
		q2_      = -1.f * q2;
		cosTheta = -cosTheta;
	}

	cosTheta = clamp(cosTheta, 0.f, 1.f);
	float angle = acos(cosTheta);

	if (abs(angle) < .001f) {
		return normalize(quat(
			mix(q1.w, q2_.w, t),
			mix(q1.x, q2_.x, t),
			mix(q1.y, q2_.y, t),
			mix(q1.z, q2_.z, t)
			));
	}

	return (sin((1.f - t) * angle) * q1 + sin(t * angle) * q2_) * (1.f / sin(angle));
}

quat mirror(quat const& q0, quat const& q1) {
	return 2.f * dot(q0, q1) * q1 - q0;
}
