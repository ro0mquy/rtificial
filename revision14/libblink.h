static const char libblink_source[] = "\
#version 400\n\
\n\
uniform vec3 view_position;\n\
uniform vec3 view_up;\n\
uniform vec3 view_direction;\n\
uniform vec2 res;\n\
uniform float time;\n\
uniform int notes[32];\n\
uniform float envelopes[32];\n\
uniform float aenvelopes[32];\n\
uniform float senvelopes[32];\n\
\n\
layout(location = 0) out vec4 out_color;\n\
layout(location = 1) out float out_depth;\n\
\n\
float TAU = 6.28318530718;\n\
\n\
mat3 get_camera() {\n\
	vec3 view_right = cross(view_direction, view_up);\n\
	return mat3(view_right, view_up, -view_direction);\n\
}\n\
\n\
vec3 get_direction() {\n\
	return get_camera() * normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y , -1.));\n\
}\n\
\n\
#define DECLARE_MARCH(name) vec3 name(vec3, vec3, out int);\n\
#define DEFINE_MARCH(name, f) \\\n\
vec3 name(vec3 p, vec3 direction, out int i) {\\\n\
	float walked = 0.;\\\n\
	for (i=0; i < 150; i++) {\\\n\
		float dist = f(p)[0];\\\n\
		p += direction * dist;\\\n\
		dist = abs(dist);\\\n\
		walked += dist;\\\n\
\\\n\
		if (dist < .001 * walked) break;\\\n\
	}\\\n\
	return p;\\\n\
}\n\
\n\
#define DECLARE_SHADOW_RAY(name) float name(vec3, vec3);\n\
#define DEFINE_SHADOW_RAY(name, f) \\\n\
float name(vec3 hit, vec3 light) {\\\n\
	vec3 dir = normalize(hit - light);\\\n\
	vec3 p = light;\\\n\
	float walked = 0.;\\\n\
	for(int i = 0; i < 100; i++) {\\\n\
		float dist = f(p)[0];\\\n\
		p += dir * dist;\\\n\
		dist = abs(dist);\\\n\
		walked += dist;\\\n\
		if(dist < .001 * walked) break;\\\n\
	}\\\n\
	return float((distance(hit, light) - distance(light, p)) < .1);\\\n\
}\n\
\n\
// iq's softshadows with penumbra\n\
// light: position of light\n\
// hit: the point hit by raymarching\n\
// hardness: the hardness of the shadow (2, 8, 32, 128 are good values)\n\
#define DECLARE_SOFTSHADOW(name) float name(vec3, vec3, float);\n\
#define DEFINE_SOFTSHADOW(name, f) \\\n\
float softshadow(vec3 hit, vec3 light, float hardness) {\\\n\
	vec3 direction = normalize(light - hit);\\\n\
	float max_travel = .95 * distance(hit, light);\\\n\
	float traveled = .001;\\\n\
	float res = 1.;\\\n\
\\\n\
	for (; traveled < max_travel;) {\\\n\
		float dist = f(hit + traveled*direction)[0];\\\n\
\\\n\
		/* if doing small steps, we hit something */ \\\n\
		if (dist < 0.01 * traveled) {\\\n\
			return 0.;\\\n\
		}\\\n\
\\\n\
		/* save the smallest distance to an object */ \\\n\
		res = min(res, hardness * dist / traveled);\\\n\
		traveled += dist;\\\n\
	}\\\n\
	return res;\\\n\
}\n\
\n\
#define DECLARE_NORMAL(name) vec3 name(vec3);\n\
#define DEFINE_NORMAL(name, f) \\\n\
vec3 name(vec3 p) {\\\n\
	vec2 epilepsilon = vec2(.001, 0.);\\\n\
	return normalize(vec3(\\\n\
		f(p + epilepsilon.xyy)[0] - f(p - epilepsilon.xyy)[0],\\\n\
		f(p + epilepsilon.yxy)[0] - f(p - epilepsilon.yxy)[0],\\\n\
		f(p + epilepsilon.yyx)[0] - f(p - epilepsilon.yyx)[0]\\\n\
	));\\\n\
}\n\
\n\
// ambient occlusion and subsurface scattering\n\
#define DECLARE_AO(name) float name(vec3, vec3, float, float);\n\
#define DEFINE_AO(name, f) \\\n\
float ao(vec3 p, vec3 n, float d, float i) {\\\n\
	float o, s = sign(d);\\\n\
	for(o = s * .5 + .5; i > 0; i--) {\\\n\
		o -= (i * d - f(p + n * i * d * s)[0]) / exp2(i);\\\n\
	}\\\n\
	return o;\\\n\
}\n\
\n\
mat3 rX(float theta) {\n\
	return mat3(\n\
		1., 0., 0.,\n\
		0., cos(theta), sin(theta),\n\
		0., -sin(theta), cos(theta)\n\
	);\n\
}\n\
\n\
mat3 rY(float theta) {\n\
	return mat3(\n\
		cos(theta), 0., -sin(theta),\n\
		0., 1., 0.,\n\
		sin(theta), 0., cos(theta)\n\
	);\n\
}\n\
\n\
mat3 rZ(float theta) {\n\
	return mat3(\n\
		cos(theta), sin(theta), 0.,\n\
		-sin(theta), cos(theta), 0.,\n\
		0., 0., 1.\n\
	);\n\
}\n\
\n\
// hier kommt der witz!\n\
vec2 min_material(vec2 a, vec2 b) {\n\
	return mix(a, b, a.x > b.x);\n\
}\n\
\n\
// smooth minimum, k is the difference between the two values for which to smooth (eg. k = 0.1)\n\
float smin(float a, float b, float k) {\n\
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0 );\n\
	return mix(b, a, h) - k * h * (1.0 - h);\n\
}\n\
\n\
// smooth maximum, k is the difference between the two values for which to smooth (eg. k = 0.1)\n\
float smax(float a, float b, float k) {\n\
	float h = clamp(0.5 - 0.5 * (b - a) / k, 0.0, 1.0 );\n\
	return mix(b, a, h) + k * h * (1.0 - h);\n\
}\n\
\n\
// c: x value of maximum\n\
// w: pulse width / 2\n\
// x: x\n\
// see http://www.iquilezles.org/www/articles/functions/functions.htm\n\
float cubic_pulse(float c, float w, float x) {\n\
    x = abs(x - c);\n\
    x /= w;\n\
	return 1. - smoothstep(0., 1., x);\n\
}\n\
\n\
vec3 rgb2hsv(vec3 c) {\n\
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);\n\
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));\n\
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));\n\
\n\
    float d = q.x - min(q.w, q.y);\n\
    float e = 1.0e-10;\n\
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);\n\
}\n\
\n\
vec3 hsv2rgb(vec3 c) {\n\
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n\
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);\n\
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);\n\
}\n\
\n\
// (x^n + y^n + z^n)^1/n\n\
float length_n(vec3 p, float n) {\n\
	return pow(dot(pow(abs(p), vec3(n)), vec3(1)), 1./n);\n\
}\n\
\n\
float sphere(vec3 p, float s) {\n\
	return length(p) - s;\n\
}\n\
\n\
float box(vec3 p, vec3 b) {\n\
	p = abs(p) - b;\n\
	return max(p.x, max(p.y, p.z));\n\
}\n\
\n\
float box2(vec2 p, vec2 b) {\n\
	p = abs(p) - b;\n\
	return max(p.x, p.y);\n\
}\n\
\n\
// more accurate than box(), but slower\n\
float slowbox(vec3 p, vec3 b) {\n\
	vec3 d = abs(p) - b;\n\
	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));\n\
}\n\
\n\
// box with rounded corners, r is radius of corners\n\
float roundbox(vec3 p, vec3 b, float r) {\n\
	return slowbox(p, b) - r;\n\
}\n\
\n\
float torus(vec3 p, vec2 t) {\n\
	vec2 q = vec2(length(p.xz) - t.x, p.y);\n\
	return length(q) - t.y;\n\
}\n\
\n\
float tri_prism(vec3 p, vec2 h) {\n\
	vec3 q = abs(p);\n\
	return max(q.z - h.y, max(q.x * 0.866025 + p.y * 0.5, -p.y) -h.x * 0.5);\n\
}\n\
\n\
float hex_prism(vec3 p, vec2 h) {\n\
	vec3 q = abs(p);\n\
	return max(q.z - h.y, max(q.x + q.y * 0.57735, q.y * 1.1547) - h.x);\n\
}\n\
\n\
// c must be normalized\n\
float cone(vec3 p, vec2 c) {\n\
	float q = length(p.xy);\n\
	return dot(c, vec2(q, p.z));\n\
}\n\
\n\
// n must be normalized\n\
float plane(vec3 p, vec3 n) {\n\
	return dot(p, n.xyz);\n\
}\n\
\n\
// a and b are the endpoints\n\
// r is the radius if you want some kind of capsule\n\
float line(vec3 p, vec3 a, vec3 b, float r) {\n\
	vec3 pa = p - a;\n\
	vec3 ba = b - a;\n\
	float h = clamp(dot(pa,ba) / dot(ba,ba), 0., 1. );\n\
	return length(pa - ba*h) - r;\n\
}\n\
\n\
// f: distance function to object\n\
// p: evaluation point\n\
// s: scale factor\n\
#define scale(f, p, s) f((p)/(s))*(s)\n\
\n\
// trans*late things - using vectors!!\n\
// p: point\n\
// v: translation vector\n\
vec3 transv(vec3 p, vec3 v) {\n\
	return p - v;\n\
}\n\
\n\
// trans*late things\n\
// p: point\n\
// x: x\n\
// y: y\n\
// z: z\n\
vec3 trans(vec3 p, float x, float y, float z) {\n\
	return transv(p, vec3(x, y, z));\n\
}\n\
\n\
vec3 domrepv(vec3 p, vec3 c) {\n\
	return mod(p, c) - .5 * c;\n\
}\n\
\n\
// repeat things\n\
vec3 domrep(vec3 p, float x, float y, float z) {\n\
	return domrepv(p, vec3(x, y, z));\n\
}\n\
\n\
// Given a position, this function generates a 3D co-ordinates based,\n\
// reconstructible static noise.\n\
float noise(vec3 position) {\n\
	position.x += position.y * 57. + position.z * 21.;\n\
	return sin(cos(position.x) * position.x);\n\
\n\
	/* The following is an alternative for the previous line:\n\
	 * return fract(position.x * position.x * .0013) * 2. - 1.; */\n\
}\n\
\n\
// Given a position, this function generates a 3D co-ordinates based,\n\
// reconstructible linearly interpolated smooth noise.\n\
//\n\
// This function uses the noise() function above for its underlying\n\
// noise texture.\n\
float smooth_noise(vec3 position) {\n\
	vec3 integer = floor(position);\n\
	vec3 fractional = position - integer;\n\
\n\
	return mix(mix(mix(noise(integer),\n\
			   noise(integer + vec3(1, 0, 0)),\n\
			   fractional.x),\n\
		       mix(noise(integer + vec3(0, 1, 0)),\n\
			   noise(integer + vec3(1, 1, 0)),\n\
			   fractional.x),\n\
		       fractional.y),\n\
		   mix(mix(noise(integer + vec3(0, 0, 1)),\n\
			   noise(integer + vec3(1, 0, 1)),\n\
			   fractional.x),\n\
		       mix(noise(integer + vec3(0, 1, 1)),\n\
			   noise(integer + 1.), fractional.x),\n\
		       fractional.y),\n\
		   fractional.z) * .5 + .5;\n\
}\n\
\n\
float rand(vec2 co){\n\
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\n\
}\n\
\n\
vec2 g(float v) {\n\
	v *= 2.0 * 3.1415926;\n\
	return vec2(cos(v), sin(v));\n\
}\n\
\n\
vec2 fade(vec2 t) {\n\
	return t*t*t*(t*(t*6.0-15.0)+10.0);\n\
}\n\
\n\
float classic_noise(vec2 co) {\n\
	vec2 c = fract(co);\n\
	vec2 C = floor(co);\n\
	vec2 e = vec2(0.0, 1.0);\n\
\n\
	vec4 n = vec4(\n\
		dot(g(rand(C + e.xx)), c - e.xx),\n\
		dot(g(rand(C + e.xy)), c - e.xy),\n\
		dot(g(rand(C + e.yx)), c - e.yx),\n\
		dot(g(rand(C + e.yy)), c - e.yy)\n\
	);\n\
\n\
	vec2 u = fade(c);\n\
\n\
	return mix(\n\
		mix(n[0], n[2], u.x),\n\
		mix(n[1], n[3], u.x),\n\
		u.y);\n\
}\n\
\n\
// positive dot product\n\
float pdot(vec3 a, vec3 b) {\n\
	return max(0., dot(a, b));\n\
}\n\
\n\
// calculate lambertian reflectance (diffuse term of phong model)\n\
// input vectors have to be of unit length\n\
float lambert_norm(vec3 to_light, vec3 normal) {\n\
	return pdot(to_light, normal);\n\
}\n\
\n\
// calculate lambertian reflectance (diffuse term of phong model)\n\
float lambert(vec3 to_light, vec3 normal) {\n\
	return lambert_norm(normalize(to_light), normalize(normal));\n\
}\n\
\n\
// calculate specular term of phong model\n\
// input vectors have to be of unit length\n\
float phong_norm(vec3 to_light, vec3 normal, vec3 to_view, float exponent) {\n\
	vec3 reflection = 2. * normal * dot(normal, to_light) - to_light;\n\
	return pow(pdot(reflection, to_view), exponent);\n\
}\n\
\n\
// calculate specular term of phong model\n\
float phong(vec3 to_light, vec3 normal, vec3 to_view, float exponent) {\n\
	return phong_norm(normalize(to_light), normalize(normal), normalize(to_view), exponent);\n\
}\n\
\n\
// see oren_nayar()\n\
// sigma2 = sigma * sigma\n\
// input vectors have to be of unit length\n\
float oren_nayar_norm(vec3 to_light, vec3 normal, vec3 to_view, float sigma2) {\n\
	float A = 1 - sigma2 / (2 * (sigma2 + .33));\n\
	float B = .45 * sigma2 / (sigma2 + .09);\n\
	// orthogonalize to_light and to_view with normal to calculate angle\n\
	float cos_l_n = dot(to_light, normal);\n\
	float cos_v_n = dot(to_view, normal);\n\
	float cos_sigma_i_minus_sigma_r = dot(\n\
		normalize(to_light - normal * cos_l_n),\n\
		normalize(to_view - normal * cos_v_n)\n\
	);\n\
	// calculating sin and tan from cos avoids case differentiation\n\
	float sin_alpha_tan_beta = sqrt((1 - cos_l_n * cos_l_n) * (1 - cos_v_n * cos_v_n)) / max(cos_l_n, cos_v_n);\n\
	return max(0, cos_l_n) * (A + B * max(0, cos_sigma_i_minus_sigma_r) * sin_alpha_tan_beta);\n\
}\n\
\n\
// calculate oren-nayar reflectance\n\
// sigma is roughness (sigma = 0 equal to lambert)\n\
float oren_nayar(vec3 to_light, vec3 normal, vec3 to_view, float sigma) {\n\
	return oren_nayar_norm(normalize(to_light), normalize(normal), normalize(to_view), sigma * sigma);\n\
}\n\
\n\
// see cook_torrance()\n\
// m2 = m * m\n\
// R0 is from schlick's approximation\n\
// input vectors have to be of unit length\n\
float cook_torrance_norm(vec3 to_light, vec3 normal, vec3 to_view, float m2, float R0) {\n\
	vec3 H = .5 * (to_light + to_view);\n\
	float NdotH = dot(normal, H);\n\
	float cos2_alpha = NdotH * NdotH;\n\
	// beckmann distribution\n\
	float distribution = exp(-(1. - cos2_alpha)/(cos2_alpha * m2)) / (m2 * cos2_alpha * cos2_alpha);\n\
	// schlick's approximation\n\
	float fresnel = R0 + (1. - R0) * pow(1 - dot(H, to_view), 5.);\n\
	float VdotH = dot(to_view, H);\n\
	float LdotN = dot(to_light, normal);\n\
	float VdotN = dot(to_view, normal);\n\
	// geometric attenuation\n\
	float geometric = min(1., 2. * NdotH / VdotH * min(LdotN, VdotN));\n\
	return distribution * fresnel * geometric / (4. * LdotN * VdotN);\n\
}\n\
\n\
// calculate cook-torrance reflectance\n\
// m is roughness (rms slope of the surface microfasets)\n\
// n is the refraction index of the material\n\
float cook_torrance(vec3 to_light, vec3 normal, vec3 to_view, float m, float n) {\n\
	float R0 = (1. - n) / (1. + n);\n\
	R0 *= R0;\n\
	return max(0, cook_torrance_norm(normalize(to_light), normalize(normal), normalize(to_view), m * m, R0));\n\
}\n\
float rgb2luma(vec3 rgb) {\n\
	return dot(rgb, vec3(.2126, .7152, .0722)); // magic luminance formular\n\
}\n\
\n\
// change contrast - k changes the intensity\n\
// k > 1: more contrast\n\
// k < 1: less contrast\n\
// k == 0: black-white\n\
// k == 1: the input color\n\
// k < 0: invert color & contrast(abs(k))\n\
vec3 contrast(vec3 color, float k) {\n\
	//float luma = rgb2luma(color);\n\
	return mix(vec3(.5), color, k);\n\
}\n\
\n\
// add some kind of vignette\n\
// 0 < intensity < 1\n\
// intensity = .7 is good default\n\
float vignette(float intensity) {\n\
	float one_minus_intesity = 1. - intensity;\n\
	vec2 position = gl_FragCoord.xy / res;\n\
	position.y *= -1.;\n\
	return one_minus_intesity + intensity * 16. * position.x *position.y * (1. - position.x) * (-1. - position.y);\n\
}\n\
\n\
#line 1\n\
";
