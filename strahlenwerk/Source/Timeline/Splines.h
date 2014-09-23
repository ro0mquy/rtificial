#ifndef SPLINES_H
#define SPLINES_H

#include <glm/glm.hpp>

// taken from catmullrom.pdf
// see also wikipedia for centripetal catmull-rom splines
// rawT must be between 0 and 1
// actually float can be replaced with another scalar
template <typename genType>
genType CentripetalCatmullRomSpline(genType const& P0, genType const& P1, genType const& P2, genType const& P3, const float rawT) {
	// we calculate with deltas and not absolute times
	const float dt01 = glm::sqrt(glm::distance(P0, P1));
	const float dt12 = glm::sqrt(glm::distance(P1, P2));
	const float dt23 = glm::sqrt(glm::distance(P2, P3));
	const float dt = rawT * dt12;

	const genType L01 =              - dt  / dt01 * P0 + (dt + dt01) / dt01 * P1;
	const genType L12 =        (dt12 - dt) / dt12 * P1 +          dt / dt12 * P2;
	const genType L23 = (dt23 + dt12 - dt) / dt23 * P2 + (dt - dt12) / dt23 * P3;

	const genType L012 =        (dt12 - dt) / (dt01 + dt12) * L01 + (dt + dt01) / (dt01 + dt12) * L12;
	const genType L123 = (dt23 + dt12 - dt) / (dt12 + dt23) * L12 +          dt / (dt12 + dt23) * L23;

	const genType C12 = (dt12 - dt) / dt12 * L012 + dt / dt12 * L123;
	return C12;
}

#endif // SPLINES_H
