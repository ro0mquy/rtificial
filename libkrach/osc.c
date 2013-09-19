#include <math.h>
#include <stdlib.h>

#include "osc.h"

static float fract(float x);

float osc_sin(float x) {
	const float TAU = 6.283185;
	return sin(TAU * x);
}

float osc_tri(float x) {
	return 4. * fabs(fract(x) - .5) - 1.;
}

float osc_saw(float x) {
	return 2. * fract(x) - 1.;
}

float osc_rect(float x) {
	return 2. * signbit(fract(x) - .5) - 1.;
}

float osc_noise(void) {
	return 2. * ((float) rand() / RAND_MAX) - 1.;
}

static float fract(float x) {
	return x - floorf(x);
}
