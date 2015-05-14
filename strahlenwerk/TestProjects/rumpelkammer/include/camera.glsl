#include "quat_rotate.glsl"
#line 3 "camera"

uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat
uniform float camera_focal_length;

vec3 camGetDirection(out float screen_distance) {
	vec3 dir = vec3((gl_FragCoord.xy - .5 * res) / res.x,
		-camera_focal_length / .03);
	screen_distance = length(dir.xz);
	dir = normalize(dir);
	pQuatRotate(dir, camera_rotation);
	return dir;
}

vec3 camGetDirection() {
	float dummy;
	return camGetDirection(dummy);
}
