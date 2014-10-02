#ifndef bokeh_pass2_H
#define bokeh_pass2_H
const char bokeh_pass2_source[] = R"shader_source(#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) in vec2 tc;

#line 1

float kernelSize = 40. * res.x / 1920.;

layout(location = 34) uniform float f_stop;

// TODO remove manual scaling
float bokeh_rotation_angle = mix(0., -radians(60.), (f_stop * 10. - .5) / (32. - .5));
mat2 bokeh_rotation = mat2(
	cos(bokeh_rotation_angle), -sin(bokeh_rotation_angle),
	sin(bokeh_rotation_angle), cos(bokeh_rotation_angle)
);

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

#line 4

layout(binding = 2) uniform sampler2D upwards; // vec4
layout(binding = 3) uniform sampler2D both; // vec4
layout(location = 0) out vec4 out_color;

vec4 gatherDirection(sampler2D texture, vec3 baseColor, float baseCoC, vec2 dir) {
	dir *= bokeh_rotation;
	vec2 pixelSize = 1./res;
	vec4 accum = vec4(vec3(0.), baseCoC);
	float sum = 0.;

	for(int i = 0; i < kernelSize; i++) {
		float dist = i + .5;
		vec4 other = textureLod(texture, tc + dist * pixelSize * dir, 0.);
		sum += gatherAndApply(other.rgb, other.a, baseCoC, dist, accum);
	}

	accum.rgb = sum > 1e-6 ? accum.rgb / sum : baseColor;
	return accum;
}

void main() {
	vec4 base = textureLod(both, tc, 0.);

	vec4 downLeftBlur = gatherDirection(upwards, base.rgb, base.a, vec2(cos(radians(30.)), sin(radians(30.))));
	vec4 downRightBlur = gatherDirection(both, base.rgb * .5, base.a, vec2(cos(radians(150.)), sin(radians(150.))));

	out_color = vec4((downLeftBlur.rgb + downRightBlur.rgb) / 3., max(abs(downLeftBlur.a), abs(downRightBlur.a)));
}
)shader_source";
#endif
