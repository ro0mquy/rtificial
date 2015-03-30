vec3 sun_dir = vec3(0.000000, 0.224951, 0.974370);
vec3 sun_radiance = vec3(12.817642, 11.758746, 10.021456);
vec3 sky_radiance(vec3 d) {
	vec3 color;
	float cos_gamma = dot(d, sun_dir);
	float gamma = acos(cos_gamma);
	float cos_theta = d.y;
	float rayM = cos_gamma * cos_gamma;
	float zenith = sqrt(cos_theta);
	{
		float A = -1.167633;
		float B = -0.267518;
		float C = 0.285294;
		float D = 1.675510;
		float E = -2.374641;
		float F = 0.214081;
		float G = 0.270609;
		float H = 0.888835;
		float I = 0.674745;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[0] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	{
		float A = -1.193493;
		float B = -0.318828;
		float C = 0.248083;
		float D = 2.467026;
		float E = -3.144857;
		float F = 0.244170;
		float G = 0.183239;
		float H = 1.415013;
		float I = 0.667284;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[1] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	{
		float A = -1.198544;
		float B = -0.383872;
		float C = -0.603962;
		float D = 0.851110;
		float E = -0.093279;
		float F = 0.378240;
		float G = 0.095607;
		float H = 2.120160;
		float I = 0.675487;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[2] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	return sun_radiance * color;
}
