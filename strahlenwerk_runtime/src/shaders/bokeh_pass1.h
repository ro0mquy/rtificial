#ifndef bokeh_pass1_H
#define bokeh_pass1_H
const char bokeh_pass1_source[] =R"shader_source(#version 430)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(location = 0) in vec2 tc;)shader_source"
R"shader_source()shader_source"
R"shader_source(#line 1)shader_source"
R"shader_source()shader_source"
R"shader_source(float kernelSize = 40. * res.x / 1920.;)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(location = 34) uniform float f_stop;)shader_source"
R"shader_source()shader_source"
R"shader_source(// TODO remove manual scaling)shader_source"
R"shader_source(float bokeh_rotation_angle = mix(0., -radians(60.), (f_stop * 10. - .5) / (32. - .5));)shader_source"
R"shader_source(mat2 bokeh_rotation = mat2()shader_source"
R"shader_source(	cos(bokeh_rotation_angle), -sin(bokeh_rotation_angle),)shader_source"
R"shader_source(	sin(bokeh_rotation_angle), cos(bokeh_rotation_angle))shader_source"
R"shader_source();)shader_source"
R"shader_source()shader_source"
R"shader_source(float gatherAndApply(vec3 color, float CoC, float baseCoC, float dist, inout vec4 outColor) {)shader_source"
R"shader_source(	bool blurNear = CoC < 0.;)shader_source"
R"shader_source(	float absCoC = abs(CoC);)shader_source"
R"shader_source()shader_source"
R"shader_source(	if((absCoC > dist) && (blurNear || (baseCoC > 0. && absCoC < baseCoC * 2.))) {)shader_source"
R"shader_source(		if(blurNear) {)shader_source"
R"shader_source(			if(outColor.a < 0.) {)shader_source"
R"shader_source(				outColor.a = min(outColor.a, CoC);)shader_source"
R"shader_source(			} else {)shader_source"
R"shader_source(				if(-CoC > outColor.a) {)shader_source"
R"shader_source(					outColor.a = CoC;)shader_source"
R"shader_source(				})shader_source"
R"shader_source(			})shader_source"
R"shader_source(		} else {)shader_source"
R"shader_source(			// experimental)shader_source"
R"shader_source(			outColor.a = max(outColor.a, CoC);)shader_source"
R"shader_source(		})shader_source"
R"shader_source(		float frac = clamp(absCoC - dist, 0., 1.);)shader_source"
R"shader_source(		outColor.rgb += frac * color;)shader_source"
R"shader_source(		return frac;)shader_source"
R"shader_source(	})shader_source"
R"shader_source(	return 0.;)shader_source"
R"shader_source(})shader_source"
R"shader_source()shader_source"
R"shader_source(#line 4)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(binding = 0) uniform sampler2D color; // vec3)shader_source"
R"shader_source(layout(binding = 1) uniform sampler2D coc; // float)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(location = 0) out vec4 upwards;)shader_source"
R"shader_source(layout(location = 1) out vec4 both;)shader_source"
R"shader_source()shader_source"
R"shader_source(vec4 gatherDirection(vec3 baseColor, float baseCoC, vec2 dir) {)shader_source"
R"shader_source(	dir *= bokeh_rotation;)shader_source"
R"shader_source(	vec2 pixelSize = 1./res;)shader_source"
R"shader_source(	vec4 accum = vec4(vec3(0.), baseCoC);)shader_source"
R"shader_source(	float sum = 0.;)shader_source"
R"shader_source()shader_source"
R"shader_source(	for(int i = 0; i < kernelSize; i++) {)shader_source"
R"shader_source(		float dist = i + .5;)shader_source"
R"shader_source(		vec3 otherColor = textureLod(color, tc + dist * pixelSize * dir, 0.).rgb;)shader_source"
R"shader_source(		float otherCoC = textureLod(coc, tc + dist * pixelSize * dir, 0.).r;)shader_source"
R"shader_source(		sum += gatherAndApply(otherColor, otherCoC, baseCoC, dist, accum);)shader_source"
R"shader_source(	})shader_source"
R"shader_source()shader_source"
R"shader_source(	accum.rgb = sum > 1e-6 ? accum.rgb / sum : baseColor;)shader_source"
R"shader_source(	return accum;)shader_source"
R"shader_source(})shader_source"
R"shader_source()shader_source"
R"shader_source(void main() {)shader_source"
R"shader_source(	vec3 baseColor = textureLod(color, tc, 0.).rgb;)shader_source"
R"shader_source(	float baseCoC = textureLod(coc, tc, 0.).r;)shader_source"
R"shader_source()shader_source"
R"shader_source(	vec4 upwardsBlur = gatherDirection(baseColor, baseCoC, vec2(0., -1.));)shader_source"
R"shader_source(	vec4 downLeftBlur = gatherDirection(baseColor, baseCoC, vec2(cos(radians(30.)), sin(radians(30.))));)shader_source"
R"shader_source()shader_source"
R"shader_source(	vec4 bothBlurs;)shader_source"
R"shader_source(	bothBlurs.rgb = upwardsBlur.rgb + downLeftBlur.rgb;)shader_source"
R"shader_source(	/*)shader_source"
R"shader_source(	if(abs(upwardsBlur.a) > abs(downLeftBlur.a)) {)shader_source"
R"shader_source(		bothBlurs.a = upwardsBlur.a;)shader_source"
R"shader_source(	} else {)shader_source"
R"shader_source(		bothBlurs.a = downLeftBlur.a;)shader_source"
R"shader_source(	})shader_source"
R"shader_source(	*/)shader_source"
R"shader_source(	// experimental)shader_source"
R"shader_source(	if(downLeftBlur.a >= 0.) {)shader_source"
R"shader_source(		if(upwardsBlur.a >= 0.) {)shader_source"
R"shader_source(			bothBlurs.a = max(downLeftBlur.a, upwardsBlur.a);)shader_source"
R"shader_source(		} else {)shader_source"
R"shader_source(			bothBlurs.a = upwardsBlur.a;)shader_source"
R"shader_source(		})shader_source"
R"shader_source(	} else {)shader_source"
R"shader_source(		if(upwardsBlur.a >= 0.) {)shader_source"
R"shader_source(			bothBlurs.a = downLeftBlur.a;)shader_source"
R"shader_source(		} else {)shader_source"
R"shader_source(			bothBlurs.a = min(downLeftBlur.a, upwardsBlur.a);)shader_source"
R"shader_source(		})shader_source"
R"shader_source(	})shader_source"
R"shader_source()shader_source"
R"shader_source(	upwards = upwardsBlur;)shader_source"
R"shader_source(	both = bothBlurs;)shader_source"
R"shader_source(})shader_source"
;
#endif
