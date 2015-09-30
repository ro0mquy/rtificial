#ifndef DIFFUSE_FILTER_SOURCE_H
#define DIFFUSE_FILTER_SOURCE_H

const char diffuseFilterSource[] = R"shader(#version 430
layout(binding = 21) uniform samplerCube environment;

layout(location = 0) uniform vec2 res;
layout(location = 35) uniform vec3 camera_direction;
layout(location = 36) uniform vec3 camera_up;
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

vec3 importanceSampleCosine(vec3 n, vec2 Xi) {
	float r = sqrt(Xi.x);
	float phi = 2. * acos(-1.) * Xi.y;

	vec3 H = vec3(r * vec2(cos(phi), sin(phi)), sqrt(max(0., 1. - Xi.x)));

	vec3 UpVector = abs(n.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 TangentX = normalize( cross( UpVector , n ) );
	vec3 TangentY = cross( n, TangentX );
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + n * H.z;
}

vec3 cubemap_direction() {
	vec2 c = -(gl_FragCoord.xy / res - .5);
	vec3 direction = normalize(vec3(c, -.5));
	vec3 camera_right = cross(camera_direction, camera_up);
	return mat3(camera_right, camera_up, -camera_direction) * direction;
}

void main() {
	vec3 normal = cubemap_direction();
	const uint n_samples = 2048;
	vec3 color_accumulator = vec3(0.);
	for (uint i = 0; i < n_samples; i++) {
		vec2 Xi = Hammersley(i, n_samples);
		color_accumulator += textureLod(environment, importanceSampleCosine(normal, Xi), 0.).rgb;
	}
	color = color_accumulator / n_samples;
}
)shader";

#endif
