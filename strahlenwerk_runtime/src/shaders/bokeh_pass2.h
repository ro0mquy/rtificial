#ifndef bokeh_pass2_H
#define bokeh_pass2_H
const char bokeh_pass2_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 v;layout(location=0)in vec2 s;
#line 1
float n=40.*v.x/1920.;layout(location=1)uniform float l;float w=mix(0.,-radians(60.),(l*10.-.5)/31.5);mat2 m=mat2(cos(w),-sin(w),sin(w),cos(w));float t(vec3 v,float w,float s,float r,inout vec4 l){bool n=w<0.;float c=abs(w);if(c>r&&(n||s>0.&&c<s*2.)){if(n){if(l.w<0.)l.w=min(l.w,w);else{if(-w>l.w)l.w=w;}}else l.w=max(l.w,w);float m=clamp(c-r,0.,1.);l.xyz+=m*v;return m;}return 0.;}
#line 4
layout(binding=3)uniform sampler2D r;layout(binding=4)uniform sampler2D c;layout(location=0)out vec4 f;vec4 t(sampler2D w,vec3 r,float f,vec2 l){l*=m;vec2 m=1./v;vec4 c=vec4(vec3(0.),f);float z=0.;for(int g=0;g<n;g++){float a=g+.5;vec4 i=textureLod(w,s+a*m*l,0.);z+=t(i.xyz,i.w,f,a,c);}c.xyz=z>1e-06?c.xyz/z:r;return c;}void main(){vec4 l=textureLod(c,s,0.),w=t(r,l.xyz,l.w,vec2(cos(radians(30.)),sin(radians(30.)))),n=t(c,l.xyz*.5,l.w,vec2(cos(radians(150.)),sin(radians(150.))));f=vec4((w.xyz+n.xyz)/3.,max(abs(w.w),abs(n.w)));})shader_source";
#endif
