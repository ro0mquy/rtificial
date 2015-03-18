#include "vec2.h"
#include "stdmath.h"

vec2::vec2() :
	vec2(0., 0.)
{ }

vec2::vec2(float x_, float y_) :
	x(x_), y(y_)
{ }

vec2 operator+(vec2 const& v1, vec2 const& v2) {
	return vec2(
			v1.x + v2.x,
			v1.y + v2.y);
}

vec2 operator-(vec2 const& v1, vec2 const& v2) {
	return vec2(
			v1.x - v2.x,
			v1.y - v2.y);
}

vec2 operator*(float const& s, vec2 const& v) {
	return vec2(
			s * v.x,
			s * v.y);
}

vec2 operator*(vec2 const& v, float const& s) {
	return vec2(
			v.x * s,
			v.y * s);
}

float length(vec2 const& v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

float distance(vec2 const& v1, vec2 const& v2) {
	return length(v1 - v2);
}

vec2 mix(vec2 const& v1, vec2 const& v2, const float& t) {
	return (1. - t) * v1 + t * v2;
}
