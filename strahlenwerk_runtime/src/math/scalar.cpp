#include "scalar.h"

double mix(double const& s1, double const& s2, double const& t) {
	return (1. - t) * s1 + t * s2;
}
