#include "post_head.glsl"
#include "bokeh.glsl"
#line 4

uniform sampler2D upwards; // vec4
uniform sampler2D both; // vec4
out vec4 out_color;

vec4 gatherDirection(sampler2D texture, vec3 baseColor, float baseCoC, vec2 dir) {
	vec2 pixelSize = 1./res;
	vec4 accum = vec4(vec3(0.), baseCoC);
	float sum = 0.;

	for(int i = 0; i < kernelSize; i++) {
		float dist = i + .5;
		vec4 other = textureLod(texture, tc + dist * pixelSize * dir, 0.);
		sum += gatherAndApply(other.rgb, other.a, baseCoC, dist, accum);
	}

	accum.rgb /= max(sum, 1e-6);
	return accum;
}

void main() {
	vec4 base = textureLod(both, tc, 0.);

	vec4 downLeftBlur = gatherDirection(upwards, base.rgb, base.a, vec2(cos(radians(30.)), sin(radians(30.))));
	vec4 downRightBlur = gatherDirection(both, base.rgb, base.a, vec2(cos(radians(150.)), sin(radians(150.))));

	out_color = vec4((downLeftBlur.rgb + downRightBlur.rgb) / 3., max(abs(downLeftBlur.a), abs(downRightBlur.a)));
}
