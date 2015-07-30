#include "post.glsl"
#include "noise.glsl"
#include "camera.glsl"
#include "helper.glsl"
#line 6

uniform sampler2D depth; // float level(1)
out float out_ao;

// level(1)

int maxStepsPerRay = 10;
float bias = .03;
float falloff = 2;
uniform float post_ao_radius;
uniform float post_ao_n_rays;
uniform float post_ao_intensity;
uniform float post_ao_avg_occlusion;
uniform bool post_disable_ao;

vec2 sampleDirection(int i, float rotation) {
	float phi = Tau / post_ao_n_rays * (i + rotation);
	return vec2(cos(phi), sin(phi));
}

vec3 getViewPosition(vec2 tc) {
	float sampleDepth = textureLod(depth, tc, 0).r;
	float screen_d;
	return sampleDepth * camGetDirectionSS(tc * (res - 1) + .5, screen_d);
}

vec2 snapToTexel(vec2 coord) {
	return coord;
	vec2 maxScreenCoords = res - 1;
	return round(coord * maxScreenCoords) / maxScreenCoords;
}

float getSampleOcclusion(vec2 tc, vec3 centerViewPos, vec3 centerNormal, vec3 tangent, inout float topOcclusion) {
	vec3 sampleViewPos = getViewPosition(tc);

	vec3 horizonVector = sampleViewPos - centerViewPos;
	float horizonVectorLength = length(horizonVector);

	if (dot(tangent, horizonVector) < 0) {
		// "estimate"
		return post_ao_avg_occlusion;
	}

	float occlusion = dot(centerNormal, horizonVector) / horizonVectorLength;
	float diff = max(occlusion - topOcclusion, 0);
	topOcclusion = max(occlusion, topOcclusion);

	float distanceFactor = 1 -  square(saturate(horizonVectorLength / falloff));
	return diff * distanceFactor;
}

float getRayOcclusion(vec2 origin, vec2 direction, float jitter, vec2 projectedRadii, vec3 centerViewPos, vec3 centerNormal, int numStepsPerRay) {
	vec2 texelSizedStep = direction / res;

	vec3 tangent = getViewPosition(origin + texelSizedStep) - centerViewPos;
	tangent -= dot(centerNormal, tangent) * centerNormal;

	vec2 stepUV = snapToTexel(direction * projectedRadii / (numStepsPerRay - 1));

	vec2 jitteredOffset = mix(texelSizedStep, stepUV, jitter);
	vec2 uv = snapToTexel(origin + jitteredOffset);

	float topOcclusion = bias;
	float occlusion = 0;
	for (int i = 0; i < numStepsPerRay; i++) {
		occlusion += getSampleOcclusion(uv, centerViewPos, centerNormal, tangent, topOcclusion);
		uv += stepUV;
	}

	return occlusion;
}

void main() {
	if (post_disable_ao) {
		out_ao = 1;
		return;
	}

	float centerDepth = textureLod(depth, tc, 0).r;
	float screen_d;
	vec3 centerViewPos = centerDepth * camGetDirectionSS(gl_FragCoord.xy, screen_d);
	vec3 centerNormal = normalize(cross(dFdy(centerViewPos), dFdx(centerViewPos)));

	float rotationJitter = valueNoise(gl_FragCoord.xy + 3827.5) * .5;
	float positionJitter = valueNoise(gl_FragCoord.xy + 827.5) * .5 + .5;

	vec2 projectedRadii = post_ao_radius * screen_d / centerDepth * vec2(1, res.y / res.x);
	float screenRadius = projectedRadii.x * res.x;

	float totalOcclusion = 0;
	if (screenRadius >= 1) {
		int numStepsPerRay = min(maxStepsPerRay, int(screenRadius));

		for (int i = 0; i < post_ao_n_rays; i++) {
			vec2 sampleDir = sampleDirection(i, rotationJitter);
			totalOcclusion += getRayOcclusion(tc, sampleDir, positionJitter, projectedRadii, centerViewPos, centerNormal, numStepsPerRay);
		}

		totalOcclusion *= post_ao_intensity / post_ao_n_rays;
	}

	float ao = 1 - saturate(totalOcclusion);
	out_ao = ao;
}
