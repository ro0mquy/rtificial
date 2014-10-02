#include "scene_head.glsl"
#include "rtificial.glsl"
#include "bobbel.glsl"
#line 5

uniform vec3 synapse_color; // color
uniform float synapse_gap; // float
uniform float synapse_aa; // float
uniform float synapse_bb; // float
uniform float synapse_cc; // float
uniform float synapse_dd; // float
uniform float synapse_dist_fog_a; // float
uniform float synapse_dist_fog_b; // float

Material materials[3] = Material[3](
	Material(vec3(1.), .5, 0.),
	Material(synapse_color, 0.7, 1.),
	Material(vec3(0.,1.,0.), .2, 1.)
);

const float MATERIAL_ID_BOUNDING = 0.;
const float MATERIAL_ID_SYNAPSE  = 1.;
const float MATERIAL_ID_BOBBEL   = 2.;


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

	color *= smoothstep(synapse_dist_fog_a, synapse_dist_fog_b, -distance(hit, camera_position));

	output_color(color, distance(hit, camera_position));
}

vec2 f(vec3 p) {
	// domrep with rotation
	vec3 dr_factor = vec3(50.);
	vec3 q = domrepv(p, dr_factor);
	q *= rZ(2*rand(3*floor(p/dr_factor)));
	q *= rY(1.5*rand(floor(p/dr_factor)));

	// mirror
	q.x = abs(q.x);
	q.x -= 10 * synapse_gap;

	// main body
	float sphere1 = sphere(q, 1.);
	q.x -= 2.;
	float sphere2 = sphere(q, 2.);

	// tail
	q = rZ(-TAU*sin(q.x/5.)/30.) * q;
	q = rY(-TAU*sin(q.x/2.+time*0.5)/50.)*q;
	float capsule = line(q, vec3(2.,0.,0.), vec3(20.,0.,0.), mix(synapse_aa, synapse_bb, smoothstep(10.*synapse_cc, 10*synapse_dd, q.x)));

	vec2 synapse = vec2(smin(smax(-sphere1, sphere2, 1.), capsule, 1.), MATERIAL_ID_SYNAPSE);

	// bobbel
	vec3 p_bbl = p;
	p_bbl = trans(p_bbl, dr_factor/2., -dr_factor/2., -dr_factor/2.);
	p_bbl *= rY(-0.1*TAU);
	float scale_factor = 0.15;
	float bbl = conicbobbel(p_bbl / scale_factor, 2.) * scale_factor;
	vec2 bobbel = vec2(bbl, MATERIAL_ID_BOBBEL);

	vec2 bounding = vec2(-sphere(p - camera_position, -synapse_dist_fog_a), MATERIAL_ID_BOUNDING);
	return min_material(synapse, min_material(bounding, bobbel));
}
