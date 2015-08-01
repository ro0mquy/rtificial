#ifndef glumanda_H
#define glumanda_H
const char glumanda_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(layout(location=95)uniform float f;layout(location=96)uniform float v;layout(location=97)uniform float y;layout(location=98)uniform float m;layout(location=99)uniform float s;layout(location=100)uniform float i;layout(location=101)uniform float x;layout(location=102)uniform float z;layout(location=103)uniform float e;layout(location=104)uniform float l;layout(location=105)uniform float c;layout(location=106)uniform float d;layout(location=107)uniform float p;layout(location=108)uniform float r;layout(location=109)uniform float a;layout(location=110)uniform float M;layout(location=111)uniform float n;layout(location=112)uniform float t;layout(location=113)uniform float h;layout(location=64)uniform float S;layout(location=65)uniform float w;layout(location=114)uniform float k;layout(location=115)uniform float u;layout(location=116)uniform float o;layout(location=117)uniform float b;layout(location=118)uniform float g;layout(location=119)uniform float q;layout(location=120)uniform float W;layout(location=121)uniform float I;layout(location=122)uniform float F;layout(location=123)uniform float L;layout(location=124)uniform float C;layout(location=125)uniform float D;layout(location=126)uniform float Z;layout(location=127)uniform float Y;layout(location=128)uniform float X;layout(location=129)uniform float V;layout(location=130)uniform float U;layout(location=131)uniform float T;
)shader_source"
R"shader_source(#line 3"fragment"
)shader_source"
R"shader_source(out vec3 R;out float Q;layout(location=0)uniform vec2 P;layout(location=66)uniform float O;
)shader_source"
R"shader_source(#line 2"quat_rotate"
)shader_source"
R"shader_source(vec3 N[12]=vec3[12](vec3(1,1,0),vec3(-1,1,0),vec3(1,-1,0),vec3(-1,-1,0),vec3(1,0,1),vec3(-1,0,1),vec3(1,0,-1),vec3(-1,0,-1),vec3(0,1,1),vec3(0,-1,1),vec3(0,1,-1),vec3(0,-1,-1));
)shader_source"
R"shader_source(#line 4"camera"
)shader_source"
R"shader_source(layout(location=67)uniform vec3 K;layout(location=68)uniform vec4 J;layout(location=69)uniform float H;layout(location=70)uniform float G;float E=H/G;
)shader_source"
R"shader_source(#line 2"helper"
)shader_source"
R"shader_source(const float B=3.14159,A=6.28319,j=2.71828,ab=1.61803,ac=1000.,ad=1./0.;
)shader_source"
R"shader_source(#line 4"sdf/domain"
)shader_source"
R"shader_source(#line 3"sdf/operators"
)shader_source"
R"shader_source(#line 4"material"
)shader_source"
R"shader_source(struct MaterialId{float id;vec3 coord;vec4 misc;};struct MatWrap{float f;MaterialId m;};struct Material{vec3 color;float roughness;float metallic;float height;vec3 emission;};bool ae=false;float af=ac;MaterialId ag=MaterialId(0.,vec3(0.),vec4(0.));
)shader_source"
R"shader_source(#line 5"sdf/distances"
)shader_source"
R"shader_source(#line 4"lighting"
)shader_source"
R"shader_source(layout(binding=0)uniform sampler2D ah;layout(binding=1)uniform samplerCube ai;layout(binding=2)uniform samplerCube aj;layout(binding=3)uniform samplerCube ak;struct SphereLight{vec3 position;float radius;vec3 color;float power;};
)shader_source"
R"shader_source(#line 11"march"
)shader_source"
R"shader_source(layout(location=132)uniform float al;layout(location=133)uniform float am;layout(location=134)uniform vec3 an;layout(location=135)uniform float ao;const float ap=423.;bool aq=true,ar=false,as=false,at=false,au=false,av=false,aw=false,ax=aq,ay=ar;layout(location=136)uniform float az;
)shader_source"
R"shader_source(#line 4
)shader_source"
R"shader_source(layout(location=137)uniform float ba;const float bb=0.,bc=1.,bd=2.,be=3.,bf=4.,bg=5.,bh=6.,bi=7.,bj=8.;void bk(inout vec3 f,vec4 v){vec3 a=2*cross(v.xyz,f);f+=v.w*a+cross(v.xyz,a);}uint bk(uint f){return f=f+2127912214+(f<<12),f=f^-949894596^f>>19,f=f+374761393+(f<<5),f=f+-744332180^f<<9,f=f+-42973499+(f<<3),f=f^-1252372727^f>>16,f;}float bl(uint v){return float(v%(1<<20))/float(1<<20);}float bm(int v){return bl(bk(v));}float bn(ivec2 v){return bl(bk(v.x+bk(v.y)));}float bo(ivec3 v){return bl(bk(v.x+bk(v.y+bk(v.z))));}float bp(float v){int a=int(floor(v));float f=fract(v),i=bo(a),x=bo(a+1);return mix(i,x,f);}float bq(vec2 v){ivec2 a=ivec2(floor(v));vec2 f=fract(v);float d=0,i=bo(a),x=bo(a+ivec2(0,1)),n=bo(a+ivec2(1,0)),y=bo(a+ivec2(1,1));return mix(mix(i,n,f.x),mix(x,y,f.x),f.y)*2.-1.;}float br(vec3 v){ivec3 a=ivec3(floor(v));vec3 f=fract(v);float d=0,i=bo(a),x=bo(a+ivec3(0,1,0)),n=bo(a+ivec3(1,0,0)),y=bo(a+ivec3(1,1,0)),l=bo(a+ivec3(0,0,1)),e=bo(a+ivec3(0,1,1)),z=bo(a+ivec3(1,0,1)),b=bo(a+ivec3(1,1,1));return mix(mix(mix(i,n,f.x),mix(x,y,f.x),f.y),mix(mix(l,z,f.x),mix(e,b,f.x),f.y),f.z)*2.-1.;}float bs(vec2 f){float a=.5*(sqrt(3.)-1.);ivec2 v=ivec2(floor(f+(f.x+f.y)*a));float y=(3.-sqrt(3.))/6.;f-=v-(v.x+v.y)*y;ivec2 d=f.x>f.y?ivec2(1,0):ivec2(0,1);vec2 c[3]=vec2[3](f,f-d+y,f-1.+2.*y);ivec3 i=ivec3(v.x)+ivec3(0,d.x,1),n=ivec3(v.y)+ivec3(0,d.y,1);float r=0;for(uint l=0;l<3;l++){float m=max(0,.5-c[l].x*c[l].x-c[l].y*c[l].y);m*=m;uint b=bk(i[l]+bk(n[l]))%12;r+=m*m*dot(N[b].xy,c[l]);}return 70.*r;}float bt(vec3 f){float a=1./3.,y=1./6.;ivec3 v=ivec3(floor(f+(f.x+f.y+f.z)*a));f-=v-(v.x+v.y+v.z)*y;vec3 x=step(f.yzx,f);ivec3 d=ivec3(min(x,(1.-x).zxy)),n=ivec3(max(x,(1.-x).zxy));vec3 c[4]=vec3[4](f,f-d+y,f-n+2.*y,f-1.+3.*y);int i[4]=int[4](v.x,v.x+d.x,v.x+n.x,v.x+1),l[4]=int[4](v.y,v.y+d.y,v.y+n.y,v.y+1),b[4]=int[4](v.z,v.z+d.z,v.z+n.z,v.z+1);float r=0;for(uint m=0;m<4;m++){float e=max(0.,.6-c[m].x*c[m].x-c[m].y*c[m].y-c[m].z*c[m].z);e*=e;uint z=bk(i[m]+bk(l[m]+bk(b[m])))%12;r+=e*e*dot(N[z],c[m]);}return 32.*r;}float bu(vec2 f){float a=0.;for(int r=0;r<3;r++)a+=exp2(-r)*br(f),f*=2.;return a/1.75;}float bv(vec3 f){float a=0.;for(int r=0;r<3;r++)a+=exp2(-r)*br(f),f*=2.;return a/1.75;}float bw(vec2 f){float a=0.;for(int r=0;r<3;r++)a+=exp2(-r)*bt(f),f*=2.;return a/1.75;}float bx(vec3 f){float a=0.;for(int r=0;r<3;r++)a+=exp2(-r)*bt(f),f*=2.;return a/1.75;}vec3 bl(vec2 v,out float f){vec3 a=vec3((v-.5*P)/P.x,H/G);f=length(a.xz);a=normalize(a);return a;}vec3 bk(){vec2 a=vec2(bt(vec2(O*w,23)),bt(vec2(O*w,283)))*S;vec3 v=bl(gl_FragCoord.xy+a*P,E);v.z=-v.z;bk(v,J);return v;}float by(float f){return.5*f/(E*P.x);}float bz(vec2 f){return min(f.x,f.y);}float ca(vec3 f){return min(min(f.x,f.y),f.z);}float cb(vec2 f){return max(f.x,f.y);}float cc(vec3 f){return max(max(f.x,f.y),f.z);}float bk(float v,float f,float a){return min(min(v,f),a);}float bk(float v,float f,float a,float y){return min(min(min(v,f),a),y);}float bl(float v,float f,float a){return max(max(v,f),a);}float bl(float v,float f,float a,float y){return max(max(max(v,f),a),y);}float cd(vec2 f){return f.x+f.y;}float ce(vec3 f){return f.x+f.y+f.z;}float cf(vec2 f){return f.x*f.y;}float cg(vec3 f){return f.x*f.y*f.z;}float ch(float f){return f*f;}vec2 ci(vec2 f){return f*f;}vec3 cj(vec3 f){return f*f;}float ck(float f){return 1./f;}vec2 cl(vec2 f){return 1./f;}vec3 cm(vec3 f){return 1./f;}float bm(vec2 v,float f){return pow(ce(pow(abs(v),vec2(f))),1./f);}float bn(vec3 v,float f){return pow(ce(pow(abs(v),vec3(f))),1./f);}float cn(float v){return clamp(v,0.,1.);}vec2 co(vec2 v){return clamp(v,0.,1.);}vec3 cp(vec3 v){return clamp(v,0.,1.);}float bm(float f,float v,float a){return clamp((a-f)/(v-f),0.,1.);}float bn(float f,float v,float a){float i=clamp((a-f)/(v-f),0.,1.);return i*i*i*(i*(i*6.-15.)+10.);}float bo(vec2 v,vec2 f){return max(0.,dot(v,f));}float bp(vec3 v,vec3 f){return max(0.,dot(v,f));}float cq(float f){return f<0.?-1.:1.;}vec2 cr(vec2 f){return vec2(f.x<0.?-1.:1.,f.y<0.?-1.:1.);}vec3 cs(vec3 f){return vec3(f.x<0.?-1.:1.,f.y<0.?-1.:1.,f.z<0.?-1.:1.);}vec2 ct(float v){return vec2(cos(v),sin(v));}vec3 bq(float v,float f){float a=cos(f),y=sin(v),i=sin(f),x=cos(v);return vec3(i*x,a,i*y);}float bo(float f,float v,float a){if(a>f)return a;float i=2.*v-f,y=2.*f-3.*v,n=a/f;return(i*n+y)*n*n+v;}float br(float f,float a){float v=f*a;return v*exp(1.-v);}float bp(float f,float v,float a){return a=abs(a-f),a/=v,1.-smoothstep(0.,1.,a);}float bq(float f,float v,float a){return exp(-f*pow(a,v));}float bs(float f,float v){return pow(4.*v*(1.-v),f);}float br(float f,float v,float a){float i=pow(f+v,f+v)/(pow(f,f)*pow(v,v));return i*pow(a,f)*pow(1.-a,v);}float bs(float f,float v,float a){return pow(a,f)*pow(1.-a,v);}float cu(vec3 v){return dot(v,vec3(.2126,.7152,.0722));}vec3 cv(vec3 f){vec4 v=vec4(0.,-1./3.,2./3.,-1.),a=mix(vec4(f.zy,v.wz),vec4(f.yz,v.xy),step(f.z,f.y)),m=mix(vec4(a.xyw,f.x),vec4(f.x,a.yzx),step(a.x,f.x));float d=m.x-min(m.w,m.y),y=1e-10;return vec3(abs(m.z+(m.w-m.y)/(6.*d+y)),d/(m.x+y),m.x);}vec3 cw(vec3 f){vec4 v=vec4(1.,2./3.,1./3.,3.);vec3 a=abs(fract(f.xxx+v.xyz)*6.-v.www);return f.z*mix(v.xxx,clamp(a-v.xxx,0.,1.),f.y);}void bt(inout float f,float a){f-=a;}void bu(inout vec2 f,vec2 a){f-=a;}void bt(inout vec2 v,float f,float a){bm(v,vec2(f,a));}void bv(inout vec3 f,vec3 a){f-=a;}void bm(inout vec3 v,float f,float a,float y){bm(v,vec3(f,a,y));}void bw(inout vec2 f,float v){f*=mat2(cos(v),sin(v),-sin(v),cos(v));}void bx(inout vec3 v,float f){bw(v.yz,f);}void by(inout vec3 v,float f){bw(v.zx,f);}void bz(inout vec3 v,float f){bw(v.xy,f);}float ca(inout float f,float v){f+=.5*v;float a=floor(f/v);f=mod(f,v)-.5*v;return a;}vec2 cb(inout vec2 f,vec2 v){f+=.5*v;vec2 a=floor(f/v);f=mod(f,v)-.5*v;return a;}vec2 bu(inout vec2 v,float f,float a){return bn(v,vec2(f,a));}vec3 cc(inout vec3 f,vec3 v){f+=.5*v;vec3 a=floor(f/v);f=mod(f,v)-.5*v;return a;}vec3 bn(inout vec3 v,float f,float a,float y){return bn(v,vec3(f,a,y));}void cd(inout float f,float v){f*=mod(v,2.)*2.-1.;}void ce(inout vec2 f,vec2 v){f*=mod(v,2.)*2.-1.;}void cf(inout vec3 f,vec3 v){f*=mod(v,2.)*2.-1.;}float cg(inout float f,float v){float a=bn(f,v);f*=mod(a,2.)*2.-1.;return a;}vec2 ch(inout vec2 f,vec2 v){vec2 a=bn(f,v);f*=mod(a,2.)*2.-1.;return a;}vec3 ci(inout vec3 f,vec3 v){vec3 a=bn(f,v);f*=mod(a,2.)*2.-1.;return a;}vec2 cj(inout vec2 f,vec2 v){vec2 a=ci(f,v);f-=.5*v;if(f.x>f.y)f.xy=f.yx;return floor(.5*a);}vec3 ck(inout vec3 c,vec3 f){vec3 a=ci(c,f);c-=.5*f;if(c.x<c.y){if(c.y>c.z){if(c.x<c.z)c=c.xzy;else c=c.zxy;}}else{if(c.z<c.y)c=c.zyx;else{if(c.z<c.x)c=c.yzx;else c=c.yxz;}}return floor(.5*a);}float cl(inout float f,float v){float a=.5*v,i=0.;if(f>a)f+=a,i=floor(f/v),f=mod(f,v)-a;return i;}vec2 cm(inout vec2 f,vec2 v){vec2 a=.5*v,i=vec2(0.);if(f.x>a.x)f.x+=a.x,i.x=floor(f.x/v.x),f.x=mod(f.x,v.x)-a.x;if(f.y>a.y)f.y+=a.y,i.y=floor(f.y/v.y),f.y=mod(f.y,v.y)-a.y;return i;}vec3 cn(inout vec3 f,vec3 v){vec3 a=.5*v,i=vec3(0.);if(f.x>a.x)f.x+=a.x,i.x=floor(f.x/v.x),f.x=mod(f.x,v.x)-a.x;if(f.y>a.y)f.y+=a.y,i.y=floor(f.y/v.y),f.y=mod(f.y,v.y)-a.y;if(f.z>a.z)f.z+=a.z,i.z=floor(f.z/v.z),f.z=mod(f.z,v.z)-a.z;return i;}float bo(inout float f,float v,float a,float m){f+=.5*v;float i=floor(f/v);f=mod(f,v)-.5*v;if(i>m)f+=v*(i-m),i=m;else if(i<a)f+=v*(i-a),i=a;return i;}vec2 bp(inout vec2 f,vec2 v,vec2 a,vec2 m){f+=.5*v;vec2 i=floor(f/v);f=mod(f,v)-.5*v;if(i.x>m.x)f.x+=v.x*(i.x-m.x),i.x=m.x;else if(i.x<a.x)f.x+=v.x*(i.x-a.x),i.x=a.x;if(i.y>m.y)f.y+=v.y*(i.y-m.y),i.y=m.y;else if(i.y<a.y)f.y+=v.y*(i.y-a.y),i.y=a.y;return i;}vec3 bq(inout vec3 f,vec3 v,vec3 a,vec3 m){f+=.5*v;vec3 i=floor(f/v);f=mod(f,v)-.5*v;if(i.x>m.x)f.x+=v.x*(i.x-m.x),i.x=m.x;else if(i.x<a.x)f.x+=v.x*(i.x-a.x),i.x=a.x;if(i.y>m.y)f.y+=v.y*(i.y-m.y),i.y=m.y;else if(i.y<a.y)f.y+=v.y*(i.y-a.y),i.y=a.y;if(i.z>m.z)f.z+=v.z*(i.z-m.z),i.z=m.z;else if(i.z<a.z)f.z+=v.z*(i.z-a.z),i.z=a.z;return i;}float br(inout vec2 f,float v,float a,float m){float y=m,x=A/v,d=bn(y,x),n=length(f);f=n*bq(y);bm(f.x,a);return d;}float bv(inout vec2 f,float v,float a){return br(f,v,a,atan(f.y,f.x));}float bw(inout vec2 f,float v,float a){float i=length(f),d=floor(i/v);i=mod(i,v);f=i*bq(a);return d;}float co(inout vec2 f,float v){return bw(f,v,atan(f.y,f.x));}float cx(inout float f){float a=cs(f);f=abs(f);return a;}vec2 cy(inout vec2 f){vec2 a=cs(f);f=abs(f);return a;}vec3 cz(inout vec3 f){vec3 a=cs(f);f=abs(f);return a;}float cp(inout float f,float v){float a=cz(f);bm(f,v);return a;}vec2 cq(inout vec2 f,vec2 v){vec2 a=cz(f);bm(f,v);return a;}vec3 cr(inout vec3 f,vec3 v){vec3 a=cz(f);bm(f,v);return a;}vec3 cs(inout vec2 f,vec2 v){vec3 c;c.xy=cr(f,v);if(f.y>f.x)c.z=-1.,f.xy=f.yx;else c.z=1.;return c;}vec3 ct(inout vec3 f,vec3 v){f=f.zyx;vec3 a=cr(f,v);if(f.x<f.y){if(f.y>f.z){if(f.x<f.z)f=f.xzy;else f=f.zxy;}}else{if(f.z<f.y)f=f.zyx;else{if(f.z<f.x)f=f.yzx;else f=f.yxz;}}f=f.zyx;return a;}vec2 cu(inout vec2 f,float v){vec2 a=f;cz(a);vec2 i=vec2(0);if(a.y>a.x)f=f.yx,i.x=1;i.y=cr(f.x,v);f.y*=i.y;return i;}vec2 cv(inout vec3 f,float v){vec3 a=f;cz(a);vec2 i=vec2(0);if(a.z>a.x||a.y>a.x){if(a.y>a.z)f=f.yxz,i.x=1;else f=f.zyx,i.x=2;}i.y=cr(f.x,v);f.y*=i.y;return i;}float bx(inout vec3 f,vec3 v,float a){float i=dot(f,v)+a;if(i<0.)f-=2.*i*v;return cs(i);}float by(inout vec3 f,vec3 v,vec3 a){bm(f,v);float d=cp(dot(f,a)/dot(a,a));f-=a*d;bm(f,-v);return d;}void da(inout float f){f=-f;}void db(inout vec2 f){f=-f;}void dc(inout vec3 f){f=-f;}float bz(float f,float v,float a){if(f<a&&v<a){float y=clamp(.5+.5*(v-f)/a,0.,1.),i=mix(v,f,y)-a*y*(1.-y);return i;}float i=min(f,v);return i;}float ca(float f,float v,float a){if(abs(f)<a&&abs(v)<a){float y=clamp(.5-.5*(v-f)/a,0.,1.),i=mix(v,f,y)+a*y*(1.-y);return i;}float d=max(f,v);return d;}float cb(float f,float v,float a){return ca(f,-v,a);}float cc(float f,float v,float a){return bz(f,v,a);}float cd(float f,float v,float a){return ca(f,v,a);}float ce(float f,float v,float a){float y=min(f,v);if(f<a&&v<a){vec2 d=vec2(f,v),i=vec2(a);float m=a-distance(i,d);return min(m,y);}return y;}float cf(float f,float v,float a){float y=max(f,v);if(f>-a&&v>-a){vec2 d=vec2(f,v),i=vec2(-a);float m=distance(i,d)-a;return max(m,y);}return y;}float cg(float f,float v,float a){return cf(f,-v,a);}float ch(float f,float v,float a){float y=min(f,v),i=sqrt(.5)*(f+v-a);return min(i,y);}float ci(float f,float v,float a){float y=max(f,v),i=sqrt(.5)*(f+v+a);return max(i,y);}float cj(float f,float v,float a){return ci(f,-v,a);}float ck(float f,float v,float a){float d=min(f,v),i=sqrt(.5)*(f+v-a);return bz(i,d,.1*a);}float cl(float f,float v,float a){float d=max(f,v),i=sqrt(.5)*(f+v+a);return ca(i,d,.1*a);}float cm(float f,float v,float a){return cl(f,-v,a);}float bs(float f,float v,float a,float i){float y=min(f,v);if(f<2.*a&&v<2.*a){vec2 r=vec2(f,v);float d=sqrt(.5)*a/(i+sqrt(.5)-1.);r.y-=a-d;bw(r,-A/8.);bq(r.x,2.*d,0.,i-1.);float l=length(r)-d;l=min(l,r.y);return min(l,y);}return y;}float bt(float f,float v,float a,float i){float y=max(f,v);if(f>-a&&v>-a){vec2 r=vec2(f,v);float d=sqrt(.5)*a/(i+sqrt(.5)-1.);r.x-=-a;r.y-=-d;bw(r,-A/8.);bq(r.x,2.*d,0.,i-1.);float l=length(r)-d;l=min(l,r.y);return max(l,y);}return y;}float bu(float f,float v,float a,float y){return bt(f,-v,a,y);}float bv(float f,float v,float a,float y){float d=min(f,v);vec2 i=vec2(f,v);float m=a/y*sqrt(.5);i.y-=a-sqrt(.5)*m;i.x-=sqrt(.5)*m;bw(i,-A/8.);bn(i.x,2.*m);i.x=abs(i.x);float n=m*sqrt(.5),r=dot(i,vec2(sqrt(.5)))-n;r=min(r,i.y);return min(r,d);}float bw(float f,float v,float a,float y){return-bv(-f,-v,a,y);}float bx(float f,float v,float a,float y){return bw(f,-v,a,y);}float cw(float f,float v){vec2 a=vec2(f,v);return max(ca(a),0.)-length(min(a,0.));}float cx(float f,float v){vec2 a=vec2(f,v);return min(cc(a),0.)+length(max(a,0.));}float cy(float f,float v){return cx(f,-v);}void cz(float f,MaterialId a){if(ae){if(f<af)af=f,ag=a;}else af=min(af,f);}void dd(MatWrap f){da(f.f,f.m);}MatWrap da(MatWrap f,MatWrap v){return f.f<v.f?f:v;}MatWrap db(MatWrap f,MatWrap v){return f.f>v.f?f:v;}MatWrap dc(MatWrap f,MatWrap v){return v.f=-v.f,dd(f,v);}void dd(float f,MaterialId v){if(ae){if(f>af)af=f,ag=v;}else af=max(af,f);}MaterialId de(float f,vec3 v){return MaterialId(f,v,vec4(0.));}Material de(vec3 f){return Material(f,.5,0,0,vec3(0));}void by(float f,MaterialId v,float a,float y){float i=af,x=sqrt(.5)*(i+f-a);da(f,v);da(x,de(y,vec3(i,f,0.)));}void bk(float f,MaterialId v,float a,float y,float i){float d=af;vec2 r=vec2(d,f);float m=a/y*sqrt(.5);r.y-=a-sqrt(.5)*m;r.x-=sqrt(.5)*m;bw(r,-A/8.);bn(r.x,2.*m);r.x=abs(r.x);float n=m*sqrt(.5),l=dot(r,vec2(sqrt(.5)))-n;l=min(l,r.y);da(f,v);da(l,de(i,vec3(r,0.)));}void bz(MatWrap f,float v,float a,float y){bl(f.f,f.m,v,a,y);}void ca(float f,MaterialId v,float a,float y){float i=af;if(i<a&&f<a){float d=clamp(.5+.5*(f-i)/a,0.,1.),m=mix(f,i,d)-a*d*(1.-d);af=m;ag=de(y,vec3(i,f,d));}da(f,v);}MatWrap cb(MatWrap f,MatWrap v,float a,float y){MatWrap i=da(f,v);float d=sqrt(.5)*(f.f+v.f-a);return da(i,MatWrap(d,de(y,vec3(f.f,v.f,0.))));}MatWrap cc(MatWrap f,MatWrap v,float a,float y){float i=f.f,x=v.f;if(i<a&&x<a){float d=clamp(.5+.5*(x-i)/a,0.,1.),l=mix(x,i,d)-a*d*(1.-d);return MatWrap(l,de(y,vec3(i,x,0.)));}MatWrap d=da(f,v);return d;}MatWrap bl(MatWrap f,MatWrap v,float a,float y,float i){float d=f.f,x=v.f;vec2 r=vec2(d,x);float m=a/y*sqrt(.5);r.y-=a-sqrt(.5)*m;r.x-=sqrt(.5)*m;bw(r,-A/8.);bn(r.x,2.*m);r.x=abs(r.x);float n=m*sqrt(.5),l=dot(r,vec2(sqrt(.5)))-n;l=min(l,r.y);MatWrap b=MatWrap(l,de(i,vec3(r,0.))),e=da(f,v);e=da(e,b);return e;}float df(vec3 f,float v){return length(f)-v;}float dg(vec2 f,float v){return length(f)-v;}float cn(vec3 f,float v,float a){float i=dg(f.xz,v),y=abs(f.y)-a;return cx(i,y);}float co(vec3 f,float v,float a){float i=dg(f.xz,v),y=abs(f.y)-a;return max(i,y);}float df(vec3 f){return min(cc(f),0.)+length(max(f,0.));}float dh(vec3 f,float v){return df(f+v)-v;}float dg(vec3 f){return cc(f);}float dh(vec2 f){return min(cc(f),0.)+length(max(f,0.));}float di(vec2 f,float v){return dh(f+v)-v;}float di(vec2 f){return cc(f);}float dj(vec3 f,vec3 v){vec3 a=abs(f)-v;return min(cc(a),0.)+length(max(a,0.));}float cd(vec3 f,float v,float a,float y){return dk(f,vec3(v,a,y));}float dk(vec3 f,float v){return dk(f,vec3(v));}float cp(vec3 f,vec3 v,float a){return dk(f,v-a)-a;}float cq(vec3 f,float v,float a){return cq(f,vec3(v),a);}float dl(vec3 f,vec3 v){return cc(abs(f)-v);}float ce(vec3 f,float v,float a,float y){return dm(f,vec3(v,a,y));}float dm(vec3 f,float v){return dm(f,vec3(v));}float dn(vec2 f,vec2 v){vec2 a=abs(f)-v;return min(cc(a),0.)+length(max(a,0.));}float cr(vec2 f,float v,float a){return do(f,vec2(v,a));}float do(vec2 f,float v){return do(f,vec2(v));}float cs(vec2 f,vec2 v,float a){return do(f,v-a)-a;}float ct(vec2 f,float v,float a){return ct(f,vec2(v),a);}float dp(vec2 f,vec2 v){return cc(abs(f)-v);}float cu(vec2 f,float v,float a){return dq(f,vec2(v,a));}float dq(vec2 f,float v){return dq(f,vec2(v));}float dr(vec3 f,vec3 v){return dot(f,v);}float cv(vec3 f,float v,float a){return dr(f,bq(v,a));}float ds(vec2 f,vec2 v){return dot(f,v);}float dt(vec2 f,float v){return ds(f,bq(v));}float du(vec2 f,float v){return max(dt(vec2(abs(f.x),f.y),radians(30)),-f.y)-.5*v;}float cw(vec3 f,float v,float a){float i=du(f.xz,v),y=abs(f.y)-a;return cx(i,y);}float cx(vec3 f,float v,float a){float i=du(f.xz,v),y=abs(f.y)-a;return max(i,y);}float dv(vec2 f,float v){float a=v*sqrt(.5);vec2 i=abs(f);float d=ds(i,vec2(sqrt(.5)))-a;return d;}float dw(vec2 f,float v){float a=radians(54.),y=radians(-18.),i=v*cos(A/5./2.);vec2 d=vec2(abs(f.x),f.y);float m=dt(d,a),x=-f.y,n=dt(d,y),c=bl(m,x,n)-i;return c;}float cy(vec3 f,float v,float a){float i=dw(f.xz,v),y=abs(f.y)-a;return cx(i,y);}float cz(vec3 f,float v,float a){float i=dw(f.xz,v),y=abs(f.y)-a;return max(i,y);}float dx(vec2 f,float v){float a=v*cos(A/6./2.);vec2 i=abs(f);float d=dt(i,radians(30.)),y=i.y,n=max(d,y)-a;return n;}float da(vec3 f,float v,float a){float i=dx(f.xz,v),y=abs(f.y)-a;return cx(i,y);}float db(vec3 f,float v,float a){float i=dx(f.xz,v),y=abs(f.y)-a;return max(i,y);}float bk(vec2 f,float v,float a,float m,float y,float i,float d){float x=atan(f.y,f.x),l=length(f),n=m/4.,e=cos(n*x),b=sin(n*x),c=e/v,r=b/a,z=c<0.?-1.:1.,s=r<0.?-1.:1.,M=c*z,A=r*s,t=pow(M,i),p=pow(A,d),B=t/M,R=p/A,u=t+p,S=pow(u,-1./y),g=-i*B*z/v*b,q=d*R*s/a*e,h=n/y*S/u,K=h*h*(g*g+2.*g*q+q*q),o=(l-S)/sqrt(1+K);return o;}float dc(vec3 f,float v,float a){vec2 i=vec2(dg(f.xz,v),f.y);return dg(i,a);}float dy(vec3 f,vec2 v){return dy(f,v.x,v.y);}float dd(vec3 f,float v,float a){vec2 i=vec2(dq(f.xz,v),f.y);return do(i,a);}float de(vec3 f,float v,float a){vec2 i=vec2(dg(f.xz,v),f.y);return do(i,a);}float cf(vec3 f,float v,float a,float y){float d=length(f.xz),i=atan(f.z,f.x);i-=clamp(i,-y,y);f.xz=d*bq(i);f.x-=v;return df(f,a);}float dz(vec3 f,vec2 v){vec2 a=vec2(length(f.xz),f.y);return ds(a,v);}float ea(vec3 f,float v){vec2 a=vec2(length(f.xz),f.y);return dt(a,v);}float cg(vec3 f,float v,float y,float m){float s=v-y,i=2*m,x=sqrt(s*s+i*i);vec2 z=vec2(i/x,s/x);vec3 e=f;bm(e.y,y*z.x/z.y+m);float l=dz(e,z),c=abs(f.y)-m;return max(l,c);}float df(vec3 f,float v,vec3 y){float m=cp(dot(f,y)/dot(y,y));return df(f-y*m,v);}float dg(vec3 f,float v,float y){return f.x-=clamp(f.x,-y,y),df(f,v);}float ch(vec3 f,float v,float y,vec2 m){float s=v*m.x/m.y;f.x=abs(f.x);f.x-=min(y,f.x);f.x-=s;return dz(f.yxz,m);}float ci(vec3 f,float v,float y,float m){return ch(f,v,y,bq(m));}float eb(vec3 f,float v){vec3 y=abs(f);y.y-=v;float m=ds(y.xy,vec2(.816497,.57735)),s=ds(y.zy,vec2(.816497,.57735)),i=max(m,s);return i;}float dh(vec3 f,float v,float y){float m=length(f.xz),s=atan(f.z,f.x);m-=s*v/A;vec2 i=vec2(m,f.y);bn(i.x,v);float x=dg(i,y);return x;}float di(vec2 f,float v,float y){vec2 m=f,s=f;m.x-=v;s.x-=-v;float i=length(m)+length(s);i=i*.5-y;return i;}float dj(vec3 f,float v,float y){vec3 m=f,s=f;m.x-=v;s.x-=-v;float i=length(m)+length(s);i=i*.5-y;return i;}float dk(vec3 f,float v,float y){vec3 m=f;m.xz=abs(m.xz);m.y-=v;float s=dt(m.xy,y),i=dt(m.zy,y),x=max(s,i);return x;}vec3 cj(vec3 f,vec3 v,float y,float m){float s=cp(1-m);return normalize(mix(f,v,s*(sqrt(s)+m)));}vec3 ck(vec3 f,vec3 v,vec3 y,float m){float s=cp(dot(f,v));vec3 i=2.*dot(f,v)*f-v,x=cj(f,i,s,sqrt(m)),z=textureLod(ak,x,sqrt(m)*5.).xyz;vec2 e=textureLod(ah,vec2(m,s),0.).xy;return z*(y*e.x+e.y);}vec3 dl(vec3 f,vec3 v,Material y){vec3 m=textureLod(aj,f,0.).xyz,s=y.color*m+ck(f,v,vec3(.04),y.roughness),i=ck(f,v,y.color,y.roughness);return mix(s,i,y.metallic);}vec3 dm(vec3 f,vec3 v,float y){f.xz-=K.xz;float m=cj(dot(v,f))-dot(f,f)+y*y,s=-dot(v,f)+sqrt(m);return textureLod(ai,normalize(f+s*v),0.).xyz;}vec3 bk(vec3 f,float v,vec3 y,vec3 m,vec3 s,Material i){return i.color=.5*s+.5,dl(s,-y,i);}float dn(vec3 f,vec3 v,SphereLight y){vec3 m=y.position-f;float s=dot(m,m);vec3 i=m*inversesqrt(s);float x=acos(dot(v,i)),z=sqrt(s),e=z/y.radius,l=sqrt(cj(e)-1),c=-l/tan(x),d=0;if(e*cos(x)>1)d=cos(x)/cj(e);else d=1./(B*cj(e))*(cos(x)*acos(c)-l*sin(x)*sqrt(1-c*c))+1/B*atan(sin(x)*sqrt(1-cj(c))/l);d*=B;return d;}vec3 bm(vec3 f,vec3 v,vec3 y,float m,SphereLight s){vec3 i=reflect(-y,v),x=cj(v,i,bp(v,y),m),z=s.position-f,e=dot(z,x)*x-z,l=z+e*cp(s.radius/length(e));return l;}float ec(float f,float v){float y=cj(v),m=(1-y)/y*f;return sqrt(1+m);}float cl(vec3 f,vec3 v,vec3 y,float m){vec3 s=normalize(f+v);float i=bp(y,v),x=bp(y,f),z=bp(y,s),e=cj(m),l=cj(e),c=max(B*cj(cj(z)*(l-1)+1),1e-08);c*=ec(l,x)+ec(l,i);c*=2*i*x;return max(l/c,0);}vec3 do(vec3 f,vec3 v,vec3 y){vec3 m=normalize(f+v);float s=bp(f,m);return y+(1-y)*exp2((-5.55473*s-6.98316)*s);}vec3 bl(vec3 f,float v,vec3 y,vec3 m,vec3 s,Material i,SphereLight x){vec3 z=x.position-m;float e=dot(z,z);z/=sqrt(e);float l=bp(z,s),c=x.power/(4*cj(B)*cj(x.radius));c*=dn(m,s,x);vec3 d=i.color*(max(0,l*c)/B),p=bm(m,s,-y,i.roughness,x);float r=cp(i.roughness+x.radius/(2*sqrt(e))),a=dot(p,p);p/=sqrt(a);float M=cj(i.roughness/r),n=x.power*M/(4*B*a);vec3 t=do(-y,p,i.color);float h=n*l*cl(-y,p,s,i.roughness);vec3 S=do(-y,p,vec3(.04));return mix(d+S*h,h*t,i.metallic);}float dj(vec3 f){return abs(dr(f,normalize(an))-ao);}void bl(){int f=int(am);if(f==0)aq=true,ar=false,as=false,at=false,au=false,av=false,aw=false;else if(f==1)aq=true,ar=true,as=true,at=false,au=false,av=false,aw=false;else if(f==2)aq=false,ar=true,as=true,at=false,au=false,av=false,aw=false;else if(f==3)aq=true,ar=false,as=false,at=false,au=true,av=true,aw=false;else if(f==4)aq=true,ar=true,as=true,at=false,au=true,av=true,aw=false;else if(f==5)aq=false,ar=true,as=true,at=false,au=true,av=true,aw=false;else aq=true,ar=false,as=false,at=false,au=false,av=false,aw=false;ax=aq;ay=ar;}vec3 dp(float f,vec3 v,float y){float m=abs(sin(B*10.*f));m=1.-(1.-smoothstep(8.,15.,y))*(1.-m);float s=abs(sin(B*f));s=1.-(.8+.2*smoothstep(6.,10.,y))*(1.-smoothstep(60.,80.,y))*(1.-s);float i=abs(sin(B/10.*f));i=1.-(.8+.2*smoothstep(30.,50.,y))*(1.-smoothstep(80.,150.,y))*(1.-i);float x=dj(v);vec3 z=vec3(0.),e=vec3(.29804,.18824,.43922),l=vec3(.12549,.52941,.36078),c=vec3(.02095,.19046,.60548),d=mix(e,l,smoothstep(.1*x,x,f));if(f<0.)d=c;d=cv(d);d.y*=1.-smoothstep(x,10.*x,abs(f));d=cw(d);d=mix(z,d,m);d=mix(z,d,s);d=mix(z,d,i);return d;}void dq(vec3 f,inout vec3 v,float y){vec3 m=dFdx(f),s=dFdy(f),i=cross(s,v),x=cross(v,m);float z=dot(m,i),e=dFdx(y),l=dFdy(y);vec3 c=(e*i+l*x)*sign(z),d=normalize(abs(z)*v-c);v=d;}float cm(vec3 f,float v,float y,float m){float s=g+sin(y)*M;s*=v;float i=A/10*y/(5+3.5*sin(O));by(f,i);br(f.xz,U,s,i+m);float x=h*v,z=dk(f,vec3(x,x,v));bm(f.x,x+q*v);float e=cg(f.zxy,x-.05*v,x*p,q*v);return ch(z,e,.05*v);}float ed(vec3 f,float v){float y=g+M;y+=h+q*2;y*=v;float m=max(dg(f.xz,y),-f.y);if(m>1)return m;float s=2*h*v,i=cn(f.y,s),x=atan(f.z,f.x);bm(f.y,s);float z=cm(f,v,i+1,x);bm(f.y,-s);float e=cm(f,v,i,x);bm(f.y,-s);float l=cm(f,v,i-1,x);return min(z,min(e,l));}float dk(vec3 f){vec3 v=f;ct(v.xy,vec2(7));dc(v.x);cz(v.y);bz(v,.1*A);float y=ci(v.z,3),m=a;m+=F*sin(A*ba);by(v,m*A);ci(v.y,3);bz(v,m*A);cr(v.x,.01);float s=v.x;bm(v.x,-.3);float i=dr(v,bq(-.125*A,.2*B).yxz);s=bv(s,i,.6,4);return s;}float ee(vec3 f,float v){vec3 y=f;float m=dq(y.xy,vec2(3,v));vec3 s=y;bm(s.y,-4);float i=dq(s.xy,vec2(.3,4));m=min(m,i);return m;}float ef(vec3 f,float v){vec3 y=f;br(y.xz,10,0.,v);float m=dq(y.zy,.1);return m;}float dl(vec3 f){vec3 v=ct(f.xz,vec2(C));by(f,A*Z);vec3 y=f;float m=.3;y.y-=m;y.z-=X;float s=atan(y.z,y.x);vec3 i=f;float x=ee(i,m);da(x,MaterialId(bj,i,vec4(v,0.)));vec3 z=y;float e=ef(z,s);bl(e,de(bc,z),.1,4,bd);float l=dk(f);da(l,MaterialId(bb,f,vec4(v,0.)));vec3 c=y;float d=bo(int(ci(y.z,2)*cr(y.x,2))),p=1.,r=fract(d+ba/p*(1.+floor(u*p*d))),a=1.-smoothstep(0.,.9,r),M=V+Y*step(.5,mod(ci(y.z,2)+1.,3.));c.x-=o+M*a;float n=1.-smoothstep(.9,1.,r),t=dk(c,W*n);MaterialId h=de(be,c);h.misc.xy=vec2(r,d);da(t,h);vec3 S=y;bm(S.y,.5);float w=cg(S,.4,.3,.5),k=dg(S.xz,I);w=max(w,-k);da(w,de(bf,S));bm(S.y,1+1.5+sin(A*(.1*(ci(y.z,2)+2.*cr(y.x,2))+ba))*.5-2);float b=cg(S,.3,.02,1.5);w=min(w,b);da(b,de(bg,S));vec3 g=y;bm(g.y,.5);float q=cr(g.y,.1),F=.1*A*ba*q;br(g.xz,10,0.,s+F);bm(g.x,.3);bz(g,.3*sin(A*ba));float L=du(g.zx,.2);L=max(L,abs(g.y)-.03);cb(L,de(bh,g),.01,bi);return af;}float eg(vec3 f,bool v){af=ac;ae=v;if(ay)da(dj(f),de(ap,f));if(ax)dl(f);return af;}vec3 eh(vec3 f,float v){vec3 y[6]=vec3[6](vec3(v,0,0),vec3(0,v,0),vec3(0,0,v),vec3(-v,0,0),vec3(0,-v,0),vec3(0,0,-v));float m[6]=float[6](0,0,0,0,0,0);for(int s=0;s<6;s++)m[s]=eg(f+y[s],false);return vec3(m[0]-m[3],m[1]-m[4],m[2]-m[5]);}vec3 ei(vec3 f,float v){return normalize(eh(f,v));}vec3 dm(vec3 f){float v=max(5e-05,by(distance(f,K)));v*=al;return dm(f,v);}vec3 ej(vec3 f,vec3 v){vec3 y=dm(f);if(dot(v,y)>0.){y=normalize(dFdx(y)+y);if(dot(v,y)>0.)y=normalize(dFdy(y)+y);}return y;}vec3 ek(vec3 f,float v){return eh(f,v)/(2.*v);}vec3 dn(vec3 f){float v=max(5e-05,by(distance(f,K)));v*=al;return dn(f,v);}float bk(vec3 f,vec3 v,float y,float m,float s,int i,float x,bool z){float e=y,l=ad,c=y,d=0,p=0,r=cs(eg(f,false));for(int a=0;a<i;++a){float M=r*eg(v*e+f,false),n=abs(M);bool t=x>1&&n+d<p;if(t)p-=x*p,x=1;else p=M*x;d=n;float h=n/e;if(!t&&h<l)c=e,l=h;if(!t&&h<s||e>m){break;}e+=p;}if((e>m||l>s)&&!z)return ad;return c;}float dr(vec3 f,vec3 v,float y){float m=bk(f,v,.001,y,by(1),256,1.2,false);if(isinf(m))return m;for(int s=0;s<3;s++)m+=eg(f+m*v,false)-by(m);return m;}vec3 ds(vec3 f,vec3 v,float y){ax=as;ay=at;float m=eg(f,false);vec3 s=dm(f);ax=aq;ay=ar;vec3 i=dFdx(f),x=dFdy(f);int z=5,e=5;vec3 l=vec3(0);for(int c=0;c<e;c++){for(int d=0;d<z;d++){vec2 p=(vec2(d,c)+.5)/vec2(z,e)-.5;vec3 r=p.x*i+p.y*x;float a=m+dot(s,r);l+=dp(a,v,y);}}return l/float(z*e);}vec3 dt(vec3 f,float v,vec3 y){return ds(y,f,v);}vec3 do(vec3 f){ax=av;ay=aw;vec3 v=dn(f);float y=length(v);ax=aq;ay=ar;vec3 m=vec3(.18014,.74453,.03288),s=vec3(.71547,.03995,.02537),i=vec3(1.);i=mix(i,m,1.-smoothstep(.8,1.,y));i=mix(i,s,smoothstep(1.,1.2,y));return i;}vec3 bm(vec3 f,float v,vec3 y,vec3 m,vec3 s,MaterialId i,Material x){vec3 z=x.emission;return dl(s,-y,x)+z;}vec3 cn(vec3 f,float v,vec3 y,vec3 m){return m;}Material dp(MaterialId p){Material a=de(vec3(1));a.roughness=.1;vec3 M=vec3(63,124,126)/255.,n=vec3(41,40,57)/255.,h=vec3(157,29,8)/255.,S=vec3(214,47,39)/255.,w=vec3(242,110,4)/255.;if(p.id==be){float u=p.misc.x,o=p.misc.y,b=min(1.,u+T);b=br(d,3.,b);float g=2.+floor(3.*o),q=mod(u+o,1./g)*g;q=br(23.,3.,q);float W=b+k*q+D;W/=1.+k+D;W*=1000.*c;a.emission=W*M;}else if(p.id==bb){vec3 u=p.coord,o=p.misc.xyz;float b=bm(l-e,l,u.y);b=pow(b,z);float g=cg(o)*u.z-ba*r*x,q=max(0.,br(23.,3.,fract(g/r)));q*=max(0.,br(23.,3.,fract(g/r/x)));float W=b*q;a.emission=1000.*W*s*M;}else if(p.id==bj){vec3 u=p.coord,o=p.misc.xyz;u.y-=m;float b=cg(o)*u.z-ba*i;b=mod(b,t)/t;b=max(0.,2.*b-1.);b=pow(b,y);float g=step(v,fract(u.y/L));g*=b;a.emission=1000.*g*f*M;}if(p.id==bb)a.color=h;else if(p.id==bc||p.id==bd||p.id==bf||p.id==bg)a.color=n;else if(p.id==bj)a.color=S;else if(p.id==bh||p.id==bi)a.color=n;return a;}void main(){bl();vec3 f=K,v=bk();float y=dr(f,v,az);if(isinf(y))R=dm(f,v,az),Q=az;else{vec3 m=f+y*v;float s=eg(m,true);MaterialId i=ag;vec3 x=ej(m,v);if(i.id==ap){vec3 z=dt(f,y,m);if(au){vec3 e=do(m);z=mix(z,e,.5);}Material e=de(z);e.roughness=1.;R=dl(x,-v,e);}else{if(au){vec3 z=do(m);Material e=de(z);e.roughness=0.;R=dl(x,-v,e);}else{Material z=dp(i);z.roughness=cj(z.roughness);R=bm(f,y,v,m,x,i,z);}}Q=y;}R=cn(f,y,v,R);R*=exp2(b);}
)shader_source";
#endif
