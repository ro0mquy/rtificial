#include "scalar.h"
#include "stdmath.h"

float distance(float const& s1, float const& s2) {
	return abs(s1 - s2);
}

float mix(float const& s1, float const& s2, float const& t) {
	return (1. - t) * s1 + t * s2;
}

float clamp(const float x, const float min, const float max) {
	if (x < min) {
		return min;
	}
	if (x > max) {
		return max;
	}
	return x;
}
