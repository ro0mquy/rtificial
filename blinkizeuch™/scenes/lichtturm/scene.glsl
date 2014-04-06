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

vec3[] mat_colors = vec3[](
		vec3(.0),
		vec3(.5)
);

void main(void) {
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if (i < 100) {
		vec3 normal = calc_normal(hit);
		float material = f(hit)[1];
		final_color += vec3(.95 * lambert(light_direction, normal) + .05);
		final_color *= mat_colors[int(material)];

		vec3 line_start = vec3(0, 0, 2);
		vec3 line_end   = vec3(2, 0, 0);
		float distance = distance(hit, view_position);
		//*
		// kind of raymarching
		float minimal_dist = line(hit, line_start, line_end, 0);
		for (float t = 0; t < distance; ) {
			vec3 p = view_position + t * dir;
			float dist = line(p, line_start, line_end, 0);
			minimal_dist = min(minimal_dist, dist);
			t += dist;
			if (dist < .001) break;
		}
		float alpha = .7 / (pow(minimal_dist * 4, 4) + 1);
		// */
		/*
		// fixed stepsize
		float alpha = 0;
		float step_width = 1.;
		for (float t = 0; t < distance; t += step_width) {
			vec3 p = view_position + t * dir;
			float dist = line(p, line_start, line_end, 0);
			alpha += 1 / (dist * dist + 1);
		}
		alpha /= floor(distance / step_width) + 1.;
		// */

		final_color += color_foo1 * alpha;
	}
	out_color = pow(final_color, vec3(1/2.2));
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
