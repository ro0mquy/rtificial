vec2 f(vec3 p);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)
//DEFINE_SHADOW_RAY(shadow_ray, f)
DEFINE_SOFTSHADOW(softshadow, f)

vec3 colors[4];

mat3 rot_box1;
mat3 rot_box2;

void main(void) {
	colors[0] = vec3(1.);
	colors[1] = vec3(1., 0., 0.);
	colors[2] = vec3(0., 1., 0.);
	colors[3] = vec3(1.);

	rot_box1 = rY(radians(20.));
	rot_box2 = rY(radians(-20.));

	vec3 direction = get_direction();
	int i;
	vec3 p = march(view_position, direction, i);

	vec3 color = vec3(0.);
	if(i < 100) {
		int material = int(f(p).y);
		vec3 normal = calc_normal(p);
		/*
		vec4 l = max(normal * mat4x3(
			normalize(vec3( .25, 3.5,  .25) - p),
			normalize(vec3(-.25, 3.5,  .25) - p),
			normalize(vec3( .25, 3.5, -.25) - p),
			normalize(vec3(-.25, 3.5, -.25) - p)
		), vec4(0.));
		if(material == 3) l = vec4(1.);
		color = colors[material] * dot(l, vec4(.25));
		*/

		// ceiling light is always white
		if (material == 3) {
			color = colors[3];
		} else {
			vec3 light = vec3(0., 3.4, 0.);
			vec3 to_light = normalize(light - p);
			float factor = max(0., dot(normal, to_light));
			color = colors[material] * factor;
			//color *= shadow_ray(p, light);
			color *= max(.05, softshadow(p, light, 32.));
			//color += 0.1 * ao(p, normal, .15, 5);
		}
	}

	out_color = color;
}

vec2 f(vec3 p) {
	float room_size = 3.5;
	float back = p.z + room_size;
	float bottom = p.y + room_size;
	float top = -p.y + room_size;
	float white_walls = min(back, min(bottom, top));
	float box_size = .8;
	float box1 = box(rot_box1 * p - vec3(-box_size * 1.5, -3.5 + box_size * 2, -box_size), vec3(box_size, box_size * 2., box_size));
	float box2 = box(rot_box2 * p - vec3( box_size *1.5, -3.5 + box_size,  box_size), vec3(box_size));
	vec2 white_objects = vec2(min(white_walls, min(box1, box2)), 0.);
	vec2 red_wall = vec2(p.x + room_size, 1.);
	vec2 green_wall = vec2(-p.x + room_size, 2.);
	vec2 light = vec2(box(p - vec3(0., room_size, 0.), vec3(.5, .001, .5)), 3.);
	return min_material(min_material(white_objects, red_wall), min_material(green_wall, light));
}

