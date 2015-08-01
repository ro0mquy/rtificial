#include "quat_rotate.glsl"
#include "noise.glsl"
#line 4 "camera"

uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat
uniform float camera_focal_length;
uniform float camera_sensor_width;

// gets set with camGetDirection
float screen_distance = camera_focal_length / camera_sensor_width;

vec3 camGetDirectionSS(vec2 coord, out float screen_d) {
	vec3 dir = vec3((coord - .5 * res) / res.x,
		camera_focal_length / camera_sensor_width);
	screen_d = length(dir.xz);
	dir = normalize(dir);
	return dir;
}

vec3 camGetDirection() {
	vec2 shake = vec2(
		smoothNoise(vec2(time * camera_shake_freq_rt_float, 23)),
		smoothNoise(vec2(time * camera_shake_freq_rt_float, 283))
	) * camera_shake_intensity_rt_float;
	vec3 dir = camGetDirectionSS(gl_FragCoord.xy, screen_distance);
	dir.z = -dir.z;
	pQuatRotate(dir, camera_rotation);
	return dir;
}

float camGetPixelSize(float dist) {
	return .5 * dist / (screen_distance * res.x);
}
