
vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_untenrum;
uniform vec3 color_house1;

void main(void) {
	vec3 dir = get_direction();
	int i;
	vec3 hit = march(view_position, dir, i);
	vec3 color = vec3(0);
	if(i < 100) {
		vec3 normal = calc_normal(hit);
		vec3 light = vec3(20, 20, 20);
		int material = int(f(hit)[1]);
		if(material == 0) {
			out_color = vec4(vec3(0), 1);
			return;
		} else if(material == 1) {
			color = color_untenrum;
		} else if(material == 2) {
			color = color_house1;
		}
		color *= max(dot(normal, normalize(light - hit)), 0);
		color += 0.2 * ao(hit, normal, .15, 5);
	}
	out_color = vec4(color, 1);
}

vec2 f(vec3 p) {
	vec2 aussrenrum = vec2(-sphere(TRANSv(p, view_position), 400), 0);
	// ist die normale, oben wird man dich loben!
	vec2 untenrum = vec2(plane(TRANS(p, 0, -20, 0), vec3(0, 1, 0)), 1);
	vec2 house1 = vec2(box(p, vec3(5, 20, 5)), 2);
	return min_material(min_material(aussrenrum, untenrum), house1);
}
