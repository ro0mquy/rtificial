// fancy functions TODO

uniform vec3 camera_position;
uniform vec3 camera_direction;
uniform vec3 camera_up;

float TAU = 6.28318530718;

// TODO camera FOV
mat3 get_camera() {
	vec3 view_right = cross(camera_direction, camera_up);
	return mat3(view_right, camera_up, -camera_direction);
}

vec3 get_direction() {
	return get_camera() * normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y , -1.));
}

vec2 f(vec3 p);

vec3 march_adv(vec3 p, vec3 direction, out int i, int iterations, float stepsize) {
	float walked = 0.;
	for (i=0; i < iterations; i++) {
		float dist = f(p)[0] * stepsize;
		p += direction * dist;
		dist = abs(dist);
		walked += dist;

		if (dist < .001 * walked) break;
	}
	return p;
}

vec3 march(vec3 p, vec3 direction, out int i) {
	return march_adv(p, direction, i, 100, 1.);
}

vec3 calc_normal(vec3 p) {
	vec2 epilepsilon = vec2(.001, 0.);
	return normalize(vec3(
		f(p + epilepsilon.xyy)[0] - f(p - epilepsilon.xyy)[0],
		f(p + epilepsilon.yxy)[0] - f(p - epilepsilon.yxy)[0],
		f(p + epilepsilon.yyx)[0] - f(p - epilepsilon.yyx)[0]
	));
}

float sphere(vec3 p, float s) {
	return length(p) - s;
}

// a and b are the endpoints
// r is the radius if you want some kind of capsule
float line(vec3 p, vec3 a, vec3 b, float r) {
	vec3 pa = p - a;
	vec3 ba = b - a;
	float h = clamp(dot(pa,ba) / dot(ba,ba), 0., 1. );
	return length(pa - ba*h) - r;
}

float torus(vec3 p, vec2 t) {
	vec2 q = vec2(length(p.xz) - t.x, p.y);
	return length(q) - t.y;
}

float box(vec3 p, vec3 b) {
	p = abs(p) - b;
	return max(p.x, max(p.y, p.z));
}

float box2(vec2 p, vec2 b) {
	p = abs(p) - b;
	return max(p.x, p.y);
}

// more accurate than box(), but slower
float slowbox(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));
}

// box with rounded corners, r is radius of corners
float roundbox(vec3 p, vec3 b, float r) {
	return slowbox(p, b) - r;
}

// hier kommt der witz!
vec2 min_material(vec2 a, vec2 b) {
	return mix(a, b, a.x > b.x);
}

// smooth minimum, k is the difference between the two values for which to smooth (eg. k = 0.1)
float smin(float a, float b, float k) {
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );
	return mix(b, a, h) - k * h * (1.0 - h);
}

// smooth maximum, k is the difference between the two values for which to smooth (eg. k = 0.1)
float smax(float a, float b, float k) {
	float h = clamp(0.5 - 0.5 * (b - a) / k, 0.0, 1.0 );
	return mix(b, a, h) + k * h * (1.0 - h);
}

struct Material {
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

// positive dot product
float pdot(vec3 a, vec3 b) {
	return max(0., dot(a, b));
}

float square(float a) {
	return a * a;
}

vec3 apply_light(vec3 p, vec3 N, vec3 V, Material material, SphereLight light) {
	vec3 L = light.center - p;

	// calculate representative point
	vec3 r = reflect(-V, N);
	vec3 centerToRay = dot(L, r) * r - L;
	vec3 closestPoint = L + centerToRay * clamp(light.radius/length(centerToRay), 0., 1.);
	vec3 L_spec = normalize(closestPoint);

	// specular
	float PI = acos(-1.);
	vec3 H = .5 * (L_spec + V);
	float NdotH = dot(N, H);
	float NdotL= pdot(N, L_spec);
	float NdotV = dot(N, V);
	float alpha = square(material.roughness);
	float alpha2 = square(alpha);
	float k = square(material.roughness + 1.);
	float specular_without_F = NdotL * alpha2 / (4. * mix(NdotL, 1., k) * mix(NdotV, 1., k) * PI * square(square(NdotH) * (alpha2 - 1.) + 1.));
	float alphastrich = clamp(alpha + light.radius * .5 / length(closestPoint), 0., 1.);
	float spec_norm = alpha2 / (alphastrich * alphastrich);

	float VdotH = dot(V, H);
	float fresnel_power = exp2((-5.55473 * VdotH - 6.98316) * VdotH);
	float F_dielectric = .04 + (1. - .04) * fresnel_power;
	vec3 F_metal = material.color + (1. - material.color) * fresnel_power;

	vec3 dielectric_color = .5 * (material.color / PI * NdotL + specular_without_F * F_dielectric);
	vec3 metal_color = specular_without_F * F_metal;
	float light_distance = distance(p, closestPoint);
	float foo = square(1. - square(square(light_distance / light.radius)));
	float falloff = clamp(foo, 0., 1.) / (square(light_distance) + 1.);
	return mix(dielectric_color, metal_color, material.metallic) * falloff * light.intensity * light.color * spec_norm;
}

vec3 domrepv(vec3 p, vec3 c) {
	return mod(p, c) - .5 * c;
}

// repeat things
vec3 domrep(vec3 p, float x, float y, float z) {
	return domrepv(p, vec3(x, y, z));
}

// trans*late things - using vectors!!
// p: point
// v: translation vector
vec3 transv(vec3 p, vec3 v) {
	return p - v;
}

// trans*late things
// p: point
// x: x
// y: y
// z: z
vec3 trans(vec3 p, float x, float y, float z) {
	return transv(p, vec3(x, y, z));
}

mat3 rX(float theta) {
	return mat3(
		1., 0., 0.,
		0., cos(theta), sin(theta),
		0., -sin(theta), cos(theta)
	);
}

mat3 rY(float theta) {
	return mat3(
		cos(theta), 0., -sin(theta),
		0., 1., 0.,
		sin(theta), 0., cos(theta)
	);
}

mat3 rZ(float theta) {
	return mat3(
		cos(theta), sin(theta), 0.,
		-sin(theta), cos(theta), 0.,
		0., 0., 1.
	);
}

