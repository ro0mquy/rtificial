#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
	float a[3][3];
} mat3x3;

typedef struct {
	float a[4][4];
} mat4x4;

float mat3x3_determinant(mat3x3 m);
mat4x4 mat4x4_new(float a[4][4]);
mat4x4 mat4x4_s_mult(float s, mat4x4 m);
mat4x4 mat4x4_mult(mat4x4 a, mat4x4 b);
mat4x4 mat4x4_transpose(mat4x4 m);
mat4x4 mat4x4_invert(mat4x4 m);

#endif
