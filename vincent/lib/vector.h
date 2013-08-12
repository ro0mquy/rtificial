#ifndef VECTOR_H
#define VECTOR_H

typedef struct { const float x, y, z; } vec3;

vec3 vec3_new(float x, float y, float z);

float vec3_length(vec3);

float vec3_dot(vec3, vec3);

float vec3_distance(vec3, vec3);

vec3 vec3_normalize(vec3);

vec3 vec3_inv(vec3);

vec3 vec3_add(vec3, vec3);

vec3 vec3_sub(vec3, vec3);

vec3 vec3_mult(vec3, vec3);

vec3 vec3_div(vec3, vec3);

vec3 vec3_s_add(float, vec3);

vec3 vec3_s_mult(float, vec3);

vec3 vec3_cross(vec3, vec3);

#endif
