#include "vec3.h"
#include <cmath>

vec3::vec3(double x_, double y_, double z_) :
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

vec3 operator*(double const& s, vec3 const& v) {
	return vec3(
			s * v.x,
			s * v.y,
			s * v.z);
}

vec3 operator*(vec3 const& v, double const& s) {
	return vec3(
			v.x * s,
			v.y * s,
			v.z * s);
}

double length(vec3 const& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

double distance(vec3 const& v1, vec3 const& v2) {
	return length(v1 - v2);
}

vec3 mix(vec3 const& v1, vec3 const& v2, const double& t) {
	return (1. - t) * v1 + t * v2;
}
