#include "scene_head.glsl"
#include "rtificial.glsl"

uniform vec3 sphere1_color; // color
uniform vec3 sphere2_color; // color

uniform vec3 sphere1_pos; // vec3

vec3 colors[4] = vec3[4](
		sphere1_color,
		sphere2_color,
		vec3(.03, .0, .0),
		vec3(.0)
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
			mat = Material(colors[material], 0.2, 1.);
		} else if(material == 2.) {
			float size = 2.;
			float stripes = mod(floor(hit.x / size), 2.);
			mat = Material(colors[material] * stripes, 0.5, 0.);
		} else if(material == 3.) {
			mat = Material(colors[material], 1., 0.);
		}
		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
	}

	output_color(color, distance(hit, camera_position));
}

vec2 f(vec3 p) {
	p.z += 3.;
	p.x -= 1.;
	vec2 sphere1 = vec2(sphere(p, .7), 0.);
	p.x += 2.;
	vec2 sphere2 = vec2(sphere(transv(p, 10*sphere1_pos), .7), 1.);
	vec2 bottom = vec2(p.y + 2., 2.);
	vec2 bounding = vec2(-sphere(p - camera_position, 50.), 3.);
	return min_material(min_material(sphere1, sphere2), min_material(bottom, bounding));
}
