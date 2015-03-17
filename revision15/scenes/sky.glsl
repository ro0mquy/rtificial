#include "rtificial.glsl"
#include "sky.glsl"
#line 4

out vec4 out_color;

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
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);

	//float t = march(o, d, 50., screenDist);

	//if (isinf(t)) {
	//	out_color.rgb = vec3(0.);
	//} else {
	//	out_color.rgb = vec3(1.);
	//}

	out_color.rgb = sky_radiance(d);
	if(any(isnan(out_color.rgb))) {
		out_color.rgb = vec3(0.);
	}
/*
	float cos_theta = d.y;
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
	//out_color = vec4(log(vec3(r, g, b))/log(sky_max_rt_float), 1.);
	*/
}
