// fancy functions TODO

uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat

uniform float time;

#ifndef FOCAL_LENGTH
uniform float focal_length;
#endif

float TAU = 6.28318530718;

/*
mat3 get_camera() {
	vec3 view_right = cross(camera_direction, camera_up);
	return mat3(view_right, camera_up, -camera_direction);
}
*/

// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
vec3 quat_rotate(vec3 v, vec4 q) {
	vec3 t = 2 * cross(q.xyz, v);
	return v + q.w * t + cross(q.xyz, t);
	// *hex hex*
}

vec3 get_direction() {
	vec3 dir = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.x , -focal_length / .03));
	return quat_rotate(dir, camera_rotation);
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
	return slowbox(p, b - r) - r;
}

// c must be normalized
float cone(vec3 p, vec2 c) {
	float q = length(p.xy);
	return dot(c, vec2(q, p.z));
}

// n must be normalized
float plane(vec3 p, vec3 n) {
	return dot(p, n.xyz);
}

float cylinder(vec3 p, float radius, float thicknesshalf) {
	float circle = length(p.xy) - radius;
	return max(circle, abs(p.z) - thicknesshalf);
}

// hier kommt der witz!
vec2 min_material(vec2 a, vec2 b) {
	return mix(a, b, float(a.x > b.x));
}

// smooth minimum, k is the difference between the two values for which to smooth (eg. k = 0.1)
float smin(float a, float b, float k) {
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );
	return mix(b, a, h) - k * h * (1.0 - h);
}

vec2 smin_material(vec2 a, vec2 b, float k) {
	return vec2(smin(a.x, b.x, k), a.x > b.x ? b.y : a.y);
}

// be careful when nesting! (just don't)
vec2 smin_smaterial(vec2 a, vec2 b, float k) {
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );
	return vec2(mix(b.x, a.x, h) - k * h * (1.0 - h), mix(b.y, a.y, h));
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

vec3 emit_light(vec3 color, float intensity) {
	return color * intensity;
}

vec3 domrepv(vec3 p, vec3 c) {
	return mod(p, c) - .5 * c;
}

// repeat things
vec3 domrep(vec3 p, float x, float y, float z) {
	return domrepv(p, vec3(x, y, z));
}

// f: distance function to object
// p: evaluation point
// s: scale factor
#define scale(f, p, s) f((p)/(s))*(s)

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

mat2 rot2D(float theta) {
	return mat2(cos(theta), -sin(theta), sin(theta), cos(theta));
}

// impulse from iq's website
// k is the sharpness of the curve
float impulse(float k, float x) {
	const float h = k * x;
	return h * exp(1. - h);
}

float linstep(float edge0, float edge1, float x) {
	return clamp((x - edge0) / (edge1 - edge0), 0., 1.);
}


/*
Noise - nützlich für fast alles! Daher auch gleich mal ne Menge verschiedenen.
Wir haben klassichen Perlin Noise (cnoise - classical noise), sowie Value Noise (vnoise), jeweils für 2D und 3D.
Perlin Noise ist schicker Gradient Noise, und sieht deshalb viel besser aus. Ist aber auch teurer.
Daher gibts auch noch den schnellen Value Noise, für wenn mans eh nicht sieht.
Außerdem noch fbm Varianten davon (cfbm, vfbm), die mehrere Oktaven kombinieren und ein wenig spannender sind.
Gefühlt kommt vfbm näher an cfbm, als vnoise an cnoise, und cfbm ist noch mal ordentlich teuer.
Der Wertebereich ist jeweils [-1, 1]! (das ist keine Fakultät)
*/


