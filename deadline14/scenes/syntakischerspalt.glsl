#include "scene_head.glsl"
#include "rtificial.glsl"

uniform vec3 sphere1_color; // color
uniform float k; // float

vec3 colors[] = vec3[](
		sphere1_color,
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
	vec3 c = vec3(0.);
	p.z += 5.;
	p.x -= 1.;
	float sphere1 = sphere(p, 1.);
	p.x -= 2.;
	float sphere2 = sphere(p, 2.);
	float capsule = line(p, c+vec3(2.,0.,0.), vec3(10.,0.,0.), 1.);
	vec2 synapse = vec2(smin(smax(-sphere1, sphere2, k), capsule, k), 0.);
	vec2 bottom = vec2(p.y + 2., 2.);
	vec2 bounding = vec2(-sphere(p - camera_position, 50.), 3.);
	return min_material(synapse, min_material(bottom, bounding));
}
