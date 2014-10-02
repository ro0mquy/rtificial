#ifndef blend_H
#define blend_H
const char blend_source[] = R"shader_source(#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) in vec2 tc;

#line 1

#line 4

layout(binding = 10) uniform sampler2D bloom_big; // vec3
layout(binding = 9) uniform sampler2D bloom_medium; // vec3
layout(binding = 8) uniform sampler2D bloom_small; // vec3
layout(binding = 7) uniform sampler2D dof; // vec4
layout(binding = 0) uniform sampler2D orig; // vec3

layout(location = 33) uniform float bloom_amount;

layout(location = 0) out vec3 out_color;
layout(location = 1) out float luminance;

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
)shader_source";
#endif
