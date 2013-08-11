#ifndef VECTOR_H
#define VECTOR_H

typedef struct { const float x, y; } vec2;
typedef struct { const float x, y, z; } vec3;
typedef struct { const float x, y, z, w; } vec4;

vec2 vec2_new(float x, float y);
vec3 vec3_new(float x, float y, float z);
vec4 vec4_new(float x, float y, float z, float w);

float vec2_length(vec2);
float vec3_length(vec3);
float vec4_length(vec4);

float vec2_dot(vec2, vec2);
float vec3_dot(vec3, vec3);
float vec4_dot(vec4, vec4);

float vec2_distance(vec2, vec2);
float vec3_distance(vec3, vec3);
float vec4_distance(vec4, vec4);

vec2 vec2_normalize(vec2);
vec3 vec3_normalize(vec3);
vec4 vec4_normalize(vec4);

vec2 vec2_inv(vec2);
vec3 vec3_inv(vec3);
vec4 vec4_inv(vec4);

vec2 vec2_add(vec2, vec2);
vec3 vec3_add(vec3, vec3);
vec4 vec4_add(vec4, vec4);

vec2 vec2_sub(vec2, vec2);
vec3 vec3_sub(vec3, vec3);
vec4 vec4_sub(vec4, vec4);

vec2 vec2_mult(vec2, vec2);
vec3 vec3_mult(vec3, vec3);
vec4 vec4_mult(vec4, vec4);

vec2 vec2_div(vec2, vec2);
vec3 vec3_div(vec3, vec3);
vec4 vec4_div(vec4, vec4);

vec2 vec2_s_add(float, vec2);
vec3 vec3_s_add(float, vec3);
vec4 vec4_s_add(float, vec4);

vec2 vec2_s_mult(float, vec2);
vec3 vec3_s_mult(float, vec3);
vec4 vec4_s_mult(float, vec4);

vec3 vec3_cross(vec3, vec3);

#endif
