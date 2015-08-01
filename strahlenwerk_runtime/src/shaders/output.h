#ifndef output_H
#define output_H
const char output_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(layout(binding=29)uniform sampler2D v;in vec2 m;layout(location=0)out vec3 n;void main(){n=textureLod(v,m,0.).xyz;}
)shader_source";
#endif
