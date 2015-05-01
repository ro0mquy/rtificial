#include "march.glsl"
#line 3 "asymfuture"

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
	vec3 dielectric = .1 * color * diffuse + 10. * approximateSpecular(vec3(.04), rough, n, v);
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

struct Material_ {
	vec3 color;
	float roughness;
	float metallic;
};

struct SphereLight {
	vec3 center;
	vec3 color;
	float radius;
	float intensity;
};

vec3 apply_light(vec3 p, vec3 N, vec3 V, Material_ material, SphereLight light) {
	vec3 L = light.center - p;

	// calculate representative point
	vec3 r = reflect(-V, N);
	vec3 centerToRay = dot(L, r) * r - L;
	vec3 closestPoint = L + centerToRay * clamp(light.radius/length(centerToRay), 0., 1.);
	vec3 L_spec = normalize(closestPoint);

	float light_distance = distance(p, light.center);

	// specular
	float PI = acos(-1.);
	vec3 H = .5 * (L_spec + V);
	float NdotH = dot(N, H);
	float NdotL= pdot(N, L_spec);
	float NdotV = dot(N, V);
	float alpha = square(material.roughness);
	float alpha2 = square(alpha);
	float k = square(material.roughness + 1.);
	float specular_without_F = alpha2 / (4. * mix(NdotL, 1., k) * mix(NdotV, 1., k) * PI * square(square(NdotH) * (alpha2 - 1.) + 1.));
	float alphastrich = clamp(alpha + light.radius * .5 / light_distance, 0., 1.);
	float spec_norm = alpha2 / (alphastrich * alphastrich);

	float VdotH = dot(V, H);
	float fresnel_power = exp2((-5.55473 * VdotH - 6.98316) * VdotH);
	float F_dielectric = .04 + (1. - .04) * fresnel_power;
	vec3 F_metal = material.color + (1. - material.color) * fresnel_power;

	vec3 dielectric_color = .5 * (material.color / PI + specular_without_F * F_dielectric);
	vec3 metal_color = specular_without_F * F_metal;
	float foo = square(1. - square(square(light_distance / light.radius)));
	float falloff = clamp(foo, 0., 1.) / (square(light_distance) + 1.);
	return NdotL * mix(dielectric_color, metal_color, material.metallic) * falloff * light.intensity * light.color * spec_norm;
}

void main() {
	setDebugParameters();

	vec3 o = camera_position;
	vec3 d;
	float screen_dist = camGetDirection(d);
	float t = sdfMarch(o, d, 500., screen_dist);

	if (isinf(t)) {
		//out_color = vec3(0.);
		out_color = .07 * environmentColor(o, d, 100.);
	} else {
		vec3 hit = o + t * d;
		float dist = fMain(hit, true);
		Material material = current_material;
		vec3 normal = sdfNormal(hit);

		vec3 tex_color;
		float metallic = 0.;
		float roughness = 0.;

		if (material.id == debug_plane_material_id) {
			out_color = debugColorIsolines(hit, o, t);
			roughness = 1.;
		} else {
			if (debug_gradient_visualization) {
				out_color = debugColorGradient(hit);
			} else {
				if (material.id == 1.) {
					out_color = vec3(1.) * 1000.;
				} else {
					tex_color = af_tex_white_rt_color;

					if (material.id == 0.) {
						vec2 p = material.coord.zy;
						vec2 p_center = p;
						p_center.x -= af_tex_pos_rt_vec2;
						pMirrorTrans(p_center.x, af_tex_appart_rt_float);
						float f_center = step(af_tex_center_radius_rt_float, length(p_center));

						vec2 p_ring = p_center;
						float f_ring = step(af_tex_ring_radius_rt_float, length(p_ring));

						tex_color = mix(af_tex_black_rt_color, tex_color, f_ring);
						tex_color = mix(af_tex_orange_rt_color, tex_color, f_center);
					} else if (material.id == -1.) {
						tex_color = af_tex_black_rt_color;
					} else if (material.id == 2.) {
						vec2 p = material.coord.xy;
						p.y -= -sqrt(.5) * af_tex_triangle_size_rt_float;
						float f1 = 1. - step(0., -p.y);
						float f2 = 1. - step(af_tex_triangle_size_rt_float, f2PlaneAngle(p, radians(30.)));
						f1 = 1. - f1 * f2;

						float e = af_tex_triangle_rand_rt_float;
						float f_rand1 = 1. - step(e, -p.y);
						float f_rand2 = 1. - step(af_tex_triangle_size_rt_float + e, f2PlaneAngle(p, radians(30.)));
						f_rand1 = 1. - f_rand1 * f_rand2;

						tex_color = mix(af_tex_black_rt_color, tex_color, f_rand1);
						tex_color = mix(af_tex_orange_rt_color, tex_color, f1);
					}

					out_color = apply_light(hit, normal, -d, Material_(tex_color, .1, 0.), SphereLight(vec3(0., 10., -15.), vec3(1.), 1., 40000.));
					out_color += apply_light(hit, normal, -d, Material_(tex_color, .1, 0.), SphereLight(vec3(0., 10., 5.), vec3(1.), 1., 30000.));
					out_color += apply_light(hit, normal, -d, Material_(tex_color, .1, 0.), SphereLight(vec3(0., 5., -200.), vec3(1.), 1., 50000.));
					out_color *= 100.;
				}
			}
		}

		//out_color *= .95 * pdot(normal, normalize(vec3(1., 2., .5))) + .05;
		out_color += ambientColor(normal, -d, tex_color, roughness, metallic);
		out_color *= 1. - smoothstep(400., 500., t);

	}
}

