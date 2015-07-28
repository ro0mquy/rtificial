#include "helper.glsl"

uniform float camera_sensor_width;
uniform float camera_focus_dist;
uniform float camera_focal_length;
uniform float camera_f_stop;

float calcCoC(float dist) {
	float focus_dist = camera_focus_dist;
	float f = camera_focal_length;
	float N = camera_f_stop;
	float coc = abs(dist - focus_dist)/dist * (f * f) / (N * (focus_dist - f)) / camera_sensor_width * 1280.;
	const float MAX_COC = 32;
	return min(MAX_COC, coc);
}

float sampleAlpha(float sampleCoC) {
	float singlePixelRadius = .5;
	return min(
		1. / (Pi * square(sampleCoC)),
		1. / (Pi * square(singlePixelRadius))
	);
}