float mod289(float x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float permute(float x) {
  return mod289(((x*34.0)+1.0)*x);
}

vec2 permute(vec2 x) {
  return mod289(((x*34.0)+1.0)*x);
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

vec4 permute(vec4 x) {
  return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r) {
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

vec3 fade(vec3 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// see noise section above
float cnoise(vec2 P) {
	vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
	vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
	Pi = mod289(Pi); // To avoid truncation effects in permutation
	vec4 ix = Pi.xzxz;
	vec4 iy = Pi.yyww;
	vec4 fx = Pf.xzxz;
	vec4 fy = Pf.yyww;

	vec4 i = permute(permute(ix) + iy);

	vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
	vec4 gy = abs(gx) - 0.5 ;
	vec4 tx = floor(gx + 0.5);
	gx = gx - tx;

	vec2 g00 = vec2(gx.x,gy.x);
	vec2 g10 = vec2(gx.y,gy.y);
	vec2 g01 = vec2(gx.z,gy.z);
	vec2 g11 = vec2(gx.w,gy.w);

	vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
	g00 *= norm.x;
	g01 *= norm.y;
	g10 *= norm.z;
	g11 *= norm.w;

	float n00 = dot(g00, vec2(fx.x, fy.x));
	float n10 = dot(g10, vec2(fx.y, fy.y));
	float n01 = dot(g01, vec2(fx.z, fy.z));
	float n11 = dot(g11, vec2(fx.w, fy.w));

	vec2 fade_xy = fade(Pf.xy);
	vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
	float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
	return 2.3 * n_xy;
}

// see noise section above
float cnoise(vec3 P) {
	vec3 Pi0 = floor(P); // Integer part for indexing
	vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
	Pi0 = mod289(Pi0);
	Pi1 = mod289(Pi1);
	vec3 Pf0 = fract(P); // Fractional part for interpolation
	vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
	vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
	vec4 iy = vec4(Pi0.yy, Pi1.yy);
	vec4 iz0 = Pi0.zzzz;
	vec4 iz1 = Pi1.zzzz;

	vec4 ixy = permute(permute(ix) + iy);
	vec4 ixy0 = permute(ixy + iz0);
	vec4 ixy1 = permute(ixy + iz1);

	vec4 gx0 = ixy0 * (1.0 / 7.0);
	vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
	gx0 = fract(gx0);
	vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
	vec4 sz0 = step(gz0, vec4(0.0));
	gx0 -= sz0 * (step(0.0, gx0) - 0.5);
	gy0 -= sz0 * (step(0.0, gy0) - 0.5);

	vec4 gx1 = ixy1 * (1.0 / 7.0);
	vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
	gx1 = fract(gx1);
	vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
	vec4 sz1 = step(gz1, vec4(0.0));
	gx1 -= sz1 * (step(0.0, gx1) - 0.5);
	gy1 -= sz1 * (step(0.0, gy1) - 0.5);

	vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
	vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
	vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
	vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
	vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
	vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
	vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
	vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

	vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
	g000 *= norm0.x;
	g010 *= norm0.y;
	g100 *= norm0.z;
	g110 *= norm0.w;
	vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
	g001 *= norm1.x;
	g011 *= norm1.y;
	g101 *= norm1.z;
	g111 *= norm1.w;

	float n000 = dot(g000, Pf0);
	float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
	float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
	float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
	float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
	float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
	float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
	float n111 = dot(g111, Pf1);

	vec3 fade_xyz = fade(Pf0);
	vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
	vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
	float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
	return 2.2 * n_xyz;
}

float rand(vec2 c) {
    vec2 m = mod289(c);
	vec2 h = permute(m);
    return fract(permute(h.x * h.y + m.x + m.y)/41.) * 2. - 1.;
}

float rand(vec3 c) {
    vec3 m = mod289(c);
	vec3 h = permute(m);
    return fract(permute(h.x * h.y * h.z + m.x + m.y + m.z)/41.) * 2. - 1.;
}

// see noise section above
float vnoise(vec2 c) {
	vec2 c0 = floor(c);
    vec2 t = fract(c);

    vec2 o = vec2(1., 0.);
    float v00 = rand(c0 + o.yy);
    float v01 = rand(c0 + o.yx);
    float v10 = rand(c0 + o.xy);
    float v11 = rand(c0 + o.xx);

    t = fade(t);
    return mix(mix(v00, v10, t.x), mix(v01, v11, t.x), t.y);
}

// see noise section above
float vnoise(vec3 c) {
	vec3 c0 = floor(c);
    vec3 t = fract(c);

    vec2 o = vec2(1., 0.);
    float v000 = rand(c0 + o.yyy);
    float v001 = rand(c0 + o.yyx);
    float v010 = rand(c0 + o.yxy);
    float v011 = rand(c0 + o.yxx);
    float v100 = rand(c0 + o.xyy);
    float v101 = rand(c0 + o.xyx);
    float v110 = rand(c0 + o.xxy);
    float v111 = rand(c0 + o.xxx);

	t = fade(t);
	return mix(
		mix(
			mix(v000, v100, t.x),
			mix(v010, v110, t.x),
			t.y),
		mix(
			mix(v001, v101, t.x),
			mix(v011, v111, t.x),
			t.y),
		t.z);
}

// see noise section above
float cfbm(vec2 c) {
	return (cnoise(c) + cnoise(c * 2.) * .5 + cnoise(c * 4.) * .25)/1.75;
}

// see noise section above
float cfbm(vec3 c) {
	return (cnoise(c) + cnoise(c * 2.) * .5 + cnoise(c * 4.) * .25)/1.75;
}

// see noise section above
float vfbm(vec2 c) {
	return (vnoise(c) + vnoise(c * 2.) * .5 + vnoise(c * 4.) * .25)/1.75;
}

// see noise section above
float vfbm(vec3 c) {
	return (vnoise(c) + vnoise(c * 2.) * .5 + vnoise(c * 4.) * .25)/1.75;
}

float ao(vec3 p, vec3 n, float d, float i) {
	float o, s = sign(d);
	for(o = s * .5 + .5; i > 0; i--) {
		o -= (i * d - f(p + n * i * d * s)[0]) / exp2(i);
	}
	return o;
}
