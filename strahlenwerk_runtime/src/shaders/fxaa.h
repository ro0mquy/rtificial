#ifndef fxaa_H
#define fxaa_H
const char fxaa_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 v;layout(location=0)in vec2 m;
#line 1
layout(binding=12)uniform sampler2D l;layout(location=0)out vec3 n;void main(){n=textureLod(l,m,0.).xyz;})shader_source";
#endif
