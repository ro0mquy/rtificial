#ifndef mienenkraft_environment_H
#define mienenkraft_environment_H
const char mienenkraft_environment_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(vec3 sun_dir = vec3(0.000000, 0.224951, 0.974370);
)shader_source"
R"shader_source(vec3 sun_radiance = vec3(12.817642, 11.758746, 10.021456);
)shader_source"
R"shader_source(vec3 sky_radiance(vec3 d) {
)shader_source"
R"shader_source(	vec3 color;
)shader_source"
R"shader_source(	float cos_gamma = dot(d, sun_dir);
)shader_source"
R"shader_source(	float gamma = acos(cos_gamma);
)shader_source"
R"shader_source(	float cos_theta = d.y;
)shader_source"
R"shader_source(	float rayM = cos_gamma * cos_gamma;
)shader_source"
R"shader_source(	float zenith = sqrt(cos_theta);
)shader_source"
R"shader_source(	{
)shader_source"
R"shader_source(		float A = -1.167633;
)shader_source"
R"shader_source(		float B = -0.267518;
)shader_source"
R"shader_source(		float C = 0.285294;
)shader_source"
R"shader_source(		float D = 1.675510;
)shader_source"
R"shader_source(		float E = -2.374641;
)shader_source"
R"shader_source(		float F = 0.214081;
)shader_source"
R"shader_source(		float G = 0.270609;
)shader_source"
R"shader_source(		float H = 0.888835;
)shader_source"
R"shader_source(		float I = 0.674745;
)shader_source"
R"shader_source(		float expM = exp(E * gamma);
)shader_source"
R"shader_source(		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
)shader_source"
R"shader_source(		color[0] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	{
)shader_source"
R"shader_source(		float A = -1.193493;
)shader_source"
R"shader_source(		float B = -0.318828;
)shader_source"
R"shader_source(		float C = 0.248083;
)shader_source"
R"shader_source(		float D = 2.467026;
)shader_source"
R"shader_source(		float E = -3.144857;
)shader_source"
R"shader_source(		float F = 0.244170;
)shader_source"
R"shader_source(		float G = 0.183239;
)shader_source"
R"shader_source(		float H = 1.415013;
)shader_source"
R"shader_source(		float I = 0.667284;
)shader_source"
R"shader_source(		float expM = exp(E * gamma);
)shader_source"
R"shader_source(		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
)shader_source"
R"shader_source(		color[1] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	{
)shader_source"
R"shader_source(		float A = -1.198544;
)shader_source"
R"shader_source(		float B = -0.383872;
)shader_source"
R"shader_source(		float C = -0.603962;
)shader_source"
R"shader_source(		float D = 0.851110;
)shader_source"
R"shader_source(		float E = -0.093279;
)shader_source"
R"shader_source(		float F = 0.378240;
)shader_source"
R"shader_source(		float G = 0.095607;
)shader_source"
R"shader_source(		float H = 2.120160;
)shader_source"
R"shader_source(		float I = 0.675487;
)shader_source"
R"shader_source(		float expM = exp(E * gamma);
)shader_source"
R"shader_source(		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
)shader_source"
R"shader_source(		color[2] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return sun_radiance * color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 4
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(layout(location = 35) uniform vec3 camera_direction;
)shader_source"
R"shader_source(layout(location = 36) uniform vec3 camera_up;
)shader_source"
R"shader_source(out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	vec2 c = -(gl_FragCoord.xy / res - .5);
)shader_source"
R"shader_source(	vec3 direction = normalize(vec3(c, -.5));
)shader_source"
R"shader_source(	vec3 camera_right = cross(camera_direction, camera_up);
)shader_source"
R"shader_source(	vec3 d = mat3(camera_right, camera_up, -camera_direction) * direction;
)shader_source"
R"shader_source(	vec3 o = vec3(0., 5., 0.);
)shader_source"
R"shader_source(	vec3 p = o;
)shader_source"
R"shader_source(	for (int i = 0; i < 100; i++) {
)shader_source"
R"shader_source(		float t = 1000. - length(p);
)shader_source"
R"shader_source(		p += t * d;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	if(p.y > 1e-6) {
)shader_source"
R"shader_source(		vec3 sky_d = normalize(p);
)shader_source"
R"shader_source(		out_color.rgb = max(sky_radiance(sky_d), vec3(0.));
)shader_source"
R"shader_source(		float angle = cos(radians(4.));
)shader_source"
R"shader_source(		// some kind of direct sun
)shader_source"
R"shader_source(		out_color.rgb += 30. * smoothstep(angle * .999, angle, dot(sky_d, sun_dir)) * sun_radiance;
)shader_source"
R"shader_source(	} else {
)shader_source"
R"shader_source(		out_color.rgb = sun_dir.y * sun_radiance;
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
