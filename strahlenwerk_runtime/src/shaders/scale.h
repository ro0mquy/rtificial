#ifndef scale_H
#define scale_H
const char scale_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(in vec2 v;layout(location=0)uniform vec2 s;layout(binding=5)uniform sampler2D l;layout(binding=6)uniform sampler2D b;layout(location=0)out vec3 n;layout(location=1)out float g;void main(){n=texture(l,v,0.).xyz,g=textureLod(b,v,0.).x;}
)shader_source";
#endif
