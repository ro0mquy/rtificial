#include <math.h>
#include <stdlib.h>

#include "osc.h"

static float fract(float t);

float osc_sin(const float t) {
	const float TAU = 6.283185;
	return sin(TAU * t);
}

float osc_tri(const float t) {
	return 4. * fabs(fract(t) - .5) - 1.;
}

float osc_saw(const float t) {
	return 2. * fract(t) - 1.;
}

float osc_rect(const float t) {
	return 2. * signbit(fract(t) - .5) - 1.;
}

float osc_noise(void) {
	return 2. * ((float) rand() / RAND_MAX) - 1.;
}

static float fract(const float t) {
	return t - floorf(t);
}
