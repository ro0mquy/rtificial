#ifndef tonemap_H
#define tonemap_H
const char tonemap_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 23) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 74) uniform float post_tonemap_exposure;
)shader_source"
R"shader_source(layout(location = 75) uniform vec3 post_color_lift; // color
)shader_source"
R"shader_source(layout(location = 76) uniform vec3 post_color_gamma; // color
)shader_source"
R"shader_source(layout(location = 77) uniform vec3 post_color_gain; // color
)shader_source"
R"shader_source(
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
R"shader_source(	// + 1. for (filmic) bias
)shader_source"
R"shader_source(	out_color = texture2D(color, tc).rgb * exp2(post_tonemap_exposure + 1.);
)shader_source"
R"shader_source(	out_color = tonemap(out_color)/tonemap(vec3(W));
)shader_source"
R"shader_source(	out_color = clamp(out_color, 0., 1.);
)shader_source"
R"shader_source(	out_color = pow(max(vec3(0.), post_color_gain * 2. * (out_color + (2. * post_color_lift - 1.) * (1. - out_color))), 1./max(post_color_gamma * 2., 1e-6));
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
