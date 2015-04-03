#version 430
#include "sky1.glsl"
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
	vec3 p = o;
	for (int i = 0; i < 100; i++) {
		float t = 1000. - length(p);
		p += t * d;
	}
	if(p.y > 1e-6) {
		vec3 sky_d = normalize(p);
		out_color.rgb = max(sky_radiance(sky_d), vec3(0.));
		float angle = cos(radians(4.));
		// some kind of direct sun
		out_color.rgb += 30. * smoothstep(angle * .999, angle, dot(sky_d, sun_dir)) * sun_radiance;
	} else {
		out_color.rgb = sun_dir.y * sun_radiance * vec3(0.05459, 0.05459, 0.06191);
	}
}
