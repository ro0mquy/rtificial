#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 conic_bobbel_color; // color
uniform vec3 conic_ring_color; // color

uniform float conic_smooth_factor; // float
uniform float manual_time; // float
uniform float conic_ring_metalic; // float
uniform float conic_ring_roughness; // float

vec3 colors[] = vec3[](
		vec3(.03, .0, .0),
		vec3(.0),
		conic_bobbel_color,
		conic_ring_color
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
		if(material == 0.) {
			float size = 2.;
			float stripes = mod(floor(hit.x / size), 2.);
			mat = Material(colors[material] * stripes, 0.5, 0.);
		} else if(material == 1.) {
			mat = Material(colors[material], 1., 0.);
		} else if (material == 3.) {
			mat = Material(colors[material], conic_ring_roughness, conic_ring_metalic);
		} else {
			mat = Material(colors[material], 0.2, 1.);
		}
		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
	}

	output_color(color, 4.01);//distance(hit, camera_position));
}

vec2 f(vec3 p) {
	vec3 p_bobbel = trans(p, -1., 0., -3.);
	float f_bobbel = p.y;

	//float time = 10. * manual_time;
	vec3 p_cone = trans(p_bobbel, 10. * time, 0., 0.);
	p_cone.x = mod(p_cone.x, 10.) - 5.;
	float f_cone = p.y;
	float line_radius = mix(.1, .5, smoothstep(0., 2., p_cone.x));
	f_cone = line(p_cone, vec3(2., 0., 0.), vec3(0., 0., 0.), line_radius);

	float T = 1.;
	vec3 p_ring = trans(p_cone, 2. - 3. * mod(time, T) / T, 0., 0.);
	float f_ring = p.y;
	float ring_radius = 1.5 * impulse(10., mod(time, T) / T);
	f_ring = abs(sphere(p_ring, ring_radius)) - .2 * ring_radius;
	f_ring = smax(f_ring, abs(p_ring.x) - .03, .02);

	vec3 p_ring2 = trans(p_cone, 2. - 3. * mod(time + T/2.5, T) / T, 0., 0.);
	float f_ring2 = p.y;
	float ring_radius2 = 1.5 * impulse(10., mod(time + T/2.5, T) / T);
	f_ring2 = abs(sphere(p_ring2, ring_radius2)) - .2 * ring_radius2;
	f_ring2 = smax(f_ring2, abs(p_ring2.x) - .03, .02);
	f_ring = min(f_ring, f_ring2);

	//f_bobbel = smin(f_cone, f_ring, conic_smooth_factor);
	//f_bobbel = f_ring;

	vec2 m_cone = vec2(f_cone, 2.);
	vec2 m_ring = vec2(f_ring, 3.);
	vec2 m_bobbel = smin_material(m_cone, m_ring, conic_smooth_factor);

	//vec2 m_bobbel = vec2(f_bobbel, 2.);
	vec2 bottom = vec2(p.y + 2., 0.);
	vec2 bounding = vec2(-sphere(p - camera_position, 50.), 1.);
	return min_material(m_bobbel, min_material(bottom, bounding));
}
