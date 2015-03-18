#include "scalar.h"
#include "stdmath.h"

float distance(float const& s1, float const& s2) {
	return abs(s1 - s2);
}

float mix(float const& s1, float const& s2, float const& t) {
	return (1. - t) * s1 + t * s2;
}
