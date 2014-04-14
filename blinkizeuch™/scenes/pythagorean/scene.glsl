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

vec3 colors[] = vec3[](
	vec3(0),
	vec3(1),
	vec3(.1,1.,.2),
	vec3(.6,.9,.5),
	vec3(1.,.6,.8)
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
	float m;
	float bloom = 0.;
	if(material >= mat_tree1) {
		normal = calc_normal(hit);
		final_color += .1;
		m = 0.;
		vec2 coord = floor(hit.xz / 20);
		bloom = color.x * wakeup;
		color.x = rand(coord * .25);
		color = hsv2rgb(color);
	} else if(material == mat_floor) {
		normal = calc_normal_floor(hit);
		m = .42;
	} else if(material == mat_kugel) {
		normal = calc_normal(hit);
		color = vec3(1, 0, 0);
		final_color += .1 * color;
		bloom = senvelopes[2] + senvelopes[3];
		bloom += wakeup;
		final_color += .2 * wakeup_limited;
	}
	if(material != 0) {
		final_color += color * oren_nayar(to_light, normal, -dir, m);
		final_color *= ao(hit, normal, .3, 5.);
	} else {
		vec2 spherical = vec2(acos(dir.y) / 3.141, abs(atan(dir.z, dir.x) / 3.141));
		final_color = hsv2rgb(vec3(.67, .5 + .5 * fbm(spherical * 20.), .45));
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

	for(int i = 0; i < 15; i++) {
		float q_a = a * p_a;
		vec3 q = trans(p, 0, p_a , -p_a);
		q = rX(alpha) * q;
		q = trans(q, 0, q_a, q_a);

		float r_a = b * p_a;
		vec3 r = trans(p, 0, p_a, p_a);
		r = rX(-beta) * r;
		r = trans(r, 0, r_a, -r_a);

		vec2 next_cubes = vec2(min(cube(r, r_a), cube(q, q_a)), mat_tree1 + i % 3);
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
	vec2 bounding = vec2(-sphere(transv(p, view_position), 200.), mat_bounding);
	vec2 k = vec2(time * 5., time * 3.);
	vec3 trans_kugel = .1 * vec3(rand(k), rand(k + 2.), rand(k + 3.)) * wakeup_limited;
	vec2 kugel = vec2(sphere(transv(p, trans_kugel), 1.), mat_kugel);
	return min_material(tree, min_material(vec2(f_floor(p), mat_floor), min_material(bounding, kugel)));
}

vec2 g(vec3 p) {
	return vec2(f_floor(p) + fbm(p.xz * .7) * .03, 2.);
}
