#version 430
//#include "sky.glsl"
#include "noise.glsl"
#include "helper.glsl"
#include "sdf/domain.glsl"
#line 6

layout(location = 0) uniform vec2 res;
layout(location = 35) uniform vec3 camera_direction;
layout(location = 36) uniform vec3 camera_up;
out vec3 out_color;

vec3 sky(vec3 d) {
	vec3 pink = sky_pink_rt_color;
	vec3 dark_blue = sky_dark_blue_rt_color;
	vec3 dark_purple = sky_dark_purple_rt_color;
	float theta = abs(acos(d.y) / Pi * 2 - 1);
	float phi = atan(d.z, d.x) / Pi;
	//dark_blue = mix(dark_blue, dark_purple, smoothNoise(vec2(phi * 2, theta) * 5.) * .5 + .5);
	dark_blue = mix(dark_purple, dark_blue, smoothNoise(d * 2) * .5 + .5);
	phi = abs(phi);
	float theta_distorted = theta + valueNoise(phi * sky_pink_freq_rt_float) * sky_pink_ampl_rt_float;
	vec3 background = mix(pink, dark_blue, smoothstep(sky_pink_offset_rt_float, sky_pink_offset_rt_float+ sky_pink_spread_rt_float, theta_distorted));
	vec3 orange = sky_orange_rt_color;
	float theta_stripes = theta;
	pTrans(theta_stripes, sky_stripes_height_rt_float);
	pDomrepInterval(theta_stripes, sky_stripes_dist_rt_float * sky_stripes_thick_rt_float, -2, 2);
	//pDomrep(theta_stripes, sky_stripes_dist_rt_float * sky_stripes_thick_rt_float);
	float f_stripes = abs(theta_stripes) - sky_stripes_thick_rt_float;
	background = mix(background, orange * 5, f_stripes < 0? 1 : 0);
	return background * 10;
}

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
	//if(p.y > 1e-6) {
		vec3 sky_d = normalize(p);
		out_color.rgb = max(sky(sky_d), vec3(0.));
		// float angle = cos(radians(4.));
		// some kind of direct sun
		// out_color.rgb += 30. * smoothstep(angle * .999, angle, dot(sky_d, sun_dir)) * sun_radiance;
	//} else {
	//	out_color.rgb = sun_dir.y * sun_radiance * vec3(0.08690, 0.10363, 0.08464);
	//}
}
