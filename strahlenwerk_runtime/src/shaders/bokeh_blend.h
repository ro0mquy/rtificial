#ifndef bokeh_blend_H
#define bokeh_blend_H
const char bokeh_blend_source[] =R"shader_source(#version 430
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
R"shader_source(#line 4
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 11) uniform sampler2D dof; // vec4
)shader_source"
R"shader_source(layout(binding = 7) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	vec4 dofColor = textureLod(dof, tc, 0.);
)shader_source"
R"shader_source(	vec3 color = textureLod(color, tc, 0.).rgb;
)shader_source"
R"shader_source(	out_color = mix(color, dofColor.rgb, smoothstep(0., 1., abs(dofColor.a) - .5));
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
