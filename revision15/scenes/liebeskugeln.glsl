#include "rtificial.glsl"
#line 3

out vec4 out_color;

const float kugel_gross_id = 0.;
const float kugel_klein_id = 1.;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 100., screenDist);

	float material_id = f(o+t*d, true)[1];

	if (isinf(t)) {
		out_color.rgb = vec3(0.);

	} else {
		if (material_id <= -1.) {
			out_color.rgb = vec3(1.,0.,0.);
		} else
		if (material_id <= kugel_gross_id) {
			out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 0.))), 0.) + .1);
		} else if (material_id <= kugel_klein_id) {
			out_color.rgb = vec3(max( 0.2* dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 0.))), 0.) + .1);
		}
	}
}

vec2 f(vec3 p, bool last_step) {
	float kugel_klein_anzahl = 5.;

	vec3 p_kugel_gross = p;
	p_kugel_gross.z += 30.;
	float radius_klein = 1./2.*lk_radius_gross_rt_float;
	float kugel_domrep_size = 2.*(kugel_klein_anzahl*radius_klein+lk_radius_gross_rt_float);
	p_kugel_gross = trans(p_kugel_gross, -0.5*kugel_domrep_size, 0.,0.);
	float kugel_gross_cell_x = kugel_klein_anzahl/(lk_radius_gross_rt_float/radius_klein)*floor(p_kugel_gross.x/(kugel_domrep_size));
	p_kugel_gross.x = domrep(p_kugel_gross, kugel_domrep_size, 0.,0.).x;
	p_kugel_gross = trans(p_kugel_gross, 0., kugel_gross_cell_x*kugel_gross_cell_x, 0.);
	float radius_gross = lk_radius_gross_rt_float;
	float kugel_gross = sphere(p_kugel_gross, radius_gross);
	vec2 obj_kugel_gross = vec2(kugel_gross, kugel_gross_id);

	vec3 p_kugel_klein = p;
	p_kugel_klein.z = p_kugel_gross.z;
	//p_kugel_klein.x += lk_radius_gross_rt_float+radius_klein;
	float kugel_klein_cell_x = 1./2.*floor(p_kugel_klein.x/(2.*radius_klein));
	p_kugel_klein.x = domrep(p_kugel_klein, vec3(2.*radius_klein)).x;
	p_kugel_klein = trans(p_kugel_klein, 0., kugel_klein_cell_x*kugel_klein_cell_x,0.);
	float kugel_klein = sphere(p_kugel_klein, radius_klein);
	vec2 obj_kugel_klein = vec2(kugel_klein, kugel_klein_id);

	return min_material(min_material(obj_kugel_gross, obj_kugel_klein), vec2(sphere(p, 1.),-1.));
}
