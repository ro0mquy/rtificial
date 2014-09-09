vec3 gaussian(sampler2D tex, vec2 coords, vec2 dir, vec2 pixelsize, float scale) {
	scale *= (4. * scale) / (1920. * pixelsize.x);
	const float N = 7 * scale;
	const float PI = acos(-1.);
	const vec3 sigma = 2.65 * scale * vec3(1.34, 1.2, 1.);
	vec3 coefficient = 1. / sqrt(2. * PI) * sigma;
	vec3 g1 = exp(-.5 / (sigma * sigma));
	vec3 g2 = g1 * g1;

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