#include "march.glsl"
#line 3 "stairs"

layout(binding = 0) uniform sampler2D brdf;
layout(binding = 1) uniform samplerCube environment;
layout(binding = 2) uniform samplerCube filteredDiffuse;
layout(binding = 3) uniform samplerCube filteredSpecular;

vec3 approximateSpecular(vec3 color, float roughness, vec3 N, vec3 V) {
	float NoV = clamp(dot(N, V), 0., 1.);
	vec3 R = 2. * dot(V, N) * N - V;

	vec3 prefiltered = textureLod(filteredSpecular, R, roughness * 5.).rgb;
	vec2 envBRDF = textureLod(brdf, vec2(roughness, NoV), 0.).rg;

	return prefiltered * (color  * envBRDF.x + envBRDF.y);
}

vec3 ambientColor(vec3 n, vec3 v, vec3 color, float rough, float metallic) {
	vec3 diffuse = textureLod(filteredDiffuse, n, 0.).rgb;
	vec3 dielectric = color * diffuse + approximateSpecular(vec3(.04), rough, n, v);
	vec3 metal = approximateSpecular(color, rough, n, v);
	return mix(dielectric, metal, metallic);
}

vec3 environmentColor(vec3 o, vec3 d, float r) {
	// hmmmmm…
	o.xz -= camera_position.xz;
	float radicand = dot(d, o) * dot(d, o) - dot(o, o) + r * r;
	if (radicand < 0.) discard; // hupsi
	float t = -dot(d, o) + sqrt(radicand);
	return textureLod(environment, normalize(o + t * d), 0.).rgb;
}

void main() {
	setDebugParameters();

	vec3 o = camera_position;
	vec3 d;
	float screen_dist = camGetDirection(d);
	float t = sdfMarch(o, d, 200., screen_dist);

	if (isinf(t)) {
		//out_color = vec3(0.);
		out_color = .07 * environmentColor(o, d, 100.);
	} else {
		vec3 hit = o + t * d;
		float dist = fMain(hit, true);
		Material material = current_material;
		vec3 normal = sdfNormal(hit);

		float metallic = 0.;
		float roughness = 0.;

		if (material.id == debug_plane_material_id) {
			out_color = debugColorIsolines(hit, o, t);
			roughness = 1.;
		} else {
			if (debug_gradient_visualization) {
				out_color = debugColorGradient(hit);
			} else {
				out_color = .5 * normal + .5;
			}
		}

		//out_color *= .95 * pdot(normal, normalize(vec3(1., 2., .5))) + .05;
		out_color = .1 * ambientColor(normal, -d, out_color, roughness, metallic);

	}
}

float fObject(vec3 p, vec2 dim, float angle) {
	pRotZ(p, -angle);
	vec3 q = p;
	pTrans(q.y, 1 + dim.y);
	return f2Box(p.xy, dim.x, dim.y);
}

float fScene(vec3 p) {
	//float f1 = fBox(p, vec3(8., 4., 2.));
	//pRotZ(p, -.2);
	//float f2 = fBox(p, vec3(2., 8., 4.));
	//float f = opUnionSmooth(f2, f1, 1.);
	//float f = fConeCapped(p.yxz, 6, 2, 0.5);

	vec3 q = p;

	pMirrorTrans(p.x, 0);
	pMirrorTrans(p.y, 30);
	p.y = -p.y;
	vec2 stepDim = vec2(2.5, 1);
	float angle = stepDim.y / stepDim.x;
	pRotZ(p, angle);
	float c = sqrt(stepDim.x * stepDim.x + stepDim.y * stepDim.y) * 2;
	pTrans(p.x, c);
	pDomrepSingle(p.x, c);
	float f = fObject(p, stepDim, angle);
	float r = stepDim.y * .3;
	f = opUnionChamfer(f, fObject(p - vec3(-c,0,0), stepDim, angle), r);
	f = opUnionChamfer(f, fObject(p - vec3(c,0,0), stepDim, angle), r);

	pDomrep(q.xz, stepDim.x * 4, 80);
	float g = f2Box(q.xz, 1, 1);
	f = min(f, g);

	mUnion(f, Material(0., p));
	return f;
}
