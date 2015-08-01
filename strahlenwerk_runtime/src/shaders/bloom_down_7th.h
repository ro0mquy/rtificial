#ifndef bloom_down_7th_H
#define bloom_down_7th_H
const char bloom_down_7th_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(in vec2 f;layout(location=0)uniform vec2 z;layout(location=66)uniform float m;
)shader_source"
R"shader_source(#line 2"helper"
)shader_source"
R"shader_source(const float v=3.14159,n=6.28319,s=2.71828,d=1.61803,b=1000.,c=1./0.;float p(vec2 f){return min(f.x,f.y);}float x(vec3 f){return min(min(f.x,f.y),f.z);}float w(vec2 f){return max(f.x,f.y);}float t(vec3 f){return max(max(f.x,f.y),f.z);}float p(float v,float f,float m){return min(min(v,f),m);}float p(float v,float f,float z,float m){return min(min(min(v,f),z),m);}float t(float v,float f,float m){return max(max(v,f),m);}float t(float v,float f,float z,float m){return max(max(max(v,f),z),m);}float e(vec2 f){return f.x+f.y;}float a(vec3 f){return f.x+f.y+f.z;}float r(vec2 f){return f.x*f.y;}float o(vec3 f){return f.x*f.y*f.z;}float l(float f){return f*f;}vec2 u(vec2 f){return f*f;}vec3 y(vec3 f){return f*f;}float i(float f){return 1./f;}vec2 L(vec2 f){return 1./f;}vec3 D(vec3 f){return 1./f;}float D(vec2 f,float v){return pow(a(pow(abs(f),vec2(v))),1./v);}float L(vec3 f,float v){return pow(a(pow(abs(f),vec3(v))),1./v);}float h(float f){return clamp(f,0.,1.);}vec2 g(vec2 f){return clamp(f,0.,1.);}vec3 Z(vec3 f){return clamp(f,0.,1.);}float D(float f,float v,float m){return clamp((m-f)/(v-f),0.,1.);}float L(float f,float v,float m){float p=clamp((m-f)/(v-f),0.,1.);return p*p*p*(p*(p*6.-15.)+10.);}float Z(vec2 f,vec2 v){return max(0.,dot(f,v));}float a(vec3 f,vec3 v){return max(0.,dot(f,v));}float Y(float f){return f<0.?-1.:1.;}vec2 X(vec2 f){return vec2(f.x<0.?-1.:1.,f.y<0.?-1.:1.);}vec3 W(vec3 f){return vec3(f.x<0.?-1.:1.,f.y<0.?-1.:1.,f.z<0.?-1.:1.);}vec2 V(float f){return vec2(cos(f),sin(f));}vec3 V(float f,float v){float z=cos(v),m=sin(f),p=sin(v),x=cos(f);return vec3(p*x,z,p*m);}float V(float f,float v,float m){if(m>f)return m;float p=2.*v-f,z=2.*f-3.*v,x=m/f;return(p*x+z)*x*x+v;}float W(float f,float v){float m=f*v;return m*exp(1.-m);}float W(float f,float v,float m){return m=abs(m-f),m/=v,1.-smoothstep(0.,1.,m);}float X(float f,float v,float m){return exp(-f*pow(m,v));}float X(float f,float m){return pow(4.*m*(1.-m),f);}float Y(float f,float v,float m){float p=pow(f+v,f+v)/(pow(f,f)*pow(v,v));return p*pow(m,f)*pow(1.-m,v);}float Z(float f,float v,float m){return pow(m,f)*pow(1.-m,v);}float U(vec3 f){return dot(f,vec3(.2126,.7152,.0722));}vec3 T(vec3 f){vec4 v=vec4(0.,-1./3.,2./3.,-1.),m=mix(vec4(f.zy,v.wz),vec4(f.yz,v.xy),step(f.z,f.y)),p=mix(vec4(m.xyw,f.x),vec4(f.x,m.yzx),step(m.x,f.x));float x=p.x-min(p.w,p.y),z=1e-10;return vec3(abs(p.z+(p.w-p.y)/(6.*x+z)),x/(p.x+z),p.x);}vec3 S(vec3 f){vec4 v=vec4(1.,2./3.,1./3.,3.);vec3 m=abs(fract(f.xxx+v.xyz)*6.-v.www);return f.z*mix(v.xxx,clamp(m-v.xxx,0.,1.),f.y);}vec3 D(vec3 f,vec3 v,vec3 m,vec3 p){float z=0.,x=1/(1+U(f)),n=1/(1+U(v)),s=1/(1+U(m)),y=1/(1+U(p));return(f*x+v*n+m*s+p*y)/(x+n+s+y);}vec3 D(vec3 f,vec3 v,vec3 m,vec3 z,bool p){if(p)return D(f,v,m,z);else return.25*f+.25*v+.25*m+.25*z;}vec3 L(sampler2D f,vec2 v,vec2 m,bool z){vec3 p=textureLod(f,v-2.*m,0.).xyz,x=textureLod(f,v+vec2(2.,-2.)*m,0.).xyz,s=textureLod(f,v+vec2(-2.,2.)*m,0.).xyz,n=textureLod(f,v+2.*m,0.).xyz,d=textureLod(f,v-vec2(0.,1.)*m,0.).xyz,b=textureLod(f,v+vec2(1.,0.)*m,0.).xyz,c=textureLod(f,v-vec2(1.,0.)*m,0.).xyz,y=textureLod(f,v+vec2(0.,1.)*m,0.).xyz,w=textureLod(f,v,0.).xyz,a=textureLod(f,v-m,0.).xyz,g=textureLod(f,v+vec2(1.,-1.)*m,0.).xyz,R=textureLod(f,v+vec2(-1.,1.)*m,0.).xyz,Q=textureLod(f,v+m,0.).xyz,e=D(a,g,R,Q,z),t=D(p,d,c,w,z),r=D(x,d,b,w,z),o=D(s,y,c,w,z),l=D(n,y,b,w,z);return e*.5+.125*t+.125*r+.125*o+.125*l;}
)shader_source"
R"shader_source(#line 5
)shader_source"
R"shader_source(layout(binding=19)uniform sampler2D R;layout(location=0)out vec3 Q;void main(){Q=L(R,f,.5/z,false);}
)shader_source";
#endif
