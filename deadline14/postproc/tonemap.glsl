#include "post_head.glsl"

// blend, tonemap, color grade

uniform sampler2D luminance; // float level(11)
uniform sampler2D bloom_big; // vec3
uniform sampler2D bloom_medium; // vec3
uniform sampler2D bloom_small; // vec3
uniform sampler2D dof; // vec3

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
	vec3 color = textureLod(dof, tc, 0.).rgb;

	// bloom
	vec3 bloom = vec3(0.);
	bloom += textureLod(bloom_big, tc, 0.).rgb;
	bloom += textureLod(bloom_medium, tc, 0.).rgb;
	bloom += textureLod(bloom_small, tc, 0.).rgb;
	bloom /= 3.;
	float bloom_amount = .1;
	color = mix(color, bloom, bloom_amount);

	// tonemap
	float avgLuminance = exp(textureLod(luminance, vec2(.5), 0.).r);
	float key = .5;
	color *= key / avgLuminance;
	color = tonemap(color)/tonemap(vec3(W));

	// color grade
	vec3 lift = vec3(0.);
	vec3 gamma = vec3(1.);
	vec3 gain = vec3(1.);
	color = gain * (color + lift * pow(1. - color, 1./gamma));

	out_color = color;
}
