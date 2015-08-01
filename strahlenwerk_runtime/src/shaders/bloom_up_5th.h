#ifndef bloom_up_5th_H
#define bloom_up_5th_H
const char bloom_up_5th_source[] =#version 430
in vec2 v;layout((location=0))uniform vec2 f;layout((location=66))uniform float s;layout((location=78))uniform float t;vec3 d(sampler2D l,vec2 v,vec2 f){vec3 n=vec3(610.,550.,440.)/440.,s=.25*textureLod(l,v,0.).xyz;for(int g=0;g<3;g++){vec2 z=f*t*n[g];s[g]+=.125*textureLod(l,v-vec2(0.,1.)*z,0.)[g];s[g]+=.125*textureLod(l,v+vec2(0.,1.)*z,0.)[g];s[g]+=.125*textureLod(l,v-vec2(1.,0.)*z,0.)[g];s[g]+=.125*textureLod(l,v+vec2(1.,0.)*z,0.)[g];s[g]+=.0625*textureLod(l,v-z,0.)[g];s[g]+=.0625*textureLod(l,v+vec2(1.,-1.)*z,0.)[g];s[g]+=.0625*textureLod(l,v+vec2(-1.,1.)*z,0.)[g];s[g]+=.0625*textureLod(l,v+z,0.)[g];}return s;}
#line 5
layout((binding=18))uniform sampler2D l;layout((binding=22))uniform sampler2D z;layout((location=0))out vec3 n;void main(){n=d(l,v,1./f)+textureLod(z,v,0.).xyz;};
#endif
