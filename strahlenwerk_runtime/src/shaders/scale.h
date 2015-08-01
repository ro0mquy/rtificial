#ifndef scale_H
#define scale_H
const char scale_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 5) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(layout(binding = 6) uniform sampler2D depth; // float
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(layout(location = 1) out float out_depth;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	out_color = texture(color, tc, 0.).rgb;
)shader_source"
R"shader_source(	out_depth = textureLod(depth, tc, 0.).r;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
