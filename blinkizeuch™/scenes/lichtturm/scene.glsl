vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;
uniform vec3 light_direction;

const float mat_bounding = 0.;
const float mat_turm = 1.;

void main(void) {
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if (i < 100) {
		vec3 normal = calc_normal(hit);
		final_color += vec3(.8 * lambert(light_direction, normal) + .2);
	}
	out_color = final_color;
}

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(p, 100.), mat_bounding);

	float turm_height = 3;
	float turm_width = .7;
	vec3 p_turm = trans(p, 0, turm_height, 0);
	float f_turm = box(p_turm, vec3(turm_width, turm_height, turm_width));
	vec2 vec_turm = vec2(f_turm, mat_turm);
	return min_material(bounding, vec_turm);
}
