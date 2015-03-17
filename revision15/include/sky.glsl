vec3 sky_radiance(vec3 d) {
	vec3 sun_dir = vec3(0.000000, 0.500000, -0.866025);
	vec3 color;
	float cos_gamma = dot(d, sun_dir);
	float gamma = acos(cos_gamma);
	float cos_theta = d.y;
	float rayM = cos_gamma * cos_gamma;
	float zenith = sqrt(cos_theta);
	{
		float A = -1.077535;
		float B = -0.167038;
		float C = 1.067469;
		float D = 2.127521;
		float E = -3.241906;
		float F = 0.464434;
		float G = 0.271425;
		float H = 0.751948;
		float I = 0.684105;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[0] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	{
		float A = -1.080161;
		float B = -0.181809;
		float C = 0.839229;
		float D = 3.275995;
		float E = -4.528001;
		float F = 0.420125;
		float G = 0.195859;
		float H = 1.698241;
		float I = 0.667290;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[1] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	{
		float A = -1.082477;
		float B = -0.239088;
		float C = 0.627911;
		float D = 0.014897;
		float E = -0.611513;
		float F = 0.355414;
		float G = 0.110903;
		float H = 2.067683;
		float I = 0.661661;
		float expM = exp(E * gamma);
		float mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);
		color[2] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);
	}
	vec3 radiances = vec3(14.505033, 16.987684, 19.659879);
	return radiances * color;
}