float fPyramid(vec3 p, float r, float phi) {
	vec3 q = p;
	q.xz = abs(q.xz);
	q.y -= r;
	float plane1 = f2PlaneAngle(q.xy, phi);
	float plane2 = f2PlaneAngle(q.zy, phi);
	float f_oktaeder = max(plane1, plane2);
	return f_oktaeder;
}

float fScene(vec3 p) {
	// http://beeple.tumblr.com/post/105573896314/asymmetrical-future

	float f = Inf;

	pMirror(p.x);

	vec3 p_gang = p;
	p_gang.y -= af_gang_height_rt_float;
	float f_gang = -f2BoxRounded(p_gang.xy, af_gang_dim_rt_vec2, af_gang_roundness_rt_float);

	vec3 p_gang_einsatz = p_gang;
	p_gang_einsatz.z -= af_gang_einsatz_z_rt_float;
	p_gang_einsatz.z *= -1.;
	pDomrepSingle(p_gang_einsatz.z, af_gang_einsatz_domrep_rt_float);
	float ratio_gang_einsatz = af_gang_einsatz_ratio_rt_float;
	vec2 dim_gang_einsatz = af_gang_dim_rt_vec2 - vec2((1. - ratio_gang_einsatz) * af_gang_dim_rt_vec2.x);
	float f_gang_einsatz = -f2BoxRounded(p_gang_einsatz.xy, dim_gang_einsatz, ratio_gang_einsatz * af_gang_roundness_rt_float);
	f_gang_einsatz = max(f_gang_einsatz, abs(p_gang_einsatz.z) - af_gang_einsatz_thickness_rt_float);
	//f_gang = opUnionRounded(f_gang, f_gang_einsatz, af_gang_einsatz_smoothness_rt_float);
	mUnion(f_gang_einsatz, Material(-1., p_gang_einsatz));

	mUnion(f_gang, Material(0., p_gang));

	vec3 p_stufe = p;
	float stufe_y = af_stufe_height_rt_float;
	float stufe_z = 3. * stufe_y;
	p_stufe.y -= stufe_y;
	float f_stufe = f2Box(p_stufe.yz, vec2(stufe_y, stufe_z));

	vec3 p_untere_stufe = p;
	float unter_stufe_x = af_untere_stufe_width_rt_float;
	float unter_stufe_y = af_untere_stufe_height_rt_float;
	float unter_stufe_z = 1.5 * stufe_z;
	p_untere_stufe.y -= unter_stufe_y;
	float f_untere_stufe = abs(p_untere_stufe.z) - unter_stufe_z;
	float f_unter_forward = abs(p_untere_stufe.x) - unter_stufe_x;
	f_unter_forward = max(f_unter_forward, -p_untere_stufe.z + unter_stufe_z);
	f_untere_stufe = opUnionRounded(f_untere_stufe, f_unter_forward, af_untere_stufe_roundness_rt_float * unter_stufe_x);
	f_untere_stufe = max(f_untere_stufe, abs(p_untere_stufe.y) - unter_stufe_y);

	f_stufe = opUnionRounded(f_stufe, f_untere_stufe, af_stufe_roundness_rt_float * stufe_y);
	mUnion(f_stufe, Material(2., p_stufe));


	vec3 p_sphere = p_stufe;
	p_sphere.y -= stufe_y + af_sphere_radius_rt_float;
	float f_sphere = fSphere(p_sphere, af_sphere_radius_rt_float);
	mUnion(f_sphere, Material(3., p_sphere));

	vec3 p_sphere_small = p_stufe;
	p_sphere_small.y -= stufe_y + af_sphere_small_radius_rt_float;
	p_sphere_small.x -= af_sphere_small_z_rt_float;
	pMirrorTrans(p_sphere_small.x, af_sphere_small_apart_rt_float);
	float f_sphere_small = fSphere(p_sphere_small, af_sphere_small_radius_rt_float);
	mUnion(f_sphere_small, Material(4., p_sphere_small));


	float pyr_phi = Tau * af_pyr_big_phi_rt_float;

	vec3 p_pyr_big = p;
	pTrans(p_pyr_big, af_pyr_big_pos_rt_vec3);
	float f_pyr_big = fPyramid(p_pyr_big, af_pyr_big_size_rt_float, pyr_phi);
	mUnion(f_pyr_big, Material(5., p_pyr_big));

	vec3 p_pyr_medium_mid = p;
	p_pyr_medium_mid.y -= 2. * stufe_y;
	pTrans(p_pyr_medium_mid, af_pyr_medium_mid_pos_rt_vec3);
	pMirrorTrans(p_pyr_medium_mid.x, af_pyr_medium_mid_apart_rt_float);
	float f_pyr_medium_mid = fPyramid(p_pyr_medium_mid, af_pyr_medium_mid_size_rt_float, pyr_phi);
	mUnion(f_pyr_medium_mid, Material(6., p_pyr_medium_mid));

	vec3 p_pyr_small_mid = p;
	p_pyr_small_mid.y -= 2. * stufe_y;
	pTrans(p_pyr_small_mid, af_pyr_small_mid_pos_rt_vec3);
	pMirrorTrans(p_pyr_small_mid.x, af_pyr_small_mid_apart_rt_float);
	float f_pyr_small_mid = fPyramid(p_pyr_small_mid, af_pyr_small_mid_size_rt_float, pyr_phi);
	mUnion(f_pyr_small_mid, Material(7., p_pyr_small_mid));

	vec3 p_pyr_medium_fg = p;
	pTrans(p_pyr_medium_fg, af_pyr_medium_fg_pos_rt_vec3);
	float f_pyr_medium_fg = fPyramid(p_pyr_medium_fg, af_pyr_medium_fg_size_rt_float, pyr_phi);
	mUnion(f_pyr_medium_fg, Material(8., p_pyr_medium_fg));

	vec3 p_pyr_small_fg = p;
	pTrans(p_pyr_small_fg, af_pyr_small_fg_pos_rt_vec3);
	float f_pyr_small_fg = fPyramid(p_pyr_small_fg, af_pyr_small_fg_size_rt_float, pyr_phi);
	f = min(f, f_pyr_small_fg);
	mUnion(f_pyr_small_fg, Material(9., p_pyr_small_fg));


	vec3 p_lampe = p;
	p_lampe.y -= 2. * af_gang_dim_rt_vec2.y;
	float f_lampe = fBox(p_lampe, af_lampe_dim_rt_vec2.x, .1, af_lampe_dim_rt_vec2.y);
	mUnion(f_lampe, Material(1., p_lampe));

	return current_dist;
}
