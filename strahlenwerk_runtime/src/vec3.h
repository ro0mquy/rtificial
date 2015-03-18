#ifndef MATH_VEC3_H
#define MATH_VEC3_H

struct vec3 {
	float x, y, z;

	vec3();
	vec3(float x_, float y_, float z_);
};

vec3 operator+(vec3 const& v1, vec3 const& v2);
vec3 operator-(vec3 const& v1, vec3 const& v2);

vec3 operator*(float const& s, vec3 const& v);
vec3 operator*(vec3 const& v, float const& s);

float length(vec3 const& v);
float distance(vec3 const& v1, vec3 const& v2);
vec3 mix(vec3 const& v1, vec3 const& v2, const float& t);

#endif // MATH_VEC3_H
