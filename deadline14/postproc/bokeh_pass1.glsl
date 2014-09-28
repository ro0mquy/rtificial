#include "post_head.glsl"
#include "bokeh.glsl"
#line 4

uniform sampler2D color; // vec3
uniform sampler2D coc; // float

out vec4 upwards;
out vec4 both;

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
