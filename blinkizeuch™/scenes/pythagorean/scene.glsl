vec2 f(vec3);
vec2 g(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_NORMAL(calc_normal_floor, g)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if(i < 100){
		int material = int(f(hit)[1]);
		vec3 light = vec3(0, 10, 0);
		vec3 normal;
		if(material == 1) {
			normal =  calc_normal(hit);
			vec3 to_light = light - hit;
			final_color = .9 * vec3(oren_nayar(to_light, normal, -dir, foo2));
			final_color += .1;
		} else if(material == 2) {
			normal = calc_normal_floor(hit);
			final_color = vec3(1.);
		}
		final_color *= ao(hit, normal, .2, 10.);
		final_color *= 1. - smoothstep(0., 200., distance(view_position, hit));
	}
	out_color = final_color;
}

float cube(vec3 p, float r) {
	// seems to be faster than
	// return box(p, vec3(1.));
	p = abs(p);
	return max(p.x, max(p.y, p.z)) - r;
}

float pythagoraen(vec3 p) {
	float alpha = (1. - 0.6 * smoothstep(0., 10., time)) * 90.;
	float beta = radians(90. - alpha);
	alpha = radians(alpha);
	float a = cos(alpha);
	float b = sqrt(1. - a * a);
	float p_a = 1.;
	float tree = cube(p, p_a);

	for(int i = 0; i < 15; i++) {
		float q_a = a * p_a;
		vec3 q = trans(p, 0, p_a , -p_a);
		q = rX(alpha) * q;
		q = trans(q, 0, q_a, q_a);
		tree = min(tree, cube(q, q_a));

		float r_a = b * p_a;
		vec3 r = trans(p, 0, p_a, p_a);
		r = rX(-beta) * r;
		r = trans(r, 0, r_a, -r_a);
		tree = min(tree, cube(r, r_a));

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
	float foo = 4. * rand(200. * floor(p.xz / c) - 123.);
	float tree = pythagoraen(rY(foo * radians(90.)) * q);
	vec2 bounding = vec2(-sphere(transv(p, view_position), 200.), 2);
	return min_material(vec2(tree,1), min_material(vec2(f_floor(p), 2.), bounding));
}

vec2 g(vec3 p) {
	return vec2(f_floor(p) + fbm(p.xz * 5.) * .15, 2.);
}
