#include "post_head.glsl"

uniform sampler2D luminance; // float level(11)
uniform float key;
uniform vec3 lift; // color
uniform vec3 gamma; // color
uniform vec3 gain; // color
uniform sampler2D blended_color; // vec3

out vec3 out_color;
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
