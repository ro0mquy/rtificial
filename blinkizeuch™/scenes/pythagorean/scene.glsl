vec2 f(vec3);
vec2 g(vec3);

float fbm(vec2 p);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_NORMAL(calc_normal_floor, g)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

#define mat_bounding 0
#define mat_floor 1
#define mat_kugel 2
#define mat_tree1 3
#define mat_tree2 4
#define mat_tree3 5
#define mat_tree4 6
#define mat_tree5 7
#define mat_tree6 8
#define mat_tree7 9
#define mat_tree8 10
#define mat_tree9 11

vec3[] tree_colors = vec3[](
	vec3(.9, .9, .95),
	vec3(.7, .7, .75),
	vec3(.1, .1, .1)

);

vec3 colors[] = vec3[](
	vec3(0),
	vec3(.4),
	vec3(1, 0, 0),
	tree_colors[0],
	tree_colors[1],
	tree_colors[2],
	tree_colors[0],
	tree_colors[1],
	tree_colors[2],
	tree_colors[1],
	tree_colors[2],
	tree_colors[0]
);

const int num_rand_colors = 6;
vec3[num_rand_colors] rand_colors = vec3[num_rand_colors](
	vec3(1, 0, 0),
	vec3(.5, .5, 0),
	vec3(0, 1, 0),
	vec3(0, .5, .5),
	vec3(0, 0, 1),
	vec3(.5, 0, .5)
);

float wakeup = smoothstep(12., 15., time);
float wakeup_limited = wakeup * step(-15, -time);

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	int material = int(f(hit)[1]);
	vec3 light = vec3(0, 10, 0);
	vec3 normal;
	vec3 color = colors[material];
	vec3 to_light = light - hit;
	float m = 0;
	float bloom = 0.;
	if(material >= mat_tree1) {
		normal = calc_normal(hit);
		m = 0;

		vec2 coord = floor(hit.xz / 20);
		material += int(time * length(coord));
		vec3 bunt = rand_colors[material % num_rand_colors];

		int[] playing = int[](10, 11, 2, 3, 31, 31, 31, 31, 31);
		int instrument = playing[material % 9];
		float intensity = clamp(2 * senvelopes[instrument], 0, 1);
		color = mix(color, bunt, intensity);
		bloom += intensity;
	} else if(material == mat_floor) {
		normal = calc_normal_floor(hit);
		m = .42;
	} else if(material == mat_kugel) {
		normal = calc_normal(hit);
		m = 2;
		float senvs = senvelopes[2] + senvelopes[3];
		final_color += (.1 + senvs + .7 * wakeup_limited) * color;
		bloom = senvs * 2;
		bloom += wakeup;
	}

	if(material == mat_kugel) {
		final_color += color * cook_torrance(to_light, normal, -dir, m, 450);
		final_color *= ao(hit, normal, .3, 5.);
	} else if(material != mat_bounding) {
		final_color += color * (.05 + .95 * oren_nayar(to_light, normal, -dir, m));
		final_color *= ao(hit, normal, .3, 5.);
	} else {
		//vec2 spherical = vec2(acos(dir.y) / 3.141, abs(atan(dir.z, dir.x) / 3.141));
		//final_color = hsv2rgb(vec3(.67, .5 + .5 * fbm(spherical * 20.), .45));
	}
	final_color *= 1. - smoothstep(0., 300., distance(view_position, hit));
	out_color.rgb = final_color;
	out_color.a = bloom;
}

float cube(vec3 p, float r) {
	// seems to be faster than
	// return box(p, vec3(1.));
	float s = sphere(p, r);
	p = abs(p);
	float f = max(p.x, max(p.y, p.z)) - r;
	return mix(s, f, 1. + .4 * smoothstep(5., 15., time));
}

vec2 pythagoraen(vec3 p) {
	float alpha = (1. - 0.6 * smoothstep(0., 10., time)) * 90.;
	float beta = radians(90. - alpha);
	alpha = radians(alpha);
	float a = cos(alpha);
	float b = sqrt(1. - a * a);
	float p_a = 1.;
	vec2 tree = vec2(cube(p, p_a), mat_tree3);

	for(int i = 0; i < 10; i++) {
		float q_a = a * p_a;
		vec3 q = trans(p, 0, p_a , -p_a);
		q = rX(alpha) * q;
		q = trans(q, 0, q_a, q_a);

		float r_a = b * p_a;
		vec3 r = trans(p, 0, p_a, p_a);
		r = rX(-beta) * r;
		r = trans(r, 0, r_a, -r_a);

		vec2 cube_1 = vec2(cube(r, r_a), mat_tree1 + (i * i) % 9);
		vec2 cube_2 = vec2(cube(q, q_a), mat_tree1 + (i * (i+1)) % 9);
		vec2 next_cubes = min_material(cube_1, cube_2);
		tree = min_material(tree, next_cubes);

		p = q;
		p_a = q_a;
		p = rY(radians(smoothstep(0., 10., time) * 90.)) * p;
	}
	return tree;
}

float fbm(vec2 p) {
	float sum = 0.;
	float amplitude = .5;
	float freq = 1.;
	for(int i = 0; i < 3; i++) {
		sum += classic_noise(p * freq) * amplitude;
		freq *= 2.;
		amplitude *= .5;
	}
	return sum;
}

float f_floor(vec3 p) {
	return p.y + 1.;
}

vec2 f(vec3 p) {
	float c = 20;
	vec3 q = domrep(p, c, 1., c);
	q.y = p.y;
	float foo = 4. * rand(400. * (floor(p.xz / c) - 123.));
	float foo2 = 4. * rand(400. * (floor(p.xz / c) - 456.));
	q = trans(q, 2. * (foo - 2.), .0, 2. * (foo2 - 2.));
	vec2 tree = pythagoraen(rY(foo * radians(90.)) * q);
	tree.y = mod(floor(tree.y + foo), 9) + mat_tree1;
	vec2 bounding = vec2(-sphere(transv(p, view_position), 200.), mat_bounding);

	vec3 trans_kugel = vec3(0);
	if (time < 15) {
		vec2 k = vec2(time * 5., time * 3.);
		trans_kugel = .1 * vec3(rand(k), rand(k + 2.), rand(k + 3.)) * wakeup_limited;
	} else if (time < 16) {
		float time = time - 15;
		float anim_duration = 1;
		time -= anim_duration / 2;
		float gravity = 20;
		float h = .5 * gravity * time * time;
		h = .5 * gravity * anim_duration * anim_duration / 4 - h;
		trans_kugel.y = h;
	}

	vec2 kugel = vec2(sphere(transv(p, trans_kugel), 1.), mat_kugel);
	return min_material(tree, min_material(vec2(f_floor(p), mat_floor), min_material(bounding, kugel)));
}

vec2 g(vec3 p) {
	return vec2(f_floor(p) + fbm(p.xz * .7) * .03, 2.);
}
