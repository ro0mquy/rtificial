#ifndef rotator_environment_H
#define rotator_environment_H
const char rotator_environment_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(vec3 sun_dir = vec3(0.000000, 0.390731, 0.920505);
)shader_source"
R"shader_source(vec3 sun_radiance = vec3(12.110122, 13.549319, 14.499019);
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
R"shader_source(		float A = -1.078444;
)shader_source"
R"shader_source(		float B = -0.157473;
)shader_source"
R"shader_source(		float C = 0.660505;
)shader_source"
R"shader_source(		float D = 2.178387;
)shader_source"
R"shader_source(		float E = -3.632607;
)shader_source"
R"shader_source(		float F = 0.451305;
)shader_source"
R"shader_source(		float G = 0.323776;
)shader_source"
R"shader_source(		float H = 1.167765;
)shader_source"
R"shader_source(		float I = 0.677045;
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
R"shader_source(		float A = -1.086754;
)shader_source"
R"shader_source(		float B = -0.183093;
)shader_source"
R"shader_source(		float C = 0.521530;
)shader_source"
R"shader_source(		float D = 2.980337;
)shader_source"
R"shader_source(		float E = -4.393964;
)shader_source"
R"shader_source(		float F = 0.493945;
)shader_source"
R"shader_source(		float G = 0.215704;
)shader_source"
R"shader_source(		float H = 1.937758;
)shader_source"
R"shader_source(		float I = 0.666791;
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
R"shader_source(		float A = -1.098371;
)shader_source"
R"shader_source(		float B = -0.254014;
)shader_source"
R"shader_source(		float C = 0.409843;
)shader_source"
R"shader_source(		float D = -0.002073;
)shader_source"
R"shader_source(		float E = -0.408308;
)shader_source"
R"shader_source(		float F = 0.449974;
)shader_source"
R"shader_source(		float G = 0.111291;
)shader_source"
R"shader_source(		float H = 2.242290;
)shader_source"
R"shader_source(		float I = 0.663794;
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
R"shader_source(		out_color.rgb = sun_dir.y * sun_radiance * vec3(0.08690, 0.10363, 0.08464);
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
