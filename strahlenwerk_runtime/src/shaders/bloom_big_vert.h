#ifndef bloom_big_vert_H
#define bloom_big_vert_H
const char bloom_big_vert_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 v;layout(location=0)in vec2 r;
#line 1
vec3 t(sampler2D v,vec2 r,vec2 z,vec2 t,float n){n*=4.*n/(1920.*t.x);const float m=7*n,x=acos(-1.);const vec3 l=2.65*n*vec3(1.34,1.2,1.);vec3 u=1./sqrt(2.*x)*l,e=exp(-.5/(l*l)),a=e*e;float s=u;vec3 f=textureLod(v,r,0.).xyz*u;for(int c=1;c<=m;c++)u*=e,e*=a,f+=textureLod(v,r-c*z*t,0.).xyz*u,f+=textureLod(v,r+c*z*t,0.).xyz*u,s+=2.*u;return f/s;}layout(binding=0)uniform sampler2D color;layout(location=0)out vec3 n;void main(){n=t(color,r,vec2(0.,1.),1./v,4.);})shader_source";
#endif
