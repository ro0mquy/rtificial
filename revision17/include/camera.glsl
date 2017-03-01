#include "quat_rotate.glsl"
#include "noise.glsl"
#include "helper.glsl"
#line 5

uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat
uniform float camera_focal_length;
uniform float camera_sensor_width;

uniform bool camera_crane_active;
uniform vec3 camera_crane_base;
uniform float camera_crane_length;
uniform float camera_crane_phi;
uniform float camera_crane_theta;

uniform bool camera_tracking_active;
uniform vec3 camera_tracking_target;
uniform float camera_tracking_roll;

uniform bool spectatormode_active;
uniform vec3 spectatormode_position;
uniform vec4 spectatormode_rotation; // quat
uniform float spectatormode_focal_length;

// gets set with camGetDirection
float screen_distance = camera_focal_length / camera_sensor_width;

float camGetFocalLength() {
	if (spectatormode_active) {
		return spectatormode_focal_length;
	}
	return camera_focal_length;
}

vec3 camGetPositionTimeline() {
	if (camera_crane_active) {
		vec3 crane_head_relative = camera_crane_length * unitVector(camera_crane_phi * Tau, camera_crane_theta * Tau);
		vec3 crane_head_absolute = camera_crane_base + crane_head_relative;
		return crane_head_absolute;
	}
	return camera_position;
}

vec3 camGetPosition() {
	if (spectatormode_active) {
		return spectatormode_position;
	}
	return camGetPositionTimeline();
}

mat3 camGetTrackingRotationMat() {
	// construct the camera coordinate system in a way that it is only dutch angled if wanted
	vec3 view_direction = normalize(camera_tracking_target - camGetPositionTimeline());
	vec3 view_right = -cross(view_direction, vec3(0., 1., 0.));

	if (view_right == vec3(0.)) {
		// view_direction parallel to y-axis
		view_right = vec3(1., 0., 0.);
	} else {
		view_right = normalize(view_right);
	}

	// rotate view_right around view_direction
	// from https://en.wikipedia.org/wiki/Rodrigues'_rotation_formula
	// simplified, because view_right and view_direction are perpendicular
	view_right = cos(camera_tracking_roll * Tau) * view_right + sin(camera_tracking_roll * Tau) * cross(view_direction, view_right);

	vec3 view_up = normalize(cross(view_direction, view_right));
	mat3 rotation_matrix = mat3(-view_right, view_up, -view_direction);
	return rotation_matrix;
}

vec3 camGetDirectionSS(vec2 coord, out float screen_d) {
	vec3 dir = vec3((coord - .5 * res) / res.x,
		camGetFocalLength() / camera_sensor_width);
	screen_d = length(dir.xz);
	dir = normalize(dir);
	return dir;
}

vec3 camGetDirection() {
	vec2 shake = vec2(
		smoothNoise(vec2(time * camera_shake_freq_rt_float, 23)),
		smoothNoise(vec2(time * camera_shake_freq_rt_float, 283))
	) * camera_shake_intensity_rt_float;
	vec3 dir = camGetDirectionSS(gl_FragCoord.xy + shake * res, screen_distance);
	dir.z = -dir.z;

	if (spectatormode_active) {
		pQuatRotate(dir, spectatormode_rotation);
	} else {
		if (camera_tracking_active) {
			dir = camGetTrackingRotationMat() * dir;
		} else {
			pQuatRotate(dir, camera_rotation);
		}
	}

	return dir;
}

float camGetPixelSize(float dist) {
	return .5 * dist / (screen_distance * res.x);
}

// rotates the object the same way the camera is rotated
void pRotateLikeCamera(inout vec3 v) {
	if (camera_tracking_active) {
		mat3 inverse_rotation_matrix = transpose(camGetTrackingRotationMat());
		v = inverse_rotation_matrix * v;
	} else {
		vec4 inverse_camera_rotation = vec4(-camera_rotation.xyz, camera_rotation.w);
		pQuatRotate(v, inverse_camera_rotation);
	}
}
