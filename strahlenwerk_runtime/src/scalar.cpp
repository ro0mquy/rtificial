#include "scalar.h"
#include <cmath>

double distance(double const& s1, double const& s2) {
	return std::abs(s1 - s2);
}

double mix(double const& s1, double const& s2, double const& t) {
	return (1. - t) * s1 + t * s2;
}
