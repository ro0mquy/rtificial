#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 background_color; // color

void main() {
	vec3 dir = get_direction();

	int i;
	vec3 p = march_adv(camera_position, dir, i, 100, .6);

	int materialId = int(f(p)[1]);

	vec3 color = vec3(0.);
	vec3 normal = calc_normal(p);
	SphereLight light1 = SphereLight(vec3(0., 10., 10.), vec3(1.), 3., 100.);
	Material mat;
	if(materialId == 1) {
		mat = Material(vec3(0.), 1., .0);
	} else if(materialId == 0) {
		mat = Material(vec3(1.), 0.1, 0.);
		color += .001 * background_color;
	} else if(materialId == 2) {
		mat = Material(vec3(1.), 1., 0.);
	}
	color += apply_light(p, normal, -dir, mat, light1);

	output_color(color, distance(camera_position, p));
	//output_color(color, 4. + .1);
}

float boden(vec3 p);
float boden_implicit(vec3 p);

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(p, 200.), 0.);

	float height = 40.; // .5 * height
	vec3 q = domrepv(p, vec3(50.));
	q.y = p.y - height;
	//mat3 xrot = rX(radians(10.) * sin(q.y + p.x));
	//mat3 yrot = rY(radians(50.) * cos(q.y + p.y));
	//q = xrot * yrot * q;
	float k = smoothstep(height * 1.2, -height, q.y);
	float i = floor(p.x/50.);
	float j = floor(p.z/50.);
	float l = sin(i + sin(time + 3.)) * cos(j + sin(time + 5.));
	float m = cos(i + sin(time + 1.)) * sin(j + sin(time + 2.));
	mat3 rotX = rX(radians( 10.) * ((1. - k) * l + (.02 * sin(q.y))));
	mat3 rotZ = rZ(radians(-10.) * ((1. - k) * m + (.02 * sin(q.y))));
	q = rotX * rotZ * q;
	float radius = 10. * k;
	float d = max(length(q.xz) - radius, abs(q.y) - height);

	vec2 object = vec2(d, 1.);

	vec2 ground = vec2(boden_implicit(p), 2.);

	return min_material(bounding, smin_material(object, ground, 10.));
}

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 g(float v) {
	v *= 2.0 * 3.1415926;
	return vec2(cos(v), sin(v));
}

vec2 fade(vec2 t) {
	return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float classic_noise(vec2 co) {
	vec2 c = fract(co);
	vec2 C = floor(co);
	vec2 e = vec2(0.0, 1.0);

	vec4 n = vec4(
		dot(g(rand(C + e.xx)), c - e.xx),
		dot(g(rand(C + e.xy)), c - e.xy),
		dot(g(rand(C + e.yx)), c - e.yx),
		dot(g(rand(C + e.yy)), c - e.yy)
	);

	vec2 u = fade(c);

	return mix(
		mix(n[0], n[2], u.x),
		mix(n[1], n[3], u.x),
		u.y);
}

float boden_implicit(vec3 p) {
	return p.y - 10. * classic_noise(.03 * p.xz);
}

float boden(vec3 p) {
	//return p.y;
	vec2 e = vec2(.01, .0);
	vec3 grad = vec3(
		boden_implicit(p + e.xyy) - boden_implicit(p - e.xyy),
		boden_implicit(p + e.yxy) - boden_implicit(p - e.yxy),
		boden_implicit(p + e.yxx) - boden_implicit(p - e.yxx)
	);
	return boden_implicit(p) / length(grad) * 2. * e.x;
}
