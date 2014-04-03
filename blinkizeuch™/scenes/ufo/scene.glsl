vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
DEFINE_SOFTSHADOW(softshadow, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

#define MAT_BOUNDING 0
#define MAT_UFO_BODY 1
#define MAT_UFO_COCKPIT 2

vec3 colors[] = vec3[](
	vec3(0.,0.01,0.04),
	color_foo1,
	color_foo2
);

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	vec3 light = vec3(10*foo1, 40, 10*foo1);
	int i;
	vec3 hit = march(view_position, dir, i);
	int material = int(f(hit)[1]);
	vec3 normal = calc_normal(hit);
	final_color += 0.1 * ao(hit, normal, .15, 5);
	final_color += softshadow(hit, light, 64.) * colors[material] * lambert(light - hit, normal);

	out_color = final_color;
}

vec2 f(vec3 p){
	float ufo_bottom = sphere(p, 15);
	float ufo_top = sphere(trans(p, 0., 0.55*50., 0.), 15);
	vec2 ufo_body = vec2(max(ufo_top, ufo_bottom), MAT_UFO_BODY);

	vec3 p2 = trans(p, 0., 50*.23, 0.);
	vec2 ufo_cockpit = vec2(max(sphere(p2, 5), -ufo_bottom), MAT_UFO_COCKPIT);

	vec2 bounding = vec2(-sphere(transv(p, view_position), 220.), MAT_BOUNDING);
	return min_material(min_material(ufo_body, ufo_cockpit), bounding);
}
