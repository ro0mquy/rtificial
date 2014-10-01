#include "post_head.glsl"
#line 4

uniform sampler2D bloom_big; // vec3
uniform sampler2D bloom_medium; // vec3
uniform sampler2D bloom_small; // vec3
uniform sampler2D dof; // vec4
uniform sampler2D orig; // vec3

uniform float bloom_amount;

out vec3 out_color;
out float luminance;

void main() {
	vec4 dofColor = textureLod(dof, tc, 0.);
	vec3 color = textureLod(orig, tc, 0.).rgb;
	color = mix(color, dofColor.rgb, smoothstep(0., 1., abs(dofColor.a) - .5));

	// bloom
	vec3 bloom = vec3(0.);
	bloom += textureLod(bloom_big, tc, 0.).rgb;
	bloom += textureLod(bloom_medium, tc, 0.).rgb;
	bloom += textureLod(bloom_small, tc, 0.).rgb;
	bloom /= 3.;
	color = mix(color, bloom, bloom_amount * .1);

	out_color = color;
	luminance = log(max(dot(color, vec3(.2126, .7152, .0722)), 1e-6)); // TODO calculate
}
