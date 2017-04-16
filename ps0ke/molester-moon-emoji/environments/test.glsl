#version 430
#line 3

layout(location = 0) uniform vec2 res;
layout(location = 35) uniform vec3 camera_direction;
layout(location = 36) uniform vec3 camera_up;
out vec3 out_color;

void main() {
	out_color.rgb = vec3(0);
}
