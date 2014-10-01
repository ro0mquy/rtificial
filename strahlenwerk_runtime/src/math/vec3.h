#ifndef MATH_VEC3_H
#define MATH_VEC3_H

struct vec3 {
	double x, y, z;

	vec3(double x_, double y_, double z_);
};

vec3 operator+(vec3 const& v1, vec3 const& v2);
vec3 operator-(vec3 const& v1, vec3 const& v2);

vec3 operator*(double const& s, vec3 const& v);
vec3 operator*(vec3 const& v, double const& s);

double length(vec3 const& v);
double distance(vec3 const& v1, vec3 const& v2);
vec3 mix(vec3 const& v1, vec3 const& v2, const double& t);

#endif // MATH_VEC3_H
