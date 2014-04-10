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
#define MAT_CHROME 1
#define MAT_BLA 2

vec3 colors[] = vec3[](
	vec3(1.),
	color_foo1,
	color_foo2
);

void main(void){
	vec3 direction = get_direction();
	vec3 final_color = vec3(0);
	vec3 light = /*view_position +*/ vec3(-40, 40, -40);
	vec3 hit = view_position;
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	vec3 color_light = vec3(.8);
	for (int reflections = 0; reflections < 3; reflections++) {
		int i;
		hit = march(hit, direction, i);
		if (i >= 100) {
			// hit nothing
			break;
		}

		int material = int(f(hit)[1]);
		vec3 normal = calc_normal(hit);
		vec3 to_light = light - hit;
		vec3 to_view = view_position - hit;

		//vec3 light_color = vec3(0.9*lambert(to_light, normal) + .1);
		vec3 light_color = vec3(0.9*cook_torrance(to_light, normal, -direction, .45, 450.) + .1);
		light_color += vec3(phong(to_light, normal, to_view, 100. /* darfs auch etwas mehr exponent sein? */));
		light_color *= color_light;
		light_color *= ao(hit, normal, .15, 5);
		if(material != MAT_BOUNDING){
			//light_color *= softshadow(hit, light, 64.);
		}

		vec3 new_color = colors[material] * light_color;

		final_color += new_color * reflection_factor * light_color_factor;

		if(material == MAT_BOUNDING){
			break;
		}

		light_color_factor *= light_color;
		reflection_factor *= .8;

		direction = reflect(direction, normal);
	}

	out_color.rgb = final_color;
	out_depth = distance(view_position, hit);
}

vec2 f(vec3 p){
	float kuchel_wand_f = smax(sphere(trans(p, 0,0,100), 100), -sphere(p, 10), 50);
	vec2 kuchel_wand_vec = vec2(kuchel_wand_f, MAT_BLA);
	//vec2 kuchel = vec2(sphere(p, 5.), MAT_CHROME);
	vec2 kuchel = vec2(torus(p.xzy, vec2(5,3)), MAT_CHROME);


	vec2 bounding = vec2(-sphere(transv(p, view_position), 220.), MAT_BOUNDING);
	return min_material(min_material(kuchel, kuchel_wand_vec), bounding);
}
