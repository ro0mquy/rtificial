#include "helper.glsl"

uniform float camera_sensor_width;
uniform float camera_focus_dist;
uniform float camera_focal_length;
uniform float camera_f_stop;
uniform float camera_f_stop_min;
uniform float camera_f_stop_max;

float calcCoC(float dist) {
	float focus_dist = camera_focus_dist;
	float f = camera_focal_length;
	float N = clamp(camera_f_stop, camera_f_stop_min, camera_f_stop_max);
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

float getFstopRelative() {
	float N = clamp(camera_f_stop, camera_f_stop_min, camera_f_stop_max);
	return (N - camera_f_stop_min) / (camera_f_stop_max - camera_f_stop_min);
}
