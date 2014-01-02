
vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

const int MAT_UNTENRUM = 1;
const int MAT_HOUSES = 2;
const int MAT_HOUSE2 = 3;

uniform vec3 color_untenrum;
uniform vec3 color_house1;
uniform vec3 color_house2;
uniform float noisiness;
uniform float corner_width;

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
		} else if(material == MAT_HOUSE2) {
			color = color_house2;
		}
		factor *= max(dot(normal, normalize(light - hit)), 0);
		factor += 0.2 * ao(hit, normal, .15, 5);
		color *= factor;
	}
	out_color = vec4(color, 1);
}

float haus_mit_ecken(vec3 p, vec3 box_dim, float corner_width) {
	float house = box(p, box_dim);
	for(int x = 0; x < 2; x++) {
		for(int z = 0; z < 2; z++) {
			house = max(house, -box(trans(p, (2 * x - 1) * box_dim.x, 0, (2 * z - 1) * box_dim.z), vec3(corner_width, box_dim.y * 1.1, corner_width)));
		}
	}
	return house;
}

vec2 f(vec3 p) {
	vec2 aussrenrum = vec2(-sphere(transv(p, view_position), 400), 0);
	// ist die normale, oben wird man dich loben!
	float house3_width = 14. / 2.;
	float house1_width = 17. / 2.;
	float house2_width = 20. / 2.;
	float house3_height = 20. / 2.;
	float house1_height = 40. / 2.;
	float house2_height = 30. / 2.;
	vec2 untenrum = vec2(plane(trans(p, 0, 0, 0), vec3(0, 1, 0)), MAT_UNTENRUM);
	vec2 house1 = vec2(haus_mit_ecken(trans(p, 0, 2 * house2_height + house1_height, 0), vec3(house1_width, house1_height, house1_width), corner_width * house1_width), MAT_HOUSES);
	vec2 house2 = vec2(haus_mit_ecken(trans(p, 0, house2_height, 0), vec3(house2_width, house2_height, house2_width),  corner_width * house2_width), MAT_HOUSE2);
	vec2 house3 = vec2(haus_mit_ecken(trans(p, 0, 2 * (house2_height + house1_height) + house3_width, 0), vec3(house3_width, house3_height, house3_width),  corner_width * house3_width), MAT_HOUSE2);
	vec2 house = min_material(house1, min_material(house2, house3));
	float houses_height = 6;
	vec3 q = trans(p, 0, houses_height, 0);
	float houses_dist = box(domrep(q, 10, q.y + .1, 10), vec3(3, houses_height, 3));
	float no_houses = box(p, vec3(20, 20, 20));
	vec2 houses = vec2(max(houses_dist, -no_houses), MAT_HOUSES);
	return min_material(min_material(aussrenrum, untenrum), min_material(house, houses));
}
