#ifndef syntakischerspalt_H
#define syntakischerspalt_H
const char syntakischerspalt_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 y;layout(location=0)out vec3 f;layout(location=1)out float v;layout(location=2)out float r;layout(location=17)uniform float m;layout(location=5)uniform float c;
#define FOCAL_LENGTH
layout(location=1)uniform float d;
#line 1
layout(location=2)uniform vec3 x;layout(location=3)uniform vec4 z;layout(location=4)uniform float l;
#ifndef FOCAL_LENGTH
layout(location=5)uniform float w;
#endif
float n=6.28319;struct Material{vec3 color;float roughness;float metallic;};struct SphereLight{vec3 center;vec3 color;float radius;float intensity;};
#line 4
layout(location=29)uniform vec3 s;layout(location=30)uniform float i;layout(location=31)uniform float M;layout(location=32)uniform float S;layout(location=33)uniform float F;layout(location=34)uniform float e;Material a[2]=Material[2](Material(vec3(1.),.5,0.),Material(s,.7,1.));
#define MATERIAL_ID_BOUNDING 0.
#define MATERIAL_ID_SYNAPSE 1.
void t(vec3 x,float l){float z=z,n=w,m=d;r=(l-z)/l*(n*n)/(m*(z-n))/.03*y.x;f=x;v=log(max(dot(x,vec3(.2126,.7152,.0722)),1e-06));}vec3 p(vec3 x,vec4 v){vec3 f=2*cross(v.xyz,x);return x+v.w*f+cross(v.xyz,f);}vec3 p(){vec3 v=normalize(vec3((gl_FragCoord.xy-.5*y)/y.x,-w/.03));return p(v,z);}float h(vec3 v,float y){return length(v)-y;}float h(vec3 v,vec3 x,vec3 f,float y){vec3 m=v-x,z=f-x;float n=clamp(dot(m,z)/dot(z,z),0.,1.);return length(m-z*n)-y;}float g(vec3 v,vec2 y){vec2 f=vec2(length(v.xz)-y.x,v.y);return length(f)-y.y;}float o(vec3 v,vec3 x){return v=abs(v)-x,max(v.x,max(v.y,v.z));}float u(vec2 v,vec2 x){return v=abs(v)-x,max(v.x,v.y);}float A(vec3 v,vec3 y){vec3 f=abs(v)-y;return min(max(f.x,max(f.y,f.z)),0.)+length(max(f,0.));}float A(vec3 v,vec3 f,float y){return A(v,f-y)-y;}float L(vec3 v,vec2 x){float f=length(v.xy);return dot(x,vec2(f,v.z));}float b(vec3 v,vec3 f){return dot(v,f.xyz);}vec2 I(vec2 v,vec2 x){return mix(v,x,float(v.x>x.x));}float I(float v,float f,float x){float y=clamp(.5+.5*(f-v)/x,0.,1.);return mix(f,v,y)-x*y*(1.-y);}vec2 L(vec2 v,vec2 f,float y){return vec2(I(v,f,y),v.x>f.x?f.y:v.y);}vec2 b(vec2 v,vec2 f,float x){float y=clamp(.5+.5*(f-v)/x,0.,1.);return vec2(mix(f.x,v.x,y)-x*y*(1.-y),mix(f.y,v.y,y));}float g(float v,float f,float x){float y=clamp(.5-.5*(f-v)/x,0.,1.);return mix(f,v,y)+x*y*(1.-y);}float N(vec3 v,vec3 y){return max(0.,dot(v,y));}float A(float y){return y*y;}vec3 A(vec3 v,vec3 f,vec3 y,Material x,SphereLight m){vec3 n=m.center-v,z=reflect(-y,f),l=dot(n,z)*z-n,d=n+l*clamp(m.radius/length(l),0.,1.),s=normalize(d);float w=distance(v,m.center),c=acos(-1.);vec3 M=.5*(s+y);float r=dot(f,M),S=N(f,s),W=dot(f,y),o=A(x.roughness),U=A(o),i=A(x.roughness+1.),a=U/(4.*mix(S,1.,i)*mix(W,1.,i)*c*A(A(r)*(U-1.)+1.)),e=clamp(o+m.radius*.5/w,0.,1.),F=U/(e*e),E=dot(y,M),k=exp2((-5.55473*E-6.98316)*E),G=.04+.96*k;vec3 p=x.color+(1.-x.color)*k,C=.5*(x.color/c+a*G),t=a*p;float H=A(1.-A(A(w/m.radius))),h=clamp(H,0.,1.)/(A(w)+1.);return S*mix(C,t,x.metallic)*h*m.intensity*m.color*F;}vec3 E(vec3 x,float y){return x*y;}vec3 T(vec3 v,vec3 y){return mod(v,y)-.5*y;}vec3 A(vec3 v,float f,float x,float y){return T(v,vec3(f,x,y));}vec3 D(vec3 v,vec3 y){return v-y;}vec3 D(vec3 v,float f,float x,float y){return D(v,vec3(f,x,y));}mat3 D(float v){return mat3(1.,0.,0.,0.,cos(v),sin(v),0.,-sin(v),cos(v));}mat3 E(float v){return mat3(cos(v),0.,-sin(v),0.,1.,0.,sin(v),0.,cos(v));}mat3 I(float v){return mat3(cos(v),sin(v),0.,-sin(v),cos(v),0.,0.,0.,1.);}mat2 L(float v){return mat2(cos(v),-sin(v),sin(v),cos(v));}float R(float v,float y){const float x=v*y;return x*exp(1.-x);}float D(float y,float v,float f){return clamp((f-y)/(v-y),0.,1.);}float N(float v){return v-floor(v*(1./289.))*289.;}vec2 R(vec2 v){return v-floor(v*(1./289.))*289.;}vec3 T(vec3 v){return v-floor(v*(1./289.))*289.;}vec4 b(vec4 v){return v-floor(v*(1./289.))*289.;}float g(float v){return b((v*34.+1.)*v);}vec2 h(vec2 v){return b((v*34.+1.)*v);}vec3 o(vec3 v){return b((v*34.+1.)*v);}vec4 p(vec4 v){return b((v*34.+1.)*v);}vec4 t(vec4 y){return 1.79284-.853735*y;}vec2 u(vec2 v){return v*v*v*(v*(v*6.-15.)+10.);}vec3 O(vec3 v){return v*v*v*(v*(v*6.-15.)+10.);}float G(vec2 v){vec4 f=floor(v.xyxy)+vec4(0.,0.,1.,1.),m=fract(v.xyxy)-vec4(0.,0.,1.,1.);f=b(f);vec4 y=f.xzxz,x=f.yyww,n=m.xzxz,d=m.yyww,l=p(p(y)+x),z=fract(l*(1./41.))*2.-1.,s=abs(z)-.5,c=floor(z+.5);z=z-c;vec2 r=vec2(z.x,s.x),w=vec2(z.y,s.y),M=vec2(z.z,s.z),i=vec2(z.w,s.w);vec4 U=t(vec4(dot(r,r),dot(M,M),dot(w,w),dot(i,i)));r*=U.x;M*=U.y;w*=U.z;i*=U.w;float S=dot(r,vec2(n.x,d.x)),W=dot(w,vec2(n.y,d.y)),o=dot(M,vec2(n.z,d.z)),E=dot(i,vec2(n.w,d.w));vec2 F=O(m.xy),C=mix(vec2(S,o),vec2(W,E),F.x);float e=mix(C.x,C.y,F.y);return 2.3*e;}float C(vec3 v){vec3 f=floor(v),y=f+vec3(1.);f=b(f);y=b(y);vec3 x=fract(v),z=x-vec3(1.);vec4 n=vec4(f.x,y.x,f.x,y.x),d=vec4(f.yy,y.yy),m=f.zzzz,s=y.zzzz,l=p(p(n)+d),U=p(l+m),c=p(l+s),r=U*(1./7.),M=fract(floor(r)*(1./7.))-.5;r=fract(r);vec4 w=vec4(.5)-abs(r)-abs(M),a=step(w,vec4(0.));r-=a*(step(0.,r)-.5);M-=a*(step(0.,M)-.5);vec4 i=c*(1./7.),o=fract(floor(i)*(1./7.))-.5;i=fract(i);vec4 S=vec4(.5)-abs(i)-abs(o),W=step(S,vec4(0.));i-=W*(step(0.,i)-.5);o-=W*(step(0.,o)-.5);vec3 C=vec3(r.x,M.x,w.x),E=vec3(r.y,M.y,w.y),e=vec3(r.z,M.z,w.z),F=vec3(r.w,M.w,w.w),H=vec3(i.x,o.x,S.x),G=vec3(i.y,o.y,S.y),h=vec3(i.z,o.z,S.z),k=vec3(i.w,o.w,S.w);vec4 A=t(vec4(dot(C,C),dot(e,e),dot(E,E),dot(F,F)));C*=A.x;e*=A.y;E*=A.z;F*=A.w;vec4 I=t(vec4(dot(H,H),dot(h,h),dot(G,G),dot(k,k)));H*=I.x;h*=I.y;G*=I.z;k*=I.w;float g=dot(C,x),u=dot(E,vec3(z.x,x.yz)),L=dot(e,vec3(x.x,z.y,x.z)),N=dot(F,vec3(z.xy,x.z)),T=dot(H,vec3(x.xy,z.z)),D=dot(G,vec3(z.x,x.y,z.z)),R=dot(h,vec3(x.x,z.yz)),Y=dot(k,z);vec3 P=O(x);vec4 B=mix(vec4(g,u,L,N),vec4(T,D,R,Y),P.z);vec2 Z=mix(B.xy,B.zw,P.y);float X=mix(Z.x,Z.y,P.x);return 2.2*X;}float Y(vec2 v){vec2 y=b(v),f=p(y);return fract(p(f.x*f.y+y.x+y.y)/41.)*2.-1.;}float U(vec3 v){vec3 y=b(v),f=p(y);return fract(p(f.x*f.y*f.z+y.x+y.y+y.z)/41.)*2.-1.;}float P(vec2 v){vec2 f=floor(v),y=fract(v),x=vec2(1.,0.);float n=U(f+x.yy),z=U(f+x.yx),w=U(f+x.xy),d=U(f+x.xx);y=O(y);return mix(mix(n,w,y.x),mix(z,d,y.x),y.y);}float H(vec3 v){vec3 f=floor(v),y=fract(v);vec2 x=vec2(1.,0.);float n=U(f+x.yyy),z=U(f+x.yyx),m=U(f+x.yxy),w=U(f+x.yxx),d=U(f+x.xyy),M=U(f+x.xyx),s=U(f+x.xxy),S=U(f+x.xxx);y=O(y);return mix(mix(mix(n,d,y.x),mix(m,s,y.x),y.y),mix(mix(z,M,y.x),mix(w,S,y.x),y.y),y.z);}float B(vec2 v){return(C(v)+C(v*2.)*.5+C(v*4.)*.25)/1.75;}float k(vec3 v){return(C(v)+C(v*2.)*.5+C(v*4.)*.25)/1.75;}float Z(vec2 v){return(H(v)+H(v*2.)*.5+H(v*4.)*.25)/1.75;}float X(vec3 v){return(H(v)+H(v*2.)*.5+H(v*4.)*.25)/1.75;}vec2 W(vec3 v){float f=50.,y=30.;vec3 z=A(v,f,0.,y);z.y=v.y;z.x=abs(z.x);z.z+=5.;z.x-=10*i;float w=h(z,1.);z.x-=2.;float m=h(z,2.);z=I(-n*sin(z.x/5.)/30.)*z;z=E(-n*sin(z.x/2.+l*.2)/50.)*z;float d=h(z,vec3(2.,0.,0.),vec3(20.,0.,0.),mix(M,S,smoothstep(10.*F,10*e,z.x)));vec2 r=vec2(I(g(-w,m,1.),d,1.),MATERIAL_ID_SYNAPSE),s=vec2(-h(v-x,50.),MATERIAL_ID_BOUNDING);return I(r,s);}vec3 B(vec3 v,vec3 f,out int x,int y,float z){float r=0.;for(x=0;x<y;x++){float m=W(v)[0]*z;v+=f*m;m=abs(m);r+=m;if(m<.001*r)break;}return v;}vec3 B(vec3 v,vec3 f,out int y){return B(v,f,y,100,1.);}vec3 V(vec3 v){vec2 y=vec2(.001,0.);return normalize(vec3(W(v+y.xyy)[0]-W(v-y.xyy)[0],W(v+y.yxy)[0]-W(v-y.yxy)[0],W(v+y.yyx)[0]-W(v-y.yyx)[0])+1e-09);}void main(){vec3 v=p();int y;vec3 f=B(x,v,y),z=vec3(0.);if(y<150){int n=int(W(f).y);vec3 m=V(f);Material d=a[n];z=A(f,m,-v,d,SphereLight(vec3(5.,9.,10.),vec3(1.),2.,100.));}t(z,distance(f,x));})shader_source";
#endif
