#ifndef fxaa_H
#define fxaa_H
const char fxaa_source[] =R"shader_source(#version 430\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 0) in vec2 tc;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(#line 1\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(binding = 13) uniform sampler2D color; // vec3\n)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(void main() {\n)shader_source"
R"shader_source(	// TODO\n)shader_source"
R"shader_source(	out_color = textureLod(color, tc, 0.).rgb;\n)shader_source"
R"shader_source(}\n)shader_source"
;
#endif
