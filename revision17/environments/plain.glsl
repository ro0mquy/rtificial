#version 430
#include "helper.glsl"
#line 4

layout(location = 0) uniform vec2 res;
layout(location = 35) uniform vec3 camera_direction;
layout(location = 36) uniform vec3 camera_up;
out vec3 out_color;

void main() {
	vec2 c = -(gl_FragCoord.xy / res - .5);
	vec3 direction = normalize(vec3(c, -.5));
	vec3 camera_right = cross(camera_direction, camera_up);
	vec3 d = mat3(camera_right, camera_up, -camera_direction) * direction;
	vec3 o = vec3(0., 5., 0.);

	vec3 color_bottom = srgb2lin(vec3(0.90780, 0.55563, 0.51623));
	vec3 color_top = srgb2lin(vec3(0.72667, 0.57653, 0.64615));
	out_color = mix(color_bottom, color_top, smoothstep(0., .1, d.y));
	//out_color = mix(pow(vec3(0.51287, 0.88513, 0.67453), vec3(2.2)), vec3(.3), 1.-iqCubicPulse(-.4, .5, d.y));

	out_color = mix(out_color, (d + 1.) / 2., .03);
}
