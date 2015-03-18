#ifndef MATH_VEC2_H
#define MATH_VEC2_H

struct vec2 {
	float x, y;

	vec2();
	vec2(float x_, float y_);
};

vec2 operator+(vec2 const& v1, vec2 const& v2);
vec2 operator-(vec2 const& v1, vec2 const& v2);

vec2 operator*(float const& s, vec2 const& v);
vec2 operator*(vec2 const& v, float const& s);

float length(vec2 const& v);
float distance(vec2 const& v1, vec2 const& v2);
vec2 mix(vec2 const& v1, vec2 const& v2, const float& t);

#endif // MATH_VEC2_H
