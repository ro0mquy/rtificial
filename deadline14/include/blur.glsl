vec3 gaussian(sampler2D tex, vec2 coords, vec2 dir, vec2 pixelsize, float scale) {
	const float N = 5 * scale;
	const float PI = acos(-1.);
	const float sigma = 2.3 * scale;
	float coefficient = 1. / sqrt(2. * PI) * sigma;
	float g1 = exp(-.5 / (sigma * sigma));
	float g2 = g1 * g1;

	float coefficientSum = coefficient;
	vec3 avg = textureLod(tex, coords, 0.).rgb * coefficient;

	for(int i = 1; i <= N; i++) {
		coefficient *= g1;
		g1 *= g2;

		avg += textureLod(tex, coords - i * dir * pixelsize, 0.).rgb * coefficient;
		avg += textureLod(tex, coords + i * dir * pixelsize, 0.).rgb * coefficient;

		coefficientSum += 2. * coefficient;
	}

	return avg / coefficientSum;
}
