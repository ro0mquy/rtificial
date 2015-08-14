#include "vec3.h"
#include "stdmath.h"

vec3::vec3() :
	vec3(0., 0., 0.)
{ }

vec3::vec3(float x_, float y_, float z_) :
	x(x_), y(y_), z(z_)
{ }

vec3 operator+(vec3 const& v1, vec3 const& v2) {
	return vec3(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z);
}

vec3 operator-(vec3 const& v1, vec3 const& v2) {
	return vec3(
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z);
}

vec3 operator*(float const& s, vec3 const& v) {
	return vec3(
			s * v.x,
			s * v.y,
			s * v.z);
}

vec3 operator*(vec3 const& v, float const& s) {
	return vec3(
			v.x * s,
			v.y * s,
			v.z * s);
}

float length(vec3 const& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float distance(vec3 const& v1, vec3 const& v2) {
	return length(v1 - v2);
}

vec3 mix(vec3 const& v1, vec3 const& v2, const float& t) {
	return (1. - t) * v1 + t * v2;
}
