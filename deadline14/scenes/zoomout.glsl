#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

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
	Material(synapse_color, 0.7, 1.)
);

const float MATERIAL_ID_BOUNDING = 0.;
const float MATERIAL_ID_SYNAPSE  = 1.;
const float MATERIAL_ID_BAHNEN   = 2.;


void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march_adv(camera_position, direction, i, 200, .7);

	vec3 color = vec3(0.);
	int material = int(f(hit).y);
	vec3 normal = calc_normal(hit);

	Material mat = materials[material];

	SphereLight light1 = SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.);
	SphereLight light2 = SphereLight(vec3(5., -9., 10.), vec3(1.), 2., 100.);
	color += apply_light(hit, normal, -direction, mat, light1);
	color += apply_light(hit, normal, -direction, mat, light2);

	color *= smoothstep(synapse_dist_fog_a, synapse_dist_fog_b, -distance(hit, camera_position));

	output_color(color, distance(hit, camera_position));
}

float synapse(vec3 q) {
	// main body
	float sphere1 = sphere(q, 1.);
	q.x -= 2.;
	float sphere2 = sphere(q, 2.);

	// tail
	q += rZ(-TAU*sin(q.x/5. + 9.)/30.) * vec3(8.) - 8.;
	float ohm_thingie = 6. * smoothstep(2., 5., q.x);
	q += rY(-TAU*sin(q.x/2.+time*0.5)/50.) * vec3(ohm_thingie) - ohm_thingie;
	float capsule = line(q, vec3(2.,0.,0.), vec3(100.,0.,0.), mix(1.3 * synapse_aa, synapse_bb, smoothstep(10.*synapse_cc, 10*synapse_dd, q.x)));

	return smin(smax(-sphere1, sphere2, 1.), capsule, 1.);
}

vec2 f(vec3 p) {
	// mirror
	vec3 p_syn1 = p;
	vec3 p_syn2 = p_syn1;
	p_syn2 *= -1.;
	p_syn1.x -= 10. * synapse_gap;
	p_syn2.x -= 10. * synapse_gap;

	// both synapses
	float f_syn1 = synapse(p_syn1);
	float f_syn2 = synapse(p_syn2);

	float f_synapse = smin(f_syn1, f_syn2, 1.3);
	vec2 m_synapse = vec2(f_synapse, MATERIAL_ID_SYNAPSE);

	float the_noise = .5 * vnoise(.01 * p);
	vec3 p_bahnen = trans(p, -20., -20., 0.);
	p_bahnen = p;
	float radius_bahnen = 1.5;
	float f_bahnen;

	{
		vec3 p_bahn = trans(p_bahnen, 20., 14., -7.);
		vec3 dir_bahn = vec3(1., 2., 1.);
		p_bahn.xy *= rot2D(the_noise);
		f_bahnen = line(p_bahn, 100. * dir_bahn, -100. * dir_bahn, radius_bahnen);
	}

	{
		vec3 p_bahn = trans(p_bahnen, -50., -34., -34.);
		vec3 dir_bahn = vec3(3., 1., -1.);
		p_bahn.xy *= rot2D(the_noise);
		f_bahnen = min(f_bahnen, line(p_bahn, 100. * dir_bahn, -100. * dir_bahn, radius_bahnen));
	}

	{
		vec3 p_bahn = trans(p_bahnen, 23., -27., 17.);
		vec3 dir_bahn = vec3(1., 1., 1.);
		p_bahn.xy *= rot2D(the_noise);
		f_bahnen = min(f_bahnen, line(p_bahn, 100. * dir_bahn, -100. * dir_bahn, radius_bahnen));
	}

	{
		vec3 p_bahn = trans(p_bahnen, 40., -34., -27.);
		vec3 dir_bahn = vec3(-1., 2., -1.);
		p_bahn.xy *= rot2D(the_noise);
		f_bahnen = min(f_bahnen, line(p_bahn, 100. * dir_bahn, -100. * dir_bahn, radius_bahnen));
	}

	{
		vec3 p_bahn = trans(p_bahnen, -10., -54., -37.);
		vec3 dir_bahn = vec3(-1., 2., -1.);
		p_bahn.xy *= rot2D(the_noise);
		f_bahnen = min(f_bahnen, line(p_bahn, 100. * dir_bahn, -100. * dir_bahn, radius_bahnen));
	}

	vec2 m_bahnen = vec2(f_bahnen, MATERIAL_ID_BAHNEN);

	vec2 m_bounding = vec2(-sphere(p - camera_position, -synapse_dist_fog_a), MATERIAL_ID_BOUNDING);
	return min_material(min_material(m_synapse, m_bahnen), m_bounding);
}
