float kernelSize = 0.;// * res.x / 1920.;

/*
//uniform float f_stop;

// TODO remove manual scaling
float bokeh_rotation_angle = mix(0., -radians(60.), (f_stop * 10. - .5) / (32. - .5));
mat2 bokeh_rotation = mat2(
	cos(bokeh_rotation_angle), -sin(bokeh_rotation_angle),
	sin(bokeh_rotation_angle), cos(bokeh_rotation_angle)
);
*/

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
		} else {
			// experimental
			outColor.a = max(outColor.a, CoC);
		}
		float frac = clamp(absCoC - dist, 0., 1.);
		outColor.rgb += frac * color;
		return frac;
	}
	return 0.;
}
