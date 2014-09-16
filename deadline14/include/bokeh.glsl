float kernelSize = 13.;

float gatherAndApply(vec3 color, float CoC, float baseCoC, float dist, inout vec4 outColor) {
	bool blurNear = CoC < 0.;
	float absCoC = abs(CoC);

	if((absCoC > dist) && (blurNear || (baseCoC > 0. && absCoC < baseCoC * 2.))) {
		if(blurNear) {
			if(outColor.a < 0.) {
				outColor.a = min(outColor.a, CoC);
			} else {
				if(-CoC > outColor.a) {
					outColor.a = CoC;
				}
			}
		}
		float frac = clamp(absCoC - dist, 0., 1.);
		outColor.rgb += frac * color;
		return frac;
	}
	return 0.;
}
