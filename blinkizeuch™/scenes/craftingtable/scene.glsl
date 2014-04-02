vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

const float mat_bounding = 0.;

vec3 light = vec3(0., 0., 50.);

vec3 materials[] = vec3[](
	vec3(1.),
	vec3(.9),
	vec3(.2)
);

void main(void) {
	vec3 dir = get_direction();
	vec3 final_color = vec3(0.);
	int i;
	vec3 hit = march(view_position, dir, i);
	if (i < 100) {
		vec3 normal = calc_normal(hit);
		vec3 to_light = light - hit;
		final_color += materials[int(f(hit)[1])] * lambert(to_light, normal);
	}
	out_color = final_color;
}

float companion_cube(vec3 p) {
	float cube = box(p, vec3(2., 2., 2.)) - .0;
	float spherey = sphere(p, 5.1);
	float corners = max(cube, -spherey) - dot(abs(p), vec3(1.)) * .2;
	float companion = smin(cube - 1.24, corners, 0.1);

	return companion;
}

vec2 f(vec3 p){
	vec2 bounding = vec2(-sphere(p - view_position, 100.), mat_bounding);
	p = rY(time * TAU * .05) * rZ(time * TAU * .164) * rX(time * TAU * .0153) * p;

	float thingie = companion_cube(p);

	vec2 thing = vec2(thingie, 0.);
	return min_material(bounding, thing);
}
