#ifndef conicbobbel_H
#define conicbobbel_H
const char conicbobbel_source[] = R"shader_source(#version 430

layout(location = 0) uniform vec2 res;
layout(location = 1) uniform float envelopes[32];

layout(location = 0) out vec3 out_color; // alpha = CoC
layout(location = 1) out float coc;

layout(location = 48) uniform float focus_dist;

layout(location = 38) uniform float focal_length;
#define FOCAL_LENGTH

layout(location = 34) uniform float f_stop;

void output_color(vec3 color, float dist) {
	float focus_dist = focus_dist;
	float f = focal_length;
	float N = f_stop;
	coc = (dist - focus_dist)/dist * (f * f) / (N * (focus_dist - f)) / 0.03 * res.x;
	out_color = color;
}

#line 1

// fancy functions TODO

layout(location = 35) uniform vec3 camera_position;
layout(location = 36) uniform vec4 camera_rotation;

layout(location = 37) uniform float time;

#ifndef FOCAL_LENGTH
layout(location = 38) uniform float focal_length;
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
	) + 1e-9);
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
	return vec2(smin(a, b, k), a.x > b.x ? b.y : a.y);
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

#line 4

layout(location = 49) uniform vec3 conic_bobbel_color; // color
layout(location = 50) uniform vec3 conic_ring_color; // color
layout(location = 51) uniform vec3 conic_lampe_color; // color

layout(location = 52) uniform float conic_smooth_factor; // float
layout(location = 53) uniform float conic_ring_intensity; // float
layout(location = 54) uniform float conic_bobbel_noifreq;
layout(location = 55) uniform float conic_bobbel_roughness;
layout(location = 56) uniform float conic_ring_animation;
layout(location = 57) uniform float conic_bobbel_xcoord;

vec3 colors[5] = vec3[5](
		vec3(.0),
		vec3(.03, .0, .0),
		conic_lampe_color,
		conic_bobbel_color,
		conic_ring_color
		);

const float material_bounding = 0.;
const float material_boden    = 1.;
const float material_lampe    = 2.;
const float material_bobbel   = 3.;
const float material_ring     = 4.;
)shader_source"
R"shader_source(void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march_adv(camera_position, direction, i, 100, 1.);

	vec3 color = vec3(0.);
	float materialId = f(hit)[1];
	vec3 normal = calc_normal(hit);

	SphereLight light1 = SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.);
	if(materialId == material_boden) {
		float size = 2.;
		float stripes = mod(floor(hit.x / size), 2.);
		Material mat = Material(colors[int(material_boden)] * stripes, 0.5, 0.);
		color = apply_light(hit, normal, -direction, mat, light1);
	} else if(materialId == material_bounding) {
		Material mat = Material(colors[int(material_bounding)], 1., 0.);
		color = apply_light(hit, normal, -direction, mat, light1);
	} else if (materialId >= material_lampe && materialId <= material_bobbel) {
		Material material1 = Material(colors[int(material_lampe)], 0.2, 1.);
		vec3 color1 = apply_light(hit, normal, -direction, material1, light1);
		Material material2 = Material(colors[int(material_bobbel)], 0.2, 1.);
		vec3 color2 = apply_light(hit, normal, -direction, material2, light1);
		float mixfactor = pow(materialId - material_lampe, 3.); // change the exponent for sharpness of transition
		color = mix(color1, color2, mixfactor);
	} else if (materialId >= material_bobbel && materialId <= material_ring) {
		Material material1 = Material(colors[int(material_bobbel)], 0.2, 1.);
		vec3 color1 = apply_light(hit, normal, -direction, material1, light1);
		vec3 color2 = emit_light(colors[int(material_ring)], conic_ring_intensity);
		float mixfactor = pow(materialId - material_bobbel, 6.); // change the exponent for sharpness of transition
		color = mix(color1, color2, mixfactor);
	} else {
		Material mat = Material(colors[int(materialId)], 0.2, 1.);
		color = apply_light(hit, normal, -direction, mat, light1);
	}

	output_color(color, 4.01);//distance(hit, camera_position));
}

