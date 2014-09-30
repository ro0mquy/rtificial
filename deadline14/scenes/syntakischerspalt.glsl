#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 synapse_color; // color
uniform float synapse_gap; // float
uniform float synapse_aa; // float
uniform float synapse_bb; // float
uniform float synapse_cc; // float
uniform float synapse_dd; // float

Material materials[2] = Material[2](
	Material(vec3(1.), .5, 0.),
	Material(synapse_color, 0.7, 1.)
);

const float MATERIAL_ID_BOUNDING = 0.;
const float MATERIAL_ID_SYNAPSE = 1.;

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march_adv(camera_position, direction, i, 100, .7);

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
	float domrep_x = 100.;
	float domrep_z = 60.;
	vec3 q = domrep(p, domrep_x, 0., domrep_z);
	q.y = p.y;
	q.x = abs(q.x);
	q.z += 5.;
	q.x -= 10 * synapse_gap;

	float sphere1 = sphere(q, 1.);
	q.x -= 2.;
	float sphere2 = sphere(q, 2.);

	q = rZ(-TAU*sin(q.x/5.)/30.) * q;
	q = rY(-TAU*sin(q.x/2.+time*0.2)/50.)*q;
	float capsule = line(q, vec3(2.,0.,0.), vec3(20.,0.,0.), mix(synapse_aa, synapse_bb, smoothstep(10.*synapse_cc, 10*synapse_dd, q.x)));

	vec2 synapse = vec2(smin(smax(-sphere1, sphere2, 1.), capsule, 1.), MATERIAL_ID_SYNAPSE);
	vec2 bounding = vec2(-sphere(p - camera_position, 500.), MATERIAL_ID_BOUNDING);
	return min_material(synapse, bounding);
}
