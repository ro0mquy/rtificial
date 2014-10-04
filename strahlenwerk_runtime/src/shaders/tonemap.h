#ifndef tonemap_H
#define tonemap_H
const char tonemap_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) in vec2 tc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 1
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 12) uniform sampler2D luminance; // float level(11)
)shader_source"
R"shader_source(layout(location = 45) uniform float key;
)shader_source"
R"shader_source(layout(location = 46) uniform vec3 lift; // color
)shader_source"
R"shader_source(layout(location = 47) uniform vec3 gamma; // color
)shader_source"
R"shader_source(layout(location = 48) uniform vec3 gain; // color
)shader_source"
R"shader_source(layout(binding = 11) uniform sampler2D blended_color; // vec3
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(float A = 0.15;
)shader_source"
R"shader_source(float B = 0.50;
)shader_source"
R"shader_source(float C = 0.10;
)shader_source"
R"shader_source(float D = 0.20;
)shader_source"
R"shader_source(float E = 0.02;
)shader_source"
R"shader_source(float F = 0.30;
)shader_source"
R"shader_source(float W = 11.2;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 tonemap(vec3 color) {
)shader_source"
R"shader_source(   return ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	vec3 color = textureLod(blended_color, tc, 0.).rgb;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// tonemap
)shader_source"
R"shader_source(	float avgLuminance = exp(textureLod(luminance, tc, 0.).r);
)shader_source"
R"shader_source(	color *= key / avgLuminance;
)shader_source"
R"shader_source(	color = tonemap(color)/tonemap(vec3(W));
)shader_source"
R"shader_source(	color = clamp(color, 0., 1.);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// color grade
)shader_source"
R"shader_source(	color = gain * (color + lift * pow(1. - color, 1./gamma));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	out_color = color;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
