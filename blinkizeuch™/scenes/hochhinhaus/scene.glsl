
vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

const int MAT_UNTENRUM = 1;
const int MAT_HOUSES = 2;
const int MAT_HOUSE2 = 3;
const int MAT_WINDOWS = 4;

uniform vec3 color_untenrum;
uniform vec3 color_house1;
uniform vec3 color_house2;
uniform float noisiness;
uniform float corner_width;
uniform float ambient_intensity;
uniform vec3 color_windows;
uniform float window_reflectivity;

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

void main(void) {
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int max_bounces = 2;
	float factor = 1;
	vec3 p = view_position;
	for(int j = 0; j < max_bounces; j++) {
		int i;
		vec3 hit = march(p, dir, i);
		if(i < 100) {
			vec3 normal = calc_normal(hit);
			vec3 light = vec3(20, 60, 20);
			int material = int(f(hit)[1]);
			vec3 color;
			bool cont = false;
			float local_factor = factor;
			if(material == 0) {
				 color = vec3(0);
			} else if(material == MAT_UNTENRUM) {
				color = color_untenrum;
				float noise = 0;
				noise += classic_noise(noisiness * hit.xz * .06125) * .5;
				noise += classic_noise(noisiness * hit.xz * .125) * .25;
				noise += classic_noise(noisiness * hit.xz * .25) * .125;
				color += noise * .1;
			} else if(material == MAT_HOUSES) {
				color = color_house1;
			} else if(material == MAT_HOUSE2) {
				color = color_house2;
			} else if(material == MAT_WINDOWS) {
				color = color_windows;
				dir = reflect(dir, normal);
				p = hit + .01 * dir;
				factor *= window_reflectivity;
				cont = true;
			}
			local_factor *= max(dot(normal, normalize(light - hit)), 0);
			local_factor += ambient_intensity * ao(hit, normal, .15, 5);
			color *= local_factor;
			//factor *= local_factor;
			final_color += color;
			if(!cont) break;
		} else {
			break;
		}
	}
	out_color = vec4(final_color, 1);
}

float corner(vec3 p, float corner_width) {
	p = abs(p);
	return max(plane(trans(p, corner_width, 0, 0), vec3(1, 0, 0)), plane(trans(p, 0, 0, corner_width), vec3(0, 0, 1)));
}

float window_xy(vec3 p, vec2 window_size) {
	p = abs(p);
	return max(plane(trans(p, 0, 0, window_size.x), vec3(0, 0, 1)), plane(trans(p, 0, window_size.y, 0), vec3(0, 1, 0)));
}

float window_zy(vec3 p, vec2 window_size) {
	p = abs(p);
	return max(plane(trans(p, window_size.x, 0, 0), vec3(1, 0, 0)), plane(trans(p, 0, window_size.y, 0), vec3(0, 1, 0)));
}

vec2 haus_mit_ecken(vec3 p, vec3 box_dim, float corner_width, int material) {
	float house = box(p, box_dim);
	house = max(house, -corner(domrep(p, 2 * box_dim.x, 1., 2 * box_dim.z), corner_width));
	vec2 window_size = vec2(.4, .8);
	vec3 p2 = p;
	p2.z = abs(p.z);
	vec3 q = trans(p2, 0, 0, box_dim.x);
	vec3 q2 = domrep(q, window_size.x * 2. + .1, window_size.y * 2. + .1, 1.);
	q2.z = q.z;
	float windows_z = window_zy(q2, window_size);
	windows_z = max(windows_z, box(q, vec3(box_dim.x - corner_width, box_dim.y, .01)));

	p2 = p;
	p2.x = abs(p.x);
	q = trans(p2, box_dim.z, 0, 0);
	q2 = domrep(q, 1., window_size.y * 2. + .1, window_size.x * 2. + .1);
	q2.x = q.x;
	float windows_x = window_xy(q2, window_size);
	windows_x = max(windows_x, box(q, vec3(.1, box_dim.y, box_dim.z - corner_width)));

	float all_windows = min(windows_z, windows_x);
	return min_material(vec2(house, material), vec2(all_windows, MAT_WINDOWS));
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
	vec2 house1 = haus_mit_ecken(trans(p, 0, 2 * house2_height + house1_height, 0), vec3(house1_width, house1_height, house1_width), corner_width * house1_width, MAT_HOUSES);
	vec2 house2 = haus_mit_ecken(trans(p, 0, house2_height, 0), vec3(house2_width, house2_height, house2_width),  corner_width * house2_width, MAT_HOUSE2);
	vec2 house3 = haus_mit_ecken(trans(p, 0, 2 * (house2_height + house1_height) + house3_width, 0), vec3(house3_width, house3_height, house3_width),  corner_width * house3_width, MAT_HOUSE2);
	vec2 house = min_material(house1, min_material(house2, house3));
	float foo = length(floor(p.xz / 10) * 10);
	float houses_height = 6 + sin(foo);
	vec3 q = trans(p, 0, houses_height, 0);
	float houses_dist = box(domrep(q, 10, q.y + .1, 10), vec3(3, houses_height, 3));
	float no_houses = box(p, vec3(20, 24, 20));
	vec2 houses = vec2(max(houses_dist, -no_houses), MAT_HOUSES);
	return min_material(min_material(aussrenrum, untenrum), min_material(house, houses));
}
