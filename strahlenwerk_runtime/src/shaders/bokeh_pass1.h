#ifndef bokeh_pass1_H
#define bokeh_pass1_H
const char bokeh_pass1_source[] = R"shader_source(#version 430

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

layout(binding = 0) uniform sampler2D color; // vec3
layout(binding = 1) uniform sampler2D coc; // float

layout(location = 0) out vec4 upwards;
layout(location = 1) out vec4 both;

vec4 gatherDirection(vec3 baseColor, float baseCoC, vec2 dir) {
	dir *= bokeh_rotation;
	vec2 pixelSize = 1./res;
	vec4 accum = vec4(vec3(0.), baseCoC);
	float sum = 0.;

	for(int i = 0; i < kernelSize; i++) {
		float dist = i + .5;
		vec3 otherColor = textureLod(color, tc + dist * pixelSize * dir, 0.).rgb;
		float otherCoC = textureLod(coc, tc + dist * pixelSize * dir, 0.).r;
		sum += gatherAndApply(otherColor, otherCoC, baseCoC, dist, accum);
	}

	accum.rgb = sum > 1e-6 ? accum.rgb / sum : baseColor;
	return accum;
}

void main() {
	vec3 baseColor = textureLod(color, tc, 0.).rgb;
	float baseCoC = textureLod(coc, tc, 0.).r;

	vec4 upwardsBlur = gatherDirection(baseColor, baseCoC, vec2(0., -1.));
	vec4 downLeftBlur = gatherDirection(baseColor, baseCoC, vec2(cos(radians(30.)), sin(radians(30.))));

	vec4 bothBlurs;
	bothBlurs.rgb = upwardsBlur.rgb + downLeftBlur.rgb;
	/*
	if(abs(upwardsBlur.a) > abs(downLeftBlur.a)) {
		bothBlurs.a = upwardsBlur.a;
	} else {
		bothBlurs.a = downLeftBlur.a;
	}
	*/
	// experimental
	if(downLeftBlur.a >= 0.) {
		if(upwardsBlur.a >= 0.) {
			bothBlurs.a = max(downLeftBlur.a, upwardsBlur.a);
		} else {
			bothBlurs.a = upwardsBlur.a;
		}
	} else {
		if(upwardsBlur.a >= 0.) {
			bothBlurs.a = downLeftBlur.a;
		} else {
			bothBlurs.a = min(downLeftBlur.a, upwardsBlur.a);
		}
	}

	upwards = upwardsBlur;
	both = bothBlurs;
}
)shader_source";
#endif
