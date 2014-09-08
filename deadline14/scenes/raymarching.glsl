#include "scene_head.glsl"
#include "rtificial.glsl"

uniform vec3 camera_position;
uniform vec3 camera_direction;
uniform vec3 camera_up;

uniform vec3 sphere1_color; // color
uniform vec3 sphere2_color; // color

vec2 f(vec3 p);
mat3 get_camera();
vec3 get_direction();
vec3 march(vec3 p, vec3 direction, out int i);
vec3 calc_normal(vec3 p);
float sphere(vec3 p, float s);
vec2 min_material(vec2 a, vec2 b);
float phong_norm(vec3 to_light, vec3 normal, vec3 to_view, float exponent);
float phong(vec3 to_light, vec3 normal, vec3 to_view, float exponent);

vec3 colors[] = vec3[](
		sphere1_color,
		sphere2_color,
		vec3(.03, .0, .0),
		vec3(1.)
		);

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march(camera_position, direction, i);

	vec3 color = vec3(0.);
	if(i < 150) {
		int material = int(f(hit).y);
		vec3 normal = calc_normal(hit);

		Material mat;
		if(material == 0. || material == 1.) {
			mat = Material(colors[material], 0.7, 1.);
		} else if(material == 2.) {
			mat = Material(colors[material], 0.5, 0.);
		} else if(material == 3.) {
			mat = Material(colors[material], 1., 0.);
		}
		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
	}

	output_color(color, 0.);
}

vec2 f(vec3 p) {
	p.z += 3.;
	p.x -= 1.;
	vec2 sphere1 = vec2(sphere(p, .7), 0.);
	p.x += 2.;
	vec2 sphere2 = vec2(sphere(p, .7), 1.);
	vec2 bottom = vec2(p.y + 2., 2.);
	vec2 bounding = vec2(-sphere(p - camera_position, 50.), 3.);
	return min_material(min_material(sphere1, sphere2), min_material(bottom, bounding));
}

///////////////////// legacy libblink code ///////////////////
mat3 get_camera() {
	vec3 view_right = cross(camera_direction, camera_up);
	return mat3(view_right, camera_up, -camera_direction);
}

vec3 get_direction() {
	return get_camera() * normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y , -1.));
}

vec3 march(vec3 p, vec3 direction, out int i) {
	float walked = 0.;
	for (i=0; i < 150; i++) {
		float dist = f(p)[0];
		p += direction * dist;
		dist = abs(dist);
		walked += dist;

		if (dist < .001 * walked) break;
	}
	return p;
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

// hier kommt der witz!
vec2 min_material(vec2 a, vec2 b) {
	return mix(a, b, a.x > b.x);
}

// calculate specular term of phong model
// input vectors have to be of unit length
float phong_norm(vec3 to_light, vec3 normal, vec3 to_view, float exponent) {
	vec3 reflection = 2. * normal * dot(normal, to_light) - to_light;
	return pow(pdot(reflection, to_view), exponent);
}

// calculate specular term of phong model
float phong(vec3 to_light, vec3 normal, vec3 to_view, float exponent) {
	return phong_norm(normalize(to_light), normalize(normal), normalize(to_view), exponent);
}
