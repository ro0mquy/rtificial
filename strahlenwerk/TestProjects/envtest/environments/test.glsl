#version 430

layout(location = 0) uniform vec2 res;
layout(location = 35) uniform vec3 camera_direction;
out vec3 color;

void main() {
	color = vec3(gl_FragCoord.xy/res.xy, 0.);
	//color = abs(camera_direction);
}
