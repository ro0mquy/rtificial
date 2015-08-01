#ifndef output_H
#define output_H
const char output_source[] =R"shader_source(
)shader_source"
R"shader_source(#version 430
)shader_source"
R"shader_source(layout(binding = 29) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	out_color = textureLod(color, tc, 0.).rgb;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
