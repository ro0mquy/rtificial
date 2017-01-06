#include "post.glsl"
#include "helper.glsl"
#line 4

/*
 * Canny Edge Detection
 * http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
 *
 * Performs non-maximum suppression on sobel-filtered data.
 */

uniform sampler2D gradient; // vec2

uniform float post_edge_threshold_upper;
//uniform float post_edge_threshold_lower;
uniform bool post_edge_nonmax;

out float out_edge;

void main() {
	vec2 grad = textureLod(gradient, tc, 0.).xy;
	float g = grad.x;
	float theta = grad.y;
	theta = theta / (1./8.*Tau);
	theta = floor(theta + 0.5) * 1./8.*Tau;
	vec2 dist = 1/res;

	// Hystersis thresholds
	// (no actual hysteresis performed)
	if (g <= post_edge_threshold_upper) {
		out_edge = 0.;
	} else if ( g > post_edge_threshold_upper) {
		if (post_edge_nonmax) {
			// Non-max suppression
			vec2 dir = dist * clamp(vec2(1/tan(theta), tan(theta)), -1., 1. );

			float back = textureLod(gradient, tc-dist, 0.).x;
			float here = g;
			float forward = textureLod(gradient, tc+dist, 0.).x;

			if (here >= back && here >= forward) {
				out_edge = 1;
			} else {
				out_edge = 0.;
			}
		} else {
			out_edge = 1;
		}
	}
}
