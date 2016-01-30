vec3 sun_dir = vec3(0.000000, 0.390731, 0.920505);
vec3 sun_radiance = vec3(12.110122, 13.549319, 14.499019);
vec3 sky_radiance(vec3 d) {
	vec3 color;
	float cos_gamma = dot(d, sun_dir);
	float gamma = acos(cos_gamma);
	float cos_theta = d.y;
	float rayM = cos_gamma * cos_gamma;
	float zenith = sqrt(cos_theta);
	{
		float A = -1.078444;
		float B = -0.157473;
		float C = 0.660505;
		float D = 2.178387;
		float E = -3.632607;
		float F = 0.451305;
		float G = 0.323776;
		float H = 1.167765;
		float I = 0.677045;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[0] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	{
		float A = -1.086754;
		float B = -0.183093;
		float C = 0.521530;
		float D = 2.980337;
		float E = -4.393964;
		float F = 0.493945;
		float G = 0.215704;
		float H = 1.937758;
		float I = 0.666791;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[1] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	{
		float A = -1.098371;
		float B = -0.254014;
		float C = 0.409843;
		float D = -0.002073;
		float E = -0.408308;
		float F = 0.449974;
		float G = 0.111291;
		float H = 2.242290;
		float I = 0.663794;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[2] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	return sun_radiance * color;
}
