#version 430

in vec2 tc;

uniform sampler2D color; // vec3
out vec3 out_color;

uniform float post_tonemap_exposure;
uniform vec3 post_color_lift; // color
uniform vec3 post_color_gamma; // color
uniform vec3 post_color_gain; // color

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
	// + 1. for (filmic) bias
	out_color = texture2D(color, tc).rgb * exp2(post_tonemap_exposure + 1.);
	out_color = tonemap(out_color)/tonemap(vec3(W));
	out_color = clamp(out_color, 0., 1.);
	out_color = pow(post_color_gain * 2. * (out_color + (2. * post_color_lift - 1.) * (1. - out_color)), 1./max(post_color_gamma * 2., 1e-6));
}
