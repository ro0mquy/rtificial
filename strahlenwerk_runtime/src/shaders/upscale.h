#ifndef upscale_H
#define upscale_H
const char upscale_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 5) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(layout(binding = 6) uniform sampler2D coc; // float
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(layout(location = 1) out float out_coc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	vec2 c = tc;
)shader_source"
R"shader_source(	out_color = texture2D(color, c).rgb;
)shader_source"
R"shader_source(	out_coc = texture2D(coc, c).r;
)shader_source"
R"shader_source(	//bar = vec3(tc, 0.);
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
