#version 430

layout(location = 0) uniform vec2 res;
layout(location = 35) uniform vec3 camera_direction;
layout(location = 36) uniform vec3 camera_up;
out vec3 out_color;

void main() {
	vec2 c = -(gl_FragCoord.xy / res - .5);
	vec3 direction = normalize(vec3(c, -.5));
	vec3 camera_right = cross(camera_direction, camera_up);
	direction = mat3(camera_right, camera_up, -camera_direction) * direction;
	float sun = 10. * smoothstep(cos(radians(10.)), 1., dot(direction, normalize(vec3(1.))));
	out_color = mix(vec3(1., 0., 0.), vec3(0., 0., 1.), direction.y * .5 + .5) + sun;
}

//
//void main() {
//	//color = vec3(gl_FragCoord.xy/res.xy, 0.);
//	// minus is for weirdness of cubemap coordinates - whatever
//	vec2 c = -(gl_FragCoord.xy / res - .5);
//	vec3 direction = normalize(vec3(c, -.5));
//	vec3 camera_right = cross(camera_direction, camera_up);
//	direction = mat3(camera_right, camera_up, -camera_direction) * direction;
//
//	color = abs(direction);
//}
/*
float kappa(float g, float cos_alpha) {
	return (1. + cos_alpha * cos_alpha) / pow((1. + g * g - 2. * g * cos_alpha), 3./2.);
}

float F(float cos_theta, float gamma, float A, float B, float C, float D, float E, float F, float G, float H, float I) {
	float cos_gamma = cos(gamma);
	return (1. + A * exp(B / (cos_theta + .01))) * (
		C + D * exp(E * gamma)
		+ F * cos_gamma * cos_gamma
		+ G * kappa(H, cos_gamma)
		+ I * sqrt(cos_theta)
	);
}

void main() {
	//vec3 o = camera_position;
	//float screenDist;
	//vec3 d = get_direction(screenDist);

	//float t = march(o, d, 50., screenDist);

	//if (isinf(t)) {
	//	out_color.rgb = vec3(0.);
	//} else {
	//	out_color.rgb = vec3(1.);
	//}
	vec2 c = -(gl_FragCoord.xy / res - .5);
	vec3 direction = normalize(vec3(c, -.5));
	vec3 camera_right = cross(camera_direction, camera_up);
	vec3 d = mat3(camera_right, camera_up, -camera_direction) * direction;

	float cos_theta = abs(d.y);
	float gamma = acos(dot(d, normalize(vec3(1.))));
	float r = F(cos_theta, gamma,
1.419612,
0.124154,
4.791365,
5.095158,
-1.294097,
3.499519,
2.247034,
0.357867,
5.376395
	);
	float g = F(cos_theta, gamma,
1.646483,
1.111128,
1.230932,
5.744060,
-0.411804,
1.254473,
2.919244,
2.679084,
1.612710
	);
	float b = F(cos_theta, gamma,
1.060530,
1.986828,
1.035899,
3.122163,
2.431327,
1.871444,
2.059332,
3.936215,
2.149067
	);
	//out_color = log(vec3(r, g, b))/log(300.);
	out_color = log(vec3(r, g, b))/100.;
}
*/
