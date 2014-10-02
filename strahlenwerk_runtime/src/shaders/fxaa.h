#ifndef fxaa_H
#define fxaa_H
const char fxaa_source[] =R"shader_source(#version 430)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(location = 0) in vec2 tc;)shader_source"
R"shader_source()shader_source"
R"shader_source(#line 1)shader_source"
R"shader_source()shader_source"
R"shader_source()shader_source"
R"shader_source(layout(binding = 13) uniform sampler2D color; // vec3)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;)shader_source"
R"shader_source()shader_source"
R"shader_source(void main() {)shader_source"
R"shader_source(	// TODO)shader_source"
R"shader_source(	out_color = textureLod(color, tc, 0.).rgb;)shader_source"
R"shader_source(})shader_source"
;
#endif
