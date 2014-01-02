
vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

const int MAT_UNTENRUM = 1;
const int MAT_HOUSES = 2;

uniform vec3 color_untenrum;
uniform vec3 color_house1;
uniform float noisiness;

void main(void) {
	vec3 dir = get_direction();
	int i;
	vec3 hit = march(view_position, dir, i);
	vec3 color = vec3(0);
	if(i < 100) {
		vec3 normal = calc_normal(hit);
		vec3 light = vec3(20, 20, 20);
		int material = int(f(hit)[1]);
		float factor = 1;
		if(material == 0) {
			out_color = vec4(vec3(0), 1);
			return;
		} else if(material == MAT_UNTENRUM) {
			color = color_untenrum;
			float noise = smooth_noise(noisiness * hit);
			color += noise * .3;
		} else if(material == MAT_HOUSES) {
			color = color_house1;
		}
		factor *= max(dot(normal, normalize(light - hit)), 0);
		factor += 0.2 * ao(hit, normal, .15, 5);
		color *= factor;
	}
	out_color = vec4(color, 1);
}

vec2 f(vec3 p) {
	vec2 aussrenrum = vec2(-sphere(transv(p, view_position), 400), 0);
	// ist die normale, oben wird man dich loben!
	vec2 untenrum = vec2(plane(trans(p, 0, -20, 0), vec3(0, 1, 0)), MAT_UNTENRUM);
	vec2 house1 = vec2(box(p, vec3(5, 20, 5)), MAT_HOUSES);
	float houses_height = 6;
	vec3 q = trans(p, 0, -(20 - houses_height), 0);
	float houses_dist = box(domrep(q, 10, q.y + .1, 10), vec3(3, houses_height, 3));
	float no_houses = box(p, vec3(20, 20, 20));
	vec2 houses = vec2(max(houses_dist, -no_houses), MAT_HOUSES);
	return min_material(min_material(aussrenrum, untenrum), min_material(house1, houses));
}
