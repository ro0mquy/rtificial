#ifndef bloom_up_final_H
#define bloom_up_final_H
const char bloom_up_final_source[] =#version 430
layout((location=78))uniform float t;vec3 d(sampler2D l,vec2 v,vec2 f){vec3 n=vec3(610.,550.,440.)/440.,s=.25*textureLod(l,v,0.).xyz;for(int m=0;m<3;m++){vec2 g=f*t*n[m];s[m]+=.125*textureLod(l,v-vec2(0.,1.)*g,0.)[m];s[m]+=.125*textureLod(l,v+vec2(0.,1.)*g,0.)[m];s[m]+=.125*textureLod(l,v-vec2(1.,0.)*g,0.)[m];s[m]+=.125*textureLod(l,v+vec2(1.,0.)*g,0.)[m];s[m]+=.0625*textureLod(l,v-g,0.)[m];s[m]+=.0625*textureLod(l,v+vec2(1.,-1.)*g,0.)[m];s[m]+=.0625*textureLod(l,v+vec2(-1.,1.)*g,0.)[m];s[m]+=.0625*textureLod(l,v+g,0.)[m];}return s;}
#line 5
in vec2 v;layout((location=0))uniform vec2 f;layout((binding=13))uniform sampler2D l;layout((binding=27))uniform sampler2D s;layout((location=0))out vec3 n;layout((location=79))uniform float m;void main(){n=textureLod(l,v,0.).xyz+textureLod(s,v,0.).xyz/7*m;};
#endif
