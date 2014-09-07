#include "post_head.glsl"

// lens distort, vignette, noise

uniform sampler2D color; // vec3
out vec3 out_color;

vec2 lens_distort(float aspect, float k, float kcube, vec2 c) {
	c = c * 2. - 1.;
	float r2 = (aspect * aspect) * (c.x * c.x) + c.y * c.y;
	float r_norm = aspect * aspect + 1.;
	float zeuch = k + kcube * sqrt(2.);
	float f = (1. + r2 * zeuch) / (1. + r_norm * zeuch);
	return (f * c) * .5 + .5;
}

// add some kind of vignette
// 0 <= intensity <= 1
// intensity = .7 is good default
float vignette(float intensity, vec2 c) {
	float one_minus_intesity = 1. - intensity;
	c.y *= -1.;
	return one_minus_intesity + intensity * 16. * c.x *c.y * (1. - c.x) * (-1. - c.y);
}

void main() {
	vec3 col;
	float k = .01;
	float kcube = .02;
	float aspect = res.x / res.y;

	col.r = textureLod(color, lens_distort(aspect, k * 1.34, kcube, tc), 0.).r;
	col.g = textureLod(color, lens_distort(aspect, k * 1.27, kcube, tc), 0.).g;
	col.b = textureLod(color, lens_distort(aspect, k, kcube, tc), 0.).b;

	float vignette_intensity = .7;
	col *= vignette(vignette_intensity, gl_FragCoord.xy / res);

	out_color = col; // TODO apply noise (ein halbes bit)
}
