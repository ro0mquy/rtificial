#ifndef final_H
#define final_H
const char final_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 f;layout(location=0)in vec2 x;
#line 1
#line 4
layout(binding=14)uniform sampler2D v;layout(location=0)out vec3 n;layout(location=5)uniform float y;layout(location=7)uniform float t;layout(location=8)uniform float m;layout(location=9)uniform float l;layout(location=10)uniform float s;layout(location=11)uniform float z;vec2 d(float y,float f,float v,vec2 x){x=x*2.-1.;float n=y*y*(x.x*x.x)+x.y*x.y,d=y*y+1.,g=f+v*sqrt(2.),s=(1.+n*g)/(1.+d*g);return s*x*.5+.5;}float d(float y,vec2 x){float n=1.-y;x.y*=-1.;return n+y*16.*x.x*x.y*(1.-x.x)*(-1.-x.y);}float d(vec2 x){return fract(sin(dot(x.xy,vec2(12.9898,78.233)))*43758.5);}vec2 r(float x){return x*=6.28319,vec2(cos(x),sin(x));}vec2 o(vec2 x){return x*x*x*(x*(x*6.-15.)+10.);}float c(vec2 x){vec2 v=fract(x),n=floor(x),g=vec2(0.,1.);vec4 f=vec4(dot(r(d(n+g.xx)),v-g.xx),dot(r(d(n+g.xy)),v-g.xy),dot(r(d(n+g.yx)),v-g.yx),dot(r(d(n+g.yy)),v-g.yy));vec2 s=o(v);return mix(mix(f[0],f[2],s.x),mix(f[1],f[3],s.x),s.y);}float g(vec2 x){return(c(x)+c(x*2.)*.5+c(x*4.)*.25)/1.75;}void main(){vec3 r;float m=t,o=m,c=f.x/f.y;r.x=textureLod(v,d(c,m*1.34,o,x),0.).x;r.y=textureLod(v,d(c,m*1.2,o,x),0.).y;r.z=textureLod(v,d(c,m,o,x),0.).z;r*=d(l,gl_FragCoord.xy/f);float a=radians(10.);n=r+z*vec3(g(vec2(1./s*gl_FragCoord.xy+31.*y)),g(vec2(1./s*gl_FragCoord.xy+33.*y)),g(vec2(1./s*gl_FragCoord.xy+32.*y)));})shader_source";
#endif
