#ifndef MATH_VEC4_H
#define MATH_VEC4_H

struct vec4 {
	float x, y, z, w;

	vec4();
	vec4(float x_, float y_, float z_, float w_);
};

vec4 operator+(vec4 const& v1, vec4 const& v2);
vec4 operator-(vec4 const& v1, vec4 const& v2);

vec4 operator*(float const& s, vec4 const& v);
vec4 operator*(vec4 const& v, float const& s);

float length(vec4 const& v);
float distance(vec4 const& v1, vec4 const& v2);
vec4 mix(vec4 const& v1, vec4 const& v2, const float& t);

#endif // MATH_VEC4_H
