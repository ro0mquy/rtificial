#include <math.h>

#include "matrix.h"

float mat3x3_determinant(mat3x3 m) {
	float a = m.a[0][0] * (m.a[1][1] * m.a[2][2] - m.a[1][2] * m.a[2][1]); // a * (ei - fh)
	float b = m.a[0][1] * (m.a[1][0] * m.a[2][2] - m.a[1][2] * m.a[2][0]); // b * (di - fg)
	float c = m.a[0][2] * (m.a[1][0] * m.a[2][1] - m.a[1][1] * m.a[2][0]); // c * (dh - eg)

	return a - b + c;
}

mat4x4 mat4x4_new(float a[4][4]) {
	mat4x4 o;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			o.a[i][j] = a[i][j];
		}
	}
	return o;
}

mat4x4 mat4x4_s_mult(float s, mat4x4 m) {
	mat4x4 o;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			o.a[i][j] = s * m.a[i][j];
		}
	}
	return o;
}

mat4x4 mat4x4_mult(mat4x4 a, mat4x4 b) {
	mat4x4 o;
	for (int i = 0 ; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			o.a[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				o.a[i][j] += a.a[i][k] * b.a[k][j];
			}
		}
	}
	return o;
}

mat4x4 mat4x4_transpose(mat4x4 m) {
	mat4x4 o;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			o.a[i][j] = m.a[j][i];
		}
	}
	return o;
}

// http://www.ccodechamp.com/c-program-to-find-inverse-of-matrix/
mat4x4 mat4x4_invert(mat4x4 m) {
	// first assemble the matrix of Minors (of determinants of submatrices)
	mat4x4 adj;
	for (int q = 0; q < 4; q++) {
		for (int p = 0; p < 4; p++) {
			// assemble the mat3x3 of which we want to calculate the determinant
			mat3x3 d;
			int a = 0;
			int b = 0;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i != q && j != p) {
						d.a[a][b] = m.a[i][j];
						if (b >= 2) {
							b = 0;
							a++;
						} else {
							b++;
						}
					}
				}
			}
			adj.a[q][p] = pow(-1, q + p) * mat3x3_determinant(d);
		}
	}

	// we devide first through the determinant and transpose then
	// because we have already all subdeterminants, we can use them
	float det = m.a[0][0] * adj.a[0][0] + m.a[0][1] * adj.a[0][1] + m.a[0][2] * adj.a[0][2] + m.a[0][3] * adj.a[0][3];
	mat4x4 adj_divided = mat4x4_s_mult(1/det, adj);

	// transposing is the last step
	mat4x4 adj_div_transposed = mat4x4_transpose(adj_divided);

	return adj_div_transposed;
}
