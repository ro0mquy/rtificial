#include "post.glsl"
#include "helper.glsl"
#line 4

uniform bool post_scanlines;
uniform sampler2D color; // vec3
out vec3 out_color;

layout(binding = 2) uniform sampler2D tex_rt_loves_deadline;
uniform float overlay_offset_x;
uniform float overlay_offset_y;
uniform float overlay_scale;

vec2 emulated_res = res/4.0;

// Hardness of scanline.
//  -8.0 = soft
// -16.0 = medium
float hardScan=-8.0;

// Hardness of pixels in scanline.
// -2.0 = soft
// -4.0 = hard
float hardPix=-2.0;

// Display warp.
// 0.0 = none
// 1.0/8.0 = extreme
vec2 warp=vec2(1.0/32.0,1.0/24.0);

// Amount of shadow mask.
float maskDark=0.5;
float maskLight=1.5;

// Nearest emulated sample given floating point position and texel offset.
// Also zero's off screen.
vec3 Fetch(vec2 pos,vec2 off) {
	pos=floor(pos*emulated_res+off)/emulated_res;
	if(max(abs(pos.x-0.5),abs(pos.y-0.5))>0.5)return vec3(0.0,0.0,0.0);
	return textureLod(color,pos.xy, 0).rgb;
}

// Distance in emulated pixels to nearest texel.
vec2 Dist(vec2 pos) {
	pos=pos*emulated_res;return -((pos-floor(pos))-vec2(0.5));
}

// 1D Gaussian.
float Gaus(float pos,float scale) {
	return exp2(scale*pos*pos);
}

// 3-tap Gaussian filter along horz line.
vec3 Horz3(vec2 pos,float off){
	vec3 b=Fetch(pos,vec2(-1.0,off));
	vec3 c=Fetch(pos,vec2( 0.0,off));
	vec3 d=Fetch(pos,vec2( 1.0,off));
	float dst=Dist(pos).x;
	// Convert distance to weight.
	float scale=hardPix;
	float wb=Gaus(dst-1.0,scale);
	float wc=Gaus(dst+0.0,scale);
	float wd=Gaus(dst+1.0,scale);
	// Return filtered sample.
	return (b*wb+c*wc+d*wd)/(wb+wc+wd);
}

// 5-tap Gaussian filter along horz line.
vec3 Horz5(vec2 pos,float off){
	vec3 a=Fetch(pos,vec2(-2.0,off));
	vec3 b=Fetch(pos,vec2(-1.0,off));
	vec3 c=Fetch(pos,vec2( 0.0,off));
	vec3 d=Fetch(pos,vec2( 1.0,off));
	vec3 e=Fetch(pos,vec2( 2.0,off));
	float dst=Dist(pos).x;
	// Convert distance to weight.
	float scale=hardPix;
	float wa=Gaus(dst-2.0,scale);
	float wb=Gaus(dst-1.0,scale);
	float wc=Gaus(dst+0.0,scale);
	float wd=Gaus(dst+1.0,scale);
	float we=Gaus(dst+2.0,scale);
	// Return filtered sample.
	return (a*wa+b*wb+c*wc+d*wd+e*we)/(wa+wb+wc+wd+we);
}

// Return scanline weight.
float Scan(vec2 pos,float off){
	float dst=Dist(pos).y;
	return Gaus(dst+off,hardScan);
}

// Allow nearest three lines to effect pixel.
vec3 Tri(vec2 pos){
	vec3 a=Horz3(pos,-1.0);
	vec3 b=Horz5(pos, 0.0);
	vec3 c=Horz3(pos, 1.0);
	float wa=Scan(pos,-1.0);
	float wb=Scan(pos, 0.0);
	float wc=Scan(pos, 1.0);
	return a*wa+b*wb+c*wc;
}

// Distortion of scanlines, and end of screen alpha.
vec2 Warp(vec2 pos){
	pos=pos*2.0-1.0;
	pos*=vec2(1.0+(pos.y*pos.y)*warp.x,1.0+(pos.x*pos.x)*warp.y);
	return pos*0.5+0.5;
}

// Shadow mask.
vec3 Mask(vec2 pos){
	pos.x+=pos.y*3.0;
	vec3 mask=vec3(maskDark,maskDark,maskDark);
	pos.x=fract(pos.x/6.0);
	if(pos.x<0.333)mask.r=maskLight;
	else if(pos.x<0.666)mask.g=maskLight;
	else mask.b=maskLight;
	return mask;
}

void main() {
	if(!post_scanlines) {
		out_color = textureLod(color, tc, 0).rgb;
		return;
	}
	//if(gl_FragCoord.x<res.x*0.333) return;
	vec2 pos=Warp(tc);
		//if(gl_FragCoord.x<res.x*0.666) {
		 ////hardScan=-12.0;
		 maskDark=maskLight=1.0;
		 pos=Warp(tc);
		//}
		out_color=Tri(pos)*Mask(gl_FragCoord.xy);
if (overlay_visible_rt_loves_deadline_rt_bool) {
		vec2 tex_coords;
		ivec2 texture_size = textureSize(tex_rt_loves_deadline, 0);
		tex_coords = gl_FragCoord.xy / res.xy;
		tex_coords = Warp(tex_coords);
		tex_coords.y = 1.-tex_coords.y;
		tex_coords.y *= texture_size.x / texture_size.y;
		tex_coords.x -= overlay_offset_x;
		tex_coords.y += overlay_offset_y * (texture_size.x / texture_size.y);
		tex_coords /= overlay_scale;
		vec4 tex_color = texture(tex_rt_loves_deadline, tex_coords);
		out_color = mix(out_color, tex_color.rgb * .5, tex_color.a * .4);
	}
}
