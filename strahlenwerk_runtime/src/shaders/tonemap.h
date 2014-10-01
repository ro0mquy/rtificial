#ifndef tonemap_H
#define tonemap_H
const char tonemap_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 v;layout(location=0)in vec2 e;
#line 1
layout(binding=12)uniform sampler2D luminance;layout(location=12)uniform float s;layout(location=13)uniform vec3 l;layout(location=14)uniform vec3 m;layout(location=15)uniform vec3 t;layout(binding=11)uniform sampler2D b;layout(location=0)out vec3 n;float g=.15,x=.5,o=.1,i=.2,p=.02,z=.3,a=11.2;vec3 d(vec3 l){return(l*(g*l+o*x)+i*p)/(l*(g*l+x)+i*z)-p/z;}void main(){vec3 v=textureLod(b,e,0.).xyz;float g=exp(textureLod(luminance,e,0.).x);v*=s/g;v=d(v)/d(vec3(a));v=clamp(v,0.,1.);v=t*(v+l*pow(1.-v,1./m));n=v;})shader_source";
#endif
