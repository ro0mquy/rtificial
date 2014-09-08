uniform float sigma;

vec3 gaussian(sampler2D tex, vec2 coords, vec2 dir, vec2 size, float sigma_factor) {
	const float N = 5. / sigma_factor;
	const float PI = acos(-1.);
	const float sigma = 10. * sigma / sigma_factor;
	float coefficient = 1. / sqrt(2. * PI) * sigma;
	float g1 = exp(-.5 / (sigma * sigma));
	float g2 = g1 * g1;

	float coefficientSum = coefficient;
	vec3 avg = textureLod(tex, coords, 0.).rgb * coefficient;

	for(int i = 1; i <= N; i++) {
		coefficient *= g1;
		g1 *= g2;

		avg += textureLod(tex, coords - i * dir * size, 0.).rgb * coefficient;
		avg += textureLod(tex, coords + i * dir * size, 0.).rgb * coefficient;

		coefficientSum += 2. * coefficient;
	}

	return avg / coefficientSum;
}
