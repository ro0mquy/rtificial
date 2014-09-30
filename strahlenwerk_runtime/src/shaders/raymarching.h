#ifndef raymarching_H
#define raymarching_H
const char raymarching_source[] = R"shader_source(#version 430
layout(location=0)uniform vec2 y;layout(location=0)out vec3 v;layout(location=1)out float f;layout(location=2)out float r;layout(location=17)uniform float m;layout(location=5)uniform float c;
#define FOCAL_LENGTH
layout(location=1)uniform float d;
#line 1
layout(location=2)uniform vec3 x;layout(location=3)uniform vec4 z;layout(location=4)uniform float i;
#ifndef FOCAL_LENGTH
layout(location=5)uniform float w;
#endif
float n=6.28319;struct Material{vec3 color;float roughness;float metallic;};struct SphereLight{vec3 center;vec3 color;float radius;float intensity;};layout(location=23)uniform vec3 l;layout(location=24)uniform vec3 s;layout(location=25)uniform vec3 F;vec3 g[4]=vec3[4](l,s,vec3(.03,0.,0.),vec3(0.));void t(vec3 x,float m){float z=z,l=w,n=d;r=(m-z)/m*(l*l)/(n*(z-l))/.03*y.x;v=x;f=log(max(dot(x,vec3(.2126,.7152,.0722)),1e-06));}vec3 p(vec3 x,vec4 v){vec3 y=2*cross(v.xyz,x);return x+v.w*y+cross(v.xyz,y);}vec3 p(){vec3 v=normalize(vec3((gl_FragCoord.xy-.5*y)/y.x,-w/.03));return p(v,z);}float h(vec3 v,float y){return length(v)-y;}float h(vec3 v,vec3 x,vec3 f,float y){vec3 m=v-x,z=f-x;float n=clamp(dot(m,z)/dot(z,z),0.,1.);return length(m-z*n)-y;}float e(vec3 v,vec2 y){vec2 x=vec2(length(v.xz)-y.x,v.y);return length(x)-y.y;}float M(vec3 v,vec3 x){return v=abs(v)-x,max(v.x,max(v.y,v.z));}float S(vec2 v,vec2 x){return v=abs(v)-x,max(v.x,v.y);}float o(vec3 v,vec3 y){vec3 f=abs(v)-y;return min(max(f.x,max(f.y,f.z)),0.)+length(max(f,0.));}float M(vec3 v,vec3 x,float y){return o(v,x-y)-y;}float a(vec3 v,vec2 x){float f=length(v.xy);return dot(x,vec2(f,v.z));}float u(vec3 v,vec3 y){return dot(v,y.xyz);}vec2 b(vec2 v,vec2 x){return mix(v,x,float(v.x>x.x));}float S(float v,float x,float y){float f=clamp(.5+.5*(x-v)/y,0.,1.);return mix(x,v,f)-y*f*(1.-f);}vec2 a(vec2 v,vec2 x,float y){return vec2(S(v,x,y),v.x>x.x?x.y:v.y);}vec2 b(vec2 v,vec2 x,float y){float f=clamp(.5+.5*(x-v)/y,0.,1.);return vec2(mix(x.x,v.x,f)-y*f*(1.-f),mix(x.y,v.y,f));}float e(float v,float x,float y){float f=clamp(.5-.5*(x-v)/y,0.,1.);return mix(x,v,f)+y*f*(1.-f);}float L(vec3 v,vec3 y){return max(0.,dot(v,y));}float L(float y){return y*y;}vec3 L(vec3 v,vec3 x,vec3 y,Material f,SphereLight m){vec3 d=m.center-v,z=reflect(-y,x),l=dot(d,z)*z-d,n=d+l*clamp(m.radius/length(l),0.,1.),s=normalize(n);float w=distance(v,m.center),c=acos(-1.);vec3 i=.5*(s+y);float r=dot(x,i),g=L(x,s),R=dot(x,y),h=L(f.roughness),k=L(h),S=L(f.roughness+1.),a=k/(4.*mix(g,1.,S)*mix(R,1.,S)*c*L(L(r)*(k-1.)+1.)),F=clamp(h+m.radius*.5/w,0.,1.),o=k/(F*F),e=dot(y,i),M=exp2((-5.55473*e-6.98316)*e),b=.04+.96*M;vec3 p=f.color+(1.-f.color)*M,E=.5*(f.color/c+a*b),t=a*p;float O=L(1.-L(L(w/m.radius))),u=clamp(O,0.,1.)/(L(w)+1.);return g*mix(E,t,f.metallic)*u*m.intensity*m.color*o;}vec3 C(vec3 x,float y){return x*y;}vec3 T(vec3 v,vec3 y){return mod(v,y)-.5*y;}vec3 C(vec3 v,float x,float f,float y){return T(v,vec3(x,f,y));}vec3 O(vec3 x,vec3 y){return x-y;}vec3 L(vec3 v,float x,float f,float y){return O(v,vec3(x,f,y));}mat3 C(float v){return mat3(1.,0.,0.,0.,cos(v),sin(v),0.,-sin(v),cos(v));}mat3 M(float v){return mat3(cos(v),0.,-sin(v),0.,1.,0.,sin(v),0.,cos(v));}mat3 O(float v){return mat3(cos(v),sin(v),0.,-sin(v),cos(v),0.,0.,0.,1.);}mat2 S(float v){return mat2(cos(v),-sin(v),sin(v),cos(v));}float N(float v,float y){const float x=v*y;return x*exp(1.-x);}float C(float y,float v,float x){return clamp((x-y)/(v-y),0.,1.);}float N(float v){return v-floor(v*(1./289.))*289.;}vec2 T(vec2 v){return v-floor(v*(1./289.))*289.;}vec3 a(vec3 v){return v-floor(v*(1./289.))*289.;}vec4 b(vec4 v){return v-floor(v*(1./289.))*289.;}float e(float v){return b((v*34.+1.)*v);}vec2 h(vec2 v){return b((v*34.+1.)*v);}vec3 o(vec3 v){return b((v*34.+1.)*v);}vec4 p(vec4 v){return b((v*34.+1.)*v);}vec4 t(vec4 y){return 1.79284-.853735*y;}vec2 u(vec2 v){return v*v*v*(v*(v*6.-15.)+10.);}vec3 H(vec3 v){return v*v*v*(v*(v*6.-15.)+10.);}float G(vec2 v){vec4 f=floor(v.xyxy)+vec4(0.,0.,1.,1.),d=fract(v.xyxy)-vec4(0.,0.,1.,1.);f=b(f);vec4 y=f.xzxz,x=f.yyww,m=d.xzxz,n=d.yyww,l=p(p(y)+x),z=fract(l*(1./41.))*2.-1.,s=abs(z)-.5,c=floor(z+.5);z=z-c;vec2 r=vec2(z.x,s.x),w=vec2(z.y,s.y),i=vec2(z.z,s.z),g=vec2(z.w,s.w);vec4 k=t(vec4(dot(r,r),dot(i,i),dot(w,w),dot(g,g)));r*=k.x;i*=k.y;w*=k.z;g*=k.w;float O=dot(r,vec2(m.x,n.x)),R=dot(w,vec2(m.y,n.y)),S=dot(i,vec2(m.z,n.z)),F=dot(g,vec2(m.w,n.w));vec2 o=H(d.xy),h=mix(vec2(O,S),vec2(R,F),o.x);float M=mix(h.x,h.y,o.y);return 2.3*M;}float E(vec3 v){vec3 f=floor(v),y=f+vec3(1.);f=b(f);y=b(y);vec3 x=fract(v),z=x-vec3(1.);vec4 m=vec4(f.x,y.x,f.x,y.x),n=vec4(f.yy,y.yy),s=f.zzzz,d=y.zzzz,l=p(p(m)+n),k=p(l+s),c=p(l+d),r=k*(1./7.),i=fract(floor(r)*(1./7.))-.5;r=fract(r);vec4 w=vec4(.5)-abs(r)-abs(i),a=step(w,vec4(0.));r-=a*(step(0.,r)-.5);i-=a*(step(0.,i)-.5);vec4 g=c*(1./7.),h=fract(floor(g)*(1./7.))-.5;g=fract(g);vec4 R=vec4(.5)-abs(g)-abs(h),S=step(R,vec4(0.));g-=S*(step(0.,g)-.5);h-=S*(step(0.,h)-.5);vec3 F=vec3(r.x,i.x,w.x),o=vec3(r.y,i.y,w.y),O=vec3(r.z,i.z,w.z),M=vec3(r.w,i.w,w.w),E=vec3(g.x,h.x,R.x),e=vec3(g.y,h.y,R.y),u=vec3(g.z,h.z,R.z),Y=vec3(g.w,h.w,R.w);vec4 L=t(vec4(dot(F,F),dot(O,O),dot(o,o),dot(M,M)));F*=L.x;O*=L.y;o*=L.z;M*=L.w;vec4 T=t(vec4(dot(E,E),dot(u,u),dot(e,e),dot(Y,Y)));E*=T.x;u*=T.y;e*=T.z;Y*=T.w;float C=dot(F,x),N=dot(o,vec3(z.x,x.yz)),G=dot(O,vec3(x.x,z.y,x.z)),A=dot(M,vec3(z.xy,x.z)),Z=dot(E,vec3(x.xy,z.z)),X=dot(e,vec3(z.x,x.y,z.z)),W=dot(u,vec3(x.x,z.yz)),V=dot(Y,z);vec3 U=H(x);vec4 Q=mix(vec4(C,N,G,A),vec4(Z,X,W,V),U.z);vec2 P=mix(Q.xy,Q.zw,U.y);float K=mix(P.x,P.y,U.x);return 2.2*K;}float A(vec2 v){vec2 y=b(v),f=p(y);return fract(p(f.x*f.y+y.x+y.y)/41.)*2.-1.;}float k(vec3 v){vec3 y=b(v),f=p(y);return fract(p(f.x*f.y*f.z+y.x+y.y+y.z)/41.)*2.-1.;}float Z(vec2 v){vec2 f=floor(v),y=fract(v),n=vec2(1.,0.);float x=k(f+n.yy),z=k(f+n.yx),w=k(f+n.xy),d=k(f+n.xx);y=H(y);return mix(mix(x,w,y.x),mix(z,d,y.x),y.y);}float Y(vec3 v){vec3 f=floor(v),y=fract(v);vec2 x=vec2(1.,0.);float m=k(f+x.yyy),z=k(f+x.yyx),w=k(f+x.yxy),n=k(f+x.yxx),d=k(f+x.xyy),s=k(f+x.xyx),i=k(f+x.xxy),R=k(f+x.xxx);y=H(y);return mix(mix(mix(m,d,y.x),mix(w,i,y.x),y.y),mix(mix(z,s,y.x),mix(n,R,y.x),y.y),y.z);}float X(vec2 v){return(E(v)+E(v*2.)*.5+E(v*4.)*.25)/1.75;}float W(vec3 v){return(E(v)+E(v*2.)*.5+E(v*4.)*.25)/1.75;}float V(vec2 v){return(Y(v)+Y(v*2.)*.5+Y(v*4.)*.25)/1.75;}float U(vec3 v){return(Y(v)+Y(v*2.)*.5+Y(v*4.)*.25)/1.75;}vec2 R(vec3 v){v.z+=3.;v.x-=1.;vec2 f=vec2(h(v,.7),0.);v.x+=2.;vec2 y=vec2(h(O(v,10*F),.7),1.),z=vec2(v.y+2.,2.),n=vec2(-h(v-x,50.),3.);return b(b(f,y),b(z,n));}vec3 A(vec3 v,vec3 x,out int f,int y,float z){float r=0.;for(f=0;f<y;f++){float m=R(v)[0]*z;v+=x*m;m=abs(m);r+=m;if(m<.001*r)break;}return v;}vec3 A(vec3 v,vec3 x,out int y){return A(v,x,y,100,1.);}vec3 Q(vec3 v){vec2 y=vec2(.001,0.);return normalize(vec3(R(v+y.xyy)[0]-R(v-y.xyy)[0],R(v+y.yxy)[0]-R(v-y.yxy)[0],R(v+y.yyx)[0]-R(v-y.yyx)[0])+1e-09);}void main(){vec3 v=p();int y;vec3 f=A(x,v,y),z=vec3(0.);if(y<150){int m=int(R(f).y);vec3 n=Q(f);Material r;if(m==0.||m==1.)r=Material(g[m],.2,1.);else if(m==2.){float i=2.,w=mod(floor(f.x/i),2.);r=Material(g[m]*w,.5,0.);}else if(m==3.)r=Material(g[m],1.,0.);z=L(f,n,-v,r,SphereLight(vec3(5.,9.,10.),vec3(1.),2.,100.));}t(z,distance(f,x));})shader_source";
#endif