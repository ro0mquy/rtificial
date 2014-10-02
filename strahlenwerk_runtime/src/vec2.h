#ifndef MATH_VEC2_H
#define MATH_VEC2_H

struct vec2 {
	double x, y;

	vec2();
	vec2(double x_, double y_);
};

vec2 operator+(vec2 const& v1, vec2 const& v2);
vec2 operator-(vec2 const& v1, vec2 const& v2);

vec2 operator*(double const& s, vec2 const& v);
vec2 operator*(vec2 const& v, double const& s);

double length(vec2 const& v);
double distance(vec2 const& v1, vec2 const& v2);
vec2 mix(vec2 const& v1, vec2 const& v2, const double& t);

#endif // MATH_VEC2_H
