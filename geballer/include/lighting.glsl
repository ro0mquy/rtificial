#include "helper.glsl"
#include "material.glsl"
#line 4 "lighting"

layout(binding = 0) uniform sampler2D brdf;
layout(binding = 1) uniform samplerCube environment;
layout(binding = 2) uniform samplerCube filteredDiffuse;
layout(binding = 3) uniform samplerCube filteredSpecular;

// n: normal
// v: vector from hit to camera (for example -dir)
// color: base color of object
// roughness: between 0 and 1
vec3 approximateSpecular(vec3 n, vec3 v, vec3 color, float roughness) {
	float NoV = saturate(dot(n, v));
	vec3 r = 2. * dot(n, v) * n - v;

	vec3 prefiltered = textureLod(filteredSpecular, r, roughness * 5.).rgb;
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
		+ approximateSpecular(n, v, vec3(mat.specular * mat.specular * .16), mat.roughness);
	vec3 metal = approximateSpecular(n, v, mat.color, mat.roughness);
	return mix(dielectric, metal, mat.metallic);
}

// o: camera origin
// d: camera view direction
// r: radius of "bounding sphere"
vec3 environmentColor(vec3 o, vec3 d, float r) {
	// hmmmmm…
	o.xz -= camera_position.xz;
	float radicand = square(dot(d, o)) - dot(o, o) + r * r;
	float t = -dot(d, o) + sqrt(radicand);
	return textureLod(environment, normalize(o + t * d), 0.).rgb;
}

// handy standard applyLights() function at your hands, just copy this into yout applyLights() function
/*
	return applyNormalLights(origin, marched, direction, hit, normal, material);
// */
vec3 applyNormalLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	material.color = .5 * normal + .5;
	vec3 color = .1 * ambientColor(normal, -direction, material);
	return color;
}

