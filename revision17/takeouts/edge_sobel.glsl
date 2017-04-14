#include "post.glsl"
#include "helper.glsl"
#line 4

/*
 * Canny Edge Detection
 * http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
 *
 * Performs kernel convolution analogous to Sobel.
 */

uniform bool post_disable_edge;
uniform sampler2D color; // vec3
uniform sampler2D depth; // float

out vec2 out_gradient;

const mat3 sobel_x = transpose(mat3(
	-1., 0., 1.,
	-2., 0., 2.,
	-1., 0., 1.
));
const mat3 sobel_y = transpose(sobel_x);

void main() {
	if (post_disable_edge) {
		out_gradient = vec2(0.);
		return;
	}
	vec2 dist = 1/res;
	float lum_sum_x = 0.;
	float lum_sum_y = 0.;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec2 mytc = tc + dist * vec2(i, j);
			float lum = rgb2luma(textureLod(color, mytc, 0.).rgb);
		//	float lum = textureLod(depth, mytc, 0.).x;
			lum_sum_x += lum*sobel_x[i+1][j+1];
			lum_sum_y += lum*sobel_y[i+1][j+1];
		}
	}

	float g = sqrt(square(lum_sum_x) + square(lum_sum_y));
	float theta = atan(lum_sum_y,lum_sum_x);

	out_gradient = vec2(g, theta);
}
