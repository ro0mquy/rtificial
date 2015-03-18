#include "vec4.h"
#include "stdmath.h"

vec4::vec4() :
	vec4(0., 0., 0., 0.)
{ }

vec4::vec4(float x_, float y_, float z_, float w_) :
	x(x_), y(y_), z(z_), w(w_)
{ }

vec4 operator+(vec4 const& v1, vec4 const& v2) {
	return vec4(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z,
			v1.w + v2.w);
}

vec4 operator-(vec4 const& v1, vec4 const& v2) {
	return vec4(
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z,
			v1.w - v2.w);
}

vec4 operator*(float const& s, vec4 const& v) {
	return vec4(
			s * v.x,
			s * v.y,
			s * v.z,
			s * v.w);
}

vec4 operator*(vec4 const& v, float const& s) {
	return vec4(
			v.x * s,
			v.y * s,
			v.z * s,
			v.w * s);
}

float length(vec4 const& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

float distance(vec4 const& v1, vec4 const& v2) {
	return length(v1 - v2);
}

vec4 mix(vec4 const& v1, vec4 const& v2, const float& t) {
	return (1. - t) * v1 + t * v2;
}
