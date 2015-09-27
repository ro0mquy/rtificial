#version 430

// inactive uniforms lead to crashes (!)
layout(location = 0) uniform vec2 res;
uniform float red;
uniform vec3 foo; // color
uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat

layout(binding = 20) uniform sampler2D brdf;
layout(binding = 21) uniform samplerCube environment;
layout(binding = 22) uniform samplerCube filteredDiffuse;
layout(binding = 23) uniform samplerCube filteredSpecular;

out vec4 out_color;

// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
vec3 quat_rotate(vec3 v, vec4 q) {
	vec3 t = 2 * cross(q.xyz, v);
	return v + q.w * t + cross(q.xyz, t);
	// *hex hex*
}

vec3 get_direction() {
	vec3 dir = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.x , -0.02 / .03));
	return quat_rotate(dir, camera_rotation);

}

float f(vec3 p) {
	return length(p) - 1.;
}

vec3 approximateSpecular(vec3 color, float roughness, vec3 N, vec3 V) {
	float NoV = clamp(dot(N, V), 0., 1.);
	vec3 R = 2. * dot(V, N) * N - V;

	vec3 prefiltered = textureLod(filteredSpecular, R, roughness * 5.).rgb;
	vec2 envBRDF = textureLod(brdf, vec2(roughness, NoV), 0.).rg;

	return prefiltered * (color  * envBRDF.x + envBRDF.y);
}

void main() {
	vec3 o = camera_position;
	vec3 d = get_direction();
	float t = 0.;

	for (int i = 0; i < 64; i++) {
		float x = f(o + d * t);
		if (abs(x) < .001) {
			vec3 p = o + d * t;
			vec2 e = vec2(.001, .0);
			vec3 normal = normalize(vec3(
				f(p + e.xyy) - f(p - e.xyy),
				f(p + e.yxy) - f(p - e.yxy),
				f(p + e.yyx) - f(p - e.yyx)
			));

			vec3 r = reflect(d, normal);
			vec3 diffuse = textureLod(filteredDiffuse, normal, 0.).rgb;
			float rough = roughness_rt_float;
			float metallic = metallic_rt_float;
			vec3 col = color_rt_color;
			vec3 dielectric = col * diffuse + approximateSpecular(vec3(.04), rough, normal, -d);
			vec3 metal =approximateSpecular(col, rough, normal, -d);
			out_color.rgb = mix(dielectric, metal, metallic);
			return;
		}
		t += x;
	}
	out_color.rgb = textureLod(environment, d, roughness_rt_float * 5.).rgb;

	//out_color = mix(foo, vec3(red, gl_FragCoord.xy / res.xy), .1);
	//out_color.rgb = textureLod(bla, get_direction(), 7.).rgb;
	//out_color.rgb = textureLod(brdf, gl_FragCoord.xy / res.xy, 0.).rgb;
}
