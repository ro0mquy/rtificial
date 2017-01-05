#include "helper.glsl"

vec2 toUnitDiskPolar(vec2 c) {
	float a = 2 * c.x - 1;
	float b = 2 * c.y - 1;
	float r;
	float phi = Pi * .25;
	if (a > -b) {
		if (a > b) {
			r = a;
			phi *= b / a;
		} else {
			r = b;
			phi *= 2 - a / b;
		}
	} else {
		if (a < b) {
			r = -a;
			phi *= 4 + b / a;
		} else {
			r = -b;
			if (abs(b) > 1e-6) {
				phi *= (6 - a / b);
			} else {
				phi = 0;
			}
		}
	}
	return vec2(r, phi);

}

vec2 bokehTapSampleCoord(vec2 o, float f, float n, float phiShutterMax) {
	vec2 polar = toUnitDiskPolar(o);
	polar.y += phiShutterMax * f;
	polar.r *= pow(cos(Pi / n) / (cos(polar.y - 2 * Pi / n * floor((n * polar.y + Pi) / (2 * Pi) ))), f);
	return polar.r * vec2(cos(polar.y), sin(polar.y));
}
