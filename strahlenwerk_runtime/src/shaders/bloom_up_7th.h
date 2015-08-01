#ifndef bloom_up_7th_H
#define bloom_up_7th_H
const char bloom_up_7th_source[] =#version 430
in vec2 v;layout((location=0))uniform vec2 f;layout((location=66))uniform float m;layout((location=78))uniform float t;vec3 d(sampler2D l,vec2 v,vec2 f){vec3 n=vec3(610.,550.,440.)/440.,m=.25*textureLod(l,v,0.).xyz;for(int d=0;d<3;d++){vec2 s=f*t*n[d];m[d]+=.125*textureLod(l,v-vec2(0.,1.)*s,0.)[d];m[d]+=.125*textureLod(l,v+vec2(0.,1.)*s,0.)[d];m[d]+=.125*textureLod(l,v-vec2(1.,0.)*s,0.)[d];m[d]+=.125*textureLod(l,v+vec2(1.,0.)*s,0.)[d];m[d]+=.0625*textureLod(l,v-s,0.)[d];m[d]+=.0625*textureLod(l,v+vec2(1.,-1.)*s,0.)[d];m[d]+=.0625*textureLod(l,v+vec2(-1.,1.)*s,0.)[d];m[d]+=.0625*textureLod(l,v+s,0.)[d];}return m;}
#line 5
layout((binding=20))uniform sampler2D l;layout((location=0))out vec3 n;void main(){n=d(l,v,1./f);};
#endif
