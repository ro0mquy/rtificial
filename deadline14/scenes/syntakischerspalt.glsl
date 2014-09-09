#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 synapse_color; // color
uniform float synapse_gap; // float
uniform float aa; // float
uniform float bb; // float
uniform float cc; // float
uniform float dd; // float

Material materials[] = Material[](
	Material(synapse_color, 0.7, 1.),
	Material(vec3(.03, .0, .0), .5, 0.),
	Material(vec3(1.), .5, 0.)
);

#define MATERIAL_ID_SYNAPSE 0.
#define MATERIAL_ID_FLOOR 1.
#define MATERIAL_ID_BOUNDING 2.

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march(camera_position, direction, i);

	vec3 color = vec3(0.);
	if(i < 150) {
		int material = int(f(hit).y);
		vec3 normal = calc_normal(hit);

		Material mat = materials[material];
		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
	}

	output_color(color, distance(hit, camera_position));
}

vec2 f(vec3 p) {
	float domrep_x = 50.;
	float domrep_z = 30.;
	vec3 q = domrep(p, domrep_x, 1., domrep_z);
	q.y = p.y;
	q.x = abs(q.x);
	q.z += 5.;
	q.x -= 10 * synapse_gap;

	float sphere1 = sphere(q, 1.);
	q.x -= 2.;
	float sphere2 = sphere(q, 2.);

	float capsule = line(q, vec3(2.,0.,0.), vec3(10.,0.,0.), mix(aa, bb, smoothstep(10.*cc, 10*dd, q.x)));
	vec2 synapse = vec2(smin(smax(-sphere1, sphere2, 1.), capsule, 1.), 0.);

	vec2 bottom = vec2(p.y + 2., 2.);
	vec2 bounding = vec2(-sphere(p - camera_position, 50.), 3.);
	return min_material(synapse, min_material(bottom, bounding));
}
