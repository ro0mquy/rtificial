#ifndef syntakischerspalt_H
#define syntakischerspalt_H
const char syntakischerspalt_source[] =R"shader_source(#version 430\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;\n)shader_source"
R"shader_source(layout(location = 1) uniform float envelopes[32];\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color; // alpha = CoC\n)shader_source"
R"shader_source(layout(location = 1) out float coc;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 48) uniform float focus_dist;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 38) uniform float focal_length;\n)shader_source"
R"shader_source(#define FOCAL_LENGTH\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 34) uniform float f_stop;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(void output_color(vec3 color, float dist) {\n)shader_source"
R"shader_source(	float focus_dist = focus_dist;\n)shader_source"
R"shader_source(	float f = focal_length;\n)shader_source"
R"shader_source(	float N = f_stop;\n)shader_source"
R"shader_source(	coc = (dist - focus_dist)/dist * (f * f) / (N * (focus_dist - f)) / 0.03 * res.x;\n)shader_source"
R"shader_source(	out_color = color;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(#line 1\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// fancy functions TODO\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 35) uniform vec3 camera_position;\n)shader_source"
R"shader_source(layout(location = 36) uniform vec4 camera_rotation;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 37) uniform float time;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(#ifndef FOCAL_LENGTH\n)shader_source"
R"shader_source(layout(location = 38) uniform float focal_length;\n)shader_source"
R"shader_source(#endif\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float TAU = 6.28318530718;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(/*\n)shader_source"
R"shader_source(mat3 get_camera() {\n)shader_source"
R"shader_source(	vec3 view_right = cross(camera_direction, camera_up);\n)shader_source"
R"shader_source(	return mat3(view_right, camera_up, -camera_direction);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(*/\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/\n)shader_source"
R"shader_source(vec3 quat_rotate(vec3 v, vec4 q) {\n)shader_source"
R"shader_source(	vec3 t = 2 * cross(q.xyz, v);\n)shader_source"
R"shader_source(	return v + q.w * t + cross(q.xyz, t);\n)shader_source"
R"shader_source(	// *hex hex*\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 get_direction() {\n)shader_source"
R"shader_source(	vec3 dir = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.x , -focal_length / .03));\n)shader_source"
R"shader_source(	return quat_rotate(dir, camera_rotation);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec2 f(vec3 p);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 march_adv(vec3 p, vec3 direction, out int i, int iterations, float stepsize) {\n)shader_source"
R"shader_source(	float walked = 0.;\n)shader_source"
R"shader_source(	for (i=0; i < iterations; i++) {\n)shader_source"
R"shader_source(		float dist = f(p)[0] * stepsize;\n)shader_source"
R"shader_source(		p += direction * dist;\n)shader_source"
R"shader_source(		dist = abs(dist);\n)shader_source"
R"shader_source(		walked += dist;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(		if (dist < .001 * walked) break;\n)shader_source"
R"shader_source(	}\n)shader_source"
R"shader_source(	return p;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 march(vec3 p, vec3 direction, out int i) {\n)shader_source"
R"shader_source(	return march_adv(p, direction, i, 100, 1.);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 calc_normal(vec3 p) {\n)shader_source"
R"shader_source(	vec2 epilepsilon = vec2(.001, 0.);\n)shader_source"
R"shader_source(	return normalize(vec3(\n)shader_source"
R"shader_source(		f(p + epilepsilon.xyy)[0] - f(p - epilepsilon.xyy)[0],\n)shader_source"
R"shader_source(		f(p + epilepsilon.yxy)[0] - f(p - epilepsilon.yxy)[0],\n)shader_source"
R"shader_source(		f(p + epilepsilon.yyx)[0] - f(p - epilepsilon.yyx)[0]\n)shader_source"
R"shader_source(	) + 1e-9);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float sphere(vec3 p, float s) {\n)shader_source"
R"shader_source(	return length(p) - s;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// a and b are the endpoints\n)shader_source"
R"shader_source(// r is the radius if you want some kind of capsule\n)shader_source"
R"shader_source(float line(vec3 p, vec3 a, vec3 b, float r) {\n)shader_source"
R"shader_source(	vec3 pa = p - a;\n)shader_source"
R"shader_source(	vec3 ba = b - a;\n)shader_source"
R"shader_source(	float h = clamp(dot(pa,ba) / dot(ba,ba), 0., 1. );\n)shader_source"
R"shader_source(	return length(pa - ba*h) - r;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float torus(vec3 p, vec2 t) {\n)shader_source"
R"shader_source(	vec2 q = vec2(length(p.xz) - t.x, p.y);\n)shader_source"
R"shader_source(	return length(q) - t.y;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float box(vec3 p, vec3 b) {\n)shader_source"
R"shader_source(	p = abs(p) - b;\n)shader_source"
R"shader_source(	return max(p.x, max(p.y, p.z));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float box2(vec2 p, vec2 b) {\n)shader_source"
R"shader_source(	p = abs(p) - b;\n)shader_source"
R"shader_source(	return max(p.x, p.y);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// more accurate than box(), but slower\n)shader_source"
R"shader_source(float slowbox(vec3 p, vec3 b) {\n)shader_source"
R"shader_source(	vec3 d = abs(p) - b;\n)shader_source"
R"shader_source(	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// box with rounded corners, r is radius of corners\n)shader_source"
R"shader_source(float roundbox(vec3 p, vec3 b, float r) {\n)shader_source"
R"shader_source(	return slowbox(p, b - r) - r;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// c must be normalized\n)shader_source"
R"shader_source(float cone(vec3 p, vec2 c) {\n)shader_source"
R"shader_source(	float q = length(p.xy);\n)shader_source"
R"shader_source(	return dot(c, vec2(q, p.z));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// n must be normalized\n)shader_source"
R"shader_source(float plane(vec3 p, vec3 n) {\n)shader_source"
R"shader_source(	return dot(p, n.xyz);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float cylinder(vec3 p, float radius, float thicknesshalf) {\n)shader_source"
R"shader_source(	float circle = length(p.xy) - radius;\n)shader_source"
R"shader_source(	return max(circle, abs(p.z) - thicknesshalf);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// hier kommt der witz!\n)shader_source"
R"shader_source(vec2 min_material(vec2 a, vec2 b) {\n)shader_source"
R"shader_source(	return mix(a, b, float(a.x > b.x));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// smooth minimum, k is the difference between the two values for which to smooth (eg. k = 0.1)\n)shader_source"
R"shader_source(float smin(float a, float b, float k) {\n)shader_source"
R"shader_source(	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );\n)shader_source"
R"shader_source(	return mix(b, a, h) - k * h * (1.0 - h);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec2 smin_material(vec2 a, vec2 b, float k) {\n)shader_source"
R"shader_source(	return vec2(smin(a, b, k), a.x > b.x ? b.y : a.y);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// be careful when nesting! (just don't)\n)shader_source"
R"shader_source(vec2 smin_smaterial(vec2 a, vec2 b, float k) {\n)shader_source"
R"shader_source(	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );\n)shader_source"
R"shader_source(	return vec2(mix(b.x, a.x, h) - k * h * (1.0 - h), mix(b.y, a.y, h));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// smooth maximum, k is the difference between the two values for which to smooth (eg. k = 0.1)\n)shader_source"
R"shader_source(float smax(float a, float b, float k) {\n)shader_source"
R"shader_source(	float h = clamp(0.5 - 0.5 * (b - a) / k, 0.0, 1.0 );\n)shader_source"
R"shader_source(	return mix(b, a, h) + k * h * (1.0 - h);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(struct Material {\n)shader_source"
R"shader_source(	vec3 color;\n)shader_source"
R"shader_source(	float roughness;\n)shader_source"
R"shader_source(	float metallic;\n)shader_source"
R"shader_source(};\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(struct SphereLight {\n)shader_source"
R"shader_source(	vec3 center;\n)shader_source"
R"shader_source(	vec3 color;\n)shader_source"
R"shader_source(	float radius;\n)shader_source"
R"shader_source(	float intensity;\n)shader_source"
R"shader_source(};\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// positive dot product\n)shader_source"
R"shader_source(float pdot(vec3 a, vec3 b) {\n)shader_source"
R"shader_source(	return max(0., dot(a, b));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float square(float a) {\n)shader_source"
R"shader_source(	return a * a;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 apply_light(vec3 p, vec3 N, vec3 V, Material material, SphereLight light) {\n)shader_source"
R"shader_source(	vec3 L = light.center - p;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	// calculate representative point\n)shader_source"
R"shader_source(	vec3 r = reflect(-V, N);\n)shader_source"
R"shader_source(	vec3 centerToRay = dot(L, r) * r - L;\n)shader_source"
R"shader_source(	vec3 closestPoint = L + centerToRay * clamp(light.radius/length(centerToRay), 0., 1.);\n)shader_source"
R"shader_source(	vec3 L_spec = normalize(closestPoint);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	float light_distance = distance(p, light.center);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	// specular\n)shader_source"
R"shader_source(	float PI = acos(-1.);\n)shader_source"
R"shader_source(	vec3 H = .5 * (L_spec + V);\n)shader_source"
R"shader_source(	float NdotH = dot(N, H);\n)shader_source"
R"shader_source(	float NdotL= pdot(N, L_spec);\n)shader_source"
R"shader_source(	float NdotV = dot(N, V);\n)shader_source"
R"shader_source(	float alpha = square(material.roughness);\n)shader_source"
R"shader_source(	float alpha2 = square(alpha);\n)shader_source"
R"shader_source(	float k = square(material.roughness + 1.);\n)shader_source"
R"shader_source(	float specular_without_F = alpha2 / (4. * mix(NdotL, 1., k) * mix(NdotV, 1., k) * PI * square(square(NdotH) * (alpha2 - 1.) + 1.));\n)shader_source"
R"shader_source(	float alphastrich = clamp(alpha + light.radius * .5 / light_distance, 0., 1.);\n)shader_source"
R"shader_source(	float spec_norm = alpha2 / (alphastrich * alphastrich);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	float VdotH = dot(V, H);\n)shader_source"
R"shader_source(	float fresnel_power = exp2((-5.55473 * VdotH - 6.98316) * VdotH);\n)shader_source"
R"shader_source(	float F_dielectric = .04 + (1. - .04) * fresnel_power;\n)shader_source"
R"shader_source(	vec3 F_metal = material.color + (1. - material.color) * fresnel_power;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec3 dielectric_color = .5 * (material.color / PI + specular_without_F * F_dielectric);\n)shader_source"
R"shader_source(	vec3 metal_color = specular_without_F * F_metal;\n)shader_source"
R"shader_source(	float foo = square(1. - square(square(light_distance / light.radius)));\n)shader_source"
R"shader_source(	float falloff = clamp(foo, 0., 1.) / (square(light_distance) + 1.);\n)shader_source"
R"shader_source(	return NdotL * mix(dielectric_color, metal_color, material.metallic) * falloff * light.intensity * light.color * spec_norm;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 emit_light(vec3 color, float intensity) {\n)shader_source"
R"shader_source(	return color * intensity;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 domrepv(vec3 p, vec3 c) {\n)shader_source"
R"shader_source(	return mod(p, c) - .5 * c;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// repeat things\n)shader_source"
R"shader_source(vec3 domrep(vec3 p, float x, float y, float z) {\n)shader_source"
R"shader_source(	return domrepv(p, vec3(x, y, z));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// trans*late things - using vectors!!\n)shader_source"
R"shader_source(// p: point\n)shader_source"
R"shader_source(// v: translation vector\n)shader_source"
R"shader_source(vec3 transv(vec3 p, vec3 v) {\n)shader_source"
R"shader_source(	return p - v;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// trans*late things\n)shader_source"
R"shader_source(// p: point\n)shader_source"
R"shader_source(// x: x\n)shader_source"
R"shader_source(// y: y\n)shader_source"
R"shader_source(// z: z\n)shader_source"
R"shader_source(vec3 trans(vec3 p, float x, float y, float z) {\n)shader_source"
R"shader_source(	return transv(p, vec3(x, y, z));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(mat3 rX(float theta) {\n)shader_source"
R"shader_source(	return mat3(\n)shader_source"
R"shader_source(		1., 0., 0.,\n)shader_source"
R"shader_source(		0., cos(theta), sin(theta),\n)shader_source"
R"shader_source(		0., -sin(theta), cos(theta)\n)shader_source"
R"shader_source(	);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(mat3 rY(float theta) {\n)shader_source"
R"shader_source(	return mat3(\n)shader_source"
R"shader_source(		cos(theta), 0., -sin(theta),\n)shader_source"
R"shader_source(		0., 1., 0.,\n)shader_source"
R"shader_source(		sin(theta), 0., cos(theta)\n)shader_source"
R"shader_source(	);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(mat3 rZ(float theta) {\n)shader_source"
R"shader_source(	return mat3(\n)shader_source"
R"shader_source(		cos(theta), sin(theta), 0.,\n)shader_source"
R"shader_source(		-sin(theta), cos(theta), 0.,\n)shader_source"
R"shader_source(		0., 0., 1.\n)shader_source"
R"shader_source(	);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(mat2 rot2D(float theta) {\n)shader_source"
R"shader_source(	return mat2(cos(theta), -sin(theta), sin(theta), cos(theta));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// impulse from iq's website\n)shader_source"
R"shader_source(// k is the sharpness of the curve\n)shader_source"
R"shader_source(float impulse(float k, float x) {\n)shader_source"
R"shader_source(	const float h = k * x;\n)shader_source"
R"shader_source(	return h * exp(1. - h);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float linstep(float edge0, float edge1, float x) {\n)shader_source"
R"shader_source(	return clamp((x - edge0) / (edge1 - edge0), 0., 1.);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(/*\n)shader_source"
R"shader_source(Noise - nützlich für fast alles! Daher auch gleich mal ne Menge verschiedenen.\n)shader_source"
R"shader_source(Wir haben klassichen Perlin Noise (cnoise - classical noise), sowie Value Noise (vnoise), jeweils für 2D und 3D.\n)shader_source"
R"shader_source(Perlin Noise ist schicker Gradient Noise, und sieht deshalb viel besser aus. Ist aber auch teurer.\n)shader_source"
R"shader_source(Daher gibts auch noch den schnellen Value Noise, für wenn mans eh nicht sieht.\n)shader_source"
R"shader_source(Außerdem noch fbm Varianten davon (cfbm, vfbm), die mehrere Oktaven kombinieren und ein wenig spannender sind.\n)shader_source"
R"shader_source(Gefühlt kommt vfbm näher an cfbm, als vnoise an cnoise, und cfbm ist noch mal ordentlich teuer.\n)shader_source"
R"shader_source(Der Wertebereich ist jeweils [-1, 1]! (das ist keine Fakultät)\n)shader_source"
R"shader_source(*/\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float mod289(float x) {\n)shader_source"
R"shader_source(  return x - floor(x * (1.0 / 289.0)) * 289.0;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec2 mod289(vec2 x) {\n)shader_source"
R"shader_source(  return x - floor(x * (1.0 / 289.0)) * 289.0;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 mod289(vec3 x) {\n)shader_source"
R"shader_source(  return x - floor(x * (1.0 / 289.0)) * 289.0;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec4 mod289(vec4 x) {\n)shader_source"
R"shader_source(  return x - floor(x * (1.0 / 289.0)) * 289.0;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float permute(float x) {\n)shader_source"
R"shader_source(  return mod289(((x*34.0)+1.0)*x);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec2 permute(vec2 x) {\n)shader_source"
R"shader_source(  return mod289(((x*34.0)+1.0)*x);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 permute(vec3 x) {\n)shader_source"
R"shader_source(  return mod289(((x*34.0)+1.0)*x);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec4 permute(vec4 x) {\n)shader_source"
R"shader_source(  return mod289(((x*34.0)+1.0)*x);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec4 taylorInvSqrt(vec4 r) {\n)shader_source"
R"shader_source(  return 1.79284291400159 - 0.85373472095314 * r;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec2 fade(vec2 t) {\n)shader_source"
R"shader_source(  return t*t*t*(t*(t*6.0-15.0)+10.0);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec3 fade(vec3 t) {\n)shader_source"
R"shader_source(  return t*t*t*(t*(t*6.0-15.0)+10.0);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// see noise section above\n)shader_source"
R"shader_source(float cnoise(vec2 P) {\n)shader_source"
R"shader_source(	vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);\n)shader_source"
R"shader_source(	vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);\n)shader_source"
R"shader_source(	Pi = mod289(Pi); // To avoid truncation effects in permutation\n)shader_source"
R"shader_source(	vec4 ix = Pi.xzxz;\n)shader_source"
R"shader_source(	vec4 iy = Pi.yyww;\n)shader_source"
R"shader_source(	vec4 fx = Pf.xzxz;\n)shader_source"
R"shader_source(	vec4 fy = Pf.yyww;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec4 i = permute(permute(ix) + iy);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;\n)shader_source"
R"shader_source(	vec4 gy = abs(gx) - 0.5 ;\n)shader_source"
R"shader_source(	vec4 tx = floor(gx + 0.5);\n)shader_source"
R"shader_source(	gx = gx - tx;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec2 g00 = vec2(gx.x,gy.x);\n)shader_source"
R"shader_source(	vec2 g10 = vec2(gx.y,gy.y);\n)shader_source"
R"shader_source(	vec2 g01 = vec2(gx.z,gy.z);\n)shader_source"
R"shader_source(	vec2 g11 = vec2(gx.w,gy.w);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));\n)shader_source"
R"shader_source(	g00 *= norm.x;\n)shader_source"
R"shader_source(	g01 *= norm.y;\n)shader_source"
R"shader_source(	g10 *= norm.z;\n)shader_source"
R"shader_source(	g11 *= norm.w;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	float n00 = dot(g00, vec2(fx.x, fy.x));\n)shader_source"
R"shader_source(	float n10 = dot(g10, vec2(fx.y, fy.y));\n)shader_source"
R"shader_source(	float n01 = dot(g01, vec2(fx.z, fy.z));\n)shader_source"
R"shader_source(	float n11 = dot(g11, vec2(fx.w, fy.w));\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec2 fade_xy = fade(Pf.xy);\n)shader_source"
R"shader_source(	vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);\n)shader_source"
R"shader_source(	float n_xy = mix(n_x.x, n_x.y, fade_xy.y);\n)shader_source"
R"shader_source(	return 2.3 * n_xy;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// see noise section above\n)shader_source"
R"shader_source(float cnoise(vec3 P) {\n)shader_source"
R"shader_source(	vec3 Pi0 = floor(P); // Integer part for indexing\n)shader_source"
R"shader_source(	vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1\n)shader_source"
R"shader_source(	Pi0 = mod289(Pi0);\n)shader_source"
R"shader_source(	Pi1 = mod289(Pi1);\n)shader_source"
R"shader_source(	vec3 Pf0 = fract(P); // Fractional part for interpolation\n)shader_source"
R"shader_source(	vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0\n)shader_source"
R"shader_source(	vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);\n)shader_source"
R"shader_source(	vec4 iy = vec4(Pi0.yy, Pi1.yy);\n)shader_source"
R"shader_source(	vec4 iz0 = Pi0.zzzz;\n)shader_source"
R"shader_source(	vec4 iz1 = Pi1.zzzz;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec4 ixy = permute(permute(ix) + iy);\n)shader_source"
R"shader_source(	vec4 ixy0 = permute(ixy + iz0);\n)shader_source"
R"shader_source(	vec4 ixy1 = permute(ixy + iz1);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec4 gx0 = ixy0 * (1.0 / 7.0);\n)shader_source"
R"shader_source(	vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;\n)shader_source"
R"shader_source(	gx0 = fract(gx0);\n)shader_source"
R"shader_source(	vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);\n)shader_source"
R"shader_source(	vec4 sz0 = step(gz0, vec4(0.0));\n)shader_source"
R"shader_source(	gx0 -= sz0 * (step(0.0, gx0) - 0.5);\n)shader_source"
R"shader_source(	gy0 -= sz0 * (step(0.0, gy0) - 0.5);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec4 gx1 = ixy1 * (1.0 / 7.0);\n)shader_source"
R"shader_source(	vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;\n)shader_source"
R"shader_source(	gx1 = fract(gx1);\n)shader_source"
R"shader_source(	vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);\n)shader_source"
R"shader_source(	vec4 sz1 = step(gz1, vec4(0.0));\n)shader_source"
R"shader_source(	gx1 -= sz1 * (step(0.0, gx1) - 0.5);\n)shader_source"
R"shader_source(	gy1 -= sz1 * (step(0.0, gy1) - 0.5);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);\n)shader_source"
R"shader_source(	vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);\n)shader_source"
R"shader_source(	vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);\n)shader_source"
R"shader_source(	vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);\n)shader_source"
R"shader_source(	vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);\n)shader_source"
R"shader_source(	vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);\n)shader_source"
R"shader_source(	vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);\n)shader_source"
R"shader_source(	vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));\n)shader_source"
R"shader_source(	g000 *= norm0.x;\n)shader_source"
R"shader_source(	g010 *= norm0.y;\n)shader_source"
R"shader_source(	g100 *= norm0.z;\n)shader_source"
R"shader_source(	g110 *= norm0.w;\n)shader_source"
R"shader_source(	vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));\n)shader_source"
R"shader_source(	g001 *= norm1.x;\n)shader_source"
R"shader_source(	g011 *= norm1.y;\n)shader_source"
R"shader_source(	g101 *= norm1.z;\n)shader_source"
R"shader_source(	g111 *= norm1.w;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	float n000 = dot(g000, Pf0);\n)shader_source"
R"shader_source(	float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));\n)shader_source"
R"shader_source(	float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));\n)shader_source"
R"shader_source(	float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));\n)shader_source"
R"shader_source(	float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));\n)shader_source"
R"shader_source(	float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));\n)shader_source"
R"shader_source(	float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));\n)shader_source"
R"shader_source(	float n111 = dot(g111, Pf1);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec3 fade_xyz = fade(Pf0);\n)shader_source"
R"shader_source(	vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);\n)shader_source"
R"shader_source(	vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);\n)shader_source"
R"shader_source(	float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);\n)shader_source"
R"shader_source(	return 2.2 * n_xyz;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float rand(vec2 c) {\n)shader_source"
R"shader_source(    vec2 m = mod289(c);\n)shader_source"
R"shader_source(	vec2 h = permute(m);\n)shader_source"
R"shader_source(    return fract(permute(h.x * h.y + m.x + m.y)/41.) * 2. - 1.;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float rand(vec3 c) {\n)shader_source"
R"shader_source(    vec3 m = mod289(c);\n)shader_source"
R"shader_source(	vec3 h = permute(m);\n)shader_source"
R"shader_source(    return fract(permute(h.x * h.y * h.z + m.x + m.y + m.z)/41.) * 2. - 1.;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// see noise section above\n)shader_source"
R"shader_source(float vnoise(vec2 c) {\n)shader_source"
R"shader_source(	vec2 c0 = floor(c);\n)shader_source"
R"shader_source(    vec2 t = fract(c);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(    vec2 o = vec2(1., 0.);\n)shader_source"
R"shader_source(    float v00 = rand(c0 + o.yy);\n)shader_source"
R"shader_source(    float v01 = rand(c0 + o.yx);\n)shader_source"
R"shader_source(    float v10 = rand(c0 + o.xy);\n)shader_source"
R"shader_source(    float v11 = rand(c0 + o.xx);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(    t = fade(t);\n)shader_source"
R"shader_source(    return mix(mix(v00, v10, t.x), mix(v01, v11, t.x), t.y);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// see noise section above\n)shader_source"
R"shader_source(float vnoise(vec3 c) {\n)shader_source"
R"shader_source(	vec3 c0 = floor(c);\n)shader_source"
R"shader_source(    vec3 t = fract(c);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(    vec2 o = vec2(1., 0.);\n)shader_source"
R"shader_source(    float v000 = rand(c0 + o.yyy);\n)shader_source"
R"shader_source(    float v001 = rand(c0 + o.yyx);\n)shader_source"
R"shader_source(    float v010 = rand(c0 + o.yxy);\n)shader_source"
R"shader_source(    float v011 = rand(c0 + o.yxx);\n)shader_source"
R"shader_source(    float v100 = rand(c0 + o.xyy);\n)shader_source"
R"shader_source(    float v101 = rand(c0 + o.xyx);\n)shader_source"
R"shader_source(    float v110 = rand(c0 + o.xxy);\n)shader_source"
R"shader_source(    float v111 = rand(c0 + o.xxx);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	t = fade(t);\n)shader_source"
R"shader_source(	return mix(\n)shader_source"
R"shader_source(		mix(\n)shader_source"
R"shader_source(			mix(v000, v100, t.x),\n)shader_source"
R"shader_source(			mix(v010, v110, t.x),\n)shader_source"
R"shader_source(			t.y),\n)shader_source"
R"shader_source(		mix(\n)shader_source"
R"shader_source(			mix(v001, v101, t.x),\n)shader_source"
R"shader_source(			mix(v011, v111, t.x),\n)shader_source"
R"shader_source(			t.y),\n)shader_source"
R"shader_source(		t.z);\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// see noise section above\n)shader_source"
R"shader_source(float cfbm(vec2 c) {\n)shader_source"
R"shader_source(	return (cnoise(c) + cnoise(c * 2.) * .5 + cnoise(c * 4.) * .25)/1.75;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// see noise section above\n)shader_source"
R"shader_source(float cfbm(vec3 c) {\n)shader_source"
R"shader_source(	return (cnoise(c) + cnoise(c * 2.) * .5 + cnoise(c * 4.) * .25)/1.75;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// see noise section above\n)shader_source"
R"shader_source(float vfbm(vec2 c) {\n)shader_source"
R"shader_source(	return (vnoise(c) + vnoise(c * 2.) * .5 + vnoise(c * 4.) * .25)/1.75;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(// see noise section above\n)shader_source"
R"shader_source(float vfbm(vec3 c) {\n)shader_source"
R"shader_source(	return (vnoise(c) + vnoise(c * 2.) * .5 + vnoise(c * 4.) * .25)/1.75;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(float ao(vec3 p, vec3 n, float d, float i) {\n)shader_source"
R"shader_source(	float o, s = sign(d);\n)shader_source"
R"shader_source(	for(o = s * .5 + .5; i > 0; i--) {\n)shader_source"
R"shader_source(		o -= (i * d - f(p + n * i * d * s)[0]) / exp2(i);\n)shader_source"
R"shader_source(	}\n)shader_source"
R"shader_source(	return o;\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(#line 4\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(layout(location = 66) uniform vec3 synapse_color; // color\n)shader_source"
R"shader_source(layout(location = 67) uniform float synapse_gap; // float\n)shader_source"
R"shader_source(layout(location = 68) uniform float synapse_aa; // float\n)shader_source"
R"shader_source(layout(location = 69) uniform float synapse_bb; // float\n)shader_source"
R"shader_source(layout(location = 70) uniform float synapse_cc; // float\n)shader_source"
R"shader_source(layout(location = 71) uniform float synapse_dd; // float\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(Material materials[2] = Material[2](\n)shader_source"
R"shader_source(	Material(vec3(1.), .5, 0.),\n)shader_source"
R"shader_source(	Material(synapse_color, 0.7, 1.)\n)shader_source"
R"shader_source();\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(const float MATERIAL_ID_BOUNDING = 0.;\n)shader_source"
R"shader_source(const float MATERIAL_ID_SYNAPSE = 1.;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(void main(void) {\n)shader_source"
R"shader_source(	vec3 direction = get_direction();\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	int i;\n)shader_source"
R"shader_source(	vec3 hit = march_adv(camera_position, direction, i, 100, .7);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec3 color = vec3(0.);\n)shader_source"
R"shader_source(	if(i < 150) {\n)shader_source"
R"shader_source(		int material = int(f(hit).y);\n)shader_source"
R"shader_source(		vec3 normal = calc_normal(hit);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(		Material mat = materials[material];\n)shader_source"
R"shader_source(		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));\n)shader_source"
R"shader_source(	}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	output_color(color, distance(hit, camera_position));\n)shader_source"
R"shader_source(}\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(vec2 f(vec3 p) {\n)shader_source"
R"shader_source(	float domrep_x = 100.;\n)shader_source"
R"shader_source(	float domrep_z = 60.;\n)shader_source"
R"shader_source(	vec3 q = domrep(p, domrep_x, 0., domrep_z);\n)shader_source"
R"shader_source(	q.y = p.y;\n)shader_source"
R"shader_source(	q.x = abs(q.x);\n)shader_source"
R"shader_source(	q.z += 5.;\n)shader_source"
R"shader_source(	q.x -= 10 * synapse_gap;\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	float sphere1 = sphere(q, 1.);\n)shader_source"
R"shader_source(	q.x -= 2.;\n)shader_source"
R"shader_source(	float sphere2 = sphere(q, 2.);\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	q = rZ(-TAU*sin(q.x/5.)/30.) * q;\n)shader_source"
R"shader_source(	q = rY(-TAU*sin(q.x/2.+time*0.2)/50.)*q;\n)shader_source"
R"shader_source(	float capsule = line(q, vec3(2.,0.,0.), vec3(20.,0.,0.), mix(synapse_aa, synapse_bb, smoothstep(10.*synapse_cc, 10*synapse_dd, q.x)));\n)shader_source"
R"shader_source(\n)shader_source"
R"shader_source(	vec2 synapse = vec2(smin(smax(-sphere1, sphere2, 1.), capsule, 1.), MATERIAL_ID_SYNAPSE);\n)shader_source"
R"shader_source(	vec2 bounding = vec2(-sphere(p - camera_position, 500.), MATERIAL_ID_BOUNDING);\n)shader_source"
R"shader_source(	return min_material(synapse, bounding);\n)shader_source"
R"shader_source(}\n)shader_source"
;
#endif
