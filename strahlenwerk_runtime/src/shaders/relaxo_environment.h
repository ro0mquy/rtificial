#ifndef relaxo_environment_H
#define relaxo_environment_H
const char relaxo_environment_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(vec3 v=vec3(0.,.390731,.920505),e=vec3(12.1101,13.5493,14.499);vec3 p(vec3 l){vec3 f;float y=dot(l,v),n=acos(y),z=l.y,s=y*y,d=sqrt(z);{float m=-1.07844,p=-.157473,i=.660505,o=2.17839,r=-3.63261,c=.451305,t=.323776,a=1.16777,x=.677045,u=exp(r*n),w=(1.+s)/pow(1.+x*x-2.*x*y,1.5);f[0]=(1.+m*exp(p/(z+.01)))*(i+o*u+c*s+t*w+a*d);}{float m=-1.08675,p=-.183093,i=.52153,x=2.98034,r=-4.39396,o=.493945,c=.215704,t=1.93776,a=.666791,u=exp(r*n),w=(1.+s)/pow(1.+a*a-2.*a*y,1.5);f[1]=(1.+m*exp(p/(z+.01)))*(i+x*u+o*s+c*w+t*d);}{float m=-1.09837,p=-.254014,i=.409843,a=-.002073,r=-.408308,x=.449974,o=.111291,c=2.24229,t=.663794,u=exp(r*n),w=(1.+s)/pow(1.+t*t-2.*t*y,1.5);f[2]=(1.+m*exp(p/(z+.01)))*(i+a*u+x*s+o*w+c*d);}return e*f;}
)shader_source"
R"shader_source(#line 4
)shader_source"
R"shader_source(layout(location=0)uniform vec2 m;layout(location=35)uniform vec3 l;layout(location=36)uniform vec3 y;out vec3 f;void main(){vec2 m=-(gl_FragCoord.xy/m-.5);vec3 n=normalize(vec3(m,-.5)),i=cross(l,y),u=mat3(i,y,-l)*n,c=vec3(0.,5.,0.),z=c;for(int w=0;w<100;w++){float a=1000.-length(z);z+=a*u;}if(z.y>1e-06){vec3 r=normalize(z);f.xyz=max(p(r),vec3(0.));float a=cos(radians(4.));f.xyz+=30.*smoothstep(a*.999,a,dot(r,v))*e;}else f.xyz=v.y*e*vec3(.0869,.10363,.08464);}
)shader_source";
#endif
