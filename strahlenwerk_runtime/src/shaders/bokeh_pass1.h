#ifndef bokeh_pass1_H
#define bokeh_pass1_H
const char bokeh_pass1_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 v;layout(location=0)in vec2 w;
#line 1
float n=40.*v.x/1920.;layout(location=1)uniform float l;float c=mix(0.,-radians(60.),(l*10.-.5)/31.5);mat2 m=mat2(cos(c),-sin(c),sin(c),cos(c));float t(vec3 w,float v,float z,float r,inout vec4 l){bool c=v<0.;float n=abs(v);if(n>r&&(c||z>0.&&n<z*2.)){if(c){if(l.w<0.)l.w=min(l.w,v);else{if(-v>l.w)l.w=v;}}else l.w=max(l.w,v);float m=clamp(n-r,0.,1.);l.xyz+=m*w;return m;}return 0.;}
#line 4
layout(binding=0)uniform sampler2D z;layout(binding=2)uniform sampler2D r;layout(location=0)out vec4 e;layout(location=1)out vec4 f;vec4 t(vec3 m,float c,vec2 l){l*=m;vec2 f=1./v;vec4 e=vec4(vec3(0.),c);float i=0.;for(int s=0;s<n;s++){float g=s+.5;vec3 b=textureLod(z,w+g*f*l,0.).xyz;float o=textureLod(r,w+g*f*l,0.).x;i+=t(b,o,c,g,e);}e.xyz=i>1e-06?e.xyz/i:m;return e;}void main(){vec3 l=textureLod(z,w,0.).xyz;float v=textureLod(r,w,0.).x;vec4 m=t(l,v,vec2(0.,-1.)),c=t(l,v,vec2(cos(radians(30.)),sin(radians(30.)))),s;s.xyz=m.xyz+c.xyz;if(c.w>=0.){if(m.w>=0.)s.w=max(c.w,m.w);else s.w=m.w;}else{if(m.w>=0.)s.w=c.w;else s.w=min(c.w,m.w);}e=m;f=s;})shader_source";
#endif
