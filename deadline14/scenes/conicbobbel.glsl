#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 conic_bobbel_color; // color
uniform vec3 conic_ring_color; // color

uniform float conic_smooth_factor; // float
uniform float manual_time; // float
uniform float conic_ring_intensity; // float

vec3 colors[] = vec3[](
		vec3(.03, .0, .0),
		vec3(.0),
		conic_bobbel_color,
		conic_ring_color
		);

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march_adv(camera_position, direction, i, 100, 1.);

	vec3 color = vec3(0.);
	if(i < 150) {
		int material = int(f(hit).y);
		vec3 normal = calc_normal(hit);

		Material mat;
		if(material == 0.) {
			float size = 2.;
			float stripes = mod(floor(hit.x / size), 2.);
			mat = Material(colors[material] * stripes, 0.5, 0.);
		} else if(material == 1.) {
			mat = Material(colors[material], 1., 0.);
		} else {
			mat = Material(colors[material], 0.2, 1.);
		}
		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
		if (material == 3.) {
			color = conic_ring_color * conic_ring_intensity * 100.;
		}
	}

	output_color(color, 4.01);//distance(hit, camera_position));
}

vec2 f(vec3 p) {
	vec3 p_bobbel = trans(p, -1., 0., -3.);
	float f_bobbel = p.y;

	//float time = 10. * manual_time;

	vec3 s_domrep = vec3(30, 1., 25.);
	vec3 p_pre_cone = trans(p_bobbel, 10. * time, 0., 0.);
	float cell_x = floor(p_pre_cone.x / s_domrep.x) - 100.;
	float cell_y = floor(p_pre_cone.y / s_domrep.y) - 100.;
	float cell_z = floor(p_pre_cone.z / s_domrep.z) - 100.;
	p_pre_cone = trans(p_pre_cone,
			sin(cell_z * cell_z) * 32.,
			0.,
			sin(cell_x * cell_x) * 50.);
	vec3 p_cone = domrepv(p_pre_cone, s_domrep);
	p_cone.y = p_pre_cone.y;
	p_cone = p_bobbel;

	float f_cone = p.y;
	float line_radius = mix(.0, .5, smoothstep(.3, 2., p_cone.x) + .3 * smoothstep(.9, 0., p_cone.x));
	f_cone = line(p_cone, vec3(2., 0., 0.), vec3(0., 0., 0.), line_radius);
	vec2 surface_coord = vec2(p_cone.x * 3. + time, atan(p_cone.y, p_cone.z));
	f_cone -= smoothstep(0., 1., cnoise(surface_coord * 1.5)) * .03;

	vec3 p_spike = trans(p_cone, 2., 0., 0.);
	p_spike.yz = vec2(length(p_spike.yz), atan(p_spike.y, p_spike.z) / TAU);
	vec3 p_pre_spike = p_spike;
	p_spike = domrep(p_spike, 1., 1., .125);
	p_spike.xy = p_pre_spike.xy;
	p_spike.y -= .8;
	p_spike.z *= 3.2;
	//p_spike = rZ(TAU * -(p_spike.y + 1.) / 7.)* p_spike;
	float f_spike = cone(p_spike.xzy, normalize(vec2(7., 1.)));
	f_cone = smin(f_cone, f_spike, .1);

	float T = 3.;
	vec3 p_ring = trans(p_cone, 2. - 3. * mod(time, T) / T, 0., 0.);
	float f_ring = p.y;
	float ring_radius = 1.5 * impulse(8., mod(time, T) / T);
	f_ring = abs(sphere(p_ring, ring_radius)) - .2 * ring_radius;
	f_ring = smax(f_ring, abs(p_ring.x) - .03, .02);

	vec3 p_ring2 = trans(p_cone, 2. - 3. * mod(time + T/2.5, T) / T, 0., 0.);
	float f_ring2 = p.y;
	float ring_radius2 = 1.5 * impulse(8., mod(time + T/2.5, T) / T);
	f_ring2 = abs(sphere(p_ring2, ring_radius2)) - .2 * ring_radius2;
	f_ring2 = smax(f_ring2, abs(p_ring2.x) - .03, .02);
	f_ring = min(f_ring, f_ring2);

	//f_bobbel = smin(f_cone, f_ring, conic_smooth_factor);
	//vec2 m_bobbel = vec2(f_bobbel, 2.);

	vec2 m_cone = vec2(f_cone, 2.);
	vec2 m_ring = vec2(f_ring, 3.);
	vec2 m_bobbel = smin_material(m_cone, m_ring, conic_smooth_factor);

	vec2 bottom = vec2(p.y + 2., 0.);
	vec2 bounding = vec2(-sphere(p - camera_position, 300.), 1.);
	return min_material(m_bobbel, min_material(bottom, bounding));
}
