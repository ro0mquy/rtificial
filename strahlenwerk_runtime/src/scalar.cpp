#include "scalar.h"

extern float abs(float x);

double distance(double const& s1, double const& s2) {
	return abs(s1 - s2);
}

double mix(double const& s1, double const& s2, double const& t) {
	return (1. - t) * s1 + t * s2;
}
