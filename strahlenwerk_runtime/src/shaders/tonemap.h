#ifndef tonemap_H
#define tonemap_H
const char tonemap_source[] = R"shader_source(#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) in vec2 tc;

#line 1


layout(binding = 12) uniform sampler2D luminance; // float level(11)
layout(location = 44) uniform float key;
layout(location = 45) uniform vec3 lift; // color
layout(location = 46) uniform vec3 gamma; // color
layout(location = 47) uniform vec3 gain; // color
layout(binding = 11) uniform sampler2D blended_color; // vec3

layout(location = 0) out vec3 out_color;
float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 11.2;

vec3 tonemap(vec3 color) {
   return ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
}

void main() {
	vec3 color = textureLod(blended_color, tc, 0.).rgb;

	// tonemap
	float avgLuminance = exp(textureLod(luminance, tc, 0.).r);
	color *= key / avgLuminance;
	color = tonemap(color)/tonemap(vec3(W));
	color = clamp(color, 0., 1.);

	// color grade
	color = gain * (color + lift * pow(1. - color, 1./gamma));

	out_color = color;
}
)shader_source";
#endif
