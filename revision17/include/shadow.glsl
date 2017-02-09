float shadowMarchAdvanced(vec3 o, vec3 d, float t_min, float t_max, int max_iterations, float omega) {
	// o, d : ray origin, direction (normalized)
	// t_min, t_max: minimum, maximum t values
	// pixelRadius: radius of a pixel at t = 1
	// forceHit: boolean enforcing to use the
	//           candidate_t value as result
	float t = t_min;
	float candidate_error = Inf;
	float previousRadius = 0;
	float stepLength = 0;
	float functionSign = sgn(fMain(o + t_min*d, false));

	for (int i = 0; i < max_iterations; ++i) {
		float signedRadius = functionSign * fMain(d*t + o, false);
		float radius = abs(signedRadius);

		bool sorFail = omega > 1 && (radius + previousRadius) < stepLength;
		if (sorFail) {
			stepLength -= omega * stepLength;
			omega = 1;
		} else {
			stepLength = signedRadius * omega;

			float error = radius / t;
			candidate_error = min(candidate_error, error);

			if (error < 0.0001) {
				return 0.;
			}
		}

		previousRadius = radius;
		t += stepLength;

		if (t > t_max) {
			break;
		}
	}

	return candidate_error;
}

float shadowMarch(vec3 o, vec3 d, float t_max, float softness) {
	float shadowing_value = shadowMarchAdvanced(o, d, .01, t_max, 256, 1.2);

	return min(1., softness * shadowing_value);
}
