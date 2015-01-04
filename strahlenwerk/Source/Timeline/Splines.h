#ifndef SPLINES_H
#define SPLINES_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

/*
// returns q0 - q1
static glm::quat quat_sub(glm::quat const& q0, glm::quat const& q1) {
	glm::quat out;
	out.x = q0.x - q1.x;
	out.y = q0.y - q1.y;
	out.z = q0.z - q1.z;
	out.w = q0.w - q1.w;
	return out;
}

// cut q0 - q1 in two halfs
static glm::quat quat_bisect(glm::quat const& q0, glm::quat const& q1) {
	return glm::normalize(q0 + q1);
}

// mirror q0 at q1
static glm::quat quat_mirror(glm::quat const& q0, glm::quat const& q1) {
	return quat_sub(2 * glm::dot(q0, q1) * q1, q0);
}
// */

glm::quat CentripetalCatmullRomSpline(glm::quat const& P0, glm::quat const& P1, glm::quat const& P2, glm::quat const& P3, const float rawT) {
	using namespace glm;

	/* centripetal catmull rom, doesn't work properly
	// we calculate with deltas and not absolute times
	// dot() gives cos(alpha), we want cos to be always >= 0
	const float dt01 = sqrt(acos(abs(dot(P0, P1))));
	const float dt12 = sqrt(acos(abs(dot(P0, P1))));
	const float dt23 = sqrt(acos(abs(dot(P0, P1))));
	const float dt = rawT * dt12;

	const quat L01 = slerp(P0, P1, (dt01 + dt) / dt01);
	const quat L12 = slerp(P1, P2,         dt  / dt12);
	const quat L23 = slerp(P2, P3, (dt12 - dt) / dt23);

	const quat L012 = slerp(L01, L12, (dt01 + dt) / (dt01 + dt12));
	const quat L123 = slerp(L12, L23,         dt  / (dt12 + dt23));

	const quat C12 = slerp(L012, L123, dt / dt12);
	return C12;
	// */

	/*
	// uniform catmull rom
	// interpolate between P1 and P2, so we need a1 and b2
	const quat a1 = quat_bisect(quat_mirror(P0, P1), P2);
	const quat b2 = quat_mirror(quat_bisect(quat_mirror(P1, P2), P3), P2);

	const quat p00 = slerp(P1, a1, rawT);
	const quat p01 = slerp(a1, b2, rawT);
	const quat p02 = slerp(b2, P2, rawT);

	const quat p10 = slerp(p00, p01, rawT);
	const quat p11 = slerp(p01, p02, rawT);

	const quat p20 = slerp(p10, p11, rawT);

	return p20;
	// */

	//*
	// squad interpolation from Quaternion.pdf
	// interpolate between P1 and P2, so we need a1 and a2
	const quat invP1 = inverse(P1);
	const quat invP2 = inverse(P2);

	const quat a1 = P1 * exp((log(invP1 * P0) + log(invP1 * P2)) / -4.f);
	const quat a2 = P2 * exp((log(invP2 * P1) + log(invP2 * P3)) / -4.f);

	const quat p12 = slerp(P1, P2, rawT);
	const quat a12 = slerp(a1, a2, rawT);

	const quat pa12 = slerp(p12, a12, 2.f * rawT * (1.f - rawT));

	return pa12;
	// */
}

#endif // SPLINES_H
