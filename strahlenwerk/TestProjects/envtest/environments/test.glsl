#version 430

layout(location = 0) uniform vec2 res;
layout(location = 35) uniform vec3 camera_direction;
layout(location = 36) uniform vec3 camera_up;
out vec3 color;

void main() {
	//color = vec3(gl_FragCoord.xy/res.xy, 0.);
	// minus is for weirdness of cubemap coordinates - whatever
	vec2 c = -(gl_FragCoord.xy / res - .5);
	vec3 direction = normalize(vec3(c, -.5));
	vec3 camera_right = cross(camera_direction, camera_up);
	direction = mat3(camera_right, camera_up, -camera_direction) * direction;

	color = direction;
}
