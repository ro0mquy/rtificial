#include "helper.glsl"
#include "material.glsl"
#line 4

layout(binding = 20) uniform sampler2D brdf;
layout(binding = 21) uniform samplerCube environment;
layout(binding = 22) uniform samplerCube filteredDiffuse;
layout(binding = 23) uniform samplerCube filteredSpecular;

struct SphereLight {
	vec3 position;
	float radius;
	vec3 color;
	float power;
};

struct PointLight {
	vec3 position;
	vec3 direction;
	float angle;
	vec3 color;
	float power;
};

vec3 getSpecularDominantDir(vec3 N, vec3 R, float NdotV, float roughness) {
	float smoothness = saturate(1 - roughness);
	return normalize(mix(N, R, smoothness * (sqrt(smoothness) + roughness)));
}

// n: normal
// v: vector from hit to camera (for example -dir)
// color: base color of object
// roughness: between 0 and 1
vec3 approximateSpecular(vec3 n, vec3 v, vec3 color, float roughness) {
	float NoV = saturate(dot(n, v));
	vec3 r = 2. * dot(n, v) * n - v;

	vec3 dir = getSpecularDominantDir(n, r, NoV, sqrt(roughness));

	vec3 prefiltered = textureLod(filteredSpecular, dir, sqrt(roughness) * 5.).rgb;
	vec2 envBRDF = textureLod(brdf, vec2(roughness, NoV), 0.).rg;

	return prefiltered * (color * envBRDF.x + envBRDF.y);
}

// n: normal
// v: vector from hit to camera (for example -dir)
// color: base color of object
// roughness: between 0 and 1
// metallic: only 0 or 1
vec3 ambientColor(vec3 n, vec3 v, Material mat) {
	vec3 diffuse = textureLod(filteredDiffuse, n, 0.).rgb;
	vec3 dielectric = mat.color * diffuse
		+ approximateSpecular(n, v, vec3(.04), mat.roughness);
	vec3 metal = approximateSpecular(n, v, mat.color, mat.roughness);
	vec3 color = mix(dielectric, metal, mat.metallic);

	return color;
}

// o: camera origin
// d: camera view direction
// r: radius of "bounding sphere"
vec3 environmentColor(vec3 o, vec3 d, float r) {
	// hmmmmm…
	o.xz -= camera_position.xz;
	float radicand = square(dot(d, o)) - dot(o, o) + r * r;
	float t = -dot(d, o) + sqrt(radicand);
	vec3 color = textureLod(environment, normalize(o + t * d), 0.).rgb;

	return color;
}

// handy standard applyLights() function at your hands, just copy this into yout applyLights() function
/*
	return applyNormalLights(origin, marched, direction, hit, normal, material);
// */
vec3 applyNormalLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	material.color = .5 * normal + .5;
	return ambientColor(normal, -direction, material);
}

float diffuseSphereLight(vec3 hit, vec3 N, SphereLight light) {
	vec3 L_unormalized = light.position - hit;
	float light_distance2 = dot(L_unormalized, L_unormalized);
	vec3 L = L_unormalized * inversesqrt(light_distance2);

	float Beta = acos(dot(N, L));
	float H = sqrt(light_distance2);
	float h = H / light.radius;
	float x = sqrt(square(h) - 1);
	float y = -x / tan(Beta);

	float illuminance = 0;
	if (h * cos(Beta) > 1) {
		illuminance = cos(Beta) / square(h);
	} else {
		illuminance = (1. / (Pi * square(h))) *
			(cos(Beta) * acos(y) - x * sin(Beta) * sqrt(1 - y * y)) +
			(1 / Pi) * atan(sin(Beta) * sqrt(1 - square(y)) / x);
	}

	illuminance *= Pi;
	return illuminance;
}

vec3 specularSphereLightClosestPoint(vec3 hit, vec3 N, vec3 V, float roughness, SphereLight light) {
	vec3 R  = reflect(-V, N);
	vec3 dominantDir = getSpecularDominantDir(N, R, pdot(N, V), roughness);
	vec3 L_unnorm = light.position - hit;
	vec3 centerToRay = dot(L_unnorm, dominantDir) * dominantDir - L_unnorm;
	vec3 closestPoint = L_unnorm + centerToRay * saturate(light.radius / length(centerToRay));
	return closestPoint;
}