float conicbobbel(vec3 p_cone, float l_body) {
	////// distfunctions for body (some day in the past it was a cone)
	float radius_mixer = smoothstep(l_body * .3, l_body * 1., p_cone.x)
		+ .3 * (1. - smoothstep(l_body * .0, l_body * .3, p_cone.x)); // used to mix between min_ and max_radius
	float max_radius = .5;
	float min_radius = .15;
	float line_radius = mix(min_radius, max_radius, radius_mixer);;
	float f_cone = line(p_cone, vec3(l_body, 0., 0.), vec3(0., 0., 0.), line_radius);

	////// akward spikes
	float num_spikes = 8.;
	float height_spikes = .8;
	float sharpness_spikes = 7.;
	// move where the center of the spikes should be
	vec3 p_spike = trans(p_cone, l_body * .9, 0., 0.);
	// make cylindrical domrep; x -> x, y -> radius, z -> angle
	p_spike.yz = vec2(length(p_spike.yz), atan(p_spike.y, p_spike.z) / TAU);
	vec3 p_pre_spike = p_spike;
	p_spike = domrep(p_spike, 1., 1., 1./num_spikes);
	p_spike.xy = p_pre_spike.xy; // don't domrep x and radius
	p_spike.y -= height_spikes;
	p_spike.z *= 3.2;
	// you may uncomment following rotation line, but then there will be lots of ugly artefacts, if you do, then change the center of the spikes to x = l_body * .9
	p_spike = rZ(TAU * -(min(p_spike.y, .0) + 1.) / 7.)* p_spike;
	float f_spike = cone(p_spike.xzy, normalize(vec2(sharpness_spikes, 1.)));
	f_cone = smin(f_cone, f_spike, .1);

	////// perlin make some noise!
	vec2 surface_coord = vec2(p_cone.x * 3. + time, atan(p_cone.y, p_cone.z));
	float noise_amplitude = smoothstep(-.6, 0., p_cone.x) * (1. - smoothstep(l_body + .3, l_body + .5, p_cone.x)) * .03;
	f_cone -= noise_amplitude * smoothstep(conic_bobbel_roughness, 1., cnoise(surface_coord * conic_bobbel_noifreq)); // smoothstep cuts the lower half of the noise to zero, -1 and 0 are both good values for conic_bobbel_roughness

	return f_cone;
}

float bobbelring(vec3 p_cone, float l_body, float factorDeltaT) {
	////// big, bright rings
	float T = 1.; // duration of one animation cyclus
	float deltaT = T * factorDeltaT; // delta between first and second ring
	float bgn_rng_anim = l_body + .7; // start point of ring animation
	float end_rng_anim = -1.; // end point of ring anmation
	float max_ring_amp = 1.5; // maximum ring amplitude
	vec3 p_ring = trans(p_cone, bgn_rng_anim + (end_rng_anim - bgn_rng_anim) * mod(conic_ring_animation + deltaT, T) / T, 0., 0.);
	float ring_radius = max_ring_amp * impulse(8., mod(conic_ring_animation + deltaT, T) / T); // first arg is impulse shape
	float ring_r_thickness = .2 * ring_radius; // thickness of ring in r direction
	float ring_x_thickness = .03; // thickness of ring in x direction
	float f_ring = 0.;
	f_ring = abs(sphere(p_ring, ring_radius)) - ring_r_thickness;
	f_ring = smax(f_ring, abs(p_ring.x) - ring_x_thickness, .02);

	return f_ring;
}

vec2 f(vec3 p) {
	vec3 p_bobbel = trans(p, conic_bobbel_xcoord, 0., -3.);

	vec3 s_domrep = vec3(30, 1., 25.); // domrep cell size, 1. probably means no domrep
	vec3 p_pre_cone = trans(p_bobbel, 10. * time, 0., 0.); // move with time
	// find the current cell
	float cell_x = floor(p_pre_cone.x / s_domrep.x);
	float cell_y = floor(p_pre_cone.y / s_domrep.y);
	float cell_z = floor(p_pre_cone.z / s_domrep.z);
	// move single cells a bit sidewards, maybe amplitude should be smaller than domrep cell
	p_pre_cone = trans(p_pre_cone,
			sin(cell_z * cell_z) * 32.,
			0.,
			sin(cell_x * cell_x) * 50.);
	vec3 p_cone = domrepv(p_pre_cone, s_domrep);
	p_cone.y = p_pre_cone.y; // dont't domrep y
	p_cone = p_bobbel; // remove for repetition and movement

	// get bobbel body and rings
	float l_body = 2.; // length of body
	float f_cone = conicbobbel(p_cone, l_body);
	float f_ring = bobbelring(p_cone, l_body, 0.);
	//f_ring = min(f_ring, bobbelring(p_cone, l_body, 1/2.5));

	////// assembling and boundings
	vec2 m_cone = vec2(f_cone, material_bobbel);
	vec2 m_ring = vec2(f_ring, material_ring);
	vec2 m_bobbel = smin_smaterial(m_cone, m_ring, conic_smooth_factor);

	// lampen sphere and tunnel for bobbel
	float radius_lampe = 10.;
	vec3 p_lampe = trans(p, -15, 0, -3);
	float f_lampe = sphere(p_lampe, radius_lampe);

	vec3 p_delle = p_bobbel;
	float f_delle = line(p_delle, vec3(1.7, 0., 0.), vec3(.1), 1.);
	f_lampe = smax(f_lampe, -f_delle, 1.);

	vec2 m_lampe = vec2(f_lampe, material_lampe);

	vec2 m_bobbel_lampe = smin_smaterial(m_bobbel, m_lampe, 1.);

	vec2 m_content = m_bobbel_lampe;
	vec2 bottom = vec2(p.y + 20., material_boden);
	vec2 bounding = vec2(-sphere(p - camera_position, 300.), material_bounding);
	return min_material(m_content, min_material(bottom, bounding));
}
)shader_source";
#endif
