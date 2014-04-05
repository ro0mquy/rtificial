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
	vec3(.5),
	vec3(.2),
	vec3(1., .62, .62),
	vec3(.9),
	vec3(.9, .1, 0),
	vec3(0, .2, .9)
);

void main(void) {
	vec3 dir = get_direction();
	vec3 final_color = vec3(0.);
	int i;
	vec3 hit = march(view_position, dir, i);
	if (i < 100) {
		vec3 normal = calc_normal(hit);
		vec3 to_light = light - hit;
		vec3 to_view = view_position - hit;
		final_color += .8 * lambert(to_light, normal) + .2;
		final_color += phong(to_light, normal, to_view, 10.);
		final_color *= materials[int(f(hit)[1])];
	}
	out_color = final_color;
}

float companion_cube_v1(vec3 p) {
	float cube = box(p, vec3(2.));
	float spherey = sphere(p, 5.1);
	float corners = smax(cube, -spherey, .01) - dot(abs(p), vec3(1.)) * .2;
	float companion = smin(cube - 1.24, corners, 0.1);
	return companion;
}

vec2 companion_cube_v2(vec3 p) {
	float corners = box(p, vec3(3.1)) - .01;
	corners = smax(corners, -sphere(p, 3.7), .05);
	float grid = box(domrep(p, 6., 6., 6.), vec3(2.));
	corners = smax(corners, grid, .01);
	corners = smax(corners, -box(p, vec3(2.72)), .01);
	corners = max(corners, dot(abs(p), vec3(1.)) - 8.3);
	vec2 corners2 = vec2(corners, 0.);

	float cube = box(p, vec3(2.7)) - .01;
	cube = smax(cube, -sphere(p, 3.2), .05);
	cube = max(cube, -box(p, vec3(2.68)));
	float small_grid = box(domrep(p, 6., 6., 6.), vec3(2.9));
	cube = smax(cube, small_grid, .01);
	vec2 cube2 = vec2(cube, 1.);

	float spherey = sphere(p, 2.7);
	vec2 spherey2 = vec2(spherey, 3.);

	return min_material(min_material(corners2, cube2), spherey2);
}

vec2 barber(vec3 p) {
	float cylinder = length(p.xz) - 1.;
	float barb = cylinder;
	float phi = acos(normalize(p.xz).y);
	phi = TAU + sign(p.x) * phi;
	phi += TAU / 8 * (p.y + time);
	phi = mod(phi, TAU);
	float material = floor(phi / TAU * 3.) + 4.;
	return vec2(barb, material);
}

vec2 f(vec3 p){
	vec2 bounding = vec2(-sphere(p - view_position, 100.), mat_bounding);
	//p = rY(time * TAU * .05) * rZ(time * TAU * .164) * rX(time * TAU * .0153) * p;

	//float thingie = companion_cube_v1(p);
	//vec2 thing = vec2(thingie, 0.);

	//vec2 thing = companion_cube_v2(p);
	vec2 thing = barber(p);

	return min_material(bounding, thing);
}
