#ifndef SPECULAR_FILTER_SOURCE_H
#define SPECULAR_FILTER_SOURCE_H

const char specularFilterSource[] = R"shader(#version 430
layout(binding = 21) uniform samplerCube environment;

layout(location = 0) uniform vec2 res;
layout(location = 35) uniform vec3 camera_direction;
layout(location = 36) uniform vec3 camera_up;
layout(location = 37) uniform int level;
out vec3 color;

float radicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N) {
	return vec2(float(i)/float(N), radicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX( vec2 Xi, float Roughness , vec3 N ) {
	float a = Roughness * Roughness;
	float Phi = 2 * acos(-1.) * Xi.x;
	float CosTheta = sqrt( (1 - Xi.y) / ( 1 + (a*a - 1) * Xi.y ) );
	float SinTheta = sqrt( 1 - CosTheta * CosTheta );
	vec3 H;
	H.x = SinTheta * cos( Phi );
	H.y = SinTheta * sin( Phi );
	H.z = CosTheta;
	vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 TangentX = normalize( cross( UpVector , N ) );
	vec3 TangentY = cross( N, TangentX );
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}


float saturate(float x) {
	return clamp(x, 0., 1.);
}

vec3 PrefilterEnvMap(float Roughness , vec3 R) {
	vec3 N = R;
	vec3 V = R;
	vec3 PrefilteredColor = vec3(0);
	const uint NumSamples = 2048;
	float TotalWeight = 0.;
	for(uint i = 0; i < NumSamples; i++ ) {
		vec2 Xi = Hammersley( i, NumSamples );
		vec3 H = ImportanceSampleGGX(Xi, Roughness , N);
		vec3 L = 2 * dot(V, H) * H - V;
		float NoL = saturate(dot(N, L));
		if( NoL > 0 ) {
			PrefilteredColor += textureLod(environment , L, 0.).rgb * NoL;
			TotalWeight += NoL;
		}
	}
	return PrefilteredColor / TotalWeight;
}
vec3 cubemap_direction() {
	vec2 c = -(gl_FragCoord.xy / res - .5);
	vec3 direction = normalize(vec3(c, -.5));
	vec3 camera_right = cross(camera_direction, camera_up);
	return mat3(camera_right, camera_up, -camera_direction) * direction;
}

void main() {
	color = PrefilterEnvMap(level / 5., cubemap_direction());
}
)shader";

#endif
