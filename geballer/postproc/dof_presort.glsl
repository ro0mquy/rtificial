#include "post.glsl"
#include "helper.glsl"
#include "dof.glsl"
#include "textureNN.glsl"
#line 6

uniform sampler2D depth; // float
uniform sampler2D closest_depth; // float
out vec3 weights;

// level(1)

vec2 depthCmp2(float depth, float closestDepth) {
	float depth_scale_foreground = 1./2.5;
	float d = depth_scale_foreground * (depth - closestDepth);
	float depthCmpBackground = smoothstep(0, 1, d);
	float depthCmpForeground = 1 - depthCmpBackground;
	return vec2(depthCmpBackground, depthCmpForeground);
}

void main() {
	float centerDepth = textureLod(depth, tc, 0).r;
	float sampleClosestDepth = textureNN(closest_depth, tc).r;
	float sampleCoC = calcCoC(centerDepth);
	vec2 depthCmp = depthCmp2(centerDepth, sampleClosestDepth);
	depthCmp *= sampleAlpha(sampleCoC);
	weights = vec3(sampleCoC, depthCmp.x, depthCmp.y);
}
