#include "quat_rotate.glsl"
#line 3 "camera"

uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat
uniform float camera_focal_length;

// gets set with camGetDirection
float screen_distance = camera_focal_length / .03;

vec3 camGetDirection() {
	vec3 dir = vec3((gl_FragCoord.xy - .5 * res) / res.x,
		-camera_focal_length / .03);
	screen_distance = length(dir.xz);
	dir = normalize(dir);
	pQuatRotate(dir, camera_rotation);
	return dir;
}

float camGetPixelSize(float dist) {
	return .5 * dist / (screen_distance * res.x);
}
