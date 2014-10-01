#ifndef blend_H
#define blend_H
const char blend_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 s;layout(location=0)in vec2 z;
#line 1
#line 4
layout(binding=10)uniform sampler2D l;layout(binding=9)uniform sampler2D b;layout(binding=8)uniform sampler2D v;layout(binding=7)uniform sampler2D m;layout(binding=0)uniform sampler2D f;layout(location=1)uniform float a;layout(location=0)out vec3 n;layout(location=1)out float g;void main(){vec4 s=textureLod(m,z,0.);vec3 m=textureLod(f,z,0.).xyz;m=mix(m,s.xyz,smoothstep(0.,1.,abs(s.w)-.5));vec3 o=vec3(0.);o+=textureLod(l,z,0.).xyz;o+=textureLod(b,z,0.).xyz;o+=textureLod(v,z,0.).xyz;o/=3.;m=mix(m,o,a*.1);n=m;g=log(max(dot(m,vec3(.2126,.7152,.0722)),1e-06));})shader_source";
#endif
