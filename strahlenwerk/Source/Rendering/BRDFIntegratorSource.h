#ifndef BRDF_INTEGRATOR_SOURCE_H
#define BRDF_INTEGRATOR_SOURCE_H

const std::string brdfIntegratorSource = R"shader(#version 430
layout(location = 0) uniform vec2 res;
out vec2 out_color;

const float PI = acos(-1.);

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

float saturate(float x) {
	return clamp(x, 0., 1.);
}

vec3 ImportanceSampleGGX( vec2 Xi, float Roughness , vec3 N ) {
	float a = Roughness * Roughness;
	float Phi = 2 * PI * Xi.x;
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

float G_Smith(float roughness, float NoV, float NoL) {
	float k = roughness * roughness / 2.;
	return NoV * NoL / ((NoV * (1. - k) + k) * (NoL * (1. - k) + k));
}

float square(float x) {
	return x * x;
}

float delta_ggx_unoptimized(float alpha, float NdotV) {
	float cot_theta = NdotV / sqrt(1 - square(NdotV));
	float a = 1. / (cot_theta * alpha);
	return .5 * (-1 + sqrt(1 + 1. / square(a)));
}

float G_Smith_real(float roughness, float NoV, float NoL) {
	roughness *= roughness;
	return 1./ (1. + delta_ggx_unoptimized(roughness, NoV) + delta_ggx_unoptimized(roughness, NoL));
}

vec2 IntegrateBRDF( float Roughness , float NoV ) {
	vec3 V;
	V.x = sqrt( 1.0 - NoV * NoV ); // sin
	V.y = 0.;
	V.z = NoV; // cos
	float A = 0.;
	float B = 0.;
	const uint NumSamples = 1024;
	const vec3 N = vec3(0., 0., 1.);
	for(int i = 0; i < NumSamples; i++ ) {
		vec2 Xi = Hammersley(i, NumSamples);
		vec3 H = ImportanceSampleGGX( Xi, Roughness , N );
		vec3 L = 2 * dot( V, H ) * H - V;
		float NoL = saturate( L.z );
		float NoH = saturate( H.z );
		float VoH = saturate( dot( V, H ) );
		if( NoL > 0 )
		{
			float G = G_Smith( Roughness , NoV, NoL );
			//float G = G_Smith_real( Roughness , NoV, NoL );
			float G_Vis = G * VoH / (NoH * NoV);
			float Fc = pow( 1 - VoH, 5 );
			A += (1 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}
	return vec2( A, B ) / NumSamples;
}

void main() {
	vec2 position = gl_FragCoord.xy / res;
	out_color = vec2(IntegrateBRDF(position.x, position.y));
}
)shader";

#endif