// not actually the delta function, adapted for use in brdfSpecularEpicSmith
float deltaGGX(float alpha2, float NdotV) {
	float NdotV2 = square(NdotV);
	float tan_theta_alpha2 = (1 - NdotV2) / NdotV2 * alpha2;
	return sqrt(1 + tan_theta_alpha2);
}

float brdfSpecularEpicSmithWithoutFresnel(vec3 V, vec3 L, vec3 N, float roughness) {
	vec3 H = normalize(V + L);
	float NdotL = pdot(N, L);
	float NdotV = pdot(N, V);
	float NdotH = pdot(N, H);
	float alpha = square(roughness);
	float alpha2 = square(alpha);

	float divisor = max(Pi * square(square(NdotH) * (alpha2 - 1) + 1), 1e-8); // specular D part
	divisor *= deltaGGX(alpha2, NdotV) + deltaGGX(alpha2, NdotL); // height-correlated smith masking-shadowing * 2
	divisor *= 2 * NdotL * NdotV; // 2 moved to G term
	return max(alpha2 / divisor, 0);
}

vec3 fresnel(vec3 V, vec3 L, vec3 F0) {
	vec3 H = normalize(V + L);
	float VdotH = pdot(V, H);
	return F0 + (1 - F0) * exp2((-5.55473 * VdotH - 6.98316) * VdotH);
}

vec3 applySphereLight(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, SphereLight light) {
	// diffuse part
	vec3 L = light.position - hit;
	float lightDistance2 = dot(L, L);
	L /= sqrt(lightDistance2);
	float NoL = pdot(L, normal);
	float incomingLight = light.power / (4 * square(Pi) * square(light.radius));
	incomingLight *= diffuseSphereLight(hit, normal, light);
	vec3 diffuse = material.color * (max(0, NoL * incomingLight) / Pi);

	// specular part
	vec3 closestPoint = specularSphereLightClosestPoint(hit, normal, -direction, material.roughness, light);
	float roughnessAlpha = saturate(material.roughness + light.radius / (2 * sqrt(lightDistance2)));
	float closestPointDistance2 = dot(closestPoint, closestPoint);
	closestPoint /= sqrt(closestPointDistance2);
	float normalization = square(material.roughness / roughnessAlpha);
	float incomingLightSpecular = light.power * normalization / (4 * Pi * closestPointDistance2);
	vec3 F_metal = fresnel(-direction, closestPoint, material.color);
	float specular = incomingLightSpecular * NoL * brdfSpecularEpicSmithWithoutFresnel(-direction, closestPoint, normal, material.roughness);

	vec3 F_dielectric = fresnel(-direction, closestPoint, vec3(.04));
	vec3 light_result = mix(diffuse + F_dielectric * specular, specular * F_metal, material.metallic);
	light_result *= light.color;
	return light_result;
}

float getDistanceAttenuation(vec3 lightVector) {
	return rcp(Pi * dot(lightVector, lightVector));
}

float getAngleAttenuation(vec3 L, PointLight light) {
	float outer = light.angle;
	float foo = 1 - smoothstep(0, outer, acos(-dot(light.direction, L)));
	return square(foo);
}

vec3 applyPointLight(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material, PointLight light) {
	vec3 lightVector = light.position - hit;
	vec3 L = normalize(lightVector);
	float attenuation = 1;
	attenuation *= getDistanceAttenuation(lightVector);
	attenuation *= getAngleAttenuation(L, light);
	float brdfSpecular = brdfSpecularEpicSmithWithoutFresnel(-direction, L, normal, material.roughness);
	vec3 dielectric = material.color / Pi + brdfSpecular * fresnel(-direction, L, vec3(.04));
	vec3 metallic = brdfSpecular * fresnel(-direction, L, material.color);
	return mix(dielectric, metallic, material.metallic) * saturate(dot(normal, L)) * attenuation * light.color * light.power;
}
