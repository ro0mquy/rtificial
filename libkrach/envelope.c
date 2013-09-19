#include <math.h>

#include "envelope.h"

float env_adsr(const float t, const float a, const float d, const float s, const float r, const float sustain_level) {
	const float attack_end = a;
	const float decay_end = attack_end + d;
	const float sustain_end = decay_end + s;

	const float magic_voodoo_constant = 7.;
	if(t < attack_end) {
		return 1. - exp2(-(t / a) * magic_voodoo_constant);
	} else if(t < decay_end) {
		return exp2(-((t - attack_end) / s) * magic_voodoo_constant) * (1. - sustain_level) + sustain_level;
	} else if(t < sustain_end) {
		return sustain_level;
	} else {
		return exp2(-((t - sustain_end) / r) * magic_voodoo_constant) * sustain_level;
	}
}
